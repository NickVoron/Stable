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





