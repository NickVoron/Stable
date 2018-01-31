

#ifndef __LZMA_LIB_H
#define __LZMA_LIB_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MY_STDAPI int MY_STD_CALL

#define LZMA_PROPS_SIZE 5





MY_STDAPI LzmaCompress(unsigned char *dest, size_t *destLen, const unsigned char *src, size_t srcLen,
  unsigned char *outProps, size_t *outPropsSize, 
  int level,      
  unsigned dictSize,  
  int lc,        
  int lp,        
  int pb,        
  int fb,        
  int numThreads 
  );



MY_STDAPI LzmaUncompress(unsigned char *dest, size_t *destLen, const unsigned char *src, SizeT *srcLen,
  const unsigned char *props, size_t propsSize);

#ifdef __cplusplus
}
#endif

#endif
