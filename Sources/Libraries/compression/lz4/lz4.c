




#define HEAPMODE 0







#if defined(CPU_HAS_EFFICIENT_UNALIGNED_MEMORY_ACCESS) \
    || defined(__ARM_FEATURE_UNALIGNED) \
    || defined(__i386__) || defined(__x86_64__) \
    || defined(_M_IX86) || defined(_M_X64) \
    || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_8__) \
    || (defined(_M_ARM) && (_M_ARM >= 7))
#  define LZ4_UNALIGNED_ACCESS 1
#else
#  define LZ4_UNALIGNED_ACCESS 0
#endif


#if defined(_MSC_VER) && defined(_WIN32_WCE)   
#  define LZ4_FORCE_SW_BITCOUNT
#endif



#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)   

#else
#  define restrict 
#endif

#ifdef _MSC_VER    
#  define FORCE_INLINE static __forceinline
#  include <intrin.h>
#  pragma warning(disable : 4127)        
#  pragma warning(disable : 4293)        
#else
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)   
#    ifdef __GNUC__
#      define FORCE_INLINE static inline __attribute__((always_inline))
#    else
#      define FORCE_INLINE static inline
#    endif
#  else
#    define FORCE_INLINE static
#  endif   
#endif  

#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#if (GCC_VERSION >= 302) || (__INTEL_COMPILER >= 800) || defined(__clang__)
#  define expect(expr,value)    (__builtin_expect ((expr),(value)) )
#else
#  define expect(expr,value)    (expr)
#endif

#define likely(expr)     expect((expr) != 0, 1)
#define unlikely(expr)   expect((expr) != 0, 0)



#include <stdlib.h>   
#define ALLOCATOR(n,s) calloc(n,s)
#define FREEMEM        free
#include <string.h>   
#define MEM_INIT       memset



#include "lz4.h"



#if defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)   
# include <stdint.h>
  typedef  uint8_t BYTE;
  typedef uint16_t U16;
  typedef uint32_t U32;
  typedef  int32_t S32;
  typedef uint64_t U64;
#else
  typedef unsigned char       BYTE;
  typedef unsigned short      U16;
  typedef unsigned int        U32;
  typedef   signed int        S32;
  typedef unsigned long long  U64;
#endif



#define STEPSIZE sizeof(size_t)

static unsigned LZ4_64bits(void) { return sizeof(void*)==8; }

static unsigned LZ4_isLittleEndian(void)
{
    const union { U32 i; BYTE c[4]; } one = { 1 };   
    return one.c[0];
}


static U16 LZ4_readLE16(const void* memPtr)
{
    if ((LZ4_UNALIGNED_ACCESS) && (LZ4_isLittleEndian()))
        return *(U16*)memPtr;
    else
    {
        const BYTE* p = (const BYTE*)memPtr;
        return (U16)((U16)p[0] + (p[1]<<8));
    }
}

static void LZ4_writeLE16(void* memPtr, U16 value)
{
    if ((LZ4_UNALIGNED_ACCESS) && (LZ4_isLittleEndian()))
    {
        *(U16*)memPtr = value;
        return;
    }
    else
    {
        BYTE* p = (BYTE*)memPtr;
        p[0] = (BYTE) value;
        p[1] = (BYTE)(value>>8);
    }
}


static U16 LZ4_read16(const void* memPtr)
{
    if (LZ4_UNALIGNED_ACCESS)
        return *(U16*)memPtr;
    else
    {
        U16 val16;
        memcpy(&val16, memPtr, 2);
        return val16;
    }
}

static U32 LZ4_read32(const void* memPtr)
{
    if (LZ4_UNALIGNED_ACCESS)
        return *(U32*)memPtr;
    else
    {
        U32 val32;
        memcpy(&val32, memPtr, 4);
        return val32;
    }
}

static U64 LZ4_read64(const void* memPtr)
{
    if (LZ4_UNALIGNED_ACCESS)
        return *(U64*)memPtr;
    else
    {
        U64 val64;
        memcpy(&val64, memPtr, 8);
        return val64;
    }
}

static size_t LZ4_read_ARCH(const void* p)
{
    if (LZ4_64bits())
        return (size_t)LZ4_read64(p);
    else
        return (size_t)LZ4_read32(p);
}


static void LZ4_copy4(void* dstPtr, const void* srcPtr)
{
    if (LZ4_UNALIGNED_ACCESS)
    {
        *(U32*)dstPtr = *(U32*)srcPtr;
        return;
    }
    memcpy(dstPtr, srcPtr, 4);
}

static void LZ4_copy8(void* dstPtr, const void* srcPtr)
{
#if GCC_VERSION!=409  
    if (LZ4_UNALIGNED_ACCESS)
    {
        if (LZ4_64bits())
            *(U64*)dstPtr = *(U64*)srcPtr;
        else
            ((U32*)dstPtr)[0] = ((U32*)srcPtr)[0],
            ((U32*)dstPtr)[1] = ((U32*)srcPtr)[1];
        return;
    }
#endif
    memcpy(dstPtr, srcPtr, 8);
}


static void LZ4_wildCopy(void* dstPtr, const void* srcPtr, void* dstEnd)
{
    BYTE* d = (BYTE*)dstPtr;
    const BYTE* s = (const BYTE*)srcPtr;
    BYTE* e = (BYTE*)dstEnd;
    do { LZ4_copy8(d,s); d+=8; s+=8; } while (d<e);
}



#define MINMATCH 4

