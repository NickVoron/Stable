#pragma once

#include "../../../basicTypes/containers/array2.h"
#include <iostream>

namespace Base {




template< class T >
std::ostream& operator<<( std::ostream& s, const Base::Array2<T>& prey )
{
	s << "ARRAY" << prey.GetSize() << std::endl;
	Base::Index it(0,0);

	for ( it.y = 0; it.y < prey.GetSize().y; it.y++ )
	{
		for ( it.x = 0; it.x < prey.GetSize().x; it.x++ )
			s << prey[ it ] << ",";
		s << std::endl;
	}
	  
	s << "ARRAY_END";

	return s;
}





template< class T >
std::istream& operator>>( std::istream& s, Base::Array2<T>& prey )
{
	s >> Base::Parsers::Token("ARRAY");
	Base::Index ind;
	s >> ind;
	prey.Generate( ind );

	for ( it.y = 0; it.y < prey.GetSize().y; it.y++ )
		for ( it.x = 0; it.x < prey.GetSize().x; it.x++ )
		{
			s >> prey[ it ];
			s >> Base::Parsers::Token(",");
		}

	s >> Base::Parsers::Token("ARRAY_END");

	return s;
}


}  // namespace Base