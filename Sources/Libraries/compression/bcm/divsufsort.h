

#ifndef _DIVSUFSORT_H
#define _DIVSUFSORT_H 1

#ifdef __cplusplus
extern "C" {
#endif 





int
divsufsort(const unsigned char *T, int *SA, int n);


int
divbwt(const unsigned char *T, unsigned char *U, int *A, int n);


#ifdef __cplusplus
} 
#endif 

#endif 
