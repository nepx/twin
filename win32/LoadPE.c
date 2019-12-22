#include <stdio.h>

#include "windows.h"

#include "peexe.h"

#include "thread.h"
#include "Segment.h"

#include "Log.h"
#include "ctype.h"

#include "setjmp.h"
#include "LoadPE.h"
#include "BinTypes.h"
#include "Resources.h"
#include "Module.h"
#include "compat.h"
#include "Kernel.h"

extern char *GetTwinFilename();
extern TWINRT DefaultTwinRT32;

DWORD WinStubAPI(char *,char *,int);

DWORD PEGetProcAddress(MODULEINFO *, int ,char *);
void  FileErrorMB(int,char *);

/*
 * Load a PE file into memory
 * read headers first...
 * get file ImageBase
 * get file SizeOfImage
 *
 * VirtualAlloc(ImageBase,SizeofImage,...)
 * read section headers
 * for each section read section...
 */

static IMAGE_NT_HEADERS NTHeader[256];
static int NTHeaderCount;

#if DEBUG
void DumpHeader(PIMAGE_FILE_HEADER );
void DumpOptionalHeader(PIMAGE_OPTIONAL_HEADER );
#endif
#if SEVERE
void DumpSectionTable(void *,PIMAGE_SECTION_HEADER , unsigned );
#endif

int LoadImportsSection(char *,void * , PIMAGE_NT_HEADERS);
void LoadExportsTable(MODULEINFO *,PIMAGE_NT_HEADERS);

int ExecEntryPoint(MODULEINFO *);

int ExecPE(MODULEINFO *,LPUSERDLL,char *,WORD);

extern HMODULE    GetModuleFromInstance(HINSTANCE);
extern HINSTANCE  TWIN_GetInstanceFromTask(HTASK);

HINSTANCE
adjustment(MODULEINFO *modinfo)
{
        LPTASKINFO lpTaskInfo;
	HINSTANCE hInst;
	HTASK     hTask;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_FILE_HEADER pImageFileHeader;

	// we need an hInstance
	hInst = GlobalAlloc(GMEM_MOVEABLE,0);
	modinfo->hInst = hInst;
	modinfo->nRefCnt++;

	pNTHeader = modinfo->lpNTHeader;
	pImageFileHeader = &pNTHeader->FileHeader;

	hTask = GetCurrentTask();

        if ( pImageFileHeader->Characteristics & IMAGE_FILE_DLL) {
	} else {
		// bind the hModule to the hInst 
		// hModule comes from our modinfo
		lpTaskInfo = GETTASKINFO(hTask);
    		lpTaskInfo->hInst = hInst;
		ActivateTask(hTask);
	}

	CreateDataInstance(hInst,(HMODULE)modinfo->ObjHead.hObj,hTask);

	return hInst;
}

