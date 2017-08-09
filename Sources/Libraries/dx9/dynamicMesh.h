// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vertexDeclaration.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "dip.h"
#include "device.h"

namespace dx9{

	
	template<class DrawerType>
	class DynamicMeshT
	{
	public:
		DynamicMeshT(){}
		~DynamicMeshT(){release();}

		void release()
		{
			vb.clear();
			ib.clear();	
		}

		template<class VertexStream> 
		void createVD(){ vd.create<Loki::MakeTypelist<VertexStream::Vertex>>(); }

		template<class VertexStream> 
		void uploadVB(const VertexStream& vs)
		{ 
			createVD<VertexStream>();
			vb.upload(vs); 
			dip.setVerticesCount(vb.getVerticesCount());
		}

		template<class IndexStream>	 
		void uploadIB(const IndexStream& is) 
		{ 
			ib.upload(is);  
			dip.setIndicesCount(ib.getIndicesCount());
		}

		template<class Mesh>
		void uploadMesh(const Mesh& m)
		{
			uploadVB(m.vertices());
			uploadIB(m.indices());
		}

		void setDeclaration() const	{ vd.set(); }
		void setIndices() const		{ ib.set(); }
		void setVertices() const	{ vb.set<0>(); }

		void draw() const
		{
			if(ib.getIndicesCount() > 0 && vb.getVerticesCount() > 0)
			{
				setDeclaration();
				setIndices();
				setVertices();
				dip.call();
			}
		}

		void setType(Buffer::BufferType t)
		{
			vb.setType(t);
			ib.setType(t);
		}

		int getFacesCount() const { return ib.getIndicesCount() / 3; }

		DrawerType dip;

	private:
		VertexBuffer vb;
		IndexBuffer ib;
		VertexDeclaration vd;
	};

	class DynamicMesh : public DynamicMeshT<DrawIndexedPrimitive>{};
	class DynamicLine : public DynamicMeshT<DrawIndexedLine>{};

	class DynamicMeshStrip : public DynamicMeshT< IndexedDrawer<D3DPT_TRIANGLESTRIP> >{};
	


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