#define COPYLENGTH 8
#define LASTLITERALS 5
#define MFLIMIT (COPYLENGTH+MINMATCH)
static const int LZ4_minLength = (MFLIMIT+1);

#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)

#define MAXD_LOG 16
#define MAX_DISTANCE ((1 << MAXD_LOG) - 1)

#define ML_BITS  4
#define ML_MASK  ((1U<<ML_BITS)-1)
#define RUN_BITS (8-ML_BITS)
#define RUN_MASK ((1U<<RUN_BITS)-1)



#define LZ4_STATIC_ASSERT(c)    { enum { LZ4_static_assert = 1/(int)(!!(c)) }; }   



static unsigned LZ4_NbCommonBytes (register size_t val)
{
    if (LZ4_isLittleEndian())
    {
        if (LZ4_64bits())
        {
#       if defined(_MSC_VER) && defined(_WIN64) && !defined(LZ4_FORCE_SW_BITCOUNT)
            unsigned long r = 0;
            _BitScanForward64( &r, (U64)val );
            return (int)(r>>3);
#       elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
            return (__builtin_ctzll((U64)val) >> 3);
#       else
            static const int DeBruijnBytePos[64] = { 0, 0, 0, 0, 0, 1, 1, 2, 0, 3, 1, 3, 1, 4, 2, 7, 0, 2, 3, 6, 1, 5, 3, 5, 1, 3, 4, 4, 2, 5, 6, 7, 7, 0, 1, 2, 3, 3, 4, 6, 2, 6, 5, 5, 3, 4, 5, 6, 7, 1, 2, 4, 6, 4, 4, 5, 7, 2, 6, 5, 7, 6, 7, 7 };
            return DeBruijnBytePos[((U64)((val & -(long long)val) * 0x0218A392CDABBD3FULL)) >> 58];
#       endif
        }
        else 
        {
#       if defined(_MSC_VER) && !defined(LZ4_FORCE_SW_BITCOUNT)
            unsigned long r;
            _BitScanForward( &r, (U32)val );
            return (int)(r>>3);
#       elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
            return (__builtin_ctz((U32)val) >> 3);
#       else
            static const int DeBruijnBytePos[32] = { 0, 0, 3, 0, 3, 1, 3, 0, 3, 2, 2, 1, 3, 2, 0, 1, 3, 3, 1, 2, 2, 2, 2, 0, 3, 1, 2, 0, 1, 0, 1, 1 };
            return DeBruijnBytePos[((U32)((val & -(S32)val) * 0x077CB531U)) >> 27];
#       endif
        }
    }
    else   
    {
        if (LZ4_64bits())
        {
#       if defined(_MSC_VER) && defined(_WIN64) && !defined(LZ4_FORCE_SW_BITCOUNT)
            unsigned long r = 0;
            _BitScanReverse64( &r, val );
            return (unsigned)(r>>3);
#       elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
            return (__builtin_clzll(val) >> 3);
#       else
            unsigned r;
            if (!(val>>32)) { r=4; } else { r=0; val>>=32; }
            if (!(val>>16)) { r+=2; val>>=8; } else { val>>=24; }
            r += (!val);
            return r;
#       endif
        }
        else 
        {
#       if defined(_MSC_VER) && !defined(LZ4_FORCE_SW_BITCOUNT)
            unsigned long r = 0;
            _BitScanReverse( &r, (unsigned long)val );
            return (unsigned)(r>>3);
#       elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
            return (__builtin_clz(val) >> 3);
#       else
            unsigned r;
            if (!(val>>16)) { r=2; val>>=8; } else { r=0; val>>=24; }
            r += (!val);
            return r;
#       endif
        }
    }
}

static unsigned LZ4_count(const BYTE* pIn, const BYTE* pMatch, const BYTE* pInLimit)
{
    const BYTE* const pStart = pIn;

    while (likely(pIn<pInLimit-(STEPSIZE-1)))
    {
        size_t diff = LZ4_read_ARCH(pMatch) ^ LZ4_read_ARCH(pIn);
        if (!diff) { pIn+=STEPSIZE; pMatch+=STEPSIZE; continue; }
        pIn += LZ4_NbCommonBytes(diff);
        return (unsigned)(pIn - pStart);
    }

    if (LZ4_64bits()) if ((pIn<(pInLimit-3)) && (LZ4_read32(pMatch) == LZ4_read32(pIn))) { pIn+=4; pMatch+=4; }
    if ((pIn<(pInLimit-1)) && (LZ4_read16(pMatch) == LZ4_read16(pIn))) { pIn+=2; pMatch+=2; }
    if ((pIn<pInLimit) && (*pMatch == *pIn)) pIn++;
    return (unsigned)(pIn - pStart);
}


#ifndef LZ4_COMMONDEFS_ONLY

#define LZ4_HASHLOG   (LZ4_MEMORY_USAGE-2)
#define HASHTABLESIZE (1 << LZ4_MEMORY_USAGE)
#define HASH_SIZE_U32 (1 << LZ4_HASHLOG)       

static const int LZ4_64Klimit = ((64 KB) + (MFLIMIT-1));
static const U32 LZ4_skipTrigger = 6;  



int LZ4_versionNumber (void) { return LZ4_VERSION_NUMBER; }
int LZ4_compressBound(int isize)  { return LZ4_COMPRESSBOUND(isize); }



