
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include "Log.h"
#include "mfs_config.h"

#include "Profile.h"

#if 0
////////////////// PROFILE DEBUG //////////////////////////
static const char* DumpLString(LString lstr) {
#define MAX_LEN  1014
    static char buffer[MAX_LEN];
    int len;
    int failed = 0;

    len = lstr.len;
    if (len > MAX_LEN-1) {
	len = MAX_LEN-7;
	failed =1 ;
    }
    memcpy(buffer, lstr.data, len);    
    *(buffer+len) = 0;
    if (failed) strcat(buffer, "FAILED");
    return buffer;
}

static int DumpINI(SECTION* secp)
{
    SECTION* section;
    ENTRY* entry;

    section = secp;
    while (section) {
	fprintf(stderr,"+++++++++++++++++++++++++++++++++++++++++++++++\n");
	fprintf(stderr,"Section at %x\n",section);
	fprintf(stderr,"Flag %i %i %i\n",section->flag & SEC_EMPTY?1:0,
		section->flag & SEC_READ?1:0, section->flag & SEC_WRITE?1:0);
	fprintf(stderr,"Name %s\n",DumpLString(section->name));
	if (section->buf.data) {
	    fprintf(stderr,"RawDbeg %s RawDend\n",DumpLString(section->buf));
	    fprintf(stderr,"RawL %i\n",section->buf.len);
	} else {
	    fprintf(stderr,"Raw : (empty)\n");
	}
	entry = section->entry;
	while (entry) {
	    fprintf(stderr,"--------------\n");
	    fprintf(stderr,"Entry at %x\n",entry);
	    fprintf(stderr,"Flag %i %i %i\n",entry->flag & ENT_EMPTY?1:0,
		    entry->flag & ENT_READ?1:0, entry->flag & ENT_WRITE?1:0);
	    fprintf(stderr,"Key %s\n",DumpLString(entry->key));
	    fprintf(stderr,"Value %s\n",DumpLString(entry->value));
	    fprintf(stderr,"RawDbeg %s RawDend\n",DumpLString(entry->buf));
	    fprintf(stderr,"RawL %i\n",entry->buf.len);
	    entry = entry->next;
	}
	section = section->next;
    }
}
///////////////////// PROFILE DEBUG //////////////////////////////
#endif /* 0 */

extern char *GetTwinFilename();

/* List of profile files loaded */
static PROFILE *filelist = NULL;

/* LString operations */
static LCString LCStrcast(const char* string) 
{
    LCString lstring;

    /* This cast does not duplicate data */
    lstring.data = string;

    if (string) 
	lstring.len  = strlen(string);
    else
	lstring.len  = 0;  

    return lstring;
}

static int LCStrLStrcasecmp(LCString lcs, LString lct)
{
    if (lcs.len != lct.len) return 1;

    return (strncasecmp(lcs.data, lct.data, lcs.len));
}

static int LStrLStrcasecmp(LString lcs, LString lct)
{
    if (lcs.len != lct.len) return 1;

    return (strncasecmp(lcs.data, lct.data, lcs.len));
}

static char* LStrchr(const LString lcs, const char ct)
{
    char* cp;
    
    cp = lcs.data;
    while (*cp != ct && cp < (lcs.data + lcs.len)) cp++;

    if (*cp == ct) return cp;

    return NULL;
}

static void LStrmatch(LString* ls, LString* lt) 
{
    ls->data = lt->data;
    ls->len  = lt->len;
}

/* Function to read a line in the .INI file data */
static BOOL ReadLine(LString* buf, LString* line)
{
    int len = 0;
    char* cp;

    if (buf->len == 0) return FALSE;

    line->data = buf->data;

    cp = line->data;
    while (buf->len > len && *cp != '\n') {
	cp++;
	len++;
    }
    line->len = len;

    /* Update position in buffer and remaining space */
    buf->data = line->data + line->len + 1;
    buf->len  = buf->len - line->len - 1;

    return TRUE;
}

/* Break .INI file into sections, 
 * sections into entries, entries into key+values
 */ 
static void  BreakIniFile(PROFILE* pfile)
{
    BreakSections(pfile);
    BreakEntries(pfile);
}

