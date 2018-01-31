// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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