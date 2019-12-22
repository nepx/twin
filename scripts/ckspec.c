
/*
 *   build xxxBin.c, and xxxBin.h
 */

#include "stdio.h"

#define MAXAPI 	4096

/* terminate p, and return next string start */

char *
mktoken(char *p)
{
	char *t;

	/* find end of p */
	for(t = p;t && *t;t++) {
		if(*t == ' ') {
			break;
		}
		if(*t == '\t') {
			break;
		}
	}
	/* terminate this one */
	if(t && *t) *t++ = 0;
	
	/* find next one... */
	for(;t && *t;t++) {
		if(*t == ' ')
			continue;
		if(*t == '\t')
			continue;
		break;
	}
	if(t && *t == 0)
		return 0;
	return t;
}

int
countargs(char *p)
{
	int count = 0;
	
	while(p && *p) {
		p = mktoken(p);
		count++;
	}

	return count;
}

int
main(int argc,char **argv)
{
	char *base = argv[1];	
	char *api = 0;
	char *ord = 0;

	FILE *fp,*fp1;
	char filename[256];
	char instr[256];
	char *token;
	char *lpstr;
	char *apitype;
	char *apiname;
	char *arglist;
	int  nargs;
	int  output = 0;

	int  n;
	int  max = 0;
	int  ordinal;

	if(argc > 2)
		api = argv[2];
	if(argc > 3)
		ord = argv[3];

	sprintf(filename,"%s",base);
	if(strcmp(filename,"-") == 0)
		fp1 = stdin;
	else
		fp1 = fopen(filename,"r");
	if(fp1 == 0) {
		printf("error opening %s\n",
			filename);
		return 0;
	}

	fp = fp1;
	while (fgets(instr,256,fp)) {
		char *eol;

		eol = (char *)strchr(instr,'\n');
		if(eol) *eol = 0;

		if(strlen(instr) == 0)
			continue;

		if(instr[0] == '#')
			continue;
	
		token = instr;
		lpstr  = mktoken(token);
		if(strlen(token) == 0) {
			token = lpstr;
			lpstr = mktoken(token);
		}

		if(strcmp(token,"name") == 0)
			continue;
		if(strcmp(token,"type") == 0)
			continue;

//  5 stdcall AppendMenuA(long long long ptr) AppendMenu32A

		ordinal = atoi(token);
		apitype = lpstr;
		apiname = mktoken(apitype);

		if(strcmp(apitype,"stub") == 0) {
			continue;
		}

		arglist = strchr(apiname,'(');
		if(arglist == 0) {
			printf("malformed %s [%s]\n",apiname,lpstr);
			continue;
		}
		if(arglist) *arglist++ = 0;

		lpstr = strchr(arglist,')');
		while(lpstr == 0) {
			char extra[80];
			char *eol;
	
			eol = (char *)strchr(extra,'\n');
			if(eol) *eol = 0;

			fgets(extra,80,fp);

			strcat(arglist,extra);	
			lpstr = strchr(arglist,')');
		
		}
		if(lpstr)   *lpstr++   = 0;

		mktoken(apiname);

		{
			char *apistr = api;
			if(api == 0)
				apistr = apiname;
			if(strcmp(apistr,apiname) == 0) {
				nargs = countargs(arglist);

				if(ord)
					printf("%s %d %s %s\n",apitype,nargs,apiname,ord);
				else
					printf("%s %d %s %d\n",apitype,nargs,apiname,ordinal);
				output++;
				if(api)
					break;
			}
		}
	}
	if(api) {
		if(output == 0 ) {
			printf("stub %s %s WINESTUB\n",api,ord);
			return 0;
		}
	}
	return -1;
}
