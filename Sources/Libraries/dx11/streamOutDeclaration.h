// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <D3D11.h>

#include "dxgi.h"
#include "inputLayout.h"

#include "geometry/library.include.h"


namespace dx11
{
	template<class VertexElement, int slotIndex, int instanceDataRate> 
	inline void streamOutElement(D3D11_SO_DECLARATION_ENTRY& element)
	{
		element.SemanticName = VertexElement::semanticName();
		element.SemanticIndex = VertexElement::semanticIndex();
		element.OutputSlot = slotIndex;
		element.StartComponent = 0;
		element.ComponentCount = VertexElement::dimensions;
	}

	template<class VertexList, int slotIndex, int instanceDataRate>	struct fillD3DStreamOutElementsArray;
	template<int slotIndex, int instanceDataRate>					struct fillD3DStreamOutElementsArray<Loki::NullType, slotIndex, instanceDataRate> {	inline static void fill(D3D11_SO_DECLARATION_ENTRY*& element){}; };
	template<class VertexList, int slotIndex, int instanceDataRate>	struct fillD3DStreamOutElementsArray
	{
		inline static void fill(D3D11_SO_DECLARATION_ENTRY*& element)
		{
			streamOutElement<VertexList::Head, slotIndex, instanceDataRate>(*element);
			fillD3DStreamOutElementsArray<VertexList::Tail, slotIndex, instanceDataRate>::fill(++element);
		}
	};

	template<int streamIdx, class VertexList>	struct fillD3DStreamOutListElementsArray;
	template<int streamIdx>						struct fillD3DStreamOutListElementsArray<streamIdx, Loki::NullType> { inline static void fill(D3D11_SO_DECLARATION_ENTRY*){} };
  	template<int streamIdx, class VertexList> 	struct fillD3DStreamOutListElementsArray
	{
		inline static void fill(D3D11_SO_DECLARATION_ENTRY*& element)
		{
			fillD3DStreamOutElementsArray<VertexList::Head::ComponentsList, streamIdx, (streamIdx == 0) ? 0 : 1>::fill(element);
			fillD3DStreamOutListElementsArray<streamIdx + 1, VertexList::Tail>::fill(element);
		}
	};

	template<class VertexList>
	struct StreamOutElementsArray
	{
		static const int elementsCount = Geometry::VertexListElementsCounter<VertexList>::count;
		static void init(D3D11_SO_DECLARATION_ENTRY* elements)	
		{ 
			fillD3DStreamOutListElementsArray<0, VertexList>::fill(elements); 
		}
	};
}









// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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