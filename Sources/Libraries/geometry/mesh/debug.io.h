// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "mesh.h"

namespace Geometry
{
	struct ComponentStreamOut
	{
		ComponentStreamOut(std::ostream& s) : os(s){}

		template<class VertexComponent>
		void operator()(const VertexComponent& component) const
		{
			os << component << " ";
		}

		std::ostream& os;
	};
}


std::ostream& operator<<(std::ostream& s, const Geometry::POS1& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POS2& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POS3& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POS4& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POST& v);


std::ostream& operator<<(std::ostream& s, const Geometry::DIFFUSE& v);
std::ostream& operator<<(std::ostream& s, const Geometry::SPECULAR& v);
std::ostream& operator<<(std::ostream& s, const Geometry::EMISSION& v);


std::ostream& operator<<(std::ostream& s, const Geometry::NORMAL& v);
std::ostream& operator<<(std::ostream& s, const Geometry::BINORMAL& v);
std::ostream& operator<<(std::ostream& s, const Geometry::TANGENT& v);


template<unsigned int usageIdx> std::ostream& operator<<(std::ostream& s, const Geometry::UV1<usageIdx>& v) { return s << "UV1<" << usageIdx << ">" << "(" << v.u << ")"; }
template<unsigned int usageIdx> std::ostream& operator<<(std::ostream& s, const Geometry::UV2<usageIdx>& v) { return s << "UV2<" << usageIdx << ">" << "(" << v.u << ", " << v.v << ")"; }
template<unsigned int usageIdx> std::ostream& operator<<(std::ostream& s, const Geometry::UV3<usageIdx>& v) { return s << "UV3<" << usageIdx << ">" << "(" << v.u << ", "  << v.v << ", "  << v.w << ")"; }
template<unsigned int usageIdx> std::ostream& operator<<(std::ostream& s, const Geometry::UV4<usageIdx>& v) { return s << "UV4<" << usageIdx << ">" << "(" << v.u << ", "  << v.v << ", "  << v.w << ", "  << v.s << ")"; }

template<class VertexList>
std::ostream& operator<<(std::ostream& s, const Geometry::Vertex<VertexList>& v)
{
	Loki::TupleFunc::exec(v.data, Geometry::ComponentStreamOut(s));

	return s;
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