int
ExecPE(MODULEINFO *modinfo,LPUSERDLL lpUserDLL,char *lpCmdline,WORD flags)
{
	static void *BaseAddress;
	IMAGE_DOS_HEADER DosHeader;
 	PIMAGE_SECTION_HEADER pSectionHeaders;
	PIMAGE_NT_HEADERS pNTHeader;
	int i,len;
	int ret;
	HFILE hFile;
	
	/* read the dos image header first */
        hFile = lpUserDLL->hfExeFile;
    	_llseek(hFile,0,0);
        ret = _lread(hFile,&DosHeader,sizeof(IMAGE_DOS_HEADER));

	if(DosHeader.e_magic == IMAGE_DOS_SIGNATURE)	{

		/* now read in the nt header */
    		_llseek(hFile,DosHeader.e_lfanew,0);

		pNTHeader = &NTHeader[NTHeaderCount++];
		ret = _lread(hFile,pNTHeader, sizeof(IMAGE_NT_HEADERS));

		/* yes, it is a win32 header */
		if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) {
			_lclose(hFile);
    			MessageBox((HWND)NULL,"No PE signature","LoadPE error",MB_OK);
			return 0;
		}

		BaseAddress = VirtualAlloc(
			(void *) pNTHeader->OptionalHeader.ImageBase,
			pNTHeader->OptionalHeader.SizeOfImage,
			MEM_COMMIT,
			PAGE_EXECUTE_READWRITE);

		logstr(LF_CONSOLE,"LoadModule32:   %s %p\n",
			modinfo->lpFileName,BaseAddress);
		modinfo->lpNTHeader = pNTHeader;
		modinfo->lpNTBaseAddress = BaseAddress;

		memcpy(BaseAddress,&DosHeader,sizeof(IMAGE_DOS_HEADER));

		memcpy(BaseAddress + DosHeader.e_lfanew,
			pNTHeader,sizeof(IMAGE_NT_HEADERS));


		pNTHeader = BaseAddress + DosHeader.e_lfanew;

#if SEVERE
		/* show the NT header */
		if(LF_LOGPE)
		  DumpHeader(&pNTHeader->FileHeader);

		/* show the Optional header */
		if(LF_LOGPE)
		  DumpOptionalHeader((PIMAGE_OPTIONAL_HEADER) 
			&pNTHeader->OptionalHeader);
#endif
		/************************************************/
		/* HACK store our modinfo in the OptionalHeader */
		/************************************************/
		pNTHeader->OptionalHeader.Reserved1 = (DWORD) modinfo;

		pSectionHeaders = (PIMAGE_SECTION_HEADER)
			((void *)BaseAddress + 		// Base Address
			DosHeader.e_lfanew +		// NT Header start
			sizeof(IMAGE_NT_HEADERS));	// NT Header

		/* now read the section headers */
		ret = _lread( hFile, pSectionHeaders, 
			sizeof(IMAGE_SECTION_HEADER)*
			pNTHeader->FileHeader.NumberOfSections);

		for(i=0; i < pNTHeader->FileHeader.NumberOfSections; i++) {
			void *LoadAddress;

			LoadAddress = 
			    RVA(BaseAddress,pSectionHeaders->VirtualAddress);

		        logstr(LF_LOGPE,"Load Section %d addr=%p name=%s\n",
				i,LoadAddress,pSectionHeaders->Name);

#ifdef SEVERE
			DumpSectionTable( LoadAddress,pSectionHeaders,i);
#endif

			/* load only non-BSS segments */
			if(!(pSectionHeaders->Characteristics &
				IMAGE_SCN_CNT_UNINITIALIZED_DATA)) 
		        {
			    _llseek(hFile,pSectionHeaders->PointerToRawData,SEEK_SET);
			    len = _lread(hFile,(char*) LoadAddress, 
				pSectionHeaders->SizeOfRawData);

			    if( len != pSectionHeaders->SizeOfRawData)
			    {
				logstr(LF_ERROR,"Failed to load section %x %x\n", i,len);
				exit(0);
			    }
			}

			pSectionHeaders++;
			

#if 0
			/* not needed, memory is zero */
			if(strcmp(pSectionHeaders[i].Name, ".bss") == 0)
			    memset((void *)LoadAddress, 0,
				   pSectionHeaders[i].Misc.VirtualSize ?
				   pSectionHeaders[i].Misc.VirtualSize :
				   pSectionHeaders[i].SizeOfRawData);
#endif

		}

		_lclose(hFile);
	
		modinfo->dwWin32flag = 1;
    		modinfo->lptwinrt    = &DefaultTwinRT32;

		adjustment(modinfo);

		/* we are dependent on other modules, go get and load those */
		if(LoadImportsSection(modinfo->lpFileName,BaseAddress,pNTHeader) == 0){
	    		FileErrorMB(ILL_FINAL,modinfo->lpFileName);
			return 0;
	        }

		LoadExportsTable(modinfo,pNTHeader);

		ret = ExecEntryPoint( modinfo);

		return (int)modinfo->hInst;
	}
	return 0;
}

