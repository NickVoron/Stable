

#ifndef __LZMA_DEC_H
#define __LZMA_DEC_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif




#ifdef _LZMA_PROB32
#define CLzmaProb UInt32
#else
#define CLzmaProb UInt16
#endif




#define LZMA_PROPS_SIZE 5

typedef struct _CLzmaProps
{
  unsigned lc, lp, pb;
  UInt32 dicSize;
} CLzmaProps;



SRes LzmaProps_Decode(CLzmaProps *p, const Byte *data, unsigned size);






#define LZMA_REQUIRED_INPUT_MAX 20

typedef struct
{
  CLzmaProps prop;
  CLzmaProb *probs;
  Byte *dic;
  const Byte *buf;
  UInt32 range, code;
  SizeT dicPos;
  SizeT dicBufSize;
  UInt32 processedPos;
  UInt32 checkDicSize;
  unsigned state;
  UInt32 reps[4];
  unsigned remainLen;
  int needFlush;
  int needInitState;
  UInt32 numProbs;
  unsigned tempBufSize;
  Byte tempBuf[LZMA_REQUIRED_INPUT_MAX];
} CLzmaDec;

#define LzmaDec_Construct(p) { (p)->dic = 0; (p)->probs = 0; }

void LzmaDec_Init(CLzmaDec *p);



typedef enum
{
  LZMA_FINISH_ANY,   
  LZMA_FINISH_END    
} ELzmaFinishMode;



typedef enum
{
  LZMA_STATUS_NOT_SPECIFIED,               
  LZMA_STATUS_FINISHED_WITH_MARK,          
  LZMA_STATUS_NOT_FINISHED,                
  LZMA_STATUS_NEEDS_MORE_INPUT,            
  LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK  
} ELzmaStatus;










   
SRes LzmaDec_AllocateProbs(CLzmaDec *p, const Byte *props, unsigned propsSize, ISzAlloc *alloc);
void LzmaDec_FreeProbs(CLzmaDec *p, ISzAlloc *alloc);

SRes LzmaDec_Allocate(CLzmaDec *state, const Byte *prop, unsigned propsSize, ISzAlloc *alloc);
void LzmaDec_Free(CLzmaDec *state, ISzAlloc *alloc);







SRes LzmaDec_DecodeToDic(CLzmaDec *p, SizeT dicLimit,
    const Byte *src, SizeT *srcLen, ELzmaFinishMode finishMode, ELzmaStatus *status);






SRes LzmaDec_DecodeToBuf(CLzmaDec *p, Byte *dest, SizeT *destLen,
    const Byte *src, SizeT *srcLen, ELzmaFinishMode finishMode, ELzmaStatus *status);






SRes LzmaDecode(Byte *dest, SizeT *destLen, const Byte *src, SizeT *srcLen,
    const Byte *propData, unsigned propSize, ELzmaFinishMode finishMode,
    ELzmaStatus *status, ISzAlloc *alloc);

#ifdef __cplusplus
}
#endif

#endif
