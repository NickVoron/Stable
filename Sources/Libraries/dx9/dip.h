#pragma once
#include <d3d9.h>

#include "gapi/library.include.h"

namespace dx9
{
	inline int EvaluatePrimitiveCount(int indicesCount, D3DPRIMITIVETYPE pt)
	{
		switch (pt)
		{
		case D3DPT_POINTLIST:	   return indicesCount + 0;
		case D3DPT_LINELIST:	   return indicesCount / 2;
		case D3DPT_LINESTRIP:	   return indicesCount - 1;
		case D3DPT_TRIANGLELIST:   return indicesCount / 3;
		case D3DPT_TRIANGLESTRIP:  return indicesCount - 2;
		case D3DPT_TRIANGLEFAN:	   return indicesCount - 2;
		}

		return 0;
	}

	struct DrawingStats 
	{
		static int callCounter;
		static int primitivesCounter;
		static void resetCounters();
	};

	template<D3DPRIMITIVETYPE primType>
	struct Drawer : public DrawingStats
	{
		inline Drawer():primitivesCount(0) {}
		unsigned int primitivesCount;
	};

	template<D3DPRIMITIVETYPE primitiveType>
	class IndexedDrawer : public Drawer<primitiveType>
	{
	public:
		inline IndexedDrawer():verticesCount(0),baseVertexIndex(0),minVertexIndex(0),startVertexIndex(0){}

		inline void setIndicesCount(unsigned int indCount)			{ this->primitivesCount = gapi::PrimitiveCountEvaluator9<primitiveType>::eval(indCount); }
		inline void setVerticesCount(unsigned int vertCount)		{verticesCount = vertCount;}
		inline void setBaseVertexIndex(unsigned int baseIndex)		{baseVertexIndex = baseIndex;}
		inline void setStartVertexIndex(unsigned int startIndex)	{startVertexIndex = startIndex;}

		inline unsigned int getVerticesCount() const				{ return verticesCount;}
		inline unsigned int getPrimitivesCount() const				{ return primitivesCount;}

		inline void call() const
		{
			++this->callCounter;
			this->primitivesCounter += this->primitivesCount;
			device->DrawIndexedPrimitive(primitiveType, baseVertexIndex, 0, verticesCount, 0, this->primitivesCount);
		}

	private:
		unsigned int verticesCount;
		unsigned int baseVertexIndex;
		unsigned int minVertexIndex;
		unsigned int startVertexIndex;
	};

	template<D3DPRIMITIVETYPE primitiveType>
	class NonIndexedDrawer : public Drawer<primitiveType>
 	{
 	public:
		inline NonIndexedDrawer():startVertexIndex(0){}

 		inline void call() const
		{
			++callCounter;
			primitivesCounter += primitivesCount;
			device->DrawPrimitive(primitiveType, startVertexIndex, primitivesCount); 
		}

		inline void setStartVertexIndex(unsigned int startIndex) {startVertexIndex = startIndex;}

	private:
		unsigned int startVertexIndex;
 	};


	class DrawIndexedPrimitive : public IndexedDrawer<D3DPT_TRIANGLELIST>{};
	class DrawIndexedLine : public IndexedDrawer<D3DPT_LINELIST>{};

}