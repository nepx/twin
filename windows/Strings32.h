

char *WinStrdupW(LPCWSTR);

int WideCharToMultiByte(UINT ,DWORD ,LPCWSTR ,int ,LPSTR ,int ,LPCSTR ,LPBOOL );

int lstrcmpW( LPCWSTR , LPCWSTR );
int lstrcmpiW( LPCWSTR , LPCWSTR );
int lstrncmpiW( LPCWSTR , LPCWSTR , int );

LPWSTR lstrcatW( LPWSTR , LPCWSTR );
LPWSTR lstrcpyW( LPWSTR , LPCWSTR );
LPWSTR lstrcpynW( LPWSTR , LPCWSTR , int );
int lstrlenW( LPCWSTR );

LPSTR lstrcatA( LPSTR , LPCSTR );
LPSTR lstrcpyA( LPSTR , LPCSTR );
LPSTR lstrcpynA( LPSTR , LPCSTR , int );

int lstrcmpA( LPCSTR , LPCSTR );
int lstrcmpiA( LPCSTR , LPCSTR );
int lstrlenA( LPCSTR );

LPWSTR lstrcpyAtoW( LPWSTR , LPCSTR );
LPSTR  lstrcpyWtoA( LPSTR , LPCWSTR );
LPWSTR lstrcpynAtoW( LPWSTR , LPCSTR , int );
LPSTR  lstrcpynWtoA( LPSTR , LPCWSTR , int );

