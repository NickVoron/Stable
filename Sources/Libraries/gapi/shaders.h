#pragma once

namespace gapi
{
	enum ShaderTypeName
	{
		VERTEX_SHADER				= 1 << 0,
		PIXEL_SHADER				= 1 << 1,
		GEOMETRY_SHADER				= 1 << 2,
		DOMAIN_SHADER				= 1 << 3,
		HULL_SHADER					= 1 << 4,
		COMPUTE_SHADER				= 1 << 5,
		GEOMETRY_STREAMOUT_SHADER	= 1 << 6,
	};
}




