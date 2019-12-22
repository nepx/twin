
#include "Bin32.h"
#include "version32Bin.h"
#include "platform.h"

unsigned long Win32Trap();

MAPTABLE MapTableVERSION[] = {
#ifdef TWIN_BIN32

   { /* 0 */  IT32_GetFileVersionInfoA, GetFileVersionInfo, "GetFileVersionInfoA"},
   { /* 1 */  IT32_GetFileVersionInfoSizeA, GetFileVersionInfoSize, "GetFileVersionInfoSizeA"},
   { /* 2 */ Win32Trap, 0, "" },
   { /* 3 */ Win32Trap, 0, "" },
   { /* 4 */ Win32Trap, 0, "" },
   { /* 5 */ Win32Trap, 0, "" },
   { /* 6 */ Win32Trap, 0, "" },
   { /* 7 */ Win32Trap, 0, "" },
   { /* 8 */ Win32Trap, 0, "" },
   { /* 9 */ Win32Trap, 0, "" },
   { /* 10 */ Win32Trap, 0, "" },
   { /* 11 */  IT32_VerQueryValueA, VerQueryValue, "VerQueryValueA"},
   { /* 12 */ Win32Trap, 0, "" },
   { /* 13 */ Win32Trap, 0, "" },
   { /* 14 */ Win32Trap, 0, "" },
   { /* 15 */  IT32_Version32, Version32, "Version32"},
   { /* 16 */ 0,0} 
#endif
};