typedef struct
{
        WORD    flag;
        LPSTR   name;
} WORD_FLAG_DESCRIPTIONS;

typedef struct
{
        DWORD   flag;
        LPSTR   name;
} DWORD_FLAG_DESCRIPTIONS;
#define IMAGE_FILE_DLL                  0x2000

/* Bitfield values and names for the IMAGE_FILE_HEADER flags */
WORD_FLAG_DESCRIPTIONS ImageFileHeaderCharacteristics[] =
{
{ IMAGE_FILE_RELOCS_STRIPPED, "RELOCS_STRIPPED" },
{ IMAGE_FILE_EXECUTABLE_IMAGE, "EXECUTABLE_IMAGE" },
{ IMAGE_FILE_LINE_NUMS_STRIPPED, "LINE_NUMS_STRIPPED" },
{ IMAGE_FILE_LOCAL_SYMS_STRIPPED, "LOCAL_SYMS_STRIPPED" },
{ IMAGE_FILE_MINIMAL_OBJECT, "MINIMAL_OBJECT" },
{ IMAGE_FILE_UPDATE_OBJECT, "UPDATE_OBJECT" },
{ IMAGE_FILE_BYTES_REVERSED_LO, "BYTES_REVERSED_LO" },
{ IMAGE_FILE_32BIT_MACHINE, "32BIT_MACHINE" },
{ IMAGE_FILE_PATCH, "PATCH" },
{ IMAGE_FILE_SYSTEM, "SYSTEM" },
{ IMAGE_FILE_DLL, "DLL" },
{ IMAGE_FILE_BYTES_REVERSED_HI, "BYTES_REVERSED_HI" },
{ IMAGE_FILE_DEBUG_STRIPPED,"DEBUGGING INFO STRIPPED" }
};

#define NUMBER_IMAGE_HEADER_FLAGS (sizeof(ImageFileHeaderCharacteristics) / sizeof(WORD_FLAG_DESCRIPTIONS))

void DumpHeader(PIMAGE_FILE_HEADER pImageFileHeader)
{
        UINT headerFieldWidth = 30;
        UINT i;
        char *szMachine;

        logstr(LF_LOGPE,"File Header\n");

        switch( pImageFileHeader->Machine )
        {
                case IMAGE_FILE_MACHINE_I386:   szMachine = "i386"; break;
                case IMAGE_FILE_MACHINE_I860:   szMachine = "i860"; break;
                case IMAGE_FILE_MACHINE_R3000:  szMachine = "R3000"; break;
                case IMAGE_FILE_MACHINE_R4000:  szMachine = "R4000"; break;
                case IMAGE_FILE_MACHINE_ALPHA:  szMachine = "alpha"; break;
                default:    szMachine = "unknown"; break;
        }

        logstr(LF_LOGPE,"  %-*s%04X (%s)\n", headerFieldWidth, "Machine:",
                                pImageFileHeader->Machine, szMachine);
        logstr(LF_LOGPE,"  %-*s%04X\n", headerFieldWidth, "Number of Sections:",
                                pImageFileHeader->NumberOfSections);
        logstr(LF_LOGPE,"  %-*s%08X\n", headerFieldWidth, "TimeDateStamp:",
                                pImageFileHeader->TimeDateStamp);
        logstr(LF_LOGPE,"  %-*s%08X\n", headerFieldWidth, "PointerToSymbolTable:",
                                pImageFileHeader->PointerToSymbolTable);
        logstr(LF_LOGPE,"  %-*s%08X\n", headerFieldWidth, "NumberOfSymbols:",
                                pImageFileHeader->NumberOfSymbols);
        logstr(LF_LOGPE,"  %-*s%04X\n", headerFieldWidth, "SizeOfOptionalHeader:",
                                pImageFileHeader->SizeOfOptionalHeader);

        logstr(LF_LOGPE,"  %-*s%04X\n", headerFieldWidth, "Characteristics:",
                                pImageFileHeader->Characteristics);

        for ( i=0; i < NUMBER_IMAGE_HEADER_FLAGS; i++ )
        {
                if ( pImageFileHeader->Characteristics &
                         ImageFileHeaderCharacteristics[i].flag )
                        logstr(LF_LOGPE,"    %s\n", ImageFileHeaderCharacteristics[i].name );
        }
	logstr(LF_LOGPE,"\n");
}