typedef struct {
    U32 hashTable[HASH_SIZE_U32];
    U32 currentOffset;
    U32 initCheck;
    const BYTE* dictionary;
    const BYTE* bufferStart;
    U32 dictSize;
} LZ4_stream_t_internal;

typedef enum { notLimited = 0, limitedOutput = 1 } limitedOutput_directive;
typedef enum { byPtr, byU32, byU16 } tableType_t;

typedef enum { noDict = 0, withPrefix64k, usingExtDict } dict_directive;
typedef enum { noDictIssue = 0, dictSmall } dictIssue_directive;

typedef enum { endOnOutputSize = 0, endOnInputSize = 1 } endCondition_directive;
typedef enum { full = 0, partial = 1 } earlyEnd_directive;





static U32 LZ4_hashSequence(U32 sequence, tableType_t const tableType)
{
    if (tableType == byU16)
        return (((sequence) * 2654435761U) >> ((MINMATCH*8)-(LZ4_HASHLOG+1)));
    else
        return (((sequence) * 2654435761U) >> ((MINMATCH*8)-LZ4_HASHLOG));
}

static U32 LZ4_hashPosition(const BYTE* p, tableType_t tableType) { return LZ4_hashSequence(LZ4_read32(p), tableType); }

static void LZ4_putPositionOnHash(const BYTE* p, U32 h, void* tableBase, tableType_t const tableType, const BYTE* srcBase)
{
    switch (tableType)
    {
    case byPtr: { const BYTE** hashTable = (const BYTE**)tableBase; hashTable[h] = p; return; }
    case byU32: { U32* hashTable = (U32*) tableBase; hashTable[h] = (U32)(p-srcBase); return; }
    case byU16: { U16* hashTable = (U16*) tableBase; hashTable[h] = (U16)(p-srcBase); return; }
    }
}

static void LZ4_putPosition(const BYTE* p, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
    U32 h = LZ4_hashPosition(p, tableType);
    LZ4_putPositionOnHash(p, h, tableBase, tableType, srcBase);
}

static const BYTE* LZ4_getPositionOnHash(U32 h, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
    if (tableType == byPtr) { const BYTE** hashTable = (const BYTE**) tableBase; return hashTable[h]; }
    if (tableType == byU32) { U32* hashTable = (U32*) tableBase; return hashTable[h] + srcBase; }
    { U16* hashTable = (U16*) tableBase; return hashTable[h] + srcBase; }   
}

static const BYTE* LZ4_getPosition(const BYTE* p, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
    U32 h = LZ4_hashPosition(p, tableType);
    return LZ4_getPositionOnHash(h, tableBase, tableType, srcBase);
}

