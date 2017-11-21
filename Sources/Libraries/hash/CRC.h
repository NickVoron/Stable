#pragma once

#include <cstddef>

namespace crc
{
	unsigned char	CRC8 ( const void *data, std::size_t length );
	unsigned short	CRC16( const void *data, std::size_t length );
	unsigned long	CRC32( const void *data, std::size_t length );
}