/* Bitfield values and names for the DllCharacteritics flags */
WORD_FLAG_DESCRIPTIONS DllCharacteristics[] =
{
{ IMAGE_LIBRARY_PROCESS_INIT, "PROCESS_INIT" },
{ IMAGE_LIBRARY_PROCESS_TERM, "PROCESS_TERM" },
{ IMAGE_LIBRARY_THREAD_INIT, "THREAD_INIT" },
{ IMAGE_LIBRARY_THREAD_TERM, "THREAD_TERM" },
};
#define NUMBER_DLL_CHARACTERISTICS (sizeof(DllCharacteristics) / sizeof(WORD_FLAG_DESCRIPTIONS))

/* Bitfield values and names for the LoaderFlags flags */
DWORD_FLAG_DESCRIPTIONS LoaderFlags[] =
{
{ IMAGE_LOADER_FLAGS_BREAK_ON_LOAD, "BREAK_ON_LOAD" },
{ IMAGE_LOADER_FLAGS_DEBUG_ON_LOAD, "DEBUG_ON_LOAD" }
};
#define NUMBER_LOADER_FLAGS (sizeof(LoaderFlags) / sizeof(DWORD_FLAG_DESCRIPTIONS))

/* Names of the data directory elements that are defined */
char *ImageDirectoryNames[] = {
        "EXPORT", "IMPORT", "RESOURCE", "EXCEPTION", "SECURITY", "BASERELOC",
        "DEBUG", "COPYRIGHT", "GLOBALPTR", "TLS", "LOAD_CONFIG" };

#define NUMBER_IMAGE_DIRECTORY_ENTRYS (sizeof(ImageDirectoryNames)/sizeof(char *))

/*
 * Dump the IMAGE_OPTIONAL_HEADER from a PE file
 */
