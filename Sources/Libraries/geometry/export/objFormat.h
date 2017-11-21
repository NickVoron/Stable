#pragma once

#include "../mesh/mesh.h"

namespace Geometry {
namespace Export {


template<class VertexType>
struct VertexProcessor
{
	typedef VertexStream<VertexType> VS;

	static void processMesh( const VS& vs, FILE* file )
	{
		using namespace Base::StrUtils;
		unsigned int s = vs.getSize();
		for(unsigned int i = 0; i < s; ++i)
		{
			const POS3& v = vs[i].pos3();
			fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
		}
	}
};

template<class IndexType>
struct FaceProcessor
{
	typedef IndexStream<IndexType> IS;

	static void processMesh( const IS& is, FILE* file )
	{
		using namespace Base::StrUtils;
		unsigned int s = is.getSize() / 3;
		for(unsigned int i = 0; i < s; ++i)
		{
			int bi = i*3;
			fprintf(file, "f %d %d %d\n", is[ bi + 0 ] + 1, is[ bi + 1 ] + 1, is[ bi + 2 ] + 1);
		}
	}
};

template<class VertexType, class IndexType>
struct ObjExporter
{
	static void exportToFile(const Mesh<VertexType, IndexType>& mesh, const char* fileName)
	{
		exportToFile(mesh.vertices(), mesh.indices(), fileName);
	}

	static void exportToFile(const VertexStream<VertexType>& vs, const IndexStream<IndexType>& is, const char* fileName)
	{
		FILE* file = fopen(fileName, "w");
		VertexProcessor<VertexType>::processMesh(vs, file);						
		FaceProcessor<IndexType>::processMesh(is, file);
		fclose(file);
	}
};

template<class VertexType, class IndexType>
void toObj(const Mesh<VertexType, IndexType>& mesh, const char* fileName)
{
	  ObjExporter<VertexType, IndexType>::exportToFile(mesh, fileName);
}

template<class VertexType, class IndexType>
void toObj(const VertexStream<VertexType>& vs, const IndexStream<IndexType>& is, const char* fileName)
{
	 ObjExporter<VertexType, IndexType>::exportToFile(vs, is, fileName);
}


}
}