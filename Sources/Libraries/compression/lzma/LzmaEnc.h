

#ifndef __LZMA_ENC_H
#define __LZMA_ENC_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LZMA_PROPS_SIZE 5

typedef struct _CLzmaEncProps
{
  int level;       
  UInt32 dictSize; 
  int lc;          
  int lp;          
  int pb;          
  int algo;        
  int fb;          
  int btMode;      
  int numHashBytes; 
  UInt32 mc;        
  unsigned writeEndMark;  
  int numThreads;  
} CLzmaEncProps;

void LzmaEncProps_Init(CLzmaEncProps *p);
void LzmaEncProps_Normalize(CLzmaEncProps *p);
UInt32 LzmaEncProps_GetDictSize(const CLzmaEncProps *props2);






typedef void * CLzmaEncHandle;

CLzmaEncHandle LzmaEnc_Create(ISzAlloc *alloc);
void LzmaEnc_Destroy(CLzmaEncHandle p, ISzAlloc *alloc, ISzAlloc *allocBig);
SRes LzmaEnc_SetProps(CLzmaEncHandle p, const CLzmaEncProps *props);
SRes LzmaEnc_WriteProperties(CLzmaEncHandle p, Byte *properties, SizeT *size);
SRes LzmaEnc_Encode(CLzmaEncHandle p, ISeqOutStream *outStream, ISeqInStream *inStream,
    ICompressProgress *progress, ISzAlloc *alloc, ISzAlloc *allocBig);
SRes LzmaEnc_MemEncode(CLzmaEncHandle p, Byte *dest, SizeT *destLen, const Byte *src, SizeT srcLen,
    int writeEndMark, ICompressProgress *progress, ISzAlloc *alloc, ISzAlloc *allocBig);





SRes LzmaEncode(Byte *dest, SizeT *destLen, const Byte *src, SizeT srcLen,
    const CLzmaEncProps *props, Byte *propsEncoded, SizeT *propsSize, int writeEndMark,
    ICompressProgress *progress, ISzAlloc *alloc, ISzAlloc *allocBig);

#ifdef __cplusplus
}
#endif

#endif