void DumpOptionalHeader(PIMAGE_OPTIONAL_HEADER optionalHeader)
{
        UINT width = 30;
        char *s;
        UINT i;

        logstr(LF_LOGPE,"Optional Header\n");

        logstr(LF_LOGPE,"  %-*s%04X\n", width, "Magic", 
		optionalHeader->Magic);
        logstr(LF_LOGPE,"  %-*s%u.%02u\n", width, "linker version",
                optionalHeader->MajorLinkerVersion,
                optionalHeader->MinorLinkerVersion);

        logstr(LF_LOGPE,"  %-*s%X\n", width, "size of code", optionalHeader->SizeOfCode);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "size of initialized data",
                optionalHeader->SizeOfInitializedData);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "size of uninitialized data",
                optionalHeader->SizeOfUninitializedData);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "entrypoint RVA",
                optionalHeader->AddressOfEntryPoint);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "base of code", 
		optionalHeader->BaseOfCode);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "base of data", 
		optionalHeader->BaseOfData);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "image base", 
		optionalHeader->ImageBase);

        logstr(LF_LOGPE,"  %-*s%X\n", width, "section align",
                optionalHeader->SectionAlignment);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "file align", 
		optionalHeader->FileAlignment);
        logstr(LF_LOGPE,"  %-*s%u.%02u\n", width, "required OS version",
                optionalHeader->MajorOperatingSystemVersion,
                optionalHeader->MinorOperatingSystemVersion);
        logstr(LF_LOGPE,"  %-*s%u.%02u\n", width, "image version",
                optionalHeader->MajorImageVersion,
                optionalHeader->MinorImageVersion);
        logstr(LF_LOGPE,"  %-*s%u.%02u\n", width, "subsystem version",
                optionalHeader->MajorSubsystemVersion,
                optionalHeader->MinorSubsystemVersion);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "Reserved1", 
		optionalHeader->Reserved1);


        logstr(LF_LOGPE,"  %-*s%X\n", width, "size of image", 
		optionalHeader->SizeOfImage);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "size of headers",
                optionalHeader->SizeOfHeaders);

        switch( optionalHeader->Subsystem )
        {
                case IMAGE_SUBSYSTEM_NATIVE: s = "Native"; break;
                case IMAGE_SUBSYSTEM_WINDOWS_GUI: s = "Windows GUI"; break;
                case IMAGE_SUBSYSTEM_WINDOWS_CUI: s = "Windows character"; break;
                case IMAGE_SUBSYSTEM_OS2_CUI: s = "OS/2 character"; break;
                case IMAGE_SUBSYSTEM_POSIX_CUI: s = "Posix character"; break;
                default: s = "unknown";
        }
        logstr(LF_LOGPE,"  %-*s%04X (%s)\n", width, "Subsystem",
                        optionalHeader->Subsystem, s);

        logstr(LF_LOGPE,"  %-*s%04X\n", width, "DLL flags",
                        optionalHeader->DllCharacteristics);
        for ( i=0; i < NUMBER_DLL_CHARACTERISTICS; i++ )
        {
                if ( optionalHeader->DllCharacteristics &
                         DllCharacteristics[i].flag )
                        logstr(LF_LOGPE,"  %s\n", 
				DllCharacteristics[i].name );
        }
        if ( optionalHeader->DllCharacteristics )
                logstr(LF_LOGPE,"\n");

        logstr(LF_LOGPE,"  %-*s%X\n", width, "stack reserve size",
                optionalHeader->SizeOfStackReserve);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "stack commit size",
                optionalHeader->SizeOfStackCommit);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "heap reserve size",
                optionalHeader->SizeOfHeapReserve);
        logstr(LF_LOGPE,"  %-*s%X\n", width, "heap commit size",
                optionalHeader->SizeOfHeapCommit);

        logstr(LF_LOGPE,"  %-*s%08X\n", width, "loader flags",
                optionalHeader->LoaderFlags);

        for ( i=0; i < NUMBER_LOADER_FLAGS; i++ )
        {
                if ( optionalHeader->LoaderFlags &
                         LoaderFlags[i].flag )
                        logstr(LF_LOGPE,"  %s", LoaderFlags[i].name );
        }

        if ( optionalHeader->LoaderFlags )
                logstr(LF_LOGPE,"\n");

        logstr(LF_LOGPE,"  %-*s%X\n", width, "RVAs & sizes",
                optionalHeader->NumberOfRvaAndSizes);

        logstr(LF_LOGPE,"Data Directory\n");
        for ( i=0; i < optionalHeader->NumberOfRvaAndSizes; i++)
        {
                logstr(LF_LOGPE,"  %-12s rva: %08X  size: %08X\n",
                        (i >= NUMBER_IMAGE_DIRECTORY_ENTRYS)
                                ? "unused" : ImageDirectoryNames[i],
                        optionalHeader->DataDirectory[i].VirtualAddress,
                        optionalHeader->DataDirectory[i].Size);
        }
}

