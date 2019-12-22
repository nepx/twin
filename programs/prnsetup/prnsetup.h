/*=======================================================================
						prnsetup.h
  =======================================================================*/

/* Status of the Exit button */
#define	CANCEL			0
#define	CLOSE			1

#define	BUFFER_SIZE		128
#define	MAX_PORT_NUM		64
#define	MAX_DEV_NUM		64

typedef struct {
	int	flags;
	char	*p1;
	char	*p2;
} ADDSTRUCT;
typedef	ADDSTRUCT	*PADDSTRUCT;
typedef	ADDSTRUCT FAR	*LPADDSTRUCT;

#define	ADD_PRINTER		1
#define	ADD_PORT		2
#define	ADD_ALIAS		3
#define NEW_PRINTER		4

#define	STR_CAPTION		0x100
#define	STR_EDIT		0x200

void GetDevPort(ADDSTRUCT*, char*);
void UpdateDefaultPrinter(HWND, HWND, int);
void ChangeExitStatus(HWND, int*);
int AddDialogBox(HWND, ADDSTRUCT *);

/* Exported function (WndProcs) */
LRESULT FAR PASCAL InvisibleWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK PrintersDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ConnectDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AddDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK BrowseDlgProc(HWND, UINT, WPARAM, LPARAM);

