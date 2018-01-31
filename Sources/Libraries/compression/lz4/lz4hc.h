
#pragma once


#if defined (__cplusplus)
extern "C" {
#endif


int LZ4_compressHC (const char* source, char* dest, int inputSize);


int LZ4_compressHC_limitedOutput (const char* source, char* dest, int inputSize, int maxOutputSize);



int LZ4_compressHC2 (const char* source, char* dest, int inputSize, int compressionLevel);
int LZ4_compressHC2_limitedOutput (const char* source, char* dest, int inputSize, int maxOutputSize, int compressionLevel);






int LZ4_sizeofStateHC(void);
int LZ4_compressHC_withStateHC               (void* state, const char* source, char* dest, int inputSize);
int LZ4_compressHC_limitedOutput_withStateHC (void* state, const char* source, char* dest, int inputSize, int maxOutputSize);

int LZ4_compressHC2_withStateHC              (void* state, const char* source, char* dest, int inputSize, int compressionLevel);
int LZ4_compressHC2_limitedOutput_withStateHC(void* state, const char* source, char* dest, int inputSize, int maxOutputSize, int compressionLevel);






#include <stddef.h>   



#define LZ4_STREAMHCSIZE        262192
#define LZ4_STREAMHCSIZE_SIZET (LZ4_STREAMHCSIZE / sizeof(size_t))
typedef struct { size_t table[LZ4_STREAMHCSIZE_SIZET]; } LZ4_streamHC_t;



LZ4_streamHC_t* LZ4_createStreamHC(void);
int             LZ4_freeStreamHC (LZ4_streamHC_t* LZ4_streamHCPtr);


void LZ4_resetStreamHC (LZ4_streamHC_t* LZ4_streamHCPtr, int compressionLevel);
int  LZ4_loadDictHC (LZ4_streamHC_t* LZ4_streamHCPtr, const char* dictionary, int dictSize);

int LZ4_compressHC_continue (LZ4_streamHC_t* LZ4_streamHCPtr, const char* source, char* dest, int inputSize);
int LZ4_compressHC_limitedOutput_continue (LZ4_streamHC_t* LZ4_streamHCPtr, const char* source, char* dest, int inputSize, int maxOutputSize);

int LZ4_saveDictHC (LZ4_streamHC_t* LZ4_streamHCPtr, char* safeBuffer, int maxDictSize);







void* LZ4_createHC (const char* inputBuffer);
char* LZ4_slideInputBufferHC (void* LZ4HC_Data);
int   LZ4_freeHC (void* LZ4HC_Data);

int   LZ4_compressHC2_continue (void* LZ4HC_Data, const char* source, char* dest, int inputSize, int compressionLevel);
int   LZ4_compressHC2_limitedOutput_continue (void* LZ4HC_Data, const char* source, char* dest, int inputSize, int maxOutputSize, int compressionLevel);

int   LZ4_sizeofStreamStateHC(void);
int   LZ4_resetStreamStateHC(void* state, const char* inputBuffer);


#if defined (__cplusplus)
}
#endif
