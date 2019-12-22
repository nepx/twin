
#include "stdio.h"

int	hInstance = 1;
void
TWIN_LibEntry_lib1()
{
	printf("lib1 .init function called\n");
}

void
TWIN_LibEntry_common()
{
	printf("lib1 TWIN_LibEntry_common function called\n");
}