/* Bitfield values and names for the IMAGE_SECTION_HEADER flags */
DWORD_FLAG_DESCRIPTIONS SectionCharacteristics[] = 
{
{ IMAGE_SCN_CNT_CODE, "CODE" },
{ IMAGE_SCN_CNT_INITIALIZED_DATA, "INITIALIZED_DATA" },
{ IMAGE_SCN_CNT_UNINITIALIZED_DATA, "UNINITIALIZED_DATA" },
{ IMAGE_SCN_LNK_INFO, "LNK_INFO" },
{ IMAGE_SCN_LNK_OVERLAY, "LNK_OVERLAY" },
{ IMAGE_SCN_LNK_REMOVE, "LNK_REMOVE" },
{ IMAGE_SCN_LNK_COMDAT, "LNK_COMDAT" },
{ IMAGE_SCN_MEM_DISCARDABLE, "MEM_DISCARDABLE" },
{ IMAGE_SCN_MEM_NOT_CACHED, "MEM_NOT_CACHED" },
{ IMAGE_SCN_MEM_NOT_PAGED, "MEM_NOT_PAGED" },
{ IMAGE_SCN_MEM_SHARED, "MEM_SHARED" },
{ IMAGE_SCN_MEM_EXECUTE, "MEM_EXECUTE" },
{ IMAGE_SCN_MEM_READ, "MEM_READ" },
{ IMAGE_SCN_MEM_WRITE, "MEM_WRITE" },
};

#define NUMBER_SECTION_CHARACTERISTICS  (sizeof(SectionCharacteristics) / sizeof(DWORD_FLAG_DESCRIPTIONS))

/*
** Dump the section table from a PE file or an OBJ
*/

#if SEVERE
void 
DumpSectionTable(void *LoadAddress,PIMAGE_SECTION_HEADER section, unsigned cSections)
{
	unsigned j;

	logstr(LF_LOGPE,"Section Table %x %d\n",LoadAddress,cSections);
	
	logstr(LF_LOGPE,"  %02X %-8.8s  %s: %08X  VirtAddr:  %08X\n",
			cSections, section->Name, "VirtSize",
			section->Misc.VirtualSize, section->VirtualAddress);
	logstr(LF_LOGPE,"    raw data offs:   %08X  raw data size: %08X\n",
			section->PointerToRawData, section->SizeOfRawData );
	logstr(LF_LOGPE,"    relocation offs: %08X  relocations:   %08X\n",
			section->PointerToRelocations, section->NumberOfRelocations);
	logstr(LF_LOGPE,"    line # offs:     %08X  line #'s:      %08X\n",
			section->PointerToLinenumbers, section->NumberOfLinenumbers );
	logstr(LF_LOGPE,"    characteristics: %08X\n", section->Characteristics);

	for ( j=0; j < NUMBER_SECTION_CHARACTERISTICS; j++ )
	{
		if ( section->Characteristics & 
			SectionCharacteristics[j].flag )
			logstr(LF_LOGPE,"      %s\n", 
				SectionCharacteristics[j].name );
	}
}
#endif