static int LZ4_compress_generic(
                 void* ctx,
                 const char* source,
                 char* dest,
                 int inputSize,
                 int maxOutputSize,
                 limitedOutput_directive outputLimited,
                 tableType_t const tableType,
                 dict_directive dict,
                 dictIssue_directive dictIssue)
{
    LZ4_stream_t_internal* const dictPtr = (LZ4_stream_t_internal*)ctx;

    const BYTE* ip = (const BYTE*) source;
    const BYTE* base;
    const BYTE* lowLimit;
    const BYTE* const lowRefLimit = ip - dictPtr->dictSize;
    const BYTE* const dictionary = dictPtr->dictionary;
    const BYTE* const dictEnd = dictionary + dictPtr->dictSize;
    const size_t dictDelta = dictEnd - (const BYTE*)source;
    const BYTE* anchor = (const BYTE*) source;
    const BYTE* const iend = ip + inputSize;
    const BYTE* const mflimit = iend - MFLIMIT;
    const BYTE* const matchlimit = iend - LASTLITERALS;

    BYTE* op = (BYTE*) dest;
    BYTE* const olimit = op + maxOutputSize;

    U32 forwardH;
    size_t refDelta=0;

    
    if ((U32)inputSize > (U32)LZ4_MAX_INPUT_SIZE) return 0;          
    switch(dict)
    {
    case noDict:
    default:
        base = (const BYTE*)source;
        lowLimit = (const BYTE*)source;
        break;
    case withPrefix64k:
        base = (const BYTE*)source - dictPtr->currentOffset;
        lowLimit = (const BYTE*)source - dictPtr->dictSize;
        break;
    case usingExtDict:
        base = (const BYTE*)source - dictPtr->currentOffset;
        lowLimit = (const BYTE*)source;
        break;
    }
    if ((tableType == byU16) && (inputSize>=LZ4_64Klimit)) return 0;   
    if (inputSize<LZ4_minLength) goto _last_literals;                  

    
    LZ4_putPosition(ip, ctx, tableType, base);
    ip++; forwardH = LZ4_hashPosition(ip, tableType);

    
    for ( ; ; )
    {
        const BYTE* match;
        BYTE* token;
        {
            const BYTE* forwardIp = ip;
            unsigned step=1;
            unsigned searchMatchNb = (1U << LZ4_skipTrigger);

            
            do {
                U32 h = forwardH;
                ip = forwardIp;
                forwardIp += step;
                step = searchMatchNb++ >> LZ4_skipTrigger;

                if (unlikely(forwardIp > mflimit)) goto _last_literals;

                match = LZ4_getPositionOnHash(h, ctx, tableType, base);
                if (dict==usingExtDict)
                {
                    if (match<(const BYTE*)source)
                    {
                        refDelta = dictDelta;
                        lowLimit = dictionary;
                    }
                    else
                    {
                        refDelta = 0;
                        lowLimit = (const BYTE*)source;
                    }
                }
                forwardH = LZ4_hashPosition(forwardIp, tableType);
                LZ4_putPositionOnHash(ip, h, ctx, tableType, base);

            } while ( ((dictIssue==dictSmall) ? (match < lowRefLimit) : 0)
                || ((tableType==byU16) ? 0 : (match + MAX_DISTANCE < ip))
                || (LZ4_read32(match+refDelta) != LZ4_read32(ip)) );
        }

        
        while ((ip>anchor) && (match+refDelta > lowLimit) && (unlikely(ip[-1]==match[refDelta-1]))) { ip--; match--; }

        {
            
            unsigned litLength = (unsigned)(ip - anchor);
            token = op++;
            if ((outputLimited) && (unlikely(op + litLength + (2 + 1 + LASTLITERALS) + (litLength/255) > olimit)))
                return 0;   
            if (litLength>=RUN_MASK)
            {
                int len = (int)litLength-RUN_MASK;
                *token=(RUN_MASK<<ML_BITS);
                for(; len >= 255 ; len-=255) *op++ = 255;
                *op++ = (BYTE)len;
            }
            else *token = (BYTE)(litLength<<ML_BITS);

            
            LZ4_wildCopy(op, anchor, op+litLength);
            op+=litLength;
        }

_next_match:
        
        LZ4_writeLE16(op, (U16)(ip-match)); op+=2;

        
        {
            unsigned matchLength;

            if ((dict==usingExtDict) && (lowLimit==dictionary))
            {
                const BYTE* limit;
                match += refDelta;
                limit = ip + (dictEnd-match);
                if (limit > matchlimit) limit = matchlimit;
                matchLength = LZ4_count(ip+MINMATCH, match+MINMATCH, limit);
                ip += MINMATCH + matchLength;
                if (ip==limit)
                {
                    unsigned more = LZ4_count(ip, (const BYTE*)source, matchlimit);
                    matchLength += more;
                    ip += more;
                }
            }
            else
            {
                matchLength = LZ4_count(ip+MINMATCH, match+MINMATCH, matchlimit);
                ip += MINMATCH + matchLength;
            }

            if ((outputLimited) && (unlikely(op + (1 + LASTLITERALS) + (matchLength>>8) > olimit)))
                return 0;    
            if (matchLength>=ML_MASK)
            {
                *token += ML_MASK;
                matchLength -= ML_MASK;
                for (; matchLength >= 510 ; matchLength-=510) { *op++ = 255; *op++ = 255; }
                if (matchLength >= 255) { matchLength-=255; *op++ = 255; }
                *op++ = (BYTE)matchLength;
            }
            else *token += (BYTE)(matchLength);
        }

        anchor = ip;

        
        if (ip > mflimit) break;

        
        LZ4_putPosition(ip-2, ctx, tableType, base);

        
        match = LZ4_getPosition(ip, ctx, tableType, base);
        if (dict==usingExtDict)
        {
            if (match<(const BYTE*)source)
            {
                refDelta = dictDelta;
                lowLimit = dictionary;
            }
            else
            {
                refDelta = 0;
                lowLimit = (const BYTE*)source;
            }
        }
        LZ4_putPosition(ip, ctx, tableType, base);
        if ( ((dictIssue==dictSmall) ? (match>=lowRefLimit) : 1)
            && (match+MAX_DISTANCE>=ip)
            && (LZ4_read32(match+refDelta)==LZ4_read32(ip)) )
        { token=op++; *token=0; goto _next_match; }

        
        forwardH = LZ4_hashPosition(++ip, tableType);
    }

_last_literals:
    
    {
        int lastRun = (int)(iend - anchor);
        if ((outputLimited) && (((char*)op - dest) + lastRun + 1 + ((lastRun+255-RUN_MASK)/255) > (U32)maxOutputSize))
            return 0;   
        if (lastRun>=(int)RUN_MASK) { *op++=(RUN_MASK<<ML_BITS); lastRun-=RUN_MASK; for(; lastRun >= 255 ; lastRun-=255) *op++ = 255; *op++ = (BYTE) lastRun; }
        else *op++ = (BYTE)(lastRun<<ML_BITS);
        memcpy(op, anchor, iend - anchor);
        op += iend-anchor;
    }

    
    return (int) (((char*)op)-dest);
}


int LZ4_compress(const char* source, char* dest, int inputSize)
{
#if (HEAPMODE)
    void* ctx = ALLOCATOR(LZ4_STREAMSIZE_U64, 8);   
#else
    U64 ctx[LZ4_STREAMSIZE_U64] = {0};      
#endif
    int result;

    if (inputSize < LZ4_64Klimit)
        result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, 0, notLimited, byU16, noDict, noDictIssue);
    else
        result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, 0, notLimited, LZ4_64bits() ? byU32 : byPtr, noDict, noDictIssue);

#if (HEAPMODE)
    FREEMEM(ctx);
#endif
    return result;
}

int LZ4_compress_limitedOutput(const char* source, char* dest, int inputSize, int maxOutputSize)
{
#if (HEAPMODE)
    void* ctx = ALLOCATOR(LZ4_STREAMSIZE_U64, 8);   
#else
    U64 ctx[LZ4_STREAMSIZE_U64] = {0};      
#endif
    int result;

    if (inputSize < LZ4_64Klimit)
        result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, maxOutputSize, limitedOutput, byU16, noDict, noDictIssue);
    else
        result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, maxOutputSize, limitedOutput, LZ4_64bits() ? byU32 : byPtr, noDict, noDictIssue);