static void BreakSections(PROFILE* pfile)
{
    char*    cp;
    LString  curpos;
    SECTION* secp;
    SECTION* next;
    LString  line;

    curpos.data = pfile->buf.data;
    curpos.len  = pfile->buf.len;

    if (pfile->secp) {
	ERRSTR((LF_ERROR,"Breaking sections of an already section-broken .INI file \n"));
	return;
    }

    secp = NULL;
    while (ReadLine(&curpos,&line)) {
	if ((cp=IsSection(line))) {  /* Found a new section */
	    /* Set length of previous one */
	    if (secp) secp->buf.len = (int) cp - (int) secp->buf.data -1; 
	    /* Allocate new one */
	    next = (SECTION*) WinMalloc(sizeof(SECTION));
	    next->buf.data  = cp;      /* Data begin right here */
	    next->name.len  = (int) strchr(cp, ']') - (int) cp - 1;
	    next->flag      = SEC_READ;
	    next->entry     = NULL;
	    if (next->name.len)
		next->name.data = cp+1;
	    next->next = NULL;
	    if (secp)
		secp->next  = next;
	    else
		pfile->secp = next;
	    secp = next;
	}
    }
    /* Do not forget the length of the last one */
    if (secp) secp->buf.len = (int) curpos.data - (int) secp->buf.data -1; 
}

static void BreakEntries(PROFILE* pfile)
{
    LString curpos;
    SECTION* secp;
    ENTRY* entry;
    ENTRY* next;
    LString line;

    secp = pfile->secp;
    while (secp) {
	/* Entries begin after the name : +1 for ] +1 for \n */
	curpos.data = secp->name.data + secp->name.len+2; 
	curpos.len  = secp->buf.len - secp->name.len-2; 
	entry       = secp->entry;
	if (entry) {
	    ERRSTR((LF_ERROR,"Breaking entries of an already entry-broken .INI file \n"));
	    return;
	}
	while(ReadLine(&curpos,&line)) {
	    next = (ENTRY*) WinMalloc(sizeof(ENTRY));
	    LStrmatch(&next->buf, &line);
	    if (IsEntry(line)) { /* Found a real entry */
		next->key       = FindKey(next);
		next->value     = FindValue(next);
		next->flag      = ENT_READ;
	    } else { /* No entry : blank line or comment or syntax error in .INI file */
		next->key   = LSTRING_NULL;
		next->value = LSTRING_NULL;
		next->flag  = ENT_EMPTY;
	    }
	    next->flag  |= ENT_READ;
	    next->next  = NULL;
	    if (entry)
		entry->next = next;
	    else
		secp->entry = next;
	    entry = next;
	}
	secp = secp->next;	
    }
}

static LString FindKey(ENTRY* entry)
{
    char* cp;
    LString key;

    cp = entry->buf.data;
    /* Trim leading blanks and tabs */
    while (*cp == ' ' || *cp == '\t') cp++;

    if (*cp == '=') { /* Error : = sign but no key ! */
	return LSTRING_NULL;
    }

    key.data = cp;

    /* Skip to '=' position and trim trailing blanks and tabs */
    cp = strchr(key.data,'=');
    cp--;
    while (*cp  == ' ' || *cp == '\t') cp--;

    key.len = (int) cp - (int) key.data + 1;

    return (key);
}

static LString FindValue(ENTRY* entry)
{
    char* cp;
    LString value;

    /* Skip to '=' position and trim leading blanks and tabs */
    cp = strchr(entry->buf.data,'=');
    cp++;
    while (*cp == ' ' || *cp == '\t') cp++;

    value.data = cp;
    
    /* Skip to '\n' position and trim trailing blanks and tabs */
    cp = strchr(value.data,'\n');
    cp--;
    while (*cp  == ' ' || *cp == '\t' || *cp == '\r') cp--;

    value.len = (int) cp - (int) value.data + 1;
    
    return (value);
}

static char* IsSection(LString line)
{
     char* cp;
    /* It's a section if the first non-blank character is a '[' 
     */   

    cp = line.data;
    while ((*cp == ' ' || *cp == '\t') && *cp != '\n') cp++;

    if (*cp == '[') return cp;

    return NULL;
}

static BOOL IsEntry(LString line)
{
    char* cp;
    /* It's an entry if it has a '=' in it and if '=' or ';' is not the first
     * non-blank character 
     */ 
    
    cp=LStrchr(line,'=');

    if (!cp) return FALSE; // No = sign

    cp = line.data;
    while (*cp == ' ' || *cp == '\t') cp++;

    if (*cp == '=' || *cp == ';') return FALSE; // First non-blank is = or ; 

    return TRUE;
}

