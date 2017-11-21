#pragma once

#include "defaultLogs/library.include.h"
#include "gapi/library.include.h"

#include "interfaces.h"

namespace dx11
{	
	//
	//
	//
	template<gapi::PrimitiveTopology pt>
	class SetPrimitiveTopology : public DrawableObject
	{
	public:
		virtual void execute(int deviceIndex)
		{
			(*devices)[deviceIndex].context->IASetPrimitiveTopology(gapi::Topology<gapi::DX11, pt>::topology);
		}
	};

	//
	//
	//
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

	//
	//
	//
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




