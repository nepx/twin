
/*
 *   build xxxBin.c, and xxxBin.h
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXAPI 	4096

struct {
	int  n;
	char *spec;
	int  args;
	char *ifname;
	char *winapi;
	char *altapi;
	char *ext;
	char *api;
} API[MAXAPI];

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
	if(t) *t++ = 0;
	
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
main(int argc,char **argv)
{
	char *base = argv[2];	
	char *name = argv[3];	
	FILE *fp1,*fp2,*fp3,*fp4,*fp5;
	int  n;
	char filename[80];
	FILE *fp;
	char ifname[80];
	char winapi[80];
	char altapi[80];
	int  max = 0;

	sprintf(filename,"%s",argv[1]);
	fp1 = fopen(filename,"r");

	sprintf(filename,"%sBin.c",base);
	fp2 = fopen(filename,"w+");
	
	sprintf(filename,"%sBin.h",base);
	fp3 = fopen(filename,"w+");

	fp = fp2;	

	fprintf(fp,"\n");
	fprintf(fp,"#include \"Bin32.h\"\n");
	fprintf(fp,"#include \"%s\"\n",filename);
	fprintf(fp,"#include \"platform.h\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"unsigned long Win32Trap();\n");
	fprintf(fp,"\n");
	fprintf(fp,"MAPTABLE MapTable%s[] = {\n",name);
	fprintf(fp,"#ifdef TWIN_BIN32\n");
	fprintf(fp,"\n");

	sprintf(filename,"%s.c",base);
	fp4 = fopen(filename,"w+");

	sprintf(filename,"%s.h",base);
	fp5 = fopen(filename,"w+");

	fp = fp3;
	fprintf(fp,"\n");

	fp = fp1;

	while (fgets(filename,80,fp)) {
		char *spec,*args,*api,*index,*alt,*prefix;

		api = strchr(filename,'\n');
		if(api) *api = 0;

		if(strlen(filename) == 0)
			continue;
		if(strncmp(filename,"stub",4) == 0)
			continue;
		if(filename[0] == '#')
			continue;
	
		spec  = filename;
		args  = mktoken(filename);
		api   = mktoken(args);
		index = mktoken(api);
		alt   = mktoken(index);
		
		prefix = "IT32_";
		if(spec && strcmp(spec,"cdecl") == 0) {
			prefix = "";
		}
		if(api == 0)	api = "";
		if(index == 0)	index = "";

		sprintf(ifname,"%s%s",prefix,api);
		sprintf(winapi,"%s",alt?alt:api);
		sprintf(altapi,"%s",api);

		n = atoi(index);
		if(n > MAXAPI) {
			fprintf(stderr,"too big %d\n",n);
			continue;
		}
		if(API[n].ifname) {
			fprintf(stderr,"duplicate %d %s\n",
				n,API[n].ifname);
			continue;
		}

		API[n].spec  = strdup(spec);
		API[n].args  = atoi(args);
		API[n].ifname  = strdup(ifname);
		API[n].winapi  = strdup(winapi);
		API[n].altapi = strdup(altapi);
		API[n].api = strdup(api);

		if(n > max) max = n;
	}

	for(n=0;n<=max;n++) {
		
		fp = fp2;
			
		if(API[n].ifname == 0) {
			fprintf(fp,"   { /* %d */ Win32Trap, 0, \"\" },\n", n); 
			continue;
		}

		if ( strcmp(API[n].spec,"stdcall") == 0) {
			fprintf(fp,"   { /* %d */  %s, %s, \"%s\"},\n",
				n,API[n].ifname,API[n].winapi,API[n].altapi);
		} else {
			fprintf(fp,"   { /* %d */  %s, %s, \"%s\"},\n",
				n,API[n].winapi,"0",API[n].altapi);
		}


		fp = fp3;

		if ( strcmp(API[n].spec,"stdcall") == 0) {
			if(strcmp(API[n].ifname,API[n].winapi))
				fprintf(fp,"unsigned long %s();\n",
					API[n].ifname);
		}

		fprintf(fp,"unsigned long %s();\n",
				API[n].winapi);
			
	}

	fp = fp2;
	fprintf(fp,"   { /* %d */ 0,0} \n",max+1);
	fprintf(fp,"#endif\n");
	fprintf(fp,"};\n");

	fp = fp4;	

	fprintf(fp, "\n"); 
	fprintf(fp, "#include \"if32.h\"\n");
	fprintf(fp, "#include \"%s.h\"\n",base);
	fprintf(fp, "#include \"platform.h\"\n");
	fprintf(fp, "\n"); 
	fprintf(fp, "#ifdef TWIN_BIN32\n");
	fprintf(fp, "\n"); 

	fp = fp5;	
	fprintf(fp, "\n"); 
	fprintf(fp, "#include \"Bin32.h\"\n");
	fprintf(fp, "\n");
	fprintf(fp, "extern MAPTABLE MapTable%s[];\n",name);
	fprintf(fp, "\n");

	for(n = 0;n<=max;n++) {	

		fp = fp5;
		if (API[n].spec == 0)
			continue;

		fprintf(fp, "void %s();\n",
				API[n].winapi);

		fp = fp4;
		if ( strcmp(API[n].spec,"stdcall") == 0) {
			fprintf(fp, "void\n");
			fprintf(fp, "IT32_%s()\n",
					API[n].api);
			fprintf(fp, "{\n");
			fprintf(fp,"    if32_stdcall_%d(MapTable%s[%d].maddr);\n",
					API[n].args,name,n);
			fprintf(fp, "}\n");
			fprintf(fp, "\n"); 
		}
	}

	fp = fp4;
	fprintf(fp, "#endif\n");
	return 0;
}


