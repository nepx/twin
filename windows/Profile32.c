
#include "windows.h"
#include "Log.h"
#include "Strings32.h"

UINT    WINAPI 
GetProfileIntA(LPCSTR section, LPCSTR entry , int rc)
{
	return GetProfileInt(section,entry,rc);
}

UINT    WINAPI 
GetProfileIntW(LPCWSTR section, LPCWSTR entry , int rc)
{
	char   entrystr[80];
	char   sectionstr[80];

	entrystr[0] = 0;
	sectionstr[0] = 0;
	WideCharToMultiByte(0,0,entry,0,entrystr,80,0,0);
	WideCharToMultiByte(0,0,section,0,sectionstr,80,0,0);

	return GetProfileInt(sectionstr,entrystr,rc);
}

int     WINAPI 
GetProfileStringA(LPCSTR section, LPCSTR entry, LPCSTR lpstr, LPSTR buffer, int count)
{
	return GetProfileString(section,entry,lpstr,buffer,count);
}

int     WINAPI 
GetPrivateProfileStringA(LPCSTR section, LPCSTR entry, LPCSTR lpstr, LPSTR buffer, int count,LPSTR lpfile)
{
	return GetPrivateProfileString(section,entry,lpstr,buffer,count,lpfile);
}

int     WINAPI 
GetPrivateProfileStringW(LPCWSTR section, LPCWSTR entry, LPWSTR lpstr, LPWSTR buffer, int count,LPCWSTR lpfile)
{
	char   entrystr[80];
	char   sectionstr[80];
	char   lpfilestr[80];
	char   lpstrstr[80];
	char   resultstr[80];

	entrystr[0] = 0;
	sectionstr[0] = 0;
	lpstrstr[0] = 0;
	lpfilestr[0] = 0;
	WideCharToMultiByte(0,0,entry,0,entrystr,80,0,0);
	WideCharToMultiByte(0,0,section,0,sectionstr,80,0,0);
	WideCharToMultiByte(0,0,lpstr,0,lpstrstr,80,0,0);
	WideCharToMultiByte(0,0,lpfile,0,lpfilestr,80,0,0);
	
	logstr(LF_LOG,"GetPrivateProfileStringW(%s,%s,%s,%d,%s)\n",
		sectionstr,entrystr,lpstrstr,count,lpfilestr);
	if(GetPrivateProfileString(sectionstr,entrystr,lpstrstr,resultstr,
		count,lpfilestr)) {
		lstrcpyAtoW(lpstr,resultstr);		
	}
	return lstrlenW(lpstr);
}

int     WINAPI 
GetProfileStringW(LPCWSTR section, LPCWSTR entry, LPWSTR lpstr, LPWSTR buffer, int count)
{
	return GetPrivateProfileStringW(section,entry,lpstr,buffer,count,0);
}

int
GetPrivateProfileIntA(LPCSTR lpapp,LPCSTR lpkey,int defid,LPCSTR lpfile)
{
	return GetPrivateProfileInt(lpapp,lpkey,defid,lpfile);
}

int
GetPrivateProfileIntW(LPCWSTR lpapp,LPCWSTR lpkey,int defid,LPCWSTR lpfile)
{
	short defstr[80];
	char numstr[80];
	logstr(LF_LOG,"GetPrivateProfileIntW()\n");
	wsprintf(numstr,"%d",defid);	
	lstrcpyAtoW(defstr,numstr);
	GetPrivateProfileStringW(lpapp,lpkey,defstr,(LPWSTR)numstr,80,lpfile);
	
	lstrcpyWtoA((LPSTR)defstr,(LPWSTR)numstr);
	return strtol((LPSTR)defstr,0,0);
}

BOOL
WritePrivateProfileSectionA(LPCSTR lpapp,LPCSTR lpstr,LPCSTR lpfile)
{
	logstr(LF_LOG,"WriteProfileSectionA(%s,%s,%s)\n",
		lpapp,lpstr,lpfile);
	return WritePrivateProfileSection(lpapp,lpstr,lpfile);
}

