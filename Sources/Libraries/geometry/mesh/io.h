// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stream/library.include.h"
#include "mesh.h"

namespace Geometry
{


template<class V>
stream::ostream& operator <<(stream::ostream& s, const Geometry::VertexStream<V>& vs)
{
	std::size_t size = vs.size(); s << size;
	if(size > 0)
	{
		s.write(&vs[0], vs.memory());
	}
	
	return s;
}

template<class V>
stream::istream& operator >>(stream::istream& s, Geometry::VertexStream<V>& vs)
{
	std::size_t size; s >> size;
	vs.resize(size);
	if(size > 0) 
	{
		s.read(&vs[0], vs.memory());
	}

	return s;
}


template<class I>
stream::ostream& operator <<(stream::ostream& s, const Geometry::IndexStream<I>& is)
{
	std::size_t size = is.size(); s << size;
	if(size > 0)
	{
		s.write(&is[0], is.memory());
	}
	
	s << is.getMaxIndexValue();
	return s;
}

template<class I>
stream::istream& operator >>(stream::istream& s, Geometry::IndexStream<I> & is)
{
	std::size_t size; s >> size;
	is.resize(size);
	if(size > 0) 
	{
		s.read(&is[0], is.memory());
	}

	int maxIdxVal; s >> maxIdxVal;
	is.setMaxValue(maxIdxVal);
	return s;
}


template<class V, class I> stream::ostream& operator <<(stream::ostream& s, const Geometry::Mesh<V, I>& m)	{	return s << m.vertices() << m.indices();	}
template<class V, class I> stream::istream& operator >>(stream::istream& s, Geometry::Mesh<V, I>& m)		{	return s >> m.vertices() >> m.indices();	}
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