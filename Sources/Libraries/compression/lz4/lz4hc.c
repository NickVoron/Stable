




static const int LZ4HC_compressionLevel_default = 9;



#include "lz4hc.h"



#if defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wunused-function"
#endif

#if defined (__clang__)
#  pragma clang diagnostic ignored "-Wunused-function"
#endif



#define LZ4_COMMONDEFS_ONLY
#include "lz4.c"



#define DICTIONARY_LOGSIZE 16
#define MAXD (1<<DICTIONARY_LOGSIZE)
#define MAXD_MASK ((U32)(MAXD - 1))

#define HASH_LOG (DICTIONARY_LOGSIZE-1)
#define HASHTABLESIZE (1 << HASH_LOG)
#define HASH_MASK (HASHTABLESIZE - 1)

#define OPTIMAL_ML (int)((ML_MASK-1)+MINMATCH)

static const int g_maxCompressionLevel = 16;



typedef struct
{
    U32 hashTable[HASHTABLESIZE];
    U16   chainTable[MAXD];
    const BYTE* end;        
    const BYTE* base;       
    const BYTE* dictBase;   
    const BYTE* inputBuffer;
    U32   dictLimit;        
    U32   lowLimit;         
    U32   nextToUpdate;
    U32   compressionLevel;
} LZ4HC_Data_Structure;



#define HASH_FUNCTION(i)       (((i) * 2654435761U) >> ((MINMATCH*8)-HASH_LOG))
#define DELTANEXT(p)           chainTable[(size_t)(p) & MAXD_MASK]
#define GETNEXT(p)             ((p) - (size_t)DELTANEXT(p))

static U32 LZ4HC_hashPtr(const void* ptr) { return HASH_FUNCTION(LZ4_read32(ptr)); }




static void LZ4HC_init (LZ4HC_Data_Structure* hc4, const BYTE* start)
{
    MEM_INIT((void*)hc4->hashTable, 0, sizeof(hc4->hashTable));
    MEM_INIT(hc4->chainTable, 0xFF, sizeof(hc4->chainTable));
    hc4->nextToUpdate = 64 KB;
    hc4->base = start - 64 KB;
    hc4->inputBuffer = start;
    hc4->end = start;
    hc4->dictBase = start - 64 KB;
    hc4->dictLimit = 64 KB;
    hc4->lowLimit = 64 KB;
}



FORCE_INLINE void LZ4HC_Insert (LZ4HC_Data_Structure* hc4, const BYTE* ip)
{
    U16* chainTable = hc4->chainTable;
    U32* HashTable  = hc4->hashTable;
    const BYTE* const base = hc4->base;
    const U32 target = (U32)(ip - base);
    U32 idx = hc4->nextToUpdate;

    while(idx < target)
    {
        U32 h = LZ4HC_hashPtr(base+idx);
        size_t delta = idx - HashTable[h];
        if (delta>MAX_DISTANCE) delta = MAX_DISTANCE;
        chainTable[idx & 0xFFFF] = (U16)delta;
        HashTable[h] = idx;
        idx++;
    }

    hc4->nextToUpdate = target;
}


FORCE_INLINE int LZ4HC_InsertAndFindBestMatch (LZ4HC_Data_Structure* hc4,   
                                               const BYTE* ip, const BYTE* const iLimit,
                                               const BYTE** matchpos,
                                               const int maxNbAttempts)
{
    U16* const chainTable = hc4->chainTable;
    U32* const HashTable = hc4->hashTable;
    const BYTE* const base = hc4->base;
    const BYTE* const dictBase = hc4->dictBase;
    const U32 dictLimit = hc4->dictLimit;
    const U32 lowLimit = (hc4->lowLimit + 64 KB > (U32)(ip-base)) ? hc4->lowLimit : (U32)(ip - base) - (64 KB - 1);
    U32 matchIndex;
    const BYTE* match;
    int nbAttempts=maxNbAttempts;
    size_t ml=0;

    
    LZ4HC_Insert(hc4, ip);
    matchIndex = HashTable[LZ4HC_hashPtr(ip)];

    while ((matchIndex>=lowLimit) && (nbAttempts))
    {
        nbAttempts--;
        if (matchIndex >= dictLimit)
        {
            match = base + matchIndex;
            if (*(match+ml) == *(ip+ml)
                && (LZ4_read32(match) == LZ4_read32(ip)))
            {
                size_t mlt = LZ4_count(ip+MINMATCH, match+MINMATCH, iLimit) + MINMATCH;
                if (mlt > ml) { ml = mlt; *matchpos = match; }
            }
        }
        else
        {
            match = dictBase + matchIndex;
            if (LZ4_read32(match) == LZ4_read32(ip))
            {
                size_t mlt;
                const BYTE* vLimit = ip + (dictLimit - matchIndex);
                if (vLimit > iLimit) vLimit = iLimit;
                mlt = LZ4_count(ip+MINMATCH, match+MINMATCH, vLimit) + MINMATCH;
                if ((ip+mlt == vLimit) && (vLimit < iLimit))
                    mlt += LZ4_count(ip+mlt, base+dictLimit, iLimit);
                if (mlt > ml) { ml = mlt; *matchpos = base + matchIndex; }   
            }
        }
        matchIndex -= chainTable[matchIndex & 0xFFFF];
    }

    return (int)ml;
}


