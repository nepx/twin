
#include "windows.h"
#include "Log.h"
#include "strings.h"
#include "Strings32.h"
#include "ctype.h"
#include "Log.h"

char *
WinStrdupW(LPCWSTR lpwstr)
{
	int    len;
	char  *lpstr;
	
	len = lstrlenW(lpwstr);
	if(len == 0)
		return "";

	lpstr = WinMalloc(len);
	if(lpstr)
		WideCharToMultiByte(0,0,(LPCWSTR)lpwstr,0,lpstr,len,0,0);
	
	return lpstr;
}

int 
MultiByteToWideChar(UINT page, DWORD flags, LPCSTR src, int srclen,
                    LPWSTR dst, int dstlen)
{
    /* if srclen == -1, len is strlen of src */
    if (srclen == -1)
         srclen = lstrlen(src);

    /* if !destination, return size needed for copy */
    if (!dst)
         return srclen*2;

    /* shorthand copy, multibyte to wide char */
    lstrcpynAtoW(dst,src,srclen); 
    return srclen*2;
}

int
WideCharToMultiByte(UINT codepage,DWORD dwFlags,LPCWSTR wide,int widelen,
                    LPSTR multi,int multilen,LPCSTR defchar,LPBOOL fuseddefault)
{
	short *cp;
	char  *buffer = multi;
	int   width   = widelen;
	int   size    = 0;
	
	if(fuseddefault)
		*fuseddefault = 0;

	if(width == 0)
		width = lstrlenW((short *)wide);

	for(cp = (short *) wide;width && *cp;width--) {
		if(*cp < 256) {
		       if(multi) 
				*buffer = (*cp & 0xff);
		} else {
		       if(multi) 
				*buffer = defchar?*defchar:'?';
		        if(fuseddefault)
				*fuseddefault = 1;
		}

		cp++;
		buffer++;
		size++;
	}

	if(multi)
		*buffer = 0;

	size += 1;	

	APISTR((LF_API,"WideCharToMultiByte(...) returns %d\n",size));
	return size;
}

LPWSTR 
lstrcatW( LPWSTR dst, LPCWSTR src )
{
    short * p = dst;
    logstr(LF_LOG,"lstrcatW\n");
    while (*p) p++;
    while ((*p++ = *src++));
    return dst;
}

int 
lstrcmpW( LPCWSTR str1, LPCWSTR str2 )
{
    logstr(LF_LOG,"lstrcmpW\n");
    if (str1 && !str2) {
	    while (*str1 && (*str1 == *str2)) { 
		str1++; str2++; 
	    }
	    return (int)(*str1 - *str2);
    }
    return 0;
}

int 
lstrcmpiW( LPCWSTR str1, LPCWSTR str2 )
{
    int res;

    logstr(LF_LOG,"lstrcmpiW\n");
    if (str1 && str2) {
	    while (*str1) {
		if ((res = toupper(*str1) - toupper(*str2)) != 0) 
		   return res;

		str1++;
		str2++;
	    }
	    return toupper(*str1) - toupper(*str2);
    }
    return 0;
}

int 
lstrncmpiW( LPCWSTR str1, LPCWSTR str2, int n )
{
    int res;

    logstr(LF_LOG,"lstrncmpiW\n");
    if (!n) 
	return 0;

    while ((--n > 0) && *str1) {
        if ((res = toupper(*str1) - toupper(*str2)) != 0) 
	   return res;

        str1++;
        str2++;
    }

    return toupper(*str1) - toupper(*str2);
}

LPWSTR 
lstrcpyW( LPWSTR dst, LPCWSTR src )
{
    short * p = dst;

    logstr(LF_LOG,"lstrcpyW\n");
    while ((*p++ = *src++));

    return dst;
}

LPWSTR 
lstrcpynW( LPWSTR dst, LPCWSTR src, int n )
{
    short * p = dst;

    logstr(LF_LOG,"lstrcpynW\n");
    for(;;) {
	if(n == 0)
		break;
	if(*src == 0) 
		break;
	*p = *src;
	p++;
	src++;
	n--;
    }
    *p = 0;

    return dst;
}

LPSTR
lstrcpyA( LPSTR a, LPCSTR b )
{
	return lstrcpy(a,b);
}

LPSTR
lstrcpynA( LPSTR a, LPCSTR b, int n )
{
	return lstrcpyn(a,b,n);
}

int
lstrcmpA( LPCSTR a, LPCSTR b )
{
	return lstrcmp(a,b);
}

int
lstrcmpiA( LPCSTR a, LPCSTR b )
{
	return lstrcmpi(a,b);
}

LPSTR
lstrcatA( LPSTR a, LPCSTR b )
{
	return lstrcat(a,b);
}

int
lstrlenA( LPCSTR str )
{
	return lstrlen(str);
}

int 
lstrlenW( LPCWSTR str )
{
    int len = 0;
    logstr(LF_LOG,"lstrlenW\n");

    if (!str) 
	return 0;

    while (*str++) 
	len++;

    return len;
}

LPWSTR
lstrcpyAtoW( LPWSTR dst, LPCSTR src )
{
    LPWSTR p = dst;
    while ((*p++ = (WCHAR)(unsigned char)*src++));
    return dst;
}


LPSTR
lstrcpyWtoA( LPSTR dst, LPCWSTR src )
{
    register LPSTR p = dst;
    while ((*p++ = (CHAR)*src++));
    return dst;
}

LPWSTR
lstrcpynAtoW( LPWSTR dst, LPCSTR src, int n )
{
    LPWSTR p = dst;
    while ((n-- > 1) && *src) 
	*p++ = (WCHAR)(unsigned char)*src++;
    *p = 0;
    return dst;
}

LPSTR
lstrcpynWtoA( LPSTR dst, LPCWSTR src, int n )
{
    LPSTR p = dst;
    while ((n-- > 1) && *src) 
	*p++ = (CHAR)*src++;
    *p = 0;
    return dst;
}

