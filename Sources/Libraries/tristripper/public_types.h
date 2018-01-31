








#ifndef TRI_STRIPPER_HEADER_GUARD_PUBLIC_TYPES_H
#define TRI_STRIPPER_HEADER_GUARD_PUBLIC_TYPES_H

#include <vector>




namespace triangle_stripper
{

	typedef size_t index;
	typedef std::vector<index> indices;

	enum primitive_type
	{
		TRIANGLES		= 0x0004,	
		TRIANGLE_STRIP	= 0x0005	
	};

	struct primitive_group
	{
		indices			Indices;
		primitive_type	Type;
	};

	typedef std::vector<primitive_group> primitive_vector;

}




#endif 
