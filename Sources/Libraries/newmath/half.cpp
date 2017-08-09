// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "half.h"



half_float::half_to_float::mantissa::mantissa()
{
	table[0] = 0;
	for (int i = 1; i < 1024; ++i) table[i] = convert(i);
	for (int i = 1024; i < 2048; ++i) table[i] = 0x38000000 + ((i-1024)<<13);
}

unsigned int half_float::half_to_float::mantissa::convert(unsigned int i)
{
	unsigned int m=i<<13; 
	unsigned int e=0; 
	while(!(m&0x00800000))
	{ 
		e-=0x00800000; 
		m<<=1; 
	}
	m&=~0x00800000; 
	e+=0x38800000; 
	return m | e; 
}


half_float::half_to_float::exponent::exponent()
{
	table[0] = 0;
	table[32] = 0x80000000;

	for (int i = 1; i < 31; ++i)	table[i] = i<<23;
	for (int i = 33; i < 63; ++i) 	table[i] = 0x80000000 + ((i-32)<<23);

	table[31]= 0x47800000;
	table[63]= 0xC7800000;
}

half_float::half_to_float::offset::offset()
{
	for (int i = 0; i < 64; ++i)	table[i] = 1024;
	table[0] = table[32] = 0;
}


half_float::float_to_half::tables::tables()
{
	int e;
	for(unsigned int i=0; i<256; ++i)
	{
		e=i-127;
		if(e<-24)
		{                  
			base[i|0x000]=0x0000;
			base[i|0x100]=0x8000;
			shift[i|0x000]=24;
			shift[i|0x100]=24;
		}
		else if(e<-14)
		{             
			base[i|0x000]=(0x0400>>(18-e));
			base[i|0x100]=(0x0400>>(18-e)) | 0x8000;
				shift[i|0x000]=-e-1;
			shift[i|0x100]=-e-1;
		}
		else if(e<=15)
		{             
			base[i|0x000]=((e+15)<<10);
			base[i|0x100]=((e+15)<<10) | 0x8000;
			shift[i|0x000]=13;
			shift[i|0x100]=13;
		}
		else if(e<128)
		{             
			base[i|0x000]=0x7C00;
			base[i|0x100]=0xFC00;
			shift[i|0x000]=24;
			shift[i|0x100]=24;
		}
		else
		{                       
			base[i|0x000]=0x7C00;
			base[i|0x100]=0xFC00;
			shift[i|0x000]=13;
			shift[i|0x100]=13;
		}
	}
}

half_float::float_to_half::tables t;
half_float::half_to_float::mantissa m;
half_float::half_to_float::exponent e;
half_float::half_to_float::offset o;

float half_float::half_to_float::convert(unsigned short h)
{
	unsigned int f_ = m[o[h>>10]+(h&0x3ff)]+e[h>>10];
	return *(float*)&f_;
}

unsigned short half_float::float_to_half::convert(float f_)
{
	unsigned int f = *(unsigned int*)&f_;
	return t.base[(f>>23)&0x1ff]+((f&0x007fffff)>>t.shift[(f>>23)&0x1ff]);
} 

	




// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.