#if (HEAPMODE)
    FREEMEM(ctx);
#endif
    return result;
}





void LZ4_resetStream (LZ4_stream_t* LZ4_stream)
{
    MEM_INIT(LZ4_stream, 0, sizeof(LZ4_stream_t));
}

LZ4_stream_t* LZ4_createStream(void)
{
    LZ4_stream_t* lz4s = (LZ4_stream_t*)ALLOCATOR(8, LZ4_STREAMSIZE_U64);
    LZ4_STATIC_ASSERT(LZ4_STREAMSIZE >= sizeof(LZ4_stream_t_internal));    
    LZ4_resetStream(lz4s);
    return lz4s;
}

int LZ4_freeStream (LZ4_stream_t* LZ4_stream)
{
    FREEMEM(LZ4_stream);
    return (0);
}


int LZ4_loadDict (LZ4_stream_t* LZ4_dict, const char* dictionary, int dictSize)
{
    LZ4_stream_t_internal* dict = (LZ4_stream_t_internal*) LZ4_dict;
    const BYTE* p = (const BYTE*)dictionary;
    const BYTE* const dictEnd = p + dictSize;
    const BYTE* base;

    if (dict->initCheck) LZ4_resetStream(LZ4_dict);                         

    if (dictSize < MINMATCH)
    {
        dict->dictionary = NULL;
        dict->dictSize = 0;
        return 0;
    }

    if (p <= dictEnd - 64 KB) p = dictEnd - 64 KB;
    base = p - dict->currentOffset;
    dict->dictionary = p;
    dict->dictSize = (U32)(dictEnd - p);
    dict->currentOffset += dict->dictSize;

    while (p <= dictEnd-MINMATCH)
    {
        LZ4_putPosition(p, dict, byU32, base);
        p+=3;
    }

    return dict->dictSize;
}


static void LZ4_renormDictT(LZ4_stream_t_internal* LZ4_dict, const BYTE* src)
{
    if ((LZ4_dict->currentOffset > 0x80000000) ||
        ((size_t)LZ4_dict->currentOffset > (size_t)src))   
    {
        
        U32 delta = LZ4_dict->currentOffset - 64 KB;
        const BYTE* dictEnd = LZ4_dict->dictionary + LZ4_dict->dictSize;
        int i;
        for (i=0; i<HASH_SIZE_U32; i++)
        {
            if (LZ4_dict->hashTable[i] < delta) LZ4_dict->hashTable[i]=0;
            else LZ4_dict->hashTable[i] -= delta;
        }
        LZ4_dict->currentOffset = 64 KB;
        if (LZ4_dict->dictSize > 64 KB) LZ4_dict->dictSize = 64 KB;
        LZ4_dict->dictionary = dictEnd - LZ4_dict->dictSize;
    }
}


FORCE_INLINE int LZ4_compress_continue_generic (void* LZ4_stream, const char* source, char* dest, int inputSize,
                                                int maxOutputSize, limitedOutput_directive limit)
{
    LZ4_stream_t_internal* streamPtr = (LZ4_stream_t_internal*)LZ4_stream;
    const BYTE* const dictEnd = streamPtr->dictionary + streamPtr->dictSize;

    const BYTE* smallest = (const BYTE*) source;
    if (streamPtr->initCheck) return 0;   
    if ((streamPtr->dictSize>0) && (smallest>dictEnd)) smallest = dictEnd;
    LZ4_renormDictT(streamPtr, smallest);

    
    {
        const BYTE* sourceEnd = (const BYTE*) source + inputSize;
        if ((sourceEnd > streamPtr->dictionary) && (sourceEnd < dictEnd))
        {
            streamPtr->dictSize = (U32)(dictEnd - sourceEnd);
            if (streamPtr->dictSize > 64 KB) streamPtr->dictSize = 64 KB;
            if (streamPtr->dictSize < 4) streamPtr->dictSize = 0;
            streamPtr->dictionary = dictEnd - streamPtr->dictSize;
        }
    }

    
    if (dictEnd == (const BYTE*)source)
    {
        int result;
        if ((streamPtr->dictSize < 64 KB) && (streamPtr->dictSize < streamPtr->currentOffset))
            result = LZ4_compress_generic(LZ4_stream, source, dest, inputSize, maxOutputSize, limit, byU32, withPrefix64k, dictSmall);
        else
            result = LZ4_compress_generic(LZ4_stream, source, dest, inputSize, maxOutputSize, limit, byU32, withPrefix64k, noDictIssue);
        streamPtr->dictSize += (U32)inputSize;
        streamPtr->currentOffset += (U32)inputSize;
        return result;
    }

    
    {
        int result;
        if ((streamPtr->dictSize < 64 KB) && (streamPtr->dictSize < streamPtr->currentOffset))
            result = LZ4_compress_generic(LZ4_stream, source, dest, inputSize, maxOutputSize, limit, byU32, usingExtDict, dictSmall);
        else
            result = LZ4_compress_generic(LZ4_stream, source, dest, inputSize, maxOutputSize, limit, byU32, usingExtDict, noDictIssue);
        streamPtr->dictionary = (const BYTE*)source;
        streamPtr->dictSize = (U32)inputSize;
        streamPtr->currentOffset += (U32)inputSize;
        return result;
    }
}

int LZ4_compress_continue (LZ4_stream_t* LZ4_stream, const char* source, char* dest, int inputSize)
{
    return LZ4_compress_continue_generic(LZ4_stream, source, dest, inputSize, 0, notLimited);
}