FORCE_INLINE int LZ4HC_InsertAndGetWiderMatch (
    LZ4HC_Data_Structure* hc4,
    const BYTE* const ip,
    const BYTE* const iLowLimit,
    const BYTE* const iHighLimit,
    int longest,
    const BYTE** matchpos,
    const BYTE** startpos,
    const int maxNbAttempts)
{
    U16* const chainTable = hc4->chainTable;
    U32* const HashTable = hc4->hashTable;
    const BYTE* const base = hc4->base;
    const U32 dictLimit = hc4->dictLimit;
    const BYTE* const lowPrefixPtr = base + dictLimit;
    const U32 lowLimit = (hc4->lowLimit + 64 KB > (U32)(ip-base)) ? hc4->lowLimit : (U32)(ip - base) - (64 KB - 1);
    const BYTE* const dictBase = hc4->dictBase;
    U32   matchIndex;
    int nbAttempts = maxNbAttempts;
    int delta = (int)(ip-iLowLimit);


    
    LZ4HC_Insert(hc4, ip);
    matchIndex = HashTable[LZ4HC_hashPtr(ip)];

    while ((matchIndex>=lowLimit) && (nbAttempts))
    {
        nbAttempts--;
        if (matchIndex >= dictLimit)
        {
            const BYTE* matchPtr = base + matchIndex;
            if (*(iLowLimit + longest) == *(matchPtr - delta + longest))
                if (LZ4_read32(matchPtr) == LZ4_read32(ip))
                {
                    int mlt = MINMATCH + LZ4_count(ip+MINMATCH, matchPtr+MINMATCH, iHighLimit);
                    int back = 0;

                    while ((ip+back>iLowLimit)
                           && (matchPtr+back > lowPrefixPtr)
                           && (ip[back-1] == matchPtr[back-1]))
                            back--;

                    mlt -= back;

                    if (mlt > longest)
                    {
                        longest = (int)mlt;
                        *matchpos = matchPtr+back;
                        *startpos = ip+back;
                    }
                }
        }
        else
        {
            const BYTE* matchPtr = dictBase + matchIndex;
            if (LZ4_read32(matchPtr) == LZ4_read32(ip))
            {
                size_t mlt;
                int back=0;
                const BYTE* vLimit = ip + (dictLimit - matchIndex);
                if (vLimit > iHighLimit) vLimit = iHighLimit;
                mlt = LZ4_count(ip+MINMATCH, matchPtr+MINMATCH, vLimit) + MINMATCH;
                if ((ip+mlt == vLimit) && (vLimit < iHighLimit))
                    mlt += LZ4_count(ip+mlt, base+dictLimit, iHighLimit);
                while ((ip+back > iLowLimit) && (matchIndex+back > lowLimit) && (ip[back-1] == matchPtr[back-1])) back--;
                mlt -= back;
                if ((int)mlt > longest) { longest = (int)mlt; *matchpos = base + matchIndex + back; *startpos = ip+back; }
            }
        }
        matchIndex -= chainTable[matchIndex & 0xFFFF];
    }

    return longest;
}


typedef enum { noLimit = 0, limitedOutput = 1 } limitedOutput_directive;

#define LZ4HC_DEBUG 0
#if LZ4HC_DEBUG
static unsigned debug = 0;
#endif