static int WriteKeyandValue(ENTRY* entry, char* buffer, int cb, int flag)
{
    int len1 = 0, len2 = 0;

    /* Require that we have at least one byte of data space. */
    if (cb < 2)
	return 0;

    cb -= 1; /* Reserve one byte for the terminator. */
    buffer[0] = 0; /* Be sure it's terminated */
    switch (flag) {
    case WRITE_KEY :
	len1 = entry->key.len < cb ? entry->key.len : cb;
	if (len1 > 0)
	    {
	    memcpy(buffer,entry->key.data,len1);
	    buffer[len1] = 0; /* Be sure it's terminated */
	    }
	break;
    case WRITE_VAL :
	len2 = entry->value.len < cb ? entry->value.len : cb;
	if (len2 > 0)
	    {
	    memcpy(buffer,entry->value.data,len2);
	    buffer[len2] = 0; 
	    }
	break;
    case WRITE_BOTH :
	cb -= 1; /* Reserve one more byte for the '=' sign. */
	len1 = entry->key.len < cb ? entry->key.len : cb;
	if (len1 > 0)
	    memcpy(buffer,entry->key.data,len1);
	else
	    return 0; /* Can't copy key, don't try copy value. */

	buffer[len1] = '=';
	cb -= len1; /* Account for used buffer space. */
	len1 += 1; /* Include '=' sign as part of len1. */
	buffer[len1] = 0; /* In case there's no value, terminate string. */
	len2 = entry->value.len < cb ? entry->value.len : cb;
	if (len2 > 0)
	    {
	    memcpy(buffer+len1,entry->value.data,len2);
	    buffer[len1+len2] = 0; 
	    }
	break;
    default :
	break;
    }

    return (len1 + len2);
}

static int WriteSectionName(LString name, char* buffer)
{
    *buffer = '[';
    memcpy(buffer+1, name.data, name.len);
    *(buffer+name.len+1) = ']';
    *(buffer+name.len+2) = '\0';
    return (name.len+2);
}

/* 
 * Init the file list of INI files by loading a given file 
 * (the preference file generally). We can not use MFS_ and OpenFile
 * because they use settings form the preference file !
 */
static int InitFileList(char *filename)
{
    struct stat     status;
    FILE*           fp;

    if(filelist)
	return 0;

    filelist = (PROFILE *) WinMalloc(sizeof(PROFILE));
    memset(filelist,0,sizeof(PROFILE));

    filelist->filename = WinStrdup(filename);
    filelist->flag     = 0;
    filelist->secp     = NULL;
    filelist->buf.data = NULL;
    filelist->next     = NULL;
    strcpy(filelist->ofs.szPathName,filename);

    fp = fopen(filename,"r");
	
    /* If we found it, load it*/
    if(fp) {
	/* How big is the file */
	fstat(fileno(fp), &status );
	filelist->buf.len = status.st_size;

	/* Allocate memory for the data */
	filelist->buf.data = (char*) WinMalloc(filelist->buf.len);

	/* Read the .ini file data */
	fread(filelist->buf.data, filelist->buf.len, 1, fp);

	/* Close the file */
	fclose(fp);
 	}
    /* Break raw data into sections and entries, and entries
     * into key and values
     */
    BreakIniFile(filelist);

    return 0;
}

/*
 * Function to open and load a .ini file 	
 * file == 0, then use default file		 	
 * flag	      flag for .ini file, ie. readonly/readwrite    
 */
static SECTION* LoadIniFile(LPCTSTR filename,int flag)
{
    struct stat     status;
    HFILE           hFile;

    PROFILE *next;
	
    /* Use our built-in default name if none give */
    if(filename == NULL)
	filename = DEFAULT_PROFILE_NAME;

    /* Return NULL if not a file name */ 
    if(*filename == '\0')
	return NULL;

    /* Make sure profile files list is initialized */
    InitFileList(GetTwinFilename());

    /* Do we have seen this one before */
    for(next = filelist; next; next = next->next) {
	if((next->filename && strcmp(next->filename,filename) == 0)) {
	    /* Is this a writable entry... */
	    if(flag)
		next->flag = 1;
	    /* Should this file be the first in filelist (FIFO chain) ? */
	    return next->secp;
	}
    }

    /* Create a new entry in the list and load the file in it
     * Later, we could...
     *		keep track of last time accessed
     *		keep track of if tree is dirty
     *		keep track of how many are open and/or dirty
     */
    next = filelist;
			
    filelist = (PROFILE*) WinMalloc(sizeof(PROFILE));
    memset(filelist, 0, sizeof(PROFILE));
	
    filelist->filename = WinStrdup(filename);
    filelist->next     = next;
    filelist->secp     = NULL; /* No section pointer for now */
    filelist->flag     = flag;

    hFile = OpenFile(filename,&filelist->ofs,OF_READ);
	
    /* If we have not found it create an empty entry for it */
   if(hFile < 0) {
       /* Make sure filename is absolute */
       if(filename[0] != '/') {
	   GetWindowsDirectory(filelist->ofs.szPathName,255);	
	   strcat(filelist->ofs.szPathName,"/");
       }
       strcat(filelist->ofs.szPathName,filename);
		
       /* Create the file on the disk	*/
       hFile = OpenFile(filelist->ofs.szPathName,
			&filelist->ofs,OF_CREATE|OF_EXIST);

   } else { /* We found it */
       /* How big is the file */
       MFS_STAT (filelist->ofs.szPathName, &status );
       filelist->buf.len = status.st_size;

       /* Allocate memory for the data */
       filelist->buf.data = (char*) WinMalloc(filelist->buf.len);

       /* Read the .ini file data */
       _lread(hFile, filelist->buf.data, filelist->buf.len);

       /* Close the file */
       _lclose(hFile);

       /* Break raw data into sections and entries, and entries
	* into keys and values
	*/
       BreakIniFile(filelist);
   }

   /* If the section pointer is still NULL that means that either the file
    * did not exist or was empty, in both cases create an empty section in it 
    */
   if (filelist->secp == NULL) {
       filelist->secp       = (SECTION*) WinMalloc(sizeof(SECTION));
       filelist->secp->name = LSTRING_NULL;
       filelist->secp->buf  = LSTRING_NULL;
       filelist->secp->flag = (SEC_WRITE | SEC_EMPTY);
       filelist->secp->next = NULL;
       filelist->secp->entry= NULL;
   }
  
   /* Return the first section pointer */
   return (filelist->secp);
}

