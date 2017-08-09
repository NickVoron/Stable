// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "debug.io.h"

const char* open = "(";
const char* close = ")";
const char* d = ", ";


std::ostream& operator <<(std::ostream& s, const Geometry::POS1& v)		{ return s << "POS1"		<< open << v.x << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::POS2& v)		{ return s << "POS2"		<< open << v.x << d << v.y << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::POS3& v)		{ return s << "POS3"		<< open << v.x << d << v.y << d << v.z << close;}
std::ostream& operator <<(std::ostream& s, const Geometry::POS4& v)		{ return s << "POS4"		<< open << v.x << d << v.y << d << v.z << d << v.w << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::POST& v)		{ return s << "POST"		<< open << v.x << d << v.y << d << v.z << d << v.w << close; }


std::ostream& operator <<(std::ostream& s, const Geometry::DIFFUSE& v)	{ return s << "DIFFUSE"		<< open << v.r << d << v.g << d << v.b << d << v.a << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::SPECULAR& v)	{ return s << "SPECULAR"	<< open << v.r << d << v.g << d << v.b << d << v.a << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::EMISSION& v)	{ return s << "EMISSION"	<< open << v.r << d << v.g << d << v.b << d << v.a << close; }


std::ostream& operator <<(std::ostream& s, const Geometry::NORMAL& v)	{ return s << "NORMAL"		<< open << v.x << d << v.y << d << v.z << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::BINORMAL& v)	{ return s << "BINORMAL"	<< open << v.x << d << v.y << d << v.z << close; }
std::ostream& operator <<(std::ostream& s, const Geometry::TANGENT& v)	{ return s << "TANGENT"		<< open << v.x << d << v.y << d << v.z << close; }






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