FORCE_INLINE int LZ4HC_encodeSequence (
    const BYTE** ip,
    BYTE** op,
    const BYTE** anchor,
    int matchLength,
    const BYTE* const match,
    limitedOutput_directive limitedOutputBuffer,
    BYTE* oend)
{
    int length;
    BYTE* token;

#if LZ4HC_DEBUG
    if (debug) printf("literal : %u  --  match : %u  --  offset : %u\n", (U32)(*ip - *anchor), (U32)matchLength, (U32)(*ip-match));
#endif

    
    length = (int)(*ip - *anchor);
    token = (*op)++;
    if ((limitedOutputBuffer) && ((*op + (length>>8) + length + (2 + 1 + LASTLITERALS)) > oend)) return 1;   
    if (length>=(int)RUN_MASK) { int len; *token=(RUN_MASK<<ML_BITS); len = length-RUN_MASK; for(; len > 254 ; len-=255) *(*op)++ = 255;  *(*op)++ = (BYTE)len; }
    else *token = (BYTE)(length<<ML_BITS);

    
    LZ4_wildCopy(*op, *anchor, (*op) + length);
    *op += length;

    
    LZ4_writeLE16(*op, (U16)(*ip-match)); *op += 2;

    
    length = (int)(matchLength-MINMATCH);
    if ((limitedOutputBuffer) && (*op + (length>>8) + (1 + LASTLITERALS) > oend)) return 1;   
    if (length>=(int)ML_MASK) { *token+=ML_MASK; length-=ML_MASK; for(; length > 509 ; length-=510) { *(*op)++ = 255; *(*op)++ = 255; } if (length > 254) { length-=255; *(*op)++ = 255; } *(*op)++ = (BYTE)length; }
    else *token += (BYTE)(length);

    
    *ip += matchLength;
    *anchor = *ip;

    return 0;
}