static int InternalGetProfileString(LPCSTR section, LPCSTR key, LPCSTR lpstr, LPSTR buff, int cb, LPCSTR file)
{
	SECTION *secp;
	LCString lstr_section;
	LCString lstr_key;
	ENTRY* entry;

	/* Do we want the whole section, or just the specific keys */
	if (key == NULL) {
		return InternalGetProfileSection(section,buff,cb,file,WRITE_KEY);
	}

	/* Find section header for file */
	secp = LoadIniFile(file,0);

        if(secp == NULL) {
	    return 0;
        }

	if (secp->flag & SEC_EMPTY) {
	    /* Trying to get a string from an empty INI file : default case */
	    if(lpstr)
		strncpy(buff,lpstr,cb);
	    else
		strcpy(buff,"");	    
	    return strlen(buff);
	}

	lstr_section = LCStrcast(section);
	lstr_key     = LCStrcast(key);
	/* Search for section */
	while (secp) {
	    if( LCStrLStrcasecmp(lstr_section,secp->name) == 0 ) {
		/* Found section, copy section data */
		entry = secp->entry;
		while (entry) {
		    if(LCStrLStrcasecmp(lstr_key,entry->key) == 0 ) {
			WriteKeyandValue(entry,buff,cb,WRITE_VAL);
			return strlen(buff);
		    }
		    entry = entry->next;
		}
#if 0
Should we do that ? That means that if two sections have the same name,
it could fail
		break; /* Section found, but no corresponding entry */
#endif
	    }
	    secp = secp->next;
	}

	/* default case */
	if(lpstr)
		strncpy(buff,lpstr,cb);
	else
		strcpy(buff,"");

	return strlen(buff);
}

/*********************************************************************
 * Windows APIS for READING from .INI files
 *********************************************************************/
static int
InternalGetProfileSection(LPCSTR lpszSection, LPSTR lpszReturnBuffer, 
			 DWORD cchBuffer, LPCSTR lpszFile, int flag)
{
	SECTION* secp;
	ENTRY*   entry;
	char *bp;
	int   cnt;
	int   n;
	LCString lstr_section;

	/* Find section header for file */
	secp = LoadIniFile(lpszFile,0);
	
        if(secp == NULL) {
	    return 0;
        }

	if (secp->flag & SEC_EMPTY) {
	    /* Trying to get a string from an empty INI file */
	    return 0;
	}

	lstr_section = LCStrcast(lpszSection);
	/* Search for section */
	while (secp) {
	    if(LCStrLStrcasecmp(lstr_section,secp->name) == 0) {		
		bp  = lpszReturnBuffer;	/* where to write in buffer */
		cnt = cchBuffer;	/* remaining space in buffer */
		lpszReturnBuffer[0] = 0;	/* build in double terminator */
		lpszReturnBuffer[1] = 0;

		entry = secp->entry;
		while (entry) {
		    /* Skip unused entries */
		    while (entry && entry->flag & ENT_EMPTY) {
			entry = entry->next;	
			continue;
		    }
		    if (entry == NULL) break; /* No more valid entries */

		    /* If we have no more room for strings, quit */
		    if (cnt <= (entry->key.len+1+entry->value.len))
			break;

		    /* Copy this entry into the buffer */
		    n = WriteKeyandValue(entry, bp, cnt, flag);  /* copy it */
		    bp += n;	                      /* update pointer */
		    cnt -= n;	                      /* decrement remaining space */
		    *bp++ = 0;                        /* advance to next token */
		    cnt--;		              /* additional blank */
		    
		    entry = entry->next;              /* next entry */
		}
		*bp = 0;
		return cchBuffer - cnt;
	    }

	    /* try next section */
	    secp = secp->next;
	}
	
	return 0;
}

