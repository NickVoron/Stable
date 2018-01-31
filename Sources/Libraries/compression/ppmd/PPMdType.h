
#if !defined(_PPMDTYPE_H_)
#define _PPMDTYPE_H_

#include <stdio.h>

#define _WIN32_ENVIRONMENT_



#if defined(_WIN32_ENVIRONMENT_)+defined(_DOS32_ENVIRONMENT_)+defined(_POSIX_ENVIRONMENT_)+defined(_UNKNOWN_ENVIRONMENT_) != 1
#error Only one environment must be defined
#endif 

#if defined(_WIN32_ENVIRONMENT_)
#include <windows.h>
#else 
typedef int   BOOL;
#define FALSE 0
#define TRUE  1
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   UINT;
#endif 

const DWORD PPMdSignature=0x84ACAF8F, Variant='I';
const int MAX_O=16;                         

#define _USE_PREFETCHING                    

#if !defined(_UNKNOWN_ENVIRONMENT_) && !defined(__GNUC__)
#define _FASTCALL __fastcall
#define _STDCALL  __stdcall
#else
#define _FASTCALL
#define _STDCALL
#endif 

#if defined(__GNUC__)
#define _PACK_ATTR __attribute__ ((packed))
#else 
#define _PACK_ATTR
#endif 


typedef FILE _PPMD_FILE;
#define _PPMD_E_GETC(fp)   getc(fp)
#define _PPMD_E_PUTC(c,fp) putc((c),fp)
#define _PPMD_D_GETC(fp)   getc(fp)
#define _PPMD_D_PUTC(c,fp) putc((c),fp)


#endif 
