// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <D3D11.h>

#include "geometry/library.include.h"

#include "dxgi.h"
#include "shaders.h"
#include "vertexShaderUtil.h"




namespace dx11
{
	template<class VertexElement> const wchar_t*		SemanticNameW() { return VertexElement::semanticNameW(); }
	
	template<class VertexElement, int slotIndex, int instanceDataRate> 
	inline void inputElement(D3D11_INPUT_ELEMENT_DESC& element)
	{
		element.SemanticName = VertexElement::semanticName();
		element.Format = dxgi::FormatFind<VertexElement::ScalarType, VertexElement::dimensions>::format;;
		element.InputSlot = slotIndex;
		element.InputSlotClass = (instanceDataRate == 0) ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
		element.InstanceDataStepRate = instanceDataRate;
		element.SemanticIndex = VertexElement::semanticIndex();
		element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	}

	template<class VertexList,	int slotIndex, int instanceDataRate>	struct fillD3DVertexElementsArray;
	template<					int slotIndex, int instanceDataRate>	struct fillD3DVertexElementsArray<Loki::NullType, slotIndex, instanceDataRate>	{	inline static void fill(D3D11_INPUT_ELEMENT_DESC*& element){}	};

	template<class VertexList, int slotIndex, int instanceDataRate>
	struct fillD3DVertexElementsArray
	{
		inline static void fill(D3D11_INPUT_ELEMENT_DESC*& element)
		{
			inputElement<VertexList::Head, slotIndex, instanceDataRate>(*element);
			fillD3DVertexElementsArray<VertexList::Tail, slotIndex, instanceDataRate>::fill(++element);
		}
	};

	template<int streamIdx, class VertexList> struct fillD3DVertexListElementsArray;

	template<int streamIdx> struct fillD3DVertexListElementsArray<streamIdx, Loki::NullType>	{	inline static void fill(D3D11_INPUT_ELEMENT_DESC*){} };

	template<int streamIdx, class VertexList> 
	struct fillD3DVertexListElementsArray
	{
		inline static void fill(D3D11_INPUT_ELEMENT_DESC*& element)
		{
			fillD3DVertexElementsArray<VertexList::Head::ComponentsList, streamIdx, (streamIdx == 0) ? 0 : 1>::fill(element);
			fillD3DVertexListElementsArray<streamIdx + 1, VertexList::Tail>::fill(element);
		}
	};

	template<class VertexList>
	struct ElementsArray
	{
		static const int elementsCount = Geometry::VertexListElementsCounter<VertexList>::count;
		D3D11_INPUT_ELEMENT_DESC inputElements[elementsCount];

		ElementsArray()	
		{ 
			D3D11_INPUT_ELEMENT_DESC* elements = inputElements; 
			fillD3DVertexListElementsArray<0, VertexList>::fill(elements); 
		}
	};

	
	struct DeviceInputLayout : public DeviceResource<ID3D11InputLayout, DeviceInputLayout>
	{
		template<class VertexList>
		void create(int deviceIdx, const mem::mem_desc& vertexShaderCode)
		{
			static ID3D11InputLayout* ilayout[Devices::MAX_COUNT] = { 0, 0, 0, 0 };

			if(!ilayout[deviceIdx])
			{
				ElementsArray<VertexList> elements;
				HRESULT hr = device->device->CreateInputLayout(elements.inputElements, elements.elementsCount, vertexShaderCode.data, vertexShaderCode.len, &ilayout[deviceIdx]);
			}
			
			ptr = ilayout[deviceIdx];
		}

		template<class VertexList>
		void create(int deviceIdx)
		{
			create<VertexList>(deviceIdx, VertexShaderUtil<VertexList>::code());
		}

		void set() const { device->context->IASetInputLayout(ptr); }
	};

	struct InputLayout : public DeviceInputLayout::StateSet { };
}









// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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