#ifdef TWIN32
DWORD	WINAPI 
GetPrivateProfileSection(LPCSTR lpszSection, LPSTR lpszReturnBuffer, 
			 DWORD cchBuffer, LPCSTR lpszFile)
{
    int rc;

    APISTR((LF_APICALL, 
	    "GetPrivateProfileSection(LPCSTR=%s,LPSTR=%x,int %d,LPCSTR=%s)\n",
	    lpszSection?lpszSection:"NULL",
	    lpszReturnBuffer,
	    cchBuffer,
	    (lpszFile)?lpszFile:"NULL"));

    rc = InternalGetProfileSection(lpszSection,lpszReturnBuffer,cchBuffer,
				   lpszFile,WRITE_BOTH);    

    APISTR((LF_APIRET,"GetPrivateProfileSection: returns int %d\n",rc));
    return rc;
}

int
GetProfileSection(LPCSTR lpszSection, LPSTR lpszReturnBuffer, DWORD cchBuffer)
{
    int rc;

    APISTR((LF_APICALL, 
	    "GetProfileSection(LPCSTR=%s,LPSTR=%x,int %d)\n",
	    lpszSection?lpszSection:"NULL",
	    lpszReturnBuffer,
	    cchBuffer));

    rc = GetPrivateProfileSection(lpszSection,lpszReturnBuffer,cchBuffer,0);

    APISTR((LF_APIRET,"GetProfileSection: returns int %d\n",rc));
    return rc;
}
#endif

int     WINAPI 
GetPrivateProfileString(LPCSTR lpszSection, LPCSTR lpszKey, LPCSTR lpszDefault, LPSTR lpszReturnBuffer, int cchReturnBuffer, LPCSTR lpszFile)
{
	int rc;
    	APISTR((LF_APICALL, 
	    "GetPrivateProfileString(LPCSTR=%s,LPCSTR=%s,LPCSTR=%s,LPSTR=%x,int %d,LPCSTR=%s)\n",
		lpszSection?lpszSection:"NULL",
		lpszKey?lpszKey:"NULL",
		lpszDefault?lpszDefault:"NULL",
		lpszReturnBuffer,
		cchReturnBuffer,
		(lpszFile)?lpszFile:"NULL"));

	rc = InternalGetProfileString(lpszSection,lpszKey,lpszDefault,lpszReturnBuffer,
				cchReturnBuffer,lpszFile);

        APISTR((LF_APIRET,"GetPrivateProfileString: returns int %d\n",rc));
	return rc;
}

int     WINAPI 
GetProfileString(LPCSTR lpszSection, LPCSTR lpszKey, LPCSTR lpszDefault, LPSTR lpszReturnBuffer, int cchReturnBuffer)
{
    int rc;
    APISTR((LF_APICALL,
	"GetProfileString(LPCSTR=%s,LPCSTR=%s,LPCSTR=%s,LPSTR=%x,int %d)\n",
	(lpszSection)?lpszSection:"NULL",
	(lpszKey)?lpszKey:"NULL",
	(lpszDefault)?lpszDefault:"NULL",
	lpszReturnBuffer,
	cchReturnBuffer));

    rc = InternalGetProfileString(lpszSection,lpszKey,lpszDefault,lpszReturnBuffer,
			    cchReturnBuffer,0);

    APISTR((LF_APIRET,"GetProfileString: returns int %d\n",rc));
    return rc;
}


UINT    WINAPI 
GetPrivateProfileInt(LPCSTR lpszSection, LPCSTR lpszKey, INT dwDefault, LPCSTR lpszFile)
{
	char ascii[32];

	APISTR((LF_APICALL, 
	 "GetPrivateProfileInt(LPCSTR=%s,LPCSTR=%s,int=%d)\n", 
		(lpszSection)?lpszSection:"NULL", 
		(lpszKey)?lpszKey:"NULL", 
		dwDefault));
    
	if (InternalGetProfileString(lpszSection,lpszKey,0,ascii,32,lpszFile))
		dwDefault = strtoul(ascii,0,0);

    	APISTR((LF_APIRET,"GetPrivateProfileInt: returning %d\n",dwDefault));
	return dwDefault;
}

UINT    WINAPI 
GetProfileInt(LPCSTR lpszSection, LPCSTR lpszKey, INT dwDefault)
{
	char ascii[32];

	APISTR((LF_APICALL, 
	 "GetProfileInt(LPCSTR=%s,LPCSTR=%s,int=%d)\n", 
		(lpszSection)?lpszSection:"NULL", 
		(lpszKey)?lpszKey:"NULL", 
		dwDefault));
    
	if (InternalGetProfileString(lpszSection,lpszKey,0,ascii,32,0))
		dwDefault = strtol(ascii,0,0);

    	APISTR((LF_APIRET,"GetProfileInt: returning %d\n",dwDefault));
	return dwDefault;
}


