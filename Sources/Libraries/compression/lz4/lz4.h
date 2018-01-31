
#pragma once

#if defined (__cplusplus)
extern "C" {
#endif




#define LZ4_VERSION_MAJOR    1    
#define LZ4_VERSION_MINOR    6    
#define LZ4_VERSION_RELEASE  0    
#define LZ4_VERSION_NUMBER (LZ4_VERSION_MAJOR *100*100 + LZ4_VERSION_MINOR *100 + LZ4_VERSION_RELEASE)
int LZ4_versionNumber (void);



#define LZ4_MEMORY_USAGE 14




int LZ4_compress        (const char* source, char* dest, int sourceSize);
int LZ4_decompress_safe (const char* source, char* dest, int compressedSize, int maxDecompressedSize);





#define LZ4_MAX_INPUT_SIZE        0x7E000000   
#define LZ4_COMPRESSBOUND(isize)  ((unsigned int)(isize) > (unsigned int)LZ4_MAX_INPUT_SIZE ? 0 : (isize) + ((isize)/255) + 16)


int LZ4_compressBound(int isize);



int LZ4_compress_limitedOutput (const char* source, char* dest, int sourceSize, int maxOutputSize);



int LZ4_sizeofState(void);
int LZ4_compress_withState               (void* state, const char* source, char* dest, int inputSize);
int LZ4_compress_limitedOutput_withState (void* state, const char* source, char* dest, int inputSize, int maxOutputSize);



int LZ4_decompress_fast (const char* source, char* dest, int originalSize);



int LZ4_decompress_safe_partial (const char* source, char* dest, int compressedSize, int targetOutputSize, int maxDecompressedSize);




#define LZ4_STREAMSIZE_U64 ((1 << (LZ4_MEMORY_USAGE-3)) + 4)
#define LZ4_STREAMSIZE     (LZ4_STREAMSIZE_U64 * sizeof(long long))

typedef struct { long long table[LZ4_STREAMSIZE_U64]; } LZ4_stream_t;


void LZ4_resetStream (LZ4_stream_t* LZ4_streamPtr);


LZ4_stream_t* LZ4_createStream(void);
int           LZ4_freeStream (LZ4_stream_t* LZ4_streamPtr);


int LZ4_loadDict (LZ4_stream_t* LZ4_streamPtr, const char* dictionary, int dictSize);


int LZ4_compress_continue (LZ4_stream_t* LZ4_streamPtr, const char* source, char* dest, int inputSize);


int LZ4_compress_limitedOutput_continue (LZ4_stream_t* LZ4_streamPtr, const char* source, char* dest, int inputSize, int maxOutputSize);


int LZ4_saveDict (LZ4_stream_t* LZ4_streamPtr, char* safeBuffer, int dictSize);




#define LZ4_STREAMDECODESIZE_U64  4
#define LZ4_STREAMDECODESIZE     (LZ4_STREAMDECODESIZE_U64 * sizeof(unsigned long long))
typedef struct { unsigned long long table[LZ4_STREAMDECODESIZE_U64]; } LZ4_streamDecode_t;

LZ4_streamDecode_t* LZ4_createStreamDecode(void);
int                 LZ4_freeStreamDecode (LZ4_streamDecode_t* LZ4_stream);


int LZ4_setStreamDecode (LZ4_streamDecode_t* LZ4_streamDecode, const char* dictionary, int dictSize);


int LZ4_decompress_safe_continue (LZ4_streamDecode_t* LZ4_streamDecode, const char* source, char* dest, int compressedSize, int maxDecompressedSize);
int LZ4_decompress_fast_continue (LZ4_streamDecode_t* LZ4_streamDecode, const char* source, char* dest, int originalSize);



int LZ4_decompress_safe_usingDict (const char* source, char* dest, int compressedSize, int maxDecompressedSize, const char* dictStart, int dictSize);
int LZ4_decompress_fast_usingDict (const char* source, char* dest, int originalSize, const char* dictStart, int dictSize);










void* LZ4_create (const char* inputBuffer);
int   LZ4_sizeofStreamState(void);
int   LZ4_resetStreamState(void* state, const char* inputBuffer);
char* LZ4_slideInputBuffer (void* state);


int LZ4_decompress_safe_withPrefix64k (const char* source, char* dest, int compressedSize, int maxOutputSize);
int LZ4_decompress_fast_withPrefix64k (const char* source, char* dest, int originalSize);


#if defined (__cplusplus)
}
#endif
