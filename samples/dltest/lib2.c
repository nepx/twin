
#include "stdio.h"

#define TWIN_GCCINIT
#define LIBENTRY_DECL(entry) void entry() __attribute__ ((constructor))

LIBENTRY_DECL(TWIN_LibEntry_lib2);

int	hInstance = 2;

void
TWIN_LibEntry_lib2()
{
	printf("lib2 .init function called\n");
	return;
}

void
TWIN_LibEntry_common()
{
	printf("lib2 TWIN_LibEntry_common function called\n");
}
