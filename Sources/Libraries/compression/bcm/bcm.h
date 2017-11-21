#pragma once

int impl_bcm_cq(int sourceSize);
int impl_bcm_c(const unsigned char* source, unsigned char* dest, int sourceSize);
int impl_bcm_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);