
#include "stdio.h"
#include "dlfcn.h"

typedef void (*f)();

/* dltest
 * test functionality of dlopen(), dlsym() and dladdr()
 * 
 * dlopen()  test, w/ and w/o .init function
 * dlsym()   test, can we retrieve address from symbol
 * dladdr()  test, given an address, can we find where its from
 */

int
main(int argc,char **argv)
{
	void *hso;
	f     fp;	
	int  *p;
	Dl_info d;

	printf("dlopen tests\n");
	hso = dlopen("./lib1.so",RTLD_GLOBAL|RTLD_LAZY);

	if(hso) {
		char *entry = "TWIN_LibEntry_lib1";
 		fp = (f) dlsym(hso,	entry);

		printf("test1: passed: dlopen(library): no .init function\n");
		
		if(fp) {
			(*fp)();
			printf("test 2: passed?: dlsym().init function called\n");
		} else
			printf("test 2 failed\n");
		
		dlclose(hso);
	} else 
		printf("test 1 failed\n");

	hso = dlopen("./lib2.so",RTLD_GLOBAL|RTLD_LAZY);

	if(hso) {
		char *entry = "TWIN_LibEntry_common";
		printf("test3: passed?: verify lib2 .init function called\n");

 		fp = (f) dlsym(hso,	entry);
		
		if(fp) {
			(*fp)();
			printf("test4: passed?: verify lib2 common function called\n");
		} else 
			printf("test 4 failed\n");
	
		entry = "hInstance";
 		p = (int *) dlsym(hso,	entry);
	
		printf("test5: passed?: verify %d == 2\n",*p);
		
		memset(&d,0,sizeof(Dl_info));
		dladdr(p,&d);
		printf("test6: passed?: verify %s == hInstance\n",
			d.dli_sname);
		printf("file=%s dladdr=%p address=%p\n",
			d.dli_fname,d.dli_saddr,p);
		
		dlclose(hso);
	}
	return 0;
}