int LZ4_compress_limitedOutput_continue (LZ4_stream_t* LZ4_stream, const char* source, char* dest, int inputSize, int maxOutputSize)
{
    return LZ4_compress_continue_generic(LZ4_stream, source, dest, inputSize, maxOutputSize, limitedOutput);
}



int LZ4_compress_forceExtDict (LZ4_stream_t* LZ4_dict, const char* source, char* dest, int inputSize)
{
    LZ4_stream_t_internal* streamPtr = (LZ4_stream_t_internal*)LZ4_dict;
    int result;
    const BYTE* const dictEnd = streamPtr->dictionary + streamPtr->dictSize;

    const BYTE* smallest = dictEnd;
    if (smallest > (const BYTE*) source) smallest = (const BYTE*) source;
    LZ4_renormDictT((LZ4_stream_t_internal*)LZ4_dict, smallest);

    result = LZ4_compress_generic(LZ4_dict, source, dest, inputSize, 0, notLimited, byU32, usingExtDict, noDictIssue);

    streamPtr->dictionary = (const BYTE*)source;
    streamPtr->dictSize = (U32)inputSize;
    streamPtr->currentOffset += (U32)inputSize;

    return result;
}


int LZ4_saveDict (LZ4_stream_t* LZ4_dict, char* safeBuffer, int dictSize)
{
    LZ4_stream_t_internal* dict = (LZ4_stream_t_internal*) LZ4_dict;
    const BYTE* previousDictEnd = dict->dictionary + dict->dictSize;

    if ((U32)dictSize > 64 KB) dictSize = 64 KB;   
    if ((U32)dictSize > dict->dictSize) dictSize = dict->dictSize;

    memmove(safeBuffer, previousDictEnd - dictSize, dictSize);

    dict->dictionary = (const BYTE*)safeBuffer;
    dict->dictSize = (U32)dictSize;

    return dictSize;
}





FORCE_INLINE int LZ4_decompress_generic(
                 const char* const source,
                 char* const dest,
                 int inputSize,
                 int outputSize,         

                 int endOnInput,         
                 int partialDecoding,    
                 int targetOutputSize,   
                 int dict,               
                 const BYTE* const lowPrefix,  
                 const BYTE* const dictStart,  
                 const size_t dictSize         
                 )
{
    
    const BYTE* restrict ip = (const BYTE*) source;
    const BYTE* const iend = ip + inputSize;

    BYTE* op = (BYTE*) dest;
    BYTE* const oend = op + outputSize;
    BYTE* cpy;
    BYTE* oexit = op + targetOutputSize;
    const BYTE* const lowLimit = lowPrefix - dictSize;

    const BYTE* const dictEnd = (const BYTE*)dictStart + dictSize;
    const size_t dec32table[] = {4, 1, 2, 1, 4, 4, 4, 4};
    const size_t dec64table[] = {0, 0, 0, (size_t)-1, 0, 1, 2, 3};

    const int safeDecode = (endOnInput==endOnInputSize);
    const int checkOffset = ((safeDecode) && (dictSize < (int)(64 KB)));


    
    if ((partialDecoding) && (oexit> oend-MFLIMIT)) oexit = oend-MFLIMIT;                         
    if ((endOnInput) && (unlikely(outputSize==0))) return ((inputSize==1) && (*ip==0)) ? 0 : -1;  
    if ((!endOnInput) && (unlikely(outputSize==0))) return (*ip==0?1:-1);


    
    while (1)
    {
        unsigned token;
        size_t length;
        const BYTE* match;

        
        token = *ip++;
        if ((length=(token>>ML_BITS)) == RUN_MASK)
        {
            unsigned s;
            do
            {
                s = *ip++;
                length += s;
            }
            while (likely((endOnInput)?ip<iend-RUN_MASK:1) && (s==255));
            if ((safeDecode) && unlikely((size_t)(op+length)<(size_t)(op))) goto _output_error;   
            if ((safeDecode) && unlikely((size_t)(ip+length)<(size_t)(ip))) goto _output_error;   
        }

        
        cpy = op+length;
        if (((endOnInput) && ((cpy>(partialDecoding?oexit:oend-MFLIMIT)) || (ip+length>iend-(2+1+LASTLITERALS))) )
            || ((!endOnInput) && (cpy>oend-COPYLENGTH)))
        {
            if (partialDecoding)
            {
                if (cpy > oend) goto _output_error;                           
                if ((endOnInput) && (ip+length > iend)) goto _output_error;   
            }
            else
            {
                if ((!endOnInput) && (cpy != oend)) goto _output_error;       
                if ((endOnInput) && ((ip+length != iend) || (cpy > oend))) goto _output_error;   
            }
            memcpy(op, ip, length);
            ip += length;
            op += length;
            break;     
        }
        LZ4_wildCopy(op, ip, cpy);
        ip += length; op = cpy;

        
        match = cpy - LZ4_readLE16(ip); ip+=2;
        if ((checkOffset) && (unlikely(match < lowLimit))) goto _output_error;   

        
        length = token & ML_MASK;
        if (length == ML_MASK)
        {
            unsigned s;
            do
            {
                if ((endOnInput) && (ip > iend-LASTLITERALS)) goto _output_error;
                s = *ip++;
                length += s;
            } while (s==255);
            if ((safeDecode) && unlikely((size_t)(op+length)<(size_t)op)) goto _output_error;   
        }
        length += MINMATCH;

        
        if ((dict==usingExtDict) && (match < lowPrefix))
        {
            if (unlikely(op+length > oend-LASTLITERALS)) goto _output_error;   

            if (length <= (size_t)(lowPrefix-match))
            {
                
                match = dictEnd - (lowPrefix-match);
                memcpy(op, match, length);
                op += length;
            }
            else
            {
                
                size_t copySize = (size_t)(lowPrefix-match);
                memcpy(op, dictEnd - copySize, copySize);
                op += copySize;
                copySize = length - copySize;
                if (copySize > (size_t)(op-lowPrefix))   
                {
                    BYTE* const endOfMatch = op + copySize;
                    const BYTE* copyFrom = lowPrefix;
                    while (op < endOfMatch) *op++ = *copyFrom++;
                }
                else
                {
                    memcpy(op, lowPrefix, copySize);
                    op += copySize;
                }
            }
            continue;
        }

        
        cpy = op + length;
        if (unlikely((op-match)<8))
        {
            const size_t dec64 = dec64table[op-match];
            op[0] = match[0];
            op[1] = match[1];
            op[2] = match[2];
            op[3] = match[3];
            match += dec32table[op-match];
            LZ4_copy4(op+4, match);
            op += 8; match -= dec64;
        } else { LZ4_copy8(op, match); op+=8; match+=8; }

        if (unlikely(cpy>oend-12))
        {
            if (cpy > oend-LASTLITERALS) goto _output_error;    
            if (op < oend-8)
            {
                LZ4_wildCopy(op, match, oend-8);
                match += (oend-8) - op;
                op = oend-8;
            }
            while (op<cpy) *op++ = *match++;
        }
        else
            LZ4_wildCopy(op, match, cpy);
        op=cpy;   
    }

    
    if (endOnInput)
       return (int) (((char*)op)-dest);     
    else
       return (int) (((char*)ip)-source);   

    
_output_error:
    return (int) (-(((char*)ip)-source))-1;
}