/* */
/* Dump the imports table (the .idata section) of a PE file */
/* */
int LoadImportsSection(char *BaseName,void * BaseAddress, PIMAGE_NT_HEADERS pNTHeader)
{
	PIMAGE_IMPORT_DESCRIPTOR importDesc;
	PIMAGE_THUNK_DATA thunk;
	PIMAGE_IMPORT_BY_NAME pOrdinalName;
        IMAGE_DATA_DIRECTORY    dir;
	char *libname;
	DWORD faddr;
	DWORD *ip;
	char *ModuleName = BaseName;
		
	dir=pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	importDesc = MAKEPTR((PIMAGE_IMPORT_DESCRIPTOR),
		BaseAddress,
		dir.VirtualAddress);

 	logstr(LF_LOGPE,"  DLL Imports Table: %s\n", ModuleName);

	while ( 1 )
	{
		HINSTANCE     hInst;
		HMODULE   hModule;
		MODULEINFO *modinfo;

		/* See if we've reached an empty IMAGE_IMPORT_DESCRIPTOR */
		if ( (importDesc->TimeDateStamp==0 ) && (importDesc->Name==0) )
			break;

		libname = (PBYTE)(importDesc->Name) + (int)BaseAddress;

#if VERBOSE
		logstr(LF_LOGPE,"  Hint/Name Table: %08X\n", 
			importDesc->u.Characteristics);
 		logstr(LF_LOGPE,"  TimeDateStamp:   %08X\n", 
			importDesc->TimeDateStamp);
 		logstr(LF_LOGPE,"  ForwarderChain:  %08X\n", 
			importDesc->ForwarderChain);
 		logstr(LF_LOGPE,"  First thunk RVA: %08X\n", 
			importDesc->FirstThunk);
#endif
	
		logstr(LF_LOGPE,"  Library: %s\n",libname); 

		/* these are the equivalents... */

		hInst = LoadLibrary(libname);

	        if (hInst == 0) {
		   char buffer[256];
		   strncpy(buffer,libname,200);
		   strcat(buffer," File not found.");

    		   MessageBox((HWND)NULL,buffer,"LoadPE error",MB_OK);
		   logstr(LF_LOGPE,"LoadLibrary failed: %s\n",libname); 
		   return 0;
		}

		hModule = GetModuleFromInstance(hInst);

		if(hModule == 0) {
		   logstr(LF_LOGPE,"LoadLibrary failed: %s\n",libname); 
		   return 0;
		}

		modinfo = GETMODULEINFO(hModule);

		if(modinfo == 0) {
		   logstr(LF_LOGPE,"LoadLibrary failed: %s\n",libname); 
		   return 0;
		}

		logstr(LF_LOGPE,"RESOLVE exports from %s\n",libname); 

		thunk = MAKEPTR((PIMAGE_THUNK_DATA),BaseAddress,importDesc->u.Characteristics);
		ip    = MAKEPTR((DWORD *),BaseAddress,importDesc->FirstThunk);


		while (1) {
			if (thunk->u1.AddressOfData == 0)
			   break;

			pOrdinalName = thunk->u1.AddressOfData;

			thunk++;	

			if((int) pOrdinalName & 0x80000000) {
			     int ordinal = (int)pOrdinalName & 0x7fffffff;

			     faddr = PEGetProcAddress(modinfo,ordinal,0);

			     logstr(LF_LOGPE,
				"    import ORDINAL %p %s\t@%p %s\n",
				(void *) ordinal,libname,ip, 
				faddr?"":"TRAP NO ELEMENT");

			      if (faddr == 0) {
				faddr = (DWORD) WinStubAPI(libname,
						"ORDINAL",
						ordinal);
			      }
			     *ip = faddr;

			} else {

			     pOrdinalName = (PIMAGE_IMPORT_BY_NAME) 
				     ((PBYTE) pOrdinalName + (int)BaseAddress);

			     faddr = PEGetProcAddress(modinfo,
					pOrdinalName->Hint,pOrdinalName->Name);

			     logstr(LF_LOGPE,
				"    import %-24s %s\t@%p %4d %s\n",
				pOrdinalName->Name,libname,ip, 
				pOrdinalName->Hint,faddr?"":"TRAP");

			      if (faddr == 0) {
				faddr = (DWORD) WinStubAPI(libname,
						pOrdinalName->Name,
						pOrdinalName->Hint);
			      }
			     *ip = faddr;
			}
			ip++;
		}

		importDesc++;	/* advance to next IMAGE_IMPORT_DESCRIPTOR */
	}
	return 1;
}

