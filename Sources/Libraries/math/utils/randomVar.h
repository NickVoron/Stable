#pragma once

#include "unsorted.h"	  

namespace Base {
namespace Math {

template< class T >
class RandomVarBasic {
public:
	RandomVarBasic() {}//:  minValue( Zero<T>() ), maxValue( Unit<T>() ) {}
	RandomVarBasic( const T& a, const T& b ) : minValue(a), maxValue(b) {}
	
	void set( const T& a, const T& b ){ minValue = a; maxValue = b; }
	T operator()() const;

	// selectors:
	const T& GetMaxValue() const { return maxValue; }
	const T& GetMinValue() const { return minValue; }
	
protected:
	// data:
	T minValue;
	T maxValue;
};


} //namespace Math
} // namespace Base