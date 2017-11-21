/*//////////////////////////////////////////////////////////////
    Author:
        Rever
    Declared:
        <>
    Note:
        <>

*//////////////////////////////////////////////////////////////
#pragma once

#include "../Vector3.h"
#include "../Vector2.h"
#include "../Vector4.h"
#include <ostream>
#include <vector>


// Output:
std::ostream& operator<<( std::ostream&, const Vector4& );
std::ostream& operator<<( std::ostream&, const Vector3& );
std::ostream& operator<<( std::ostream&, const Vector2& );

std::ostream &operator <<(std::ostream &, const std::vector<Vector3> & vector);


//Input:
// std::istream& operator>>( std::istream&, Vector4& );
// std::istream& operator>>( std::istream&, Vector3& );
// std::istream& operator>>( std::istream&, Vector2& );
// 


