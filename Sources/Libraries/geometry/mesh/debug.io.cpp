// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "debug.io.h"

const char* open_sym = "(";
const char* close_sym = ")";
const char* d_sym = ", ";


std::ostream& operator <<(std::ostream& s, const Geometry::POS1& v)		{ return s << "POS1"		<< open_sym << v.x << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::POS2& v)		{ return s << "POS2"		<< open_sym << v.x << d_sym << v.y << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::POS3& v)		{ return s << "POS3"		<< open_sym << v.x << d_sym << v.y << d_sym << v.z << close_sym;}
std::ostream& operator <<(std::ostream& s, const Geometry::POS4& v)		{ return s << "POS4"		<< open_sym << v.x << d_sym << v.y << d_sym << v.z << d_sym << v.w << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::POST& v)		{ return s << "POST"		<< open_sym << v.x << d_sym << v.y << d_sym << v.z << d_sym << v.w << close_sym; }


std::ostream& operator <<(std::ostream& s, const Geometry::DIFFUSE& v)	{ return s << "DIFFUSE"		<< open_sym << v.r << d_sym << v.g << d_sym << v.b << d_sym << v.a << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::SPECULAR& v)	{ return s << "SPECULAR"	<< open_sym << v.r << d_sym << v.g << d_sym << v.b << d_sym << v.a << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::EMISSION& v)	{ return s << "EMISSION"	<< open_sym << v.r << d_sym << v.g << d_sym << v.b << d_sym << v.a << close_sym; }


std::ostream& operator <<(std::ostream& s, const Geometry::NORMAL& v)	{ return s << "NORMAL"		<< open_sym << v.x << d_sym << v.y << d_sym << v.z << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::BINORMAL& v)	{ return s << "BINORMAL"	<< open_sym << v.x << d_sym << v.y << d_sym << v.z << close_sym; }
std::ostream& operator <<(std::ostream& s, const Geometry::TANGENT& v)	{ return s << "TANGENT"		<< open_sym << v.x << d_sym << v.y << d_sym << v.z << close_sym; }






// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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