static void FreeIniFile(PROFILE *last)
{
	SECTION *secp,*nextp = 0;
	ENTRY *entryp,*lastp = 0;
	if(last == 0)
		return;
    	WinFree(last->filename);
	WinFree(last->buf.data);

	for(secp=last->secp;secp;secp=nextp->next) {	
		for(entryp = secp->entry;entryp;entryp=lastp->next) {
			//logstr(LF_CONSOLE,"ENTRY: %p data=%p\n",
			//	entryp,entryp->buf.data);
			if(lastp) {
				WinFree(lastp);
			}
			lastp = entryp;	
		}
		if(secp->entry) {
			//798GWinFree(secp->entry);
		}

		//logstr(LF_CONSOLE,"SECTION: %p data=%p\n",
		//	secp,secp->name.data);
		if(nextp) {
			//WinFree(nextp->name.data);
			WinFree(nextp);
			last->secp = 0;
		}
		nextp = secp;
	}
	if(last->secp) {
		//WinFree(last->secp->name.data);
		WinFree(last->secp);
	}

	WinFree(last);
}

static int FlushIniCache(LPCSTR filename)
{
    FILE *filep;
    char* lp;
    char  newfile[128];
    SECTION *secp;
    ENTRY* entry;
    int len;
    char buffer[BUFSIZ];
    char line_feed[] = "\n";

    PROFILE *next;
    PROFILE *last = 0;
    
    /* Have we seen this one before ? */
    for(next = filelist;next;next = last->next) {

	if(last) {
		filelist = 0;
		FreeIniFile(last);
	}

	last = next;
	
	/* If we have a filename, and it doesn't match 
	 * skip flushing this file */
	if(filename && stricmp(filename, next->filename))
	    continue;
	
	/* If this file is a readwrite ini file it may 
	 * have been written */
	if(next->flag == 1 && next->secp) {
	    strcpy(newfile,next->ofs.szPathName);
	    if((lp = strrchr(newfile,'.')))
		*lp = 0;			
	    strcat(newfile,".tmp");
	    filep = fopen(newfile,"w+");
	    if(filep) {
		secp = next->secp;
		while(secp) {
		  if (!(secp->flag & SEC_EMPTY)) {
		    if (secp->flag & SEC_READ) {
			/* If we didn't touch the section, write the raw data 
			 * +1 because LStrings do not include terminating '\n' */
			fwrite(secp->buf.data, secp->buf.len+1, 1, filep);
		    } else {
			len = WriteSectionName(secp->name, buffer);
			/* Do not forget to feed the hungry line */
			strcat(buffer,line_feed); len++;
			fwrite(buffer, 1, len, filep);			
			entry = secp->entry;
			while (entry) {
			    if (entry->flag & ENT_READ) {
				/* If we didn't touch the entry, write the raw data */
				fwrite(entry->buf.data, 1, entry->buf.len+1, filep);
			    } else {
				len = WriteKeyandValue(entry, buffer, BUFSIZ, WRITE_BOTH);
				/* Do not forget to feed the hungry line */
				strcat(buffer,line_feed); len++;
			 	fwrite(buffer, 1, len, filep);
			    }
			    entry = entry->next;
			}
		    }
		  }
		  secp = secp->next;
		}
		fclose(filep);
		
		/* Remove the previous file */
		unlink(next->ofs.szPathName);
		
		/* Rename the temp file to the original file */
		rename(newfile,next->ofs.szPathName);
	    }
	}
    }
    FreeIniFile(last);     
    return 0;
}

static ENTRY* InsertEntry(ENTRY* entry, int flag, LPCSTR key, LPCSTR value)
{

    ENTRY* new_entry = (ENTRY*) WinMalloc(sizeof(ENTRY));

    if (key == NULL) {
	return NULL;
    }

    if (value == NULL) value = "";

    new_entry->buf.data   = (char*) WinMalloc(strlen(key)+strlen(value)+2);
    new_entry->buf.len    = strlen(key)+strlen(value)+2;
    sprintf(new_entry->buf.data,"%s=%s\n",key,value);
    new_entry->key.data   = new_entry->buf.data;
    new_entry->key.len    = strlen(key);
    new_entry->value.data = new_entry->buf.data+strlen(key)+1;
    new_entry->value.len  = strlen(value);
    new_entry->next       = NULL;

    /* Mark it as modified and allocated in memory, so as to
     * free it somewhere */
    new_entry->flag = (ENT_WRITE | ENT_MEM);

    if (entry) {
	if (flag) { /* We are modifying the given entry */
	    LStrmatch(&entry->buf, &new_entry->buf);
	    LStrmatch(&entry->key, &new_entry->key);
	    LStrmatch(&entry->value, &new_entry->value);
	    entry->flag     = new_entry->flag;
	} else { /* We are inserting the new one after the given entry*/
	    new_entry->next = entry->next;
	    entry->next     = new_entry;
	}
    } /* else : we are retrieving a new entry */

    return new_entry;
}

