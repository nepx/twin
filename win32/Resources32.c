
#include <stdio.h>

#include "windows.h"

#include "peexe.h"

#include "Segment.h"

#include "Log.h"
#include "ctype.h"

#include "setjmp.h"
#include "BinTypes.h"
#include "Resources.h"
#include "Module.h"
#include "Endian.h"
#include "Strings32.h"
#include "LoadPE.h"
#include "Kernel.h"
#include "peexe.h"

extern int LoadBin32MenuA(HINSTANCE hInstance, LPCSTR , HMENU *);
extern void CleanupDialog(LPBYTE);

extern FARPROC PEGetProcAddressByName(LPMODULEINFO,LPCSTR);
extern FARPROC PEGetProcAddressByOrdinal(LPMODULEINFO, int,LPCSTR);
extern FARPROC PEGetBinAddrByOrdinal(LPMODULEINFO,int,LPCSTR);

HMODULE GetModuleFromInstance(HINSTANCE);	

HMENU   TWIN_LoadMenu32(HINSTANCE , LPCTSTR ,int );
HICON   TWIN_LoadIcon32(HINSTANCE , LPCSTR ,int );
HBITMAP TWIN_LoadBitmap32(HINSTANCE , LPCSTR ,int );
int     TWIN_LoadString32(HINSTANCE , UINT ,LPSTR ,int ,int );
HCURSOR TWIN_LoadCursor32(HINSTANCE , LPCSTR ,int );
HACCEL	TWIN_LoadAccelerators32(HINSTANCE , LPCSTR , int );
HRSRC   TWIN_FindResource32(HINSTANCE ,LPCSTR ,LPCSTR ,int );
HGLOBAL TWIN_LoadResource32(HINSTANCE ,HRSRC ,int );
DWORD   TWIN_SizeofResource32(HINSTANCE , HRSRC,int );
FARPROC	TWIN_GetProcAddress32(HINSTANCE , LPCSTR , int );
HWND    TWIN_CreateDialogParam32(HINSTANCE ,LPCSTR ,HWND ,DLGPROC ,LPARAM,int);
int     TWIN_DialogBoxParam32(HINSTANCE ,LPCSTR ,HWND ,DLGPROC ,LPARAM, int);

void ShowBitmapInfoHeader(BITMAPINFOHEADER *);

#ifdef TWIN_BIN32
TWINRT DefaultTwinRT32 = {
	TWIN_LoadMenu32,
	TWIN_LoadIcon32,
	TWIN_LoadString32,
	TWIN_LoadCursor32,
	TWIN_LoadBitmap32,
	TWIN_LoadAccelerators32,
	TWIN_FindResource32,	
	TWIN_LoadResource32,
	TWIN_SizeofResource32,
	TWIN_CreateDialogParam32,
	TWIN_DialogBoxParam32,
	TWIN_GetProcAddress32,
};

FARPROC
PEGetBinAddrByOrdinal(MODULEINFO *mp,int ordinal,LPCSTR lpName)
{
        IMAGE_DATA_DIRECTORY    dir;
	PIMAGE_EXPORT_DIRECTORY exportDir;
	PIMAGE_NT_HEADERS pNTHeader;
	PDWORD functions;
	FARPROC  faddress = 0;
	PWORD ordinals;
	LPSTR fname;
	PSTR *name;
	int   i;

	/* get the image_data_directory for exported symbols */
	pNTHeader = mp->lpNTHeader;

	if(pNTHeader == 0)
	  return 0;

	dir = pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	/* convert to pointer */
	exportDir = MAKEPTR((PIMAGE_EXPORT_DIRECTORY),
		mp->lpNTBaseAddress,
		dir.VirtualAddress);

	/* any functions at all? */
	if (exportDir->NumberOfFunctions == 0) {
	   return (DWORD) 0;
	}

	/* create array of function addresses */
	functions = MAKEPTR((PDWORD),mp->lpNTBaseAddress,
		exportDir->AddressOfFunctions);

	/* create array of function ordinals */
	ordinals =  MAKEPTR((PWORD),mp->lpNTBaseAddress,
		exportDir->AddressOfNameOrdinals);

	/* create array of RVA's to function names */
	name =      MAKEPTR((PSTR *),mp->lpNTBaseAddress,
		exportDir->AddressOfNames);
	
	/* before we do linear search through name table */
	/* use quick lookup first, using the hint...  */
	/* if we have valid hint, get pointer to underlying name */

	if (ordinal > 0 && ordinal < exportDir->Base + exportDir->NumberOfNames) {
		ordinal -= exportDir->Base;

		fname = MAKEPTR((LPSTR),mp->lpNTBaseAddress,name[ordinal]);
		/* have we matched the strings? */
		if (lpName == 0 || strcasecmp((char *)fname,lpName) == 0) {
			faddress =  MAKEPTR((FARPROC),mp->lpNTBaseAddress,
				functions[ordinal]);
	   	        return faddress;
		}
			
		/* no, so do linear search, bummer its wrong??? */
		logstr(LF_LOGPE,"requested f=%s ord=%x found %s ord=%x\n",
			lpName, ordinal,
			MAKEPTR((LPSTR),mp->lpNTBaseAddress,name[ordinal]),
			ordinals[ordinal]);
	}

	/* resort to straight line linear search */
	/* should/could we ignore comparing strings, until we */
	/* are in range? */

	for ( i=0;lpName && i < exportDir->NumberOfNames; i++ ) {

		/* get the pointer to the name */
		fname = MAKEPTR((LPSTR),mp->lpNTBaseAddress,*name);

		/* does this one match what we want... */
		if (strncasecmp((char *)fname,lpName,strlen(lpName)) == 0) {
			faddress = MAKEPTR((FARPROC),mp->lpNTBaseAddress,
				*functions);
		   	break;
		}

		/* bump all the pointers */
		functions++;
		ordinals++;
		name++;		
	}

	return faddress;
}