BOOL
WritePrivateProfileSectionW(LPCWSTR lpapp,LPCWSTR lpstr,LPCWSTR lpfile)
{
	char   entrystr[80];
	char   sectionstr[80];
	char   lpstrstr[80];

	entrystr[0] = 0;
	sectionstr[0] = 0;
	lpstrstr[0] = 0;
	WideCharToMultiByte(0,0,lpapp,0,entrystr,80,0,0);
	WideCharToMultiByte(0,0,lpstr,0,sectionstr,80,0,0);
	WideCharToMultiByte(0,0,lpfile,0,lpstrstr,80,0,0);
	
	logstr(LF_LOG,"WriteProfileSectionW(%s,%s,%s)\n",
		entrystr,sectionstr,lpstrstr);
	return WritePrivateProfileSection(entrystr,sectionstr,lpstrstr);
}
BOOL 
WritePrivateProfileStringA(LPCSTR lpapp,LPCSTR lpkey,LPCSTR lpstr,LPCSTR lpFile)
{
	logstr(LF_LOG,"WriteProfileSectionW(%s,%s,%s,%s)\n",
		lpapp,lpkey,lpstr,lpFile);
	return WritePrivateProfileString(lpapp,lpkey,lpstr,lpFile);
}

BOOL 
WritePrivateProfileStringW(LPCWSTR lpapp,LPCWSTR lpkey,LPCWSTR lpstr,LPCWSTR lpfile)
{
	char   entrystr[80];
	char   sectionstr[80];
	char   lpstrstr[80];
	char   lpfilestr[80];

	entrystr[0] = 0;
	sectionstr[0] = 0;
	lpstrstr[0] = 0;
	lpfilestr[0] = 0;
	WideCharToMultiByte(0,0,lpapp,0,entrystr,80,0,0);
	WideCharToMultiByte(0,0,lpkey,0,sectionstr,80,0,0);
	WideCharToMultiByte(0,0,lpstr,0,lpstrstr,80,0,0);
	WideCharToMultiByte(0,0,lpfile,0,lpfilestr,80,0,0);
	
	logstr(LF_LOG,"WriteProfileStringA(%s,%s,%s)\n",
		entrystr,sectionstr,lpstrstr);
	return WritePrivateProfileString(entrystr,sectionstr,lpstrstr,lpfilestr);
}

BOOL
WriteProfileSectionA(LPCSTR lpapp,LPCSTR lpstr)
{
	return WritePrivateProfileSection(lpapp,lpstr,0);
}

BOOL
WriteProfileSectionW(LPCWSTR lpapp,LPCWSTR lpstr)
{
	return WritePrivateProfileSectionW(lpapp,lpstr,0);
}

BOOL WriteProfileStringA(LPCSTR lpapp,LPCSTR lpkey,LPCSTR lpstr)
{
	logstr(LF_LOG,"WriteProfileStringA(%s,%s,%s)\n",
		lpapp,lpkey,lpstr);
	return WritePrivateProfileString(lpapp,lpkey,lpstr,0);
}

BOOL WriteProfileStringW(LPCWSTR lpapp,LPCWSTR lpkey,LPCWSTR lpstr)
{
	char   entrystr[80];
	char   sectionstr[80];
	char   lpstrstr[80];

	entrystr[0] = 0;
	sectionstr[0] = 0;
	lpstrstr[0] = 0;
	WideCharToMultiByte(0,0,lpapp,0,entrystr,80,0,0);
	WideCharToMultiByte(0,0,lpkey,0,sectionstr,80,0,0);
	WideCharToMultiByte(0,0,lpstr,0,lpstrstr,80,0,0);
	
	logstr(LF_LOG,"WriteProfileStringA(%s,%s,%s)\n",
		entrystr,sectionstr,lpstrstr);
	return WritePrivateProfileString(entrystr,sectionstr,lpstrstr,0);
}