/*********************************************************************
 * Windows APIS for WRITING into .INI files
 *********************************************************************/
BOOL    WINAPI 
WritePrivateProfileString(LPCSTR lpszSection, LPCSTR lpszKey, LPCSTR lpszString, 
			  LPCSTR lpszFile)
{	
    SECTION* secp;
    SECTION* prev_secp;
    ENTRY* entry;
    ENTRY* prev_entry;
    LCString lstr_section;
    LCString lstr_key;

    APISTR((LF_APICALL, "WritePrivateProfileString(%p,%p,%p,%p)\n",
	    lpszSection,lpszKey,lpszString,lpszFile));
    
    /* As per the definition, flush the cache */
    if(lpszSection == NULL && lpszKey == NULL && lpszString == NULL) {
	APISTR((LF_APIRET, "WritePrivateProfileString flush cache\n"));
	return FlushIniCache(lpszFile);
    }

    secp = LoadIniFile(lpszFile,1);

    if(secp == NULL) {
	    APISTR((LF_APIRET, "WritePrivateProfileString returns FALSE\n"));
	    return FALSE;
    }

    /* If the first section is an empty one, that means that the file was
     * just created and/or empty, so write the given section in it and return */  
    if (secp->flag & SEC_EMPTY) {
	secp->name.data = WinStrdup(lpszSection);
	secp->name.len  = strlen(lpszSection);
	secp->flag      = SEC_WRITE;	
	secp->entry     = InsertEntry(NULL, 0, lpszKey, lpszString);
	return TRUE;	
    }

    /* Search for section */
    lstr_section = LCStrcast(lpszSection);
    lstr_key     = LCStrcast(lpszKey);
    prev_secp    = secp;
    while (secp) {
	if(LCStrLStrcasecmp(lstr_section,secp->name) == 0) {
	    /* If Key is null, then we are deleting the section */
	    if (lpszKey == NULL) {
		prev_secp->next = secp->next;
		secp = secp->next;		
		continue; 
	    } 
	    /* Found the section, now find the key */
	    entry = secp->entry;
	    prev_entry = entry;
	    while (entry) {
		if(LCStrLStrcasecmp(lstr_key, entry->key) == 0) {
		    /* If String is null, then we are deleting the entry */
		    if (lpszString == NULL) {
			prev_entry->next = entry->next;
			entry = entry->next;
			continue;
		    }
		    /* Found key so modify the entry */
		    InsertEntry(entry,1,lpszKey,lpszString);
		    secp->flag &= ~SEC_READ;
		    secp->flag |= SEC_WRITE;
		    APISTR((LF_APIRET, "WritePrivateProfileString returns TRUE\n"));
		    return TRUE;
		}
		prev_entry = entry;
		entry = entry->next;
	    }	    
	    if (lpszString == NULL) return TRUE; /* We were deleting an entry */

	    /* Key was not found, so add the value */
	    if(lpszString) {
		InsertEntry(prev_entry,0,lpszKey,lpszString);			
		secp->flag &= ~SEC_READ;
		secp->flag |= SEC_WRITE;
		APISTR((LF_APIRET, "WritePrivateProfileString returns TRUE\n"));
		return TRUE;
	    }
	    APISTR((LF_APIRET, "WritePrivateProfileString returns FALSE\n"));
	    return FALSE;
	}
	
	/* Try next section */
	prev_secp = secp;
	secp = secp->next;
    }

    if (lpszKey == NULL) return TRUE; /* We were deleting the section */

    /* Section was not found, so add a new section, after all others */
    secp = (SECTION*) WinMalloc(sizeof(SECTION)); 
    secp->name.data = WinStrdup(lpszSection);
    secp->name.len  = strlen(lpszSection);
    secp->buf       = LSTRING_NULL;
    secp->flag      = SEC_WRITE;
    secp->entry     = NULL;
    secp->next      = NULL;
	
    /* Chain the new one */
    prev_secp->next = secp;

    secp->entry = InsertEntry(NULL,0,lpszKey,lpszString);			
    
    APISTR((LF_APIRET, "WritePrivateProfileString returns 1\n"));
    return TRUE;
}

