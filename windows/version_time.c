/*    
 */
#include <time.h>
#include <stdio.h>
#include "platform.h"
#include "Release.h"

int
main(int argc,char **argv)
{
    short build;

    build = 0;			// sequence #
    if(argc > 1)
	build = (short) atoi(argv[1]);
 
    build <<= 4;
    build |= (VER_PATCH & 0xf);
    build <<= 4;
    build |= (VER_MINOR & 0xf);
    build <<= 4;
    build |= (VER_MAJOR & 0xf);

    printf("/* build level %d */\n",build);
    printf("#define VER_BUILD 0x%x\n", build);
    return 0;
}

