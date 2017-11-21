#pragma once

#include "vertexDeclaration.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "dip.h"
#include "device.h"

namespace dx9{

	// DynamicMesh
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
	
/*
	// 
	class InstancedMesh
	{
	public:

		InstancedMesh(){}
		~InstancedMesh(){release();}

		void release()	{ ivb.clear(); vb.clear(); ib.clear(); }

		template<class MeshVertex, class InstanceVertex>
		void createVD(){ vd.create<TYPELIST_2(MeshVertex, InstanceVertex)>(); }

		template<class InstanceVertex>
		void uploadInstancesVB(const VertexStream<InstanceVertex>& vs)
		{ 
			ivb.upload(vs); 
		}

		template<class MeshVertex>
		void uploadMeshVB(const VertexStream<MeshVertex>& vs)
		{ 
			vb.upload(vs); 
			dip.setVerticesCount(vb.getVerticesCount());
		}

		template<class IndexStream>	 
		void uploadMeshIB(const IndexStream& is) 
		{ 
			ib.upload(is);  
			dip.setIndicesCount(ib.getIndicesCount());
		}

		void setDeclaration()	{ vd.set(); }
		void setIndices()		{ ib.set(); }
		void setMeshVertices()		{ vb.set<0>(); }
		void setInstanceVertices()	{ vb.set<1>(); }
		
		int getInstancesCount() const { return ivb.getVerticesCount(); }

		void draw()
		{
			vd.set();
			ib.set();
			vb.set<0>();
			ivb.set<1>();

			LPDIRECT3DDEVICE9 device = dx9::device;

			device->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | (UINT)ivb.getVerticesCount() ));
			device->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));

			dip.call();

			device->SetStreamSourceFreq(0,1);
			device->SetStreamSourceFreq(1,1);
		}

		void setType(Buffer::BufferType t)
		{
			vb.setType(t);
			ib.setType(t);
		}

	private:
		VertexBuffer vb;
		VertexBuffer ivb;
		IndexBuffer ib;
		VertexDeclaration vd;
		DrawIndexedPrimitive dip;
	};*/

}

