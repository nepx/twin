#ifndef profile_h
#define profile_h

/* New type of strings without zero terminating character */
typedef struct tagLString {
    char* data;
    int   len;
} LString;

typedef struct tagLCString {
    const char* data;
    int   len;
} LCString;

static LString LSTRING_NULL = {"", 0};

/* Usefull operations on these LStrings */

/* Linked list of entries in a section */
/* ENTRY is KEY + = + value */
typedef struct tagENTRY {
    LString          key; 		/* Key */
    LString          value;		/* Value */
    LString          buf;               /* Raw data : the whole line */			
    int              flag;              /* Type of entry : valid, comment, blank,.. */
    struct tagENTRY* next;		/* Pointer to next entry */
} ENTRY;

/* Usefull operations on these entries */

/* Linked list of sections in a profile file */
typedef struct tagSECTION {
    LString            name;		/* Name of this section    */
    ENTRY*             entry;           /* Linked list of entries  */
    LString            buf;             /* Raw data : the whole sections lines */
    int                flag;            /* Type of entry : valid, comment, blank,.. */
    struct tagSECTION* next;		/* Pointer to next section */
} SECTION;

/* Usefull operations on these sections */

/* Linked list of profile files info */
typedef struct tagPROFILE {
	char*     filename;			/* Basename of file  */
	int       flag;				/* Is it writable?   */
        LString   buf;                          /* Raw data : the whole file */
	SECTION*  secp;			        /* List of sections */
	OFSTRUCT  ofs;				/* ofs structure for file */
	struct tagPROFILE *next;		/* Next structure    */
} PROFILE;

/* Various constants */
#define DEFAULT_PROFILE_NAME	      "win.ini"

#define ENT_EMPTY   0x0001                      /* Entry is a comment or a blank line */
#define ENT_READ    0x0002                      /* Entry has not been written into */
#define ENT_WRITE   0x0004                      /* Entry has been written into */
#define ENT_MEM     0x0008                      /* Entry has been created in memory */

#define SEC_EMPTY   0x0001                      /* Entry is a comment or a blank line */
#define SEC_READ    0x0002                      /* Section has not been written into */
#define SEC_WRITE   0x0004                      /* Section has been written into */

#define WRITE_KEY   0x0001
#define WRITE_VAL   0x0002
#define WRITE_BOTH  0x0003

static LCString   LCStrcast(const char* string);
static int        LCStrLStrcasecmp(LCString lcs, LString lct);
static int        LStrLStrcasecmp(LString lcs, LString lct);
static char*      LStrchr(const LString lcs, const char ct);
static void       LStrmatch(LString* ls, LString* lt);

static BOOL       ReadLine(LString* buf, LString* line);
static void       BreakIniFile(PROFILE* pfile);
static void       BreakSections(PROFILE* pfile);
static void       BreakEntries(PROFILE* pfile);
static LString    FindKey(ENTRY* entry);
static LString    FindValue(ENTRY* entry);
static char*      IsSection(LString lstring);
static BOOL       IsEntry(LString lstring);
static int 	  WriteKeyandValue(ENTRY* , char* , int , int );
static int        WriteSectionName(LString name, char* buffer);
static int        InitFileList(char *filename);
static SECTION*   LoadIniFile(LPCTSTR filename,int flag);
static int        InternalGetProfileString(LPCSTR section, LPCSTR key, LPCSTR lpstr, 
                                           LPSTR buff, int cb, LPCSTR file);
static int        InternalGetProfileSection(LPCSTR lpszSection, LPSTR lpszReturnBuffer, 
                                            DWORD cchBuffer, LPCSTR lpszFile,int flag);
static int        FlushIniCache(LPCSTR filename);
static ENTRY*     InsertEntry(ENTRY* entry, int flag, LPCSTR key, LPCSTR value);

#endif // profile_h
