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

//
std::ostream& operator<<(std::ostream& s, const Geometry::POS1& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POS2& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POS3& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POS4& v);
std::ostream& operator<<(std::ostream& s, const Geometry::POST& v);

//
std::ostream& operator<<(std::ostream& s, const Geometry::DIFFUSE& v);
std::ostream& operator<<(std::ostream& s, const Geometry::SPECULAR& v);
std::ostream& operator<<(std::ostream& s, const Geometry::EMISSION& v);

//
std::ostream& operator<<(std::ostream& s, const Geometry::NORMAL& v);
std::ostream& operator<<(std::ostream& s, const Geometry::BINORMAL& v);
std::ostream& operator<<(std::ostream& s, const Geometry::TANGENT& v);

//
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