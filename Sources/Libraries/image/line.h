#pragma once

#include "color.h"

#pragma pack(push,1)

namespace image
{


template<typename ColorT> 
struct Line
{
	typedef ColorT ColorType;

	ColorType* data = nullptr;
	int sizeX = 0;

	Line(){}
	Line(void* ptr, int sx){ init(ptr, sx); }

	void init(void* ptr, int sx)
	{
		data = (ColorType*)ptr;
		sizeX = sx;
	}

	ColorType& operator ()(int x)	const { return data[x]; }

// 	ColorType interpolate(float u)	const
// 	{
// 		float dx = u * width - 0.5f;
// 		float xf = float(int(dx)); dx -= xf;
// 		int x = int(xf);
// 		int x1 = x + 1;
// 		if (x < 0) x += int(width);  if (x1 >= int(width))  x1 -= width;
// 
// 		return Base::Lerp(line[x], line[x1], dx);
// 	}

};


// struct ColorBlock
// {
// 	unsigned short color[2];
// 	unsigned int bmp;
// };


/*
Тип, в котором хранится изображение отделён от типа, в котором возвращаются значения,
что позволяет сохранить синтаксис для работы с блочными изображениями и оптимизировать
возвращаемые значения в более простом формате, нежели битово упакованные форматы
например, R5G6B5 исходный формат возвращает значения цвета в формате R8G8B8, для
последующего отображения без перекомпрессии и потери качества интерполяции
*/
// template<> 
// class ImageLine<ColorBlock, Color<unsigned char,4> >
// {
// public:
// 	typedef Color<unsigned char,4> TOp;
// 	ColorBlock* line;
// 	unsigned int width, row;
// 
// 	ImageLine(ColorBlock* b, unsigned int w, unsigned int r) : line(b), width(w), row(r) {}
// 
// 	//возвращает тип
// 	TOp operator ()(unsigned int x) const
// 	{
// 		return TOp();
// 	}
// 
// 	//
// 	TOp interpolate(float u)	const
// 	{
// 		float dx = u * width - 0.5f;
// 		float xf = float(int(dx)); dx -= xf;
// 		int x = int(xf);
// 		int x1 = x + 1;
// 		if (x < 0) x += int(width);  if (x1 >= int(width))  x1 -= width;
// 
// 		TOp c;
// 		c =  TOp(operator()(x)) * (1.0f-dx);
// 		c += TOp(operator()(x1)) * dx;
// 		return c;
// 	}
// };


};	//art

#pragma pack(pop)