static int LZ4HC_compress_generic (
    void* ctxvoid,
    const char* source,
    char* dest,
    int inputSize,
    int maxOutputSize,
    int compressionLevel,
    limitedOutput_directive limit
    )
{
    LZ4HC_Data_Structure* ctx = (LZ4HC_Data_Structure*) ctxvoid;
    const BYTE* ip = (const BYTE*) source;
    const BYTE* anchor = ip;
    const BYTE* const iend = ip + inputSize;
    const BYTE* const mflimit = iend - MFLIMIT;
    const BYTE* const matchlimit = (iend - LASTLITERALS);

    BYTE* op = (BYTE*) dest;
    BYTE* const oend = op + maxOutputSize;

    unsigned maxNbAttempts;
    int   ml, ml2, ml3, ml0;
    const BYTE* ref=NULL;
    const BYTE* start2=NULL;
    const BYTE* ref2=NULL;
    const BYTE* start3=NULL;
    const BYTE* ref3=NULL;
    const BYTE* start0;
    const BYTE* ref0;


    
    if (compressionLevel > g_maxCompressionLevel) compressionLevel = g_maxCompressionLevel;
    if (compressionLevel < 1) compressionLevel = LZ4HC_compressionLevel_default;
    maxNbAttempts = 1 << (compressionLevel-1);
    ctx->end += inputSize;

    ip++;

    
    while (ip < mflimit)
    {
        ml = LZ4HC_InsertAndFindBestMatch (ctx, ip, matchlimit, (&ref), maxNbAttempts);
        if (!ml) { ip++; continue; }

        
        start0 = ip;
        ref0 = ref;
        ml0 = ml;

_Search2:
        if (ip+ml < mflimit)
            ml2 = LZ4HC_InsertAndGetWiderMatch(ctx, ip + ml - 2, ip + 1, matchlimit, ml, &ref2, &start2, maxNbAttempts);
        else ml2 = ml;

        if (ml2 == ml)  
        {
            if (LZ4HC_encodeSequence(&ip, &op, &anchor, ml, ref, limit, oend)) return 0;
            continue;
        }

        if (start0 < ip)
        {
            if (start2 < ip + ml0)   
            {
                ip = start0;
                ref = ref0;
                ml = ml0;
            }
        }

        
        if ((start2 - ip) < 3)   
        {
            ml = ml2;
            ip = start2;
            ref =ref2;
            goto _Search2;
        }

_Search3:
        
        if ((start2 - ip) < OPTIMAL_ML)
        {
            int correction;
            int new_ml = ml;
            if (new_ml > OPTIMAL_ML) new_ml = OPTIMAL_ML;
            if (ip+new_ml > start2 + ml2 - MINMATCH) new_ml = (int)(start2 - ip) + ml2 - MINMATCH;
            correction = new_ml - (int)(start2 - ip);
            if (correction > 0)
            {
                start2 += correction;
                ref2 += correction;
                ml2 -= correction;
            }
        }
        

        if (start2 + ml2 < mflimit)
            ml3 = LZ4HC_InsertAndGetWiderMatch(ctx, start2 + ml2 - 3, start2, matchlimit, ml2, &ref3, &start3, maxNbAttempts);
        else ml3 = ml2;

        if (ml3 == ml2) 
        {
            
            if (start2 < ip+ml)  ml = (int)(start2 - ip);
            
            if (LZ4HC_encodeSequence(&ip, &op, &anchor, ml, ref, limit, oend)) return 0;
            ip = start2;
            if (LZ4HC_encodeSequence(&ip, &op, &anchor, ml2, ref2, limit, oend)) return 0;
            continue;
        }

        if (start3 < ip+ml+3) 
        {
            if (start3 >= (ip+ml)) 
            {
                if (start2 < ip+ml)
                {
                    int correction = (int)(ip+ml - start2);
                    start2 += correction;
                    ref2 += correction;
                    ml2 -= correction;
                    if (ml2 < MINMATCH)
                    {
                        start2 = start3;
                        ref2 = ref3;
                        ml2 = ml3;
                    }
                }

                if (LZ4HC_encodeSequence(&ip, &op, &anchor, ml, ref, limit, oend)) return 0;
                ip  = start3;
                ref = ref3;
                ml  = ml3;

                start0 = start2;
                ref0 = ref2;
                ml0 = ml2;
                goto _Search2;
            }

            start2 = start3;
            ref2 = ref3;
            ml2 = ml3;
            goto _Search3;
        }

        
        if (start2 < ip+ml)
        {
            if ((start2 - ip) < (int)ML_MASK)
            {
                int correction;
                if (ml > OPTIMAL_ML) ml = OPTIMAL_ML;
                if (ip + ml > start2 + ml2 - MINMATCH) ml = (int)(start2 - ip) + ml2 - MINMATCH;
                correction = ml - (int)(start2 - ip);
                if (correction > 0)
                {
                    start2 += correction;
                    ref2 += correction;
                    ml2 -= correction;
                }
            }
            else
            {
                ml = (int)(start2 - ip);
            }
        }
        if (LZ4HC_encodeSequence(&ip, &op, &anchor, ml, ref, limit, oend)) return 0;

        ip = start2;
        ref = ref2;
        ml = ml2;

        start2 = start3;
        ref2 = ref3;
        ml2 = ml3;

        goto _Search3;
    }

    
    {
        int lastRun = (int)(iend - anchor);
        if ((limit) && (((char*)op - dest) + lastRun + 1 + ((lastRun+255-RUN_MASK)/255) > (U32)maxOutputSize)) return 0;  
        if (lastRun>=(int)RUN_MASK) { *op++=(RUN_MASK<<ML_BITS); lastRun-=RUN_MASK; for(; lastRun > 254 ; lastRun-=255) *op++ = 255; *op++ = (BYTE) lastRun; }
        else *op++ = (BYTE)(lastRun<<ML_BITS);
        memcpy(op, anchor, iend - anchor);
        op += iend-anchor;
    }

    
    return (int) (((char*)op)-dest);
}


int LZ4_compressHC2(const char* source, char* dest, int inputSize, int compressionLevel)
{
    LZ4HC_Data_Structure ctx;
    LZ4HC_init(&ctx, (const BYTE*)source);
    return LZ4HC_compress_generic (&ctx, source, dest, inputSize, 0, compressionLevel, noLimit);
}

int LZ4_compressHC(const char* source, char* dest, int inputSize) { return LZ4_compressHC2(source, dest, inputSize, 0); }

int LZ4_compressHC2_limitedOutput(const char* source, char* dest, int inputSize, int maxOutputSize, int compressionLevel)
{
    LZ4HC_Data_Structure ctx;
    LZ4HC_init(&ctx, (const BYTE*)source);
    return LZ4HC_compress_generic (&ctx, source, dest, inputSize, maxOutputSize, compressionLevel, limitedOutput);
}

int LZ4_compressHC_limitedOutput(const char* source, char* dest, int inputSize, int maxOutputSize)
{
    return LZ4_compressHC2_limitedOutput(source, dest, inputSize, maxOutputSize, 0);
}



int LZ4_sizeofStateHC(void) { return sizeof(LZ4HC_Data_Structure); }