FARPROC
PEGetProcAddress(MODULEINFO   *modinfo, int ordinal,LPCSTR lpName)
{
	FARPROC fp;

	/* check if this is a native library/ordinal lookup */
	fp = PEGetProcAddressByOrdinal(modinfo, ordinal,lpName);
	if(fp) return fp;

	return PEGetBinAddrByOrdinal(modinfo,ordinal,lpName);
}



FARPROC
TWIN_GetProcAddress32(HINSTANCE hInst,LPCSTR lpstr,int flag)
{
	HMODULE      hModule;
	MODULEINFO   *modinfo;
	FARPROC	faddress = 0;

	hModule = GetModuleFromInstance(hInst);
	modinfo = GETMODULEINFO(hModule);

	logstr(LF_LOGPE,"TWIN_GetProcAddress32(%x,%p,%d)\n",
		hInst,lpstr,flag);

	// if lpstr = #ordinal, use PEGetProcAddressByOrdinal
	if(HIWORD(lpstr) == 0) {
		faddress = PEGetBinAddrByOrdinal(modinfo,(int)lpstr,0);
		RELEASEMODULEINFO(modinfo);
		return (FARPROC) faddress;
	}

	faddress = PEGetBinAddrByOrdinal(modinfo,-1,lpstr);

	RELEASEMODULEINFO(modinfo);

	return faddress;
}

PIMAGE_RESOURCE_DIRECTORY
FindResourceDirectory( MODULEINFO *modinfo,
	PIMAGE_RESOURCE_DIRECTORY root,
	PIMAGE_RESOURCE_DIRECTORY resdir, DWORD wID,UINT uiFlags)
{
	PIMAGE_RESOURCE_DIRECTORY       subdir = 0;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY et;
	int i;
	int entries,namelen = 0;
	short szResourceName[256];
	LPWSTR lpwstr;

	et = MAKEPTR((PIMAGE_RESOURCE_DIRECTORY_ENTRY),
		resdir,sizeof(IMAGE_RESOURCE_DIRECTORY));

	if(resdir == (PIMAGE_RESOURCE_DIRECTORY) 0xffff)	
		return (PIMAGE_RESOURCE_DIRECTORY) 0xffff;

	if (HIWORD(wID)) {
	   if(uiFlags == 0) {
		lstrcpyAtoW( (LPWSTR) szResourceName, (LPCSTR) wID );
		lpwstr = szResourceName;
	   } else
		lpwstr = (LPWSTR) wID;

	   /* count of characters in lpwstr */
	   namelen = lstrlenW( lpwstr );
	   entries = resdir->NumberOfNamedEntries;

	   for (i=0;i<entries; i++) {

                PIMAGE_RESOURCE_DIR_STRING_U str =
                	MAKEPTR((PIMAGE_RESOURCE_DIR_STRING_U),
				root,
                        	et[i].u1.s.NameOffset);

                if(namelen != str->Length)
                        continue;

                if(lstrncmpiW( lpwstr, str->NameString, str->Length) == 0) {
                        subdir = MAKEPTR((PIMAGE_RESOURCE_DIRECTORY),
                                root,
                                et[i].u2.s.OffsetToDirectory);
			return subdir;
		}
	   }

	   subdir = MAKEPTR((PIMAGE_RESOURCE_DIRECTORY),
		    root, et[0].u2.s.OffsetToDirectory);
	} else {
	   entries = resdir->NumberOfNamedEntries+resdir->NumberOfIdEntries;
	   for (i=resdir->NumberOfNamedEntries;i<entries;i++) {

	       if(et[i].u1.Name == wID) {
	       	subdir = MAKEPTR((PIMAGE_RESOURCE_DIRECTORY),
		    root,
	    	    et[i].u2.s.OffsetToDirectory);
		return subdir;
	       }
	   }

	   subdir = MAKEPTR((PIMAGE_RESOURCE_DIRECTORY),
		    root, et[0].u2.s.OffsetToDirectory);
	}
	return subdir;
}

