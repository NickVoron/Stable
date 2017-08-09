// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "defaultLogs/library.include.h"
#include "gapi/library.include.h"

#include "interfaces.h"

namespace dx11
{	
	
	
	
	template<gapi::PrimitiveTopology pt>
	class SetPrimitiveTopology : public DrawableObject
	{
	public:
		virtual void execute(int deviceIndex)
		{
			(*devices)[deviceIndex].context->IASetPrimitiveTopology(gapi::Topology<gapi::DX11, pt>::topology);
		}
	};

	
	
	
	class DrawPrimitive : public DrawableObject
	{
	public:
		DrawPrimitive() : vtxCount(0), startVtxLocation(0)
		{
		}

		virtual void execute(int deviceIndex)
		{
			(*devices)[deviceIndex].context->Draw(vtxCount, startVtxLocation);
		}

		void setup(unsigned int vertexCount, unsigned int startVertexLocation)
		{
			vtxCount = vertexCount;
			startVtxLocation = startVertexLocation;
		}

	private:
		unsigned int vtxCount;
		unsigned int startVtxLocation;
	};

	
	
	
	class DrawIndexedPrimitive : public DrawableObject
	{
	public:
		DrawIndexedPrimitive() : idxCount(0), startIndex(0), baseVertex(0)
		{
		}

		virtual void execute(int deviceIndex)
		{
			(*devices)[deviceIndex].context->DrawIndexed(idxCount, startIndex, baseVertex);
		}

		void setup(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation)
		{
			idxCount = indexCount;
			startIndex = startIndexLocation;
			baseVertex = baseVertexLocation;
		}

	private:
		unsigned int idxCount;
		unsigned int startIndex;
		int baseVertex;
	};

	class DrawIndexedInstancedPrimitive : public DrawableObject
	{
	public:
		DrawIndexedInstancedPrimitive() : idxCountPerInstance(0), instCount(0), startIdxLocation(0), startInstLocation(0), baseVtxLocation(0)
		{
		}

		virtual void execute(int deviceIndex)
		{
			(*devices)[deviceIndex].context->DrawIndexedInstanced(idxCountPerInstance, instCount, startIdxLocation, baseVtxLocation, startInstLocation);
		}

		void setup(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int baseVertexLocation, unsigned int startInstanceLocation)
		{
			idxCountPerInstance = indexCountPerInstance;
			instCount = instanceCount;
			startIdxLocation = startIndexLocation;
			baseVtxLocation = baseVertexLocation;
			startInstLocation = startInstanceLocation;
		}

	private:
		unsigned int idxCountPerInstance;
		unsigned int instCount;
		unsigned int startIdxLocation;
		unsigned int startInstLocation;

		int baseVtxLocation;
	};
}








// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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