int LZ4_compressHC2_withStateHC (void* state, const char* source, char* dest, int inputSize, int compressionLevel)
{
    if (((size_t)(state)&(sizeof(void*)-1)) != 0) return 0;   
    LZ4HC_init ((LZ4HC_Data_Structure*)state, (const BYTE*)source);
    return LZ4HC_compress_generic (state, source, dest, inputSize, 0, compressionLevel, noLimit);
}

int LZ4_compressHC_withStateHC (void* state, const char* source, char* dest, int inputSize)
{ return LZ4_compressHC2_withStateHC (state, source, dest, inputSize, 0); }


int LZ4_compressHC2_limitedOutput_withStateHC (void* state, const char* source, char* dest, int inputSize, int maxOutputSize, int compressionLevel)
{
    if (((size_t)(state)&(sizeof(void*)-1)) != 0) return 0;   
    LZ4HC_init ((LZ4HC_Data_Structure*)state, (const BYTE*)source);
    return LZ4HC_compress_generic (state, source, dest, inputSize, maxOutputSize, compressionLevel, limitedOutput);
}

int LZ4_compressHC_limitedOutput_withStateHC (void* state, const char* source, char* dest, int inputSize, int maxOutputSize)
{ return LZ4_compressHC2_limitedOutput_withStateHC (state, source, dest, inputSize, maxOutputSize, 0); }





LZ4_streamHC_t* LZ4_createStreamHC(void) { return (LZ4_streamHC_t*)malloc(sizeof(LZ4_streamHC_t)); }
int LZ4_freeStreamHC (LZ4_streamHC_t* LZ4_streamHCPtr) { free(LZ4_streamHCPtr); return 0; }



void LZ4_resetStreamHC (LZ4_streamHC_t* LZ4_streamHCPtr, int compressionLevel)
{
    LZ4_STATIC_ASSERT(sizeof(LZ4HC_Data_Structure) <= sizeof(LZ4_streamHC_t));   
    ((LZ4HC_Data_Structure*)LZ4_streamHCPtr)->base = NULL;
    ((LZ4HC_Data_Structure*)LZ4_streamHCPtr)->compressionLevel = (unsigned)compressionLevel;
}

int LZ4_loadDictHC (LZ4_streamHC_t* LZ4_streamHCPtr, const char* dictionary, int dictSize)
{
    LZ4HC_Data_Structure* ctxPtr = (LZ4HC_Data_Structure*) LZ4_streamHCPtr;
    if (dictSize > 64 KB)
    {
        dictionary += dictSize - 64 KB;
        dictSize = 64 KB;
    }
    LZ4HC_init (ctxPtr, (const BYTE*)dictionary);
    if (dictSize >= 4) LZ4HC_Insert (ctxPtr, (const BYTE*)dictionary +(dictSize-3));
    ctxPtr->end = (const BYTE*)dictionary + dictSize;
    return dictSize;
}




static void LZ4HC_setExternalDict(LZ4HC_Data_Structure* ctxPtr, const BYTE* newBlock)
{
    if (ctxPtr->end >= ctxPtr->base + 4)
        LZ4HC_Insert (ctxPtr, ctxPtr->end-3);   
    
    ctxPtr->lowLimit  = ctxPtr->dictLimit;
    ctxPtr->dictLimit = (U32)(ctxPtr->end - ctxPtr->base);
    ctxPtr->dictBase  = ctxPtr->base;
    ctxPtr->base = newBlock - ctxPtr->dictLimit;
    ctxPtr->end  = newBlock;
    ctxPtr->nextToUpdate = ctxPtr->dictLimit;   
}

static int LZ4_compressHC_continue_generic (LZ4HC_Data_Structure* ctxPtr,
                                            const char* source, char* dest,
                                            int inputSize, int maxOutputSize, limitedOutput_directive limit)
{
    
    if (ctxPtr->base == NULL)
        LZ4HC_init (ctxPtr, (const BYTE*) source);

    
    if ((size_t)(ctxPtr->end - ctxPtr->base) > 2 GB)
    {
        size_t dictSize = (size_t)(ctxPtr->end - ctxPtr->base) - ctxPtr->dictLimit;
        if (dictSize > 64 KB) dictSize = 64 KB;

        LZ4_loadDictHC((LZ4_streamHC_t*)ctxPtr, (const char*)(ctxPtr->end) - dictSize, (int)dictSize);
    }

    
    if ((const BYTE*)source != ctxPtr->end) LZ4HC_setExternalDict(ctxPtr, (const BYTE*)source);

    
    {
        const BYTE* sourceEnd = (const BYTE*) source + inputSize;
        const BYTE* dictBegin = ctxPtr->dictBase + ctxPtr->lowLimit;
        const BYTE* dictEnd   = ctxPtr->dictBase + ctxPtr->dictLimit;
        if ((sourceEnd > dictBegin) && ((BYTE*)source < dictEnd))
        {
            if (sourceEnd > dictEnd) sourceEnd = dictEnd;
            ctxPtr->lowLimit = (U32)(sourceEnd - ctxPtr->dictBase);
            if (ctxPtr->dictLimit - ctxPtr->lowLimit < 4) ctxPtr->lowLimit = ctxPtr->dictLimit;
        }
    }

    return LZ4HC_compress_generic (ctxPtr, source, dest, inputSize, maxOutputSize, ctxPtr->compressionLevel, limit);
}

int LZ4_compressHC_continue (LZ4_streamHC_t* LZ4_streamHCPtr, const char* source, char* dest, int inputSize)
{
    return LZ4_compressHC_continue_generic ((LZ4HC_Data_Structure*)LZ4_streamHCPtr, source, dest, inputSize, 0, noLimit);
}

int LZ4_compressHC_limitedOutput_continue (LZ4_streamHC_t* LZ4_streamHCPtr, const char* source, char* dest, int inputSize, int maxOutputSize)
{
    return LZ4_compressHC_continue_generic ((LZ4HC_Data_Structure*)LZ4_streamHCPtr, source, dest, inputSize, maxOutputSize, limitedOutput);
}




int LZ4_saveDictHC (LZ4_streamHC_t* LZ4_streamHCPtr, char* safeBuffer, int dictSize)
{
    LZ4HC_Data_Structure* streamPtr = (LZ4HC_Data_Structure*)LZ4_streamHCPtr;
    int prefixSize = (int)(streamPtr->end - (streamPtr->base + streamPtr->dictLimit));
    if (dictSize > 64 KB) dictSize = 64 KB;
    if (dictSize < 4) dictSize = 0;
    if (dictSize > prefixSize) dictSize = prefixSize;
    memcpy(safeBuffer, streamPtr->end - dictSize, dictSize);
    {
        U32 endIndex = (U32)(streamPtr->end - streamPtr->base);
        streamPtr->end = (const BYTE*)safeBuffer + dictSize;
        streamPtr->base = streamPtr->end - endIndex;
        streamPtr->dictLimit = endIndex - dictSize;
        streamPtr->lowLimit = endIndex - dictSize;
        if (streamPtr->nextToUpdate < streamPtr->dictLimit) streamPtr->nextToUpdate = streamPtr->dictLimit;
    }
    return dictSize;
}



int LZ4_sizeofStreamStateHC(void) { return LZ4_STREAMHCSIZE; }

int LZ4_resetStreamStateHC(void* state, const char* inputBuffer)
{
    if ((((size_t)state) & (sizeof(void*)-1)) != 0) return 1;   
    LZ4HC_init((LZ4HC_Data_Structure*)state, (const BYTE*)inputBuffer);
    return 0;
}

void* LZ4_createHC (const char* inputBuffer)
{
    void* hc4 = ALLOCATOR(1, sizeof(LZ4HC_Data_Structure));
    LZ4HC_init ((LZ4HC_Data_Structure*)hc4, (const BYTE*)inputBuffer);
    return hc4;
}

int LZ4_freeHC (void* LZ4HC_Data)
{
    FREEMEM(LZ4HC_Data);
    return (0);
}



int LZ4_compressHC2_continue (void* LZ4HC_Data, const char* source, char* dest, int inputSize, int compressionLevel)
{
    return LZ4HC_compress_generic (LZ4HC_Data, source, dest, inputSize, 0, compressionLevel, noLimit);
}

int LZ4_compressHC2_limitedOutput_continue (void* LZ4HC_Data, const char* source, char* dest, int inputSize, int maxOutputSize, int compressionLevel)
{
    return LZ4HC_compress_generic (LZ4HC_Data, source, dest, inputSize, maxOutputSize, compressionLevel, limitedOutput);
}

char* LZ4_slideInputBufferHC(void* LZ4HC_Data)
{
    LZ4HC_Data_Structure* hc4 = (LZ4HC_Data_Structure*)LZ4HC_Data;
    int dictSize = LZ4_saveDictHC((LZ4_streamHC_t*)LZ4HC_Data, (char*)(hc4->inputBuffer), 64 KB);
    return (char*)(hc4->inputBuffer + dictSize);
}
