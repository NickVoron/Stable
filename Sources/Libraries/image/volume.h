#pragma once

#include "plane.h"

#pragma pack(push,1)

namespace image
{

template<typename ColorT> 
struct Volume
{
	typedef ColorT ColorType;

	ColorType* bmp;
	unsigned int slicePitch, rowPitch;
	int sizeX, sizeY, sizeZ;

	inline Volume():bmp(0), slicePitch(0), rowPitch(0), sizeX(0), sizeY(0), sizeZ(0){}
	inline Volume(void* ptr, unsigned int slicePitch, unsigned int rowPitch, int sX, int sY, int sZ){ init(ptr, slicePitch, rowPitch, sX, sY, sZ); }

	inline void init(void* ptr, unsigned int _slicePitch, unsigned int _rowPitch, int sX, int sY, int sZ)
	{
		bmp = (ColorType*)ptr;
		slicePitch = _slicePitch;
		rowPitch = _rowPitch;
		sizeX = sX;
		sizeY = sY;
		sizeZ = sZ;
	}

	inline Plane<ColorType> operator ()(int z)
	{
		return Plane<ColorType>(&bmp[z*slicePitch/sizeof(bmp[0])], rowPitch, sizeX, sizeY);
	}

};


/*
Растяжение с билинейной интерполяцией, небезопасно - нет анализа на границы изображения назначения
Поддерживает тайлинг
*/
/*
template<typename TfmtSrc, typename TfmtDest>
void stretch(const TfmtSrc& src, const Base::RectPS& rectSrc,
						 const TfmtDest& dest, const Base::RectPS& rectDest)
{	//TODO: оптимизация деления на умножения, не вычислять каждый раз х-координату
	float v = float(rectSrc.top()) / src.size.y;
	float dv = float(rectSrc.size.y) / src.size.y / rectDest.size.y;
	nm::index2 i;
	for(i.y=rectDest.top(); i.y<rectDest.bottom(); i.y++)
	{
		float u = float(rectSrc.left()) / src.size.x;
		float du = float(rectSrc.size.x) / src.size.x / rectDest.size.x;
		for(i.x = rectDest.left(); i.x < rectDest.right(); i.x++)
		{
			dest[i] = src.interpolate(u, v);
			u += du;
		}
		v += dv;
	}
}	*/

						/*
//небезопасный метод - не анализирует выход за границы, поддерживает тайлинг
template<typename TfmtSrc, typename TfmtDest>
void blit(const TfmtSrc& src, const Base::RectPS& rectSrc,
						 const TfmtDest& dest, const Base::RectPS& rectDest)
{
	//TODO: оптимизация через обращение к строкам
	nm::index2 s,d;
	s.y = rectSrc.top();
	for(d.y=rectDest.top(); d.y<rectDest.bottom(); d.y++, s.y++)
	{
		s.x = rectSrc.left();
		for(d.x = rectDest.left(); d.x < rectDest.right(); d.x++, s.x++)
			dest[d] = src[s];
	}
}



					  */
 #pragma pack(pop)

};	//art