PIMAGE_RESOURCE_DIRECTORY
LoadResourceDirectory(MODULEINFO *modinfo)
{
	DWORD BaseAddress; 
	PIMAGE_NT_HEADERS pNTHeader;
        IMAGE_DATA_DIRECTORY    dir;
	PIMAGE_RESOURCE_DIRECTORY resdir;

	if(modinfo->lpNTBaseAddress == 0)
		return (PIMAGE_RESOURCE_DIRECTORY) 0;

	BaseAddress = (DWORD) modinfo->lpNTBaseAddress;
	pNTHeader = modinfo->lpNTHeader;

	dir=pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
	resdir = MAKEPTR((PIMAGE_RESOURCE_DIRECTORY),
		BaseAddress,
		dir.VirtualAddress);

	return resdir;
}

extern LCID GetUserDefaultLCID();

/***************************************************************/

void *
TWIN_LoadBin32MenuIndirect(void *p,HMENU *lphMenu)
{
	HMENU hMenu; 
	WORD   flags;
	WORD   id;
	int    len;
	char   MenuName[64];

	// this is the string data for this resource
	hMenu = CreateMenu();
	if(hMenu == 0 || p == 0)
		return 0;

	*lphMenu = hMenu;
	do {
		id = 0;
		flags = GETWORD(p);
		p += sizeof(SHORT);
	
		if((flags & MF_POPUP) ==0) {
			id    = GETWORD(p);
			p += sizeof(SHORT);
		}

		lstrcpyWtoA(MenuName,p);
		len = strlen(MenuName);
		if(len == 0)
			flags |= MF_SEPARATOR;
		len++;

		p += len*sizeof(SHORT);

		if (flags & MF_POPUP) {
			HMENU hNewMenu;
			p = TWIN_LoadBin32MenuIndirect(p,&hNewMenu);
			AppendMenu(hMenu, flags & ~MF_END, (UINT)hNewMenu,MenuName);
		} else
			AppendMenu(hMenu, flags & ~MF_END, id,MenuName);

	} while (!(flags & MF_END));

	return p;
}

// assuming that lpstr is in WIDE format
// return hMenu in lphMenu, and 1 otherwise 
// return false
BOOL
LoadBin32MenuW(HINSTANCE hInstance, LPWSTR lpstr, HMENU *lphMenu)
{
	LPMODULEINFO modinfo;
	HMODULE     hModule;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	void	*p;
	LCID    lcid = GetUserDefaultLCID();
	WORD	version;
	HMENU   hMenu = 0;
	int     offset;

	logstr(LF_APICALL,"LoadBin32Menu(HINSTANCE %x, LPCSTR %p, HMENU *%p)\n",
		hInstance,lpstr,lphMenu);

	// get the modinfo struct from the hInstance
	hModule = GetModuleFromInstance(hInstance);	
	modinfo = GETMODULEINFO(hModule);

	//if (lpNTModule == 0) {
	 //  hMenu = LoadMenu(hInstance,(LPSTR)lpstr);
	  // *lphMenu = hMenu;
	   //logstr(LF_APIRET,"LoadBin32Menu returns HMENU %x\n",hMenu);
	   //return TRUE;
	//}

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if (rootdir == 0) {
	   logstr(LF_APIFAIL,"LoadBin32Menu returns BOOL %d\n",FALSE);
	   return FALSE;
	}

	// find the MENU type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD)RT_MENU,0);

	// find the WIDE menu we want, given ANSI
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD)lpstr/*MenuNameW*/,0);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	p = MAKEPTR((void *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	version = GETWORD(p);
	p += sizeof(SHORT);	

	switch(version) {
		case 0:
			// pickup offset to menu
			offset = GETWORD(p);
			p += sizeof(SHORT);	

			// now pointing to binary template
			p += offset;

			TWIN_LoadBin32MenuIndirect(p,&hMenu);

			if(lphMenu)
				*lphMenu = hMenu;
			break;				

		case 1:
			break;
	}
	RELEASEMODULEINFO(modinfo);
	logstr(LF_APIRET,"LoadBin32Menu returns BOOL %x\n", hMenu != 0);
	return hMenu != 0;
}

extern HANDLE ResourceAlloc(LPSTR);

HRSRC   
TWIN_FindResource32(HINSTANCE hInstance, LPCSTR lpTableName,LPCSTR lpType, int Style)
{
	LPMODULEINFO modinfo;
	HMODULE     hModule;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	LCID    lcid = GetUserDefaultLCID();

	APISTR((LF_APICALL,"FindResource32(HINSTANCE=%x,LPCSTR=%p,LPCSTR=%p,int=%d)\n",
			hInstance,lpTableName,lpType,Style));

	// get the modinfo struct from the hInstance
	hModule = GetModuleFromInstance(hInstance);	
	modinfo = GETMODULEINFO(hModule);

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if(rootdir == 0) {
	  APISTR((LF_APIRET,"FindResource32 returns 0\n"));
	  return 0;
	}

	// find the type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD) lpType,0);

	if(resdir == 0) {
	  APISTR((LF_APIRET,"FindResource32 returns 0\n"));
	  return 0;
	}

	// find the table 
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD) lpTableName,1);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,0,0);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	APISTR((LF_APIRET,"FindResource32 returns (data=%p) (HRSRC) %p\n",dt));
	return (HRSRC) dt;
}

