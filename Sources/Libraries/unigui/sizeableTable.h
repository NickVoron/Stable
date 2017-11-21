#pragma once

#include <float.h>
#include "point2.h"
#include "rect.h"

namespace unigui {
namespace math {

struct DefaultAdditionData{};


template<class T, class AddData = DefaultAdditionData>
class SizeableTable
{
public:
	typedef Point2<T> PointType;
	typedef Rect<T> RectType;
	typedef AddData AdditionData; 

	struct Line
	{
		Line():pos(0), size(0), minSize(0), autosizeCoef(0.0f), autosize(false){}
		T pos, size, minSize;
		float autosizeCoef;
		bool autosize;
	};
	
	static const int maxLineLength = 20;

public:
	SizeableTable():actualRect(RectType::COLLAPSED()), minSize(PointType::ZERO()), columnsCount(0), rowsCount(0){}

	void setLinesBaseSize(const PointType& sizeEveryLine);

	void setStructSize(int columns, int rows) { columnsCount = columns; rowsCount = rows; rebuild();}
	void getStructSize(int& columns, int& rows) const { columns = columnsCount; rows = rowsCount;}
		
	//
	const PointType& getMinSize() const {return minSize;}
	const RectType& getActualRect() const {return actualRect;}
	
	//получение данных из таблицы
	const	Line& row(int index)	const	{return rows[index];}
			Line& row(int index)			{return rows[index];}
	const	Line& column(int index) const	{return columns[index];}
			Line& column(int index)			{return columns[index];}

	Point2<int> cellIndex(const PointType& point) const;
	RectType cell(int columnIndex, int rowIndex) const;
	RectType cellsUnion(int firstColumnIndex, int lastColumnIndex, int firstRowIndex, int lastRowIndex) const;

	// построить таблицу с заданной структурой определенного размера
	void build(const RectType& rect);
	void rebuild();

	void toLeft(const RectType& parentRect);
	void toRight(const RectType& parentRect);
	void toTop(const RectType& parentRect);
	void toBottom(const RectType& parentRect);

	AddData addData[maxLineLength][maxLineLength];

private:
	void rebuild(const RectType& rect, bool rows, bool columns);
	void resetToBaseSize();
	T buildLinesVector(T position, Line* vec, T baseSize, T requestedSize, int lineElemCount);

	Line rows[maxLineLength];
	Line columns[maxLineLength];

	//размер таблицы
	int rowsCount;
	int columnsCount;
	
