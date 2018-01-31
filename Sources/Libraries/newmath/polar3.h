// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <cstddef>

class Polar3 
{
public:	
	float radius, theta, phi;

	Polar3();
	Polar3( const float radius, const float theta, const float phi );

	void Set( const float radius, const float theta, const float phi );

	float operator[]( const size_t index ) const;
	float& operator[]( const size_t index );
	Polar3 operator-() const;
	Polar3& operator=( const Polar3 &a );

};

inline Polar3::Polar3() 
{
}

inline Polar3::Polar3( const float radius, const float theta, const float phi ) 
{
	this->radius = radius;
	this->theta = theta;
	this->phi = phi;
}

inline void Polar3::Set( const float radius, const float theta, const float phi ) 
{
	this->radius = radius;
	this->theta = theta;
	this->phi = phi;
}

inline float Polar3::operator[]( const std::size_t index ) const
{
	return ( &radius )[ index ];
}

inline float &Polar3::operator[]( const std::size_t index )
{
	return ( &radius )[ index ];
}

inline Polar3 Polar3::operator-() const 
{
	return Polar3( radius, -theta, -phi );
}

inline Polar3 &Polar3::operator=( const Polar3 &a ) 
{ 
	radius = a.radius;
	theta = a.theta;
	phi = a.phi;
	return *this;
}



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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