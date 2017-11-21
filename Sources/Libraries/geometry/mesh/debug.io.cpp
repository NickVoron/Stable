#include "debug.io.h"

const char* open = "(";
const char* close = ")";
const char* d = ", ";

//
std::ostream& operator <<(std::ostream& s, const Geometry::POS1& v)		{ return s << "POS1"		<< open << v.x << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::POS2& v)		{ return s << "POS2"		<< open << v.x << d << v.y << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::POS3& v)		{ return s << "POS3"		<< open << v.x << d << v.y << d << v.z << close;}
std::ostream& operator <<(std::ostream& s, const Geometry::POS4& v)		{ return s << "POS4"		<< open << v.x << d << v.y << d << v.z << d << v.w << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::POST& v)		{ return s << "POST"		<< open << v.x << d << v.y << d << v.z << d << v.w << close; }

//
std::ostream& operator <<(std::ostream& s, const Geometry::DIFFUSE& v)	{ return s << "DIFFUSE"		<< open << v.r << d << v.g << d << v.b << d << v.a << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::SPECULAR& v)	{ return s << "SPECULAR"	<< open << v.r << d << v.g << d << v.b << d << v.a << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::EMISSION& v)	{ return s << "EMISSION"	<< open << v.r << d << v.g << d << v.b << d << v.a << close; }

//
std::ostream& operator <<(std::ostream& s, const Geometry::NORMAL& v)	{ return s << "NORMAL"		<< open << v.x << d << v.y << d << v.z << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::BINORMAL& v)	{ return s << "BINORMAL"	<< open << v.x << d << v.y << d << v.z << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::TANGENT& v)	{ return s << "TANGENT"		<< open << v.x << d << v.y << d << v.z << close; }

//