	PointType minSize; // минимальный физический размер таблицы
	RectType actualRect; //текущий прямоугольник
};

//
//
//
template<class T, class AddData>
void SizeableTable<T, AddData>::setLinesBaseSize(const Point2<T>& sizeEveryLine)
{
	minSize.x = sizeEveryLine.x * columnsCount;
	minSize.y = sizeEveryLine.y * rowsCount;

	for (int i = 0; i < rowsCount; ++i)	rows[i].minSize = sizeEveryLine.y;
	for (int i = 0; i < columnsCount; ++i)	columns[i].minSize = sizeEveryLine.x;
	rebuild();
}

template<class T, class AddData>
void SizeableTable<T, AddData>::resetToBaseSize()
{
	minSize.zero();
	
	for (int i = 0; i < rowsCount; ++i)
	{
		rows[i].size = rows[i].minSize;
		minSize.y += rows[i].minSize;
	}

	for (int i = 0; i < columnsCount; ++i)
	{
		columns[i].size = columns[i].minSize;
		minSize.x += columns[i].minSize;
	}
}

template<class T, class AddData>
Point2<int> SizeableTable<T, AddData>::cellIndex(const PointType& point) const
{
	for (int j = 0; j < rowsCount; ++j)
	{
		for (int i = 0; i < columnsCount; ++i)
		{
			if (cell(i, j).contain(point))
				return Point2<int>(i, j);
		}
	}
}

template<class T, class AddData>
typename SizeableTable<T, AddData>::RectType SizeableTable<T, AddData>::cell(int columnIndex, int rowIndex) const
{
	const Line& r = row(rowIndex);
	const Line& c = column(columnIndex);
	return RectType(c.pos, r.pos, c.size, r.size);
}

template<class T, class AddData>
typename SizeableTable<T, AddData>::RectType SizeableTable<T, AddData>::cellsUnion(int firstC, int lastC, int firstR, int lastR) const
{
	PointType size(0.0f, 0.0f);

	for(int i = firstR; i <= lastR; ++i) size.y += row(i).size;
	for(int i = firstC; i <= lastC; ++i) size.x += column(i).size;

	return RectType(PointType(column(firstC).pos, row(firstR).pos), size);
}

template<class T, class AddData>
T SizeableTable<T, AddData>::buildLinesVector(T position, Line* vec, T baseSize, T requestedSize, int lineElemCount)
{		
	requestedSize = nm::round<T>(requestedSize);
	T delta_size = requestedSize;
	int autosized = 0;
	int zerosized = 0;
	float definedAutoCoef = 0.0f;
	float minimalAutoPart = FLT_MAX;

	float autoCoefs[maxLineLength];	

	for (int i = 0; i < lineElemCount; ++i)
	{
		autoCoefs[i] = 0;

		if (vec[i].autosize)
		{
			float coef = vec[i].autosizeCoef;
			if (coef > 0 && minimalAutoPart > coef)
			{
				minimalAutoPart = coef;
			}
		}
	}


	for (int i = 0; i < lineElemCount; ++i)
	{
		Line& line = vec[i];
		autoCoefs[i] = line.autosizeCoef;
		if(!line.autosize)
		{
			delta_size -= line.minSize;
		}
		else
		{
			definedAutoCoef += line.autosizeCoef;
			++autosized;

			if (line.autosizeCoef == 0)
			{
				autoCoefs[i] = minimalAutoPart;
				definedAutoCoef += minimalAutoPart;
				++zerosized;
			}
		}		
	}

	if (zerosized == autosized && zerosized > 0)
	{
		float coef = 1.0f / zerosized;

		for (int i = 0; i < lineElemCount; ++i)
		{
			if (vec[i].autosize)
			{
				autoCoefs[i] = coef;
			}
		}
	}
	else
	{
		if (definedAutoCoef > 0.0f)
		{
			float definedAutoCoefN = 1.0f / definedAutoCoef;
			for (int i = 0; i < lineElemCount; ++i)
			{
				autoCoefs[i] *= definedAutoCoefN;
			}
		}
	}	

	if(delta_size < 0)
		delta_size = 0;

	T sizeAccum = 0;
	float sizes[maxLineLength];
	int autosizedIdx = -1;

	for (int i = 0; i < lineElemCount; ++i)
	{
		sizes[i] = (vec[i].autosize) ? nm::round(delta_size * autoCoefs[i]) : vec[i].minSize;
		sizeAccum += sizes[i];

		if (vec[i].autosize)
			autosizedIdx = i;
	}

	if (autosizedIdx >=0 && sizeAccum < requestedSize)
	{
		sizes[autosizedIdx] += (requestedSize - sizeAccum);
	}

	sizeAccum = 0;
	for (int i = 0; i < lineElemCount; ++i)
	{
		Line& line = vec[i];
		line.size = sizes[i];
		line.pos = sizeAccum + position;
		sizeAccum += line.size;
	}

	return sizeAccum;
}


template<class T, class AddData>
void SizeableTable<T, AddData>::build(const RectType& rect)
{
	resetToBaseSize();
	rebuild(rect, true, true);
}

template<class T, class AddData>
void SizeableTable<T, AddData>::rebuild(const RectType& rect, bool needBuildRows, bool needBuildColumns)
{
	actualRect.pos = rect.pos;
	if(needBuildColumns) actualRect.size.x = buildLinesVector(rect.pos.x, columns, minSize.x, rect.size.x, columnsCount);
	if(needBuildRows)	 actualRect.size.y = buildLinesVector(rect.pos.y, rows,	minSize.y, rect.size.y, rowsCount);
}

template<class T, class AddData>
void SizeableTable<T, AddData>::rebuild()
{	
	rebuild(actualRect, true, true);
}


template<class T, class AddData>
void SizeableTable<T, AddData>::toLeft(const RectType& parentRect)
{

}

template<class T, class AddData>
void SizeableTable<T, AddData>::toRight(const RectType& parentRect)
{
	RectType ar = actualRect;
	ar.toRight(parentRect);
	T delta = actualRect.right() - ar.right();
	for (int i = 0; i < columnsCount; ++i)
	{
		column(i).pos -= delta;
	}

	actualRect.pos.x = ar.pos.x;
}

template<class T, class AddData>
void SizeableTable<T, AddData>::toTop(const RectType& parentRect)
{

}

template<class T, class AddData>
void SizeableTable<T, AddData>::toBottom(const RectType& parentRect)
{

}

}

typedef math::SizeableTable<float, int> IndexedSizeableTable;
}