HGLOBAL 
TWIN_LoadResource32(HINSTANCE hInstance, HRSRC lpresource,int Style)
{
	HANDLE 	hMem;
	LPMODULEINFO modinfo;
	HMODULE     hModule;
	PIMAGE_RESOURCE_DATA_ENTRY dt = (PIMAGE_RESOURCE_DATA_ENTRY) lpresource;
	char 	*p;

	APISTR((LF_APICALL,"LoadResource32(HINSTANCE=%x,LPCSTR=%p,int=%d)\n",
		hInstance,(void *)lpresource,Style));

	// get the modinfo struct from the hInstance
	hModule = GetModuleFromInstance(hInstance);	
	modinfo = GETMODULEINFO(hModule);

	p = MAKEPTR((char *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	// this is the resource
	hMem = ResourceAlloc((LPSTR)p);

	RELEASEMODULEINFO(modinfo);

	APISTR((LF_APIRET,"LoadResource32 returns sizeof=%d (%p==%p) %x\n",
		sizeof(IMAGE_RESOURCE_DATA_ENTRY),p, GlobalLock(hMem),hMem));

	return hMem;
}

DWORD   
TWIN_SizeofResource32(HINSTANCE hInstance, HRSRC lpresource,int Style)
{
	PIMAGE_RESOURCE_DATA_ENTRY dt = (PIMAGE_RESOURCE_DATA_ENTRY) lpresource;
 	DWORD dwsize = 0;

	APISTR((LF_APICALL,"SizeofResource32(HINSTANCE=%x,LPCSTR=%p,int=%d)\n",
		hInstance,lpresource,Style));
    	if (dt)
         	dwsize = dt->Size;
	
	APISTR((LF_APIRET,"SizeofResource32 returns (%p) %x\n",dt,dwsize));
    	return dwsize;
}

HACCEL	
TWIN_LoadAccelerators32(HINSTANCE hInstance, LPCSTR lpTableName,int Style)
{
	HANDLE 	hMem;
	LPMODULEINFO modinfo;
	HMODULE     hModule;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	SHORT	*p;
	LCID    lcid = GetUserDefaultLCID();

	// get the modinfo struct from the hInstance
	hModule = GetModuleFromInstance(hInstance);	
	modinfo = GETMODULEINFO(hModule);

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if(rootdir == 0)
	  return 0;

	// find the ACCELERATOR type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD) RT_ACCELERATOR,0);

	// find the accelerator table 
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD) lpTableName,0);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	// this is the accelerator table for this resource
	p = MAKEPTR((SHORT *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	hMem = ResourceAlloc((LPSTR)p);

	RELEASEMODULEINFO(modinfo);
	return hMem;
}

char *
idtostr(LPCSTR id,char *format)
{
	if(HIWORD(id))
		return (char *)id;
	sprintf(format,"#%X",(int)id);
	return format;	
}

HICON   
TWIN_LoadIcon32(HINSTANCE hInstance, LPCSTR lpIconName,int bUnicode )
{
	char name[32];
	logstr(LF_APISTUB,"LoadIcon32(%x,%s,%d) returns 0\n",
		hInstance,idtostr(lpIconName,name),bUnicode);
	return LoadIcon(0,MAKEINTRESOURCE(530));
}

HCURSOR 
TWIN_LoadCursor32(HINSTANCE hInstance, LPCSTR lpCursorRsrc,int bUnicode)
{
	char name[32];
	logstr(LF_APISTUB,"LoadCursor32(%x,%s,%d)\n",
		hInstance,idtostr(lpCursorRsrc,name),bUnicode);
	return LoadCursor(0,MAKEINTRESOURCE(32514));
}

HMENU
TWIN_LoadMenu32(HINSTANCE hInstance, LPCTSTR lpstr,int Style)
{
	HMENU hMenu;
	short  MenuNameString[64];
	LPWSTR  lpMenuName = (LPWSTR) lpstr;

	if(HIWORD(lpstr)) {
		APISTR((LF_APICALL,"LoadMenuA(HINSTANCE=%x,LPCTSTR=%s)\n",
			hInstance,lpstr));

		/* convert ansi MENU name to  wide MENU name */
		lpMenuName = lstrcpynAtoW(MenuNameString,lpstr,64);
	} else
		APISTR((LF_APICALL,"LoadMenuA(HINSTANCE=%x,LPCTSTR=%p)\n",
			hInstance,lpstr));
	
	if(LoadBin32MenuW(hInstance, lpMenuName, &hMenu)) {
	        APISTR((LF_APIRET,"LoadMenuA: returns HMENU %d\n",hMenu));
		return hMenu;
	}

	APISTR((LF_APIRET,"LoadMenuA: returns HMENU 0\n"));
	return 0;
}

HBITMAP
TWIN_LoadBitmap32(HINSTANCE hInstance, LPCSTR lpBitmapName,int bUnicode)
{
	HBITMAP hBitmap;
	LPMODULEINFO modinfo;
	HMODULE     hModule;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	SHORT	*p;
	LCID    lcid = GetUserDefaultLCID();

	logstr(LF_APICALL,"LoadBitmap32(%x,%p,%d)\n",
		hInstance,lpBitmapName,bUnicode);

	// get the modinfo struct from the hInstance
	hModule = GetModuleFromInstance(hInstance);	
	modinfo = GETMODULEINFO(hModule);

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if(rootdir == 0)
	  return 0;

	// find the ACCELERATOR type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD) RT_BITMAP,0);

	// find the accelerator table 
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD) lpBitmapName,0);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	// this is the bitmapinfoheader for the bitmap
	p = MAKEPTR((SHORT *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	//ShowBitmapInfoHeader((BITMAPINFOHEADER *)p);

	{
		BITMAPINFO *lpbmi;
		HDC 	    hDC;
		int 	    size;
		char       *bits;

		lpbmi = (BITMAPINFO *)p;
	    
		hDC = GetDC(0);

		/* where do the bitmap bitsstart */
		/* TODO maybe no bits? dibsection? */
		size = lpbmi->bmiHeader.biBitCount==0?0:
			(1<<lpbmi->bmiHeader.biBitCount) * sizeof(RGBQUAD);
		size += sizeof(BITMAPINFOHEADER);

		/* if we have them, here they are, but check */
		bits = (char *)lpbmi + size;;

		hBitmap = CreateDIBitmap( hDC, &lpbmi->bmiHeader, CBM_INIT,
			bits, lpbmi, DIB_RGB_COLORS );

		ReleaseDC( 0, hDC );
	}

	RELEASEMODULEINFO(modinfo);

	logstr(LF_APIRET,"LoadBitmap32 returns HBITMAP %x\n",hBitmap);
	return hBitmap;
}
/***************************************************************/

static SHORT *
TWIN_LoadStringPointer(HINSTANCE hInstance, int wID,short *len)
{
	LPMODULEINFO modinfo;
	HMODULE     hModule;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	int major,minor,i;
	SHORT	*p;
	LCID    lcid = GetUserDefaultLCID();

	// get the modinfo struct from the hInstance
	hModule = GetModuleFromInstance(hInstance);	
	modinfo = GETMODULEINFO(hModule);

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if(rootdir == 0) {
	  RELEASEMODULEINFO(modinfo);
	  return 0;
	}

	// which block of strings do we want
	major =  (wID >> 4) + 1;

	// which string of the list of 16 do we want
	minor = wID & 0xf;

	// find the STRING type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD) RT_STRING,0);

	// find the block of the string we want
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD) major,0);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	// this is the string data for this resource
	p = MAKEPTR((SHORT *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	// walk through it for the string index we want
	for(i=0;i<minor;i++) {
		p += *p + 1;
	}

	// return the length of the string, the first byte in the buffer
	*len = *p++;

	RELEASEMODULEINFO(modinfo);
	return p;
}

#ifdef TWIN_BIN32
int
TWIN_LoadStringA(HINSTANCE hInstance, int wID, LPSTR lpstr,int nCount)
{
	SHORT *p;
	SHORT len;

	logstr(LF_APICALL,"LoadStringA(HINSTANCE=%x,id=%x(%x,%x),lpstr=%p,nBytes=%d)\n",
		hInstance,wID,(wID>>4)+1,wID & 0xf,lpstr,nCount);
	
	p = TWIN_LoadStringPointer(hInstance, wID,&len);

	if(p) {
		memset(lpstr,0,nCount);
		// copy the actual data, AND a terminator
		lstrcpynWtoA(lpstr,p,min(nCount,len+1));
	} else
		*lpstr = 0;

	logstr(LF_APIRET,"LoadStringA returns int %d\n",lstrlen(lpstr));

	return lstrlen(lpstr);
}

int
TWIN_LoadStringW(HINSTANCE hInstance, int wID, LPWSTR lpstr,int nCount)
{
	SHORT *p;
	SHORT len;

	*lpstr = 0;

	logstr(LF_APICALL,"LoadStringW(HINSTANCE=%x,id=%x(%x,%x),lpstr=%p,nBytes=%d)\n",
		hInstance,wID,(wID>>4)+1,wID & 0xf,lpstr,nCount);
	
	p = TWIN_LoadStringPointer(hInstance, wID,&len);

	if(p) {
		memset(lpstr,0,nCount);
		lstrcpynW(lpstr,p,min(nCount,len));
	} else
		*lpstr = 0;

	logstr(LF_APIRET,"LoadStringW returns int %d\n",lstrlenW(lpstr));
	return lstrlenW(lpstr);
}
#endif

int
TWIN_LoadString32(HINSTANCE hInstance, UINT wID,LPSTR buffer,int len,int Style)
{
	if(Style)
		return TWIN_LoadStringW(hInstance,wID,(LPWSTR)buffer,len);
	return TWIN_LoadStringA(hInstance,wID,buffer,len);
}

static LPSTR std_controls[] = {
        "BUTTON",
        "EDIT",
        "STATIC",
        "LISTBOX",
        "SCROLLBAR",
        "COMBOBOX"
}; 

static LPBYTE
ConvertDlgTemplate32(HINSTANCE hInst, void *lpdt,BOOL bUniCode)
{
        DIALOGDATA *lpdd;
        CONTROLDATA *lpcd;
        int i,len;
	unsigned short *p;
	unsigned short rid;

	p = (short *) lpdt;

        /* Begin to parse DLGTEMPLATE data into DIALOGDATA structure */

        if (!(lpdd = (DIALOGDATA *)WINMALLOC(sizeof(DIALOGDATA),0,0)))
                return NULL;
        memset(lpdd,0,sizeof(DIALOGDATA));

        lpdd->lStyle = GETDWORD(p); p++;p++;
	lpdd->dwExtStyle = GETDWORD(p); p++;p++;

        lpdd->bNumberOfItems = GETWORD(p); p++;

        lpdd->x = GETWORD(p); p++;
        lpdd->y = GETWORD(p); p++;

        lpdd->cx = GETWORD(p);p++;
	lpdd->cy = GETWORD(p);p++;
	lpdd->szMenuName = 0;
	lpdd->szClassName = 0;
	lpdd->szCaption = 0;
	lpdd->szFaceName = 0;

        /*  3 variable length strings follow DLGTEMPLATE */
	/*  get the first field value, don't move pointer yet */

	/* get id for menu */
	rid = GETWORD(p);

        /* first array is menu data, if its 0x0000 then no menu */
        if (rid) {
                if (rid == 0xffff)
                /*  if rid = 0xffff, then we have ordinal of menu */
                {
			char menuname[256];
			p++;
			rid = GETWORD(p);p++;
                        LoadString(hInst, rid, menuname, 256);
                        lpdd->szMenuName = WinStrdup(menuname);
                }
                else
                /*  we have null terminated unicode string of menu resource */
                {
			if(bUniCode) {
				len = lstrlenW(p);
				lpdd->szMenuName = WINMALLOC(len,0,0);
				WideCharToMultiByte(0,0,p,0,lpdd->szMenuName,len,0,0);
			} else {
				len = lstrlen((LPSTR)p);
				lpdd->szMenuName = (LPSTR) WinStrdup((LPSTR)p);
			}

			p += len;
                }
	}
	p++; 	/* skip over null */
       
	/* get id for class */
	rid = GETWORD(p);

        /* Second array is a class array that identifies the window 
	 * class of the control, if it is 0x0000 then it uses the 
	 * default dialog box class
	 */

        if (rid) {
                /* we have null terminated unicode string that 
		 * specifies the name of a registered window class
		 */
		if(bUniCode) {
			len = lstrlenW(p);
			lpdd->szClassName = WINMALLOC(len,0,0);
			WideCharToMultiByte(0,0,p,0,lpdd->szClassName,len,0,0);
		} else {
			len = lstrlen((LPSTR)p);
			lpdd->szClassName = (LPSTR) WinStrdup((LPSTR)p);
		}
		p += len;
        } 
	p++;		/* skip over the null */
	
        /* Third array is the title array, if it is 0x00 then there is no
         *  title.  
 	 */
	rid = GETWORD(p);

        if (rid) {
                /* we have a null terminated unicode string that 
	 	 * specifies the title of the dialog 
		 */
		if(bUniCode) {
			len = lstrlenW(p);
			lpdd->szCaption = WINMALLOC(len,0,0);
			WideCharToMultiByte(0,0,p,0,lpdd->szCaption,len,0,0);
		} else {
			len = lstrlen((LPSTR)p);
			lpdd->szCaption = (LPSTR) WinStrdup((LPSTR)p);
		}
		p += len;
        }
	p++;		/* skip over the null */

	/* this next item is only present if the DS_SETFONT bit is set */

        if (lpdd->lStyle & DS_SETFONT)
        /*  The 16 bit point size value and typeface array follow the title
            array, but only if the style member specifies DS_SETFONT */
        {
                lpdd->wPointSize = GETWORD(p);p++;
               
		if(bUniCode) {
			len = lstrlenW(p);
			lpdd->szFaceName = WINMALLOC(len,0,0);
			WideCharToMultiByte(0,0,p,0,lpdd->szFaceName,len,0,0);
		} else {
			len = lstrlen((LPSTR)p);
			lpdd->szFaceName = (LPSTR) WinStrdup((LPSTR)p);
		}
		p += len;	
		p++;
        }

        if (lpdd->bNumberOfItems == 0)
        /*  if there are no controls then we're done */
        {
                lpdd->controlinfo = NULL;
                return (LPBYTE)lpdd;
        }

        lpcd = (CONTROLDATA *)WINMALLOC(
		sizeof(CONTROLDATA) * lpdd->bNumberOfItems,0,0);
        if (!lpcd)
                return NULL;
        memset(lpcd,0,sizeof(CONTROLDATA) * lpdd->bNumberOfItems);

        for (i = 0; i < lpdd->bNumberOfItems; i++)
        {
		p = (unsigned short *)ALIGN_32BIT(p);

                /* create a CONTROLDATA structure for each DLGITEMTEMPLATE */
                lpcd[i].lStyle     = GETDWORD(p); p++;p++;
                lpcd[i].dwExtStyle = GETDWORD(p); p++;p++;

                lpcd[i].x = GETWORD(p); p++;
                lpcd[i].y = GETWORD(p); p++;
                lpcd[i].cx = GETWORD(p); p++;
                lpcd[i].cy = GETWORD(p); p++;
                lpcd[i].dwID = GETWORD(p); p++;

		if (GETWORD(p) == 0xffff) {
			p++;
			rid = GETWORD(p); 
			lpcd[i].szClass = (LPSTR)WinStrdup(
				std_controls[rid & ~0x0080]);
		} else {
			if(bUniCode) {
				len = lstrlenW(p);
				lpcd[i].szClass = WINMALLOC(len,0,0);
				WideCharToMultiByte(0,0,p,0,lpcd[i].szClass,len,0,0);
			} else {
				len = lstrlen((LPSTR)p);
			        lpcd[i].szClass = (LPSTR)p;
			}
			p += len;
		}
		p++;

                if (GETWORD(p) == 0xffff) {
			p++;
                        lpcd[i].szText = (LPSTR)(int) GETWORD(p); 
                } else {
			if(bUniCode) {
				len = lstrlenW(p);
				lpcd[i].szText = WINMALLOC(len,0,0);
				WideCharToMultiByte(0,0,p,0,lpcd[i].szText,len,0,0);
			} else {
				len = lstrlen((LPSTR)p);
				lpcd[i].szText = (LPSTR) p;
			}
			p += len;
                }
		p++;

		rid = GETWORD(p); 
                if (rid)
                {
			p += rid/2;
                }
		p++;		/* over the null */

        }

        lpdd->controlinfo = lpcd;
        return (LPBYTE)lpdd;
}

#endif

HWND InternalCreateDialog(HINSTANCE,LPSTR,HWND,DLGPROC,LPARAM);

HWND 
TWIN_CreateDialogParamA(HINSTANCE hInst,LPCSTR lpDialog,HWND hWnd,DLGPROC lpFunc,LPARAM lParam)
{
	HWND hDlg = 0;
	HMODULE  hModule;
	LPMODULEINFO modinfo;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	LCID    lcid = GetUserDefaultLCID();
	void   *lpDlgTemplate;
	DIALOGDATA *lpdd;
	
	APISTR((LF_APICALL,"CreateDialogParamA(%x,%p,%x,%p,%x)\n",
		hInst,lpDialog,hWnd,lpFunc,lParam));

	hModule = GetModuleFromInstance(hInst);	
	modinfo = GETMODULEINFO(hModule);
	
	//if (lpNTModule == 0) {
	 //  hDlg = CreateDialogParam(hInst,lpDialog,hWnd,lpFunc,lParam);
	  // APISTR((LF_APIRET,"CreateDialogParamA returns HWND %x\n",hDlg));
	   //return hDlg;
	//}

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if (rootdir == 0) {
	   APISTR((LF_APIRET,"CreateDialogParamA returns HWND %x\n",hDlg));
	   RELEASEMODULEINFO(modinfo);
	   return hDlg;
	}

	// find the DIALOG type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD)RT_DIALOG,0);

	// find the ansi dialog we want
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD)lpDialog,0);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	lpDlgTemplate = MAKEPTR((void *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	lpdd = (DIALOGDATA *)ConvertDlgTemplate32(hInst, lpDlgTemplate, TRUE);
	hDlg = InternalCreateDialog(hInst,(LPSTR)lpdd,hWnd,lpFunc,lParam);

	CleanupDialog((LPBYTE)lpdd);
	WinFree(lpdd);

	RELEASEMODULEINFO(modinfo);

	APISTR((LF_APIRET,"CreateDialogParamA returns HWND %x\n",hDlg));
	return hDlg;
}

HWND 
TWIN_CreateDialogParamW(HINSTANCE hInst,LPCSTR lpDialog,HWND hWnd,DLGPROC lpFunc,LPARAM lParam)
{
	HWND hDlg = 0;
	HMODULE  hModule;
	LPMODULEINFO modinfo;
	PIMAGE_RESOURCE_DIRECTORY resdir,rootdir;
	PIMAGE_RESOURCE_DATA_ENTRY dt;
	LCID    lcid = GetUserDefaultLCID();
	void   *lpDlgTemplate;
	DIALOGDATA *lpdd;

	APISTR((LF_APICALL,"CreateDialogParamW(%x,%p,%x,%p,%x)\n",
		hInst,lpDialog,hWnd,lpFunc,lParam));

	hModule = GetModuleFromInstance(hInst);	
	modinfo = GETMODULEINFO(hModule);
	
	//if (lpNTModule == 0) {
	 //  hDlg = CreateDialogParam(hInst,lpDialog,hWnd,lpFunc,lParam);
	  // APISTR((LF_APIRET,"CreateDialogParamW returns HWND %x\n",hDlg));
	   //return hDlg;
	//}

	// load the root directory of resources
	rootdir = LoadResourceDirectory(modinfo);

	if (rootdir == 0) {
	   RELEASEMODULEINFO(modinfo);
 	   APISTR((LF_APIRET,"CreateDialogParamW returns HWND %x\n",hDlg));
	   return hDlg;
	}

	// find the DIALOG type resource
	resdir = FindResourceDirectory(modinfo,rootdir,rootdir,(DWORD)RT_DIALOG,0);

	// find the WIDE dialog we want
	resdir = FindResourceDirectory(modinfo,rootdir,resdir,(DWORD)lpDialog,1);

	// find the one for our language, or the default
	dt     = (PIMAGE_RESOURCE_DATA_ENTRY) 
		FindResourceDirectory(modinfo,rootdir,resdir,lcid,0);

	lpDlgTemplate = MAKEPTR((void *), 
		modinfo->lpNTBaseAddress, dt->OffsetToData);

	lpdd = (DIALOGDATA *)ConvertDlgTemplate32(hInst, lpDlgTemplate, TRUE);
	hDlg = InternalCreateDialog(hInst,(LPSTR)lpdd,hWnd,lpFunc,lParam);

	CleanupDialog((LPBYTE)lpdd);
	WinFree(lpdd);

	RELEASEMODULEINFO(modinfo);
	APISTR((LF_APIRET,"CreateDialogParamW returns HWND %x\n",hDlg));
	return hDlg;
}

HWND 
TWIN_CreateDialogParam32(HINSTANCE hInst,LPCSTR lpDialog,HWND hWnd,DLGPROC lpFunc,LPARAM lParam,int bUnicode)
{
	if(bUnicode)
	  return TWIN_CreateDialogParamW(hInst,lpDialog,hWnd,lpFunc,lParam);
	return TWIN_CreateDialogParamA(hInst,lpDialog,hWnd,lpFunc,lParam);
}

int
TWIN_DialogBoxParamA(HINSTANCE hInstance,LPCSTR lpTemplate,HWND hWnd,DLGPROC lpDlgProc,LPARAM dwInitParam)
{
	logstr(LF_APISTUB,"DialogBoxParamA returns 0\n");
	return 0;
}

int
TWIN_DialogBoxParamW(HINSTANCE hInstance,LPCSTR lpTemplate,HWND hWnd,DLGPROC lpDlgProc,LPARAM dwInitParam)
{
	logstr(LF_APISTUB,"DialogBoxParamW returns 0\n");
	return 0;
}

int 
TWIN_DialogBoxParam32(HINSTANCE hInst,LPCSTR lpTemplate,HWND hWnd,DLGPROC lpDlgProc,LPARAM dwParam,int bUnicode)
{
	if(bUnicode)
	  return TWIN_DialogBoxParamW(hInst,lpTemplate,hWnd,lpDlgProc,dwParam);
	return TWIN_DialogBoxParamA(hInst,lpTemplate,hWnd,lpDlgProc,dwParam);
}

void
ShowBitmapInfoHeader(BITMAPINFOHEADER *lpbmih)
{
	logstr(LF_CONSOLE,"BITMAPINFO HEADER %p\n",lpbmih);
	logstr(LF_CONSOLE,"size=%x width=%d height=%d planes=%d bitcount=%d\n",
	    lpbmih->biSize,
	    lpbmih->biWidth,
	    lpbmih->biHeight,
	    lpbmih->biPlanes,
	    lpbmih->biBitCount);

	logstr(LF_CONSOLE,"compress=%d sizeimage=%d pels %d,%d clr=%d,%d\n",
	    lpbmih->biCompression,
	    lpbmih->biSizeImage,
	    lpbmih->biXPelsPerMeter,
	    lpbmih->biYPelsPerMeter,
	    lpbmih->biClrUsed,
	    lpbmih->biClrImportant);
}