void 
LoadExportsTable(MODULEINFO *modinfo, PIMAGE_NT_HEADERS pNTHeader)
{
	PIMAGE_EXPORT_DIRECTORY exportDir;
	DWORD i;
	PDWORD functions;
	PWORD ordinals;
	PSTR *name;
	LPSTR fname;
        IMAGE_DATA_DIRECTORY    dir;
        char *ModuleName = modinfo->lpFileName;

	/* get the image_data_directory for exported symbols */
	dir = pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	/* convert to pointer */
	exportDir = MAKEPTR((PIMAGE_EXPORT_DIRECTORY),
		modinfo->lpNTBaseAddress,
		dir.VirtualAddress);

	/* any functions at all? */
	if (exportDir->NumberOfFunctions == 0)
	   return ;

	logstr(LF_LOGPE,"Exports of %s\n",ModuleName);

	/* create array of function addresses */
	functions = MAKEPTR((PDWORD),modinfo->lpNTBaseAddress,
		exportDir->AddressOfFunctions);

	/* create array of function ordinals */
	ordinals =  MAKEPTR((PWORD),modinfo->lpNTBaseAddress,
		exportDir->AddressOfNameOrdinals);

	/* create array of RVA's to function names */
	name =      MAKEPTR((PSTR *),modinfo->lpNTBaseAddress,
		exportDir->AddressOfNames);
	
	/* resort to straight line linear search */
	/* should/could we ignore comparing strings, until we */
	/* are in range? */
	for ( i=0; i < exportDir->NumberOfNames; i++ )
	{
		/* get the pointer to the name */
		fname = MAKEPTR((LPSTR),modinfo->lpNTBaseAddress,*name);

	        logstr(LF_LOGPE,"export %x %s\n",
			*ordinals,fname);

		/* bump all the pointers */
		functions++;	/* function address */
		ordinals++;	/* function ordinal */
		name++;		/* function name */
	}

	/* the name is not in the module... */
	return ;
}

extern WORD AssignSelector(LPBYTE , WORD , BYTE , DWORD );

void
invoke_binary32(int * fp)
{
	FARPROC entry;
	entry = (FARPROC) fp;

	entry();
}

int
ExecEntryPoint(MODULEINFO *modinfo)
{
	void *BaseAddress; 
	PIMAGE_NT_HEADERS pNTHeader;
	int *fp;
	WORD	    wSel;
	HGLOBAL       hMem;
	THDB *initthread;
	PIMAGE_FILE_HEADER pImageFileHeader;
	FARPROC	    entry;
	char *lpszModuleName = modinfo->lpFileName;

	BaseAddress = modinfo->lpNTBaseAddress;
	pNTHeader   = modinfo->lpNTHeader;

	fp = MAKEPTR((int *), BaseAddress, 
			pNTHeader->OptionalHeader.AddressOfEntryPoint);

	if(fp == 0)
		return (int)modinfo->hInst;

	/* allocate a block for this one, and execute it */
	hMem = GlobalAlloc(0,sizeof(THDB));
	initthread = (THDB *) GlobalLock(hMem);

	wSel = AssignSelector((LPBYTE) initthread, 0, TRANSFER_DATA32, sizeof(THDB));

	logstr(LF_LOGPE,"invoke_binary32: %s entrypoint=%p\n", 
		lpszModuleName,fp);

	pImageFileHeader = &pNTHeader->FileHeader;

	modinfo->exit_code = 0;

        if ( pImageFileHeader->Characteristics & IMAGE_FILE_DLL) {
		if(setjmp(modinfo->env) == 0) {

			__asm__ __volatile__("movw %w0,%%fs\t\n"::"r" (wSel));
			entry = (FARPROC) fp;
			entry(modinfo->hInst,DLL_PROCESS_ATTACH,0);
			longjmp(modinfo->env,1);
		} 
	} else 
	{
		if(setjmp(modinfo->env) == 0) {

			__asm__ __volatile__("movw %w0,%%fs\t\n"::"r" (wSel));
			invoke_binary32(fp);
			longjmp(modinfo->env,1);
		} 
	}

	logstr(LF_LOGPE,"invoke_binary32: %s entrypoint=%p returns %d\n",
		lpszModuleName,fp,modinfo->exit_code);

	return (int)modinfo->hInst;
}

long
LibMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	logstr(LF_LOGPE,"LibMain(HINSTANCE %x, LPSTR %p, int %d)\n",
		hInst,hPrevInst,lpCmdLine,nCmdShow);

	return 0;
}

void DEBUG_LoadEntryPoints(void)
{
	logstr(LF_LOGPE,"LoadEntryPoints\n");
}