BOOL    WINAPI 
WriteProfileString(LPCSTR lpszSection, LPCSTR lpszKey, LPCSTR lpszString)
{
	return WritePrivateProfileString(lpszSection,lpszKey,lpszString,0);
}

DWORD	WINAPI 
GetPrivateProfileSectionNames(LPTSTR lpszReturnBuffer, DWORD nSize, LPCTSTR lpFileName)
{
	SECTION *secp;
	char *bp;
	LString sname = LSTRING_NULL;
	int   n,cnt;
	
	/* find section header for file */
	secp = LoadIniFile(lpFileName,0);

	bp = lpszReturnBuffer;			/* where to write in buffer */
	cnt = nSize;				/* remaining space in buffer */
	bp[0] = 0;				/* build in double terminator */
	bp[1] = 0;

	/* Search for section */
	while (secp) {
	    if(LStrLStrcasecmp(sname,secp->name)) {
		/* since multiple sections can have same name */
			sname = secp->name;

			strncpy(bp,secp->name.data, secp->name.len);

			n = strlen(bp);		/* we added characters to bp, how many? */
			bp += n;		/* update pointer */
			cnt -= n;		/* decrement remaining space */
			*++bp = 0;		/* advance to next location */
			cnt--;			/* additional terminator */
		}
		
		/* try next section */
		secp = secp->next;
	}

	return 0;
}

#ifdef TWIN32

BOOL WINAPI
WritePrivateProfileSection(LPCTSTR lpszSection, LPCTSTR lpszKeysAndValues,
	LPCTSTR lpszFile)
{
	char key[BUFSIZ];
	char *string;
	char *lpstr = (char *) lpszKeysAndValues;

	LOGSTR((LF_LOG,"WritePrivateProfileSection: filename %s\n",
		lpszFile));

	for ( ; *lpstr != '\0'; lpstr += strlen(lpstr) + 1)
	{
		strncpy(key, lpstr, sizeof(key));
		if ((string = strchr(key, '=')) == NULL)
			break;
		*string = '\0';		/* terminate key with '\0' */
		string += 1;
		if (!WritePrivateProfileString(lpszSection, key, string,
			lpszFile))
			return (FALSE);
	}
	return (TRUE);

}

BOOL WINAPI
WriteProfileSection(LPCTSTR lpszSection, LPCTSTR lpszKeysAndValues)
{
	return WritePrivateProfileSection(lpszSection, lpszKeysAndValues,0);
}

static char hex_digit[] = "0123456789abcdef";

static int decode_hex(int c)
{
	c = toupper(c);
	c -= '0';
	if (c > 9) c -= 7;
	return (c);
}

BOOL WINAPI
GetPrivateProfileStruct(LPCTSTR lpszSection,
	LPCTSTR lpszKey,
	LPVOID lpStruct,
	UINT uSizeStruct,
	LPCTSTR szFile)
{
	char valuebuf[BUFSIZ];
	char *src, *dst;

	LOGSTR((LF_LOG,"GetPrivateProfileStruct: filename %s\n",
		szFile));

	if (!GetPrivateProfileString(lpszSection, lpszKey, "", valuebuf,
		sizeof(valuebuf), szFile))
		return (FALSE);

	/* convert hex string to struct bytes */
	if (strlen(valuebuf) / 2 > uSizeStruct)
		return (FALSE);
	src = valuebuf;
	dst = (char *) lpStruct;
	for ( ; *src != '\0'; src += 2, dst++)
		*dst = decode_hex(src[0]) << 4 | decode_hex(src[1]);

	return (TRUE);

}

BOOL WINAPI
WritePrivateProfileStruct(LPCTSTR lpszSection,
	LPCTSTR lpszKey,
	LPVOID lpStruct,
	UINT uSizeStruct,
	LPCTSTR szFile)
{
	char valuebuf[BUFSIZ];
	char *value, *src, *dst;

	LOGSTR((LF_LOG,"WritePrivateProfileStruct: filename %s\n",
		szFile));

	if (lpStruct == NULL)
		value = NULL;
	else
	{
		value = valuebuf;
		/* encode struct bytes as hex string */
		if (2 * uSizeStruct + 1 > sizeof(valuebuf))
			return (FALSE);
		src = (char *) lpStruct;
		dst = valuebuf;
		for ( ; uSizeStruct-- > 0; src++, dst += 2)
		{
			dst[0] = hex_digit[(*src & 0xf0) >> 4];
			dst[1] = hex_digit[*src & 0x0f];
		}
		*dst = '\0';
	}

	return WritePrivateProfileString((LPSTR)lpszSection, (LPSTR)lpszKey, value, (LPSTR)szFile);

}

#endif /* TWIN32 */

