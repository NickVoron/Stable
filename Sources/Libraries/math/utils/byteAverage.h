#pragma once

namespace Base {
namespace MathUtils {

typedef unsigned char BYTE;

////////////////////////////////////////////////////////////////////////////////
//// declarations:

// count average of two BYTEs w/o overflow:
inline BYTE AverageByte( BYTE a, BYTE b );		


////////////////////////////////////////////////////////////////////////////////
//// realizations:
BYTE
AverageByte( BYTE a, BYTE b )
{
	// Результат 
	return ( a >> 1 ) + ( b >> 1 ) + (a % 2)*(b % 2);
}



} // namespace MathUtils
} // namespace Base