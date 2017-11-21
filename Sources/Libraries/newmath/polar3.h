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