int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxDecompressedSize, endOnInputSize, full, 0, noDict, (BYTE*)dest, NULL, 0);
}

int LZ4_decompress_safe_partial(const char* source, char* dest, int compressedSize, int targetOutputSize, int maxDecompressedSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxDecompressedSize, endOnInputSize, partial, targetOutputSize, noDict, (BYTE*)dest, NULL, 0);
}

int LZ4_decompress_fast(const char* source, char* dest, int originalSize)
{
    return LZ4_decompress_generic(source, dest, 0, originalSize, endOnOutputSize, full, 0, withPrefix64k, (BYTE*)(dest - 64 KB), NULL, 64 KB);
}




typedef struct
{
    BYTE* externalDict;
    size_t extDictSize;
    BYTE* prefixEnd;
    size_t prefixSize;
} LZ4_streamDecode_t_internal;


LZ4_streamDecode_t* LZ4_createStreamDecode(void)
{
    LZ4_streamDecode_t* lz4s = (LZ4_streamDecode_t*) ALLOCATOR(1, sizeof(LZ4_streamDecode_t));
    return lz4s;
}

int LZ4_freeStreamDecode (LZ4_streamDecode_t* LZ4_stream)
{
    FREEMEM(LZ4_stream);
    return 0;
}


int LZ4_setStreamDecode (LZ4_streamDecode_t* LZ4_streamDecode, const char* dictionary, int dictSize)
{
    LZ4_streamDecode_t_internal* lz4sd = (LZ4_streamDecode_t_internal*) LZ4_streamDecode;
    lz4sd->prefixSize = (size_t) dictSize;
    lz4sd->prefixEnd = (BYTE*) dictionary + dictSize;
    lz4sd->externalDict = NULL;
    lz4sd->extDictSize  = 0;
    return 1;
}


int LZ4_decompress_safe_continue (LZ4_streamDecode_t* LZ4_streamDecode, const char* source, char* dest, int compressedSize, int maxOutputSize)
{
    LZ4_streamDecode_t_internal* lz4sd = (LZ4_streamDecode_t_internal*) LZ4_streamDecode;
    int result;

    if (lz4sd->prefixEnd == (BYTE*)dest)
    {
        result = LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize,
                                        endOnInputSize, full, 0,
                                        usingExtDict, lz4sd->prefixEnd - lz4sd->prefixSize, lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize += result;
        lz4sd->prefixEnd  += result;
    }
    else
    {
        lz4sd->extDictSize = lz4sd->prefixSize;
        lz4sd->externalDict = lz4sd->prefixEnd - lz4sd->extDictSize;
        result = LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize,
                                        endOnInputSize, full, 0,
                                        usingExtDict, (BYTE*)dest, lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize = result;
        lz4sd->prefixEnd  = (BYTE*)dest + result;
    }

    return result;
}

int LZ4_decompress_fast_continue (LZ4_streamDecode_t* LZ4_streamDecode, const char* source, char* dest, int originalSize)
{
    LZ4_streamDecode_t_internal* lz4sd = (LZ4_streamDecode_t_internal*) LZ4_streamDecode;
    int result;

    if (lz4sd->prefixEnd == (BYTE*)dest)
    {
        result = LZ4_decompress_generic(source, dest, 0, originalSize,
                                        endOnOutputSize, full, 0,
                                        usingExtDict, lz4sd->prefixEnd - lz4sd->prefixSize, lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize += originalSize;
        lz4sd->prefixEnd  += originalSize;
    }
    else
    {
        lz4sd->extDictSize = lz4sd->prefixSize;
        lz4sd->externalDict = (BYTE*)dest - lz4sd->extDictSize;
        result = LZ4_decompress_generic(source, dest, 0, originalSize,
                                        endOnOutputSize, full, 0,
                                        usingExtDict, (BYTE*)dest, lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize = originalSize;
        lz4sd->prefixEnd  = (BYTE*)dest + originalSize;
    }

    return result;
}




FORCE_INLINE int LZ4_decompress_usingDict_generic(const char* source, char* dest, int compressedSize, int maxOutputSize, int safe, const char* dictStart, int dictSize)
{
    if (dictSize==0)
        return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize, safe, full, 0, noDict, (BYTE*)dest, NULL, 0);
    if (dictStart+dictSize == dest)
    {
        if (dictSize >= (int)(64 KB - 1))
            return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize, safe, full, 0, withPrefix64k, (BYTE*)dest-64 KB, NULL, 0);
        return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize, safe, full, 0, noDict, (BYTE*)dest-dictSize, NULL, 0);
    }
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize, safe, full, 0, usingExtDict, (BYTE*)dest, (BYTE*)dictStart, dictSize);
}

int LZ4_decompress_safe_usingDict(const char* source, char* dest, int compressedSize, int maxOutputSize, const char* dictStart, int dictSize)
{
    return LZ4_decompress_usingDict_generic(source, dest, compressedSize, maxOutputSize, 1, dictStart, dictSize);
}

int LZ4_decompress_fast_usingDict(const char* source, char* dest, int originalSize, const char* dictStart, int dictSize)
{
    return LZ4_decompress_usingDict_generic(source, dest, 0, originalSize, 0, dictStart, dictSize);
}


int LZ4_decompress_safe_forceExtDict(const char* source, char* dest, int compressedSize, int maxOutputSize, const char* dictStart, int dictSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize, endOnInputSize, full, 0, usingExtDict, (BYTE*)dest, (BYTE*)dictStart, dictSize);
}




int LZ4_uncompress (const char* source, char* dest, int outputSize) { return LZ4_decompress_fast(source, dest, outputSize); }
int LZ4_uncompress_unknownOutputSize (const char* source, char* dest, int isize, int maxOutputSize) { return LZ4_decompress_safe(source, dest, isize, maxOutputSize); }




int LZ4_sizeofStreamState() { return LZ4_STREAMSIZE; }

static void LZ4_init(LZ4_stream_t_internal* lz4ds, const BYTE* base)
{
    MEM_INIT(lz4ds, 0, LZ4_STREAMSIZE);
    lz4ds->bufferStart = base;
}

int LZ4_resetStreamState(void* state, const char* inputBuffer)
{
    if ((((size_t)state) & 3) != 0) return 1;   
    LZ4_init((LZ4_stream_t_internal*)state, (const BYTE*)inputBuffer);
    return 0;
}

void* LZ4_create (const char* inputBuffer)
{
    void* lz4ds = ALLOCATOR(8, LZ4_STREAMSIZE_U64);
    LZ4_init ((LZ4_stream_t_internal*)lz4ds, (const BYTE*)inputBuffer);
    return lz4ds;
}

char* LZ4_slideInputBuffer (void* LZ4_Data)
{
    LZ4_stream_t_internal* ctx = (LZ4_stream_t_internal*)LZ4_Data;
    int dictSize = LZ4_saveDict((LZ4_stream_t*)LZ4_Data, (char*)ctx->bufferStart, 64 KB);
    return (char*)(ctx->bufferStart + dictSize);
}



int LZ4_sizeofState() { return LZ4_STREAMSIZE; }

int LZ4_compress_withState (void* state, const char* source, char* dest, int inputSize)
{
    if (((size_t)(state)&3) != 0) return 0;   
    MEM_INIT(state, 0, LZ4_STREAMSIZE);

    if (inputSize < LZ4_64Klimit)
        return LZ4_compress_generic(state, source, dest, inputSize, 0, notLimited, byU16, noDict, noDictIssue);
    else
        return LZ4_compress_generic(state, source, dest, inputSize, 0, notLimited, LZ4_64bits() ? byU32 : byPtr, noDict, noDictIssue);
}

int LZ4_compress_limitedOutput_withState (void* state, const char* source, char* dest, int inputSize, int maxOutputSize)
{
    if (((size_t)(state)&3) != 0) return 0;   
    MEM_INIT(state, 0, LZ4_STREAMSIZE);

    if (inputSize < LZ4_64Klimit)
        return LZ4_compress_generic(state, source, dest, inputSize, maxOutputSize, limitedOutput, byU16, noDict, noDictIssue);
    else
        return LZ4_compress_generic(state, source, dest, inputSize, maxOutputSize, limitedOutput, LZ4_64bits() ? byU32 : byPtr, noDict, noDictIssue);
}



int LZ4_decompress_safe_withPrefix64k(const char* source, char* dest, int compressedSize, int maxOutputSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize, endOnInputSize, full, 0, withPrefix64k, (BYTE*)dest - 64 KB, NULL, 64 KB);
}

int LZ4_decompress_fast_withPrefix64k(const char* source, char* dest, int originalSize)
{
    return LZ4_decompress_generic(source, dest, 0, originalSize, endOnOutputSize, full, 0, withPrefix64k, (BYTE*)dest - 64 KB, NULL, 64 KB);
}

#endif   

