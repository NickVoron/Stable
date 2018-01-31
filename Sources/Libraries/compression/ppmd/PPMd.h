
#if !defined(_PPMD_H_)
#define _PPMD_H_

#include "PPMdType.h"

#ifdef  __cplusplus
extern "C" {
#endif

BOOL  _STDCALL StartSubAllocator(UINT SubAllocatorSize);
void  _STDCALL StopSubAllocator();          
DWORD _STDCALL GetUsedMemory();             


enum MR_METHOD { MRM_RESTART, MRM_CUT_OFF, MRM_FREEZE };


void _STDCALL EncodeFile(_PPMD_FILE* EncodedFile,_PPMD_FILE* DecodedFile,
                        int MaxOrder,MR_METHOD MRMethod);
void _STDCALL DecodeFile(_PPMD_FILE* DecodedFile,_PPMD_FILE* EncodedFile,
                        int MaxOrder,MR_METHOD MRMethod);


void _STDCALL  PrintInfo(_PPMD_FILE* DecodedFile,_PPMD_FILE* EncodedFile);

#ifdef  __cplusplus
}
#endif

#endif 
