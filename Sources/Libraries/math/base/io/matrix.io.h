#pragma once

#include "../Matrix2.h"
#include "../Matrix3.h"
#include "../Matrix4.h"
#include "../Quaternion.h"
#include <ostream>

// Output:
// std::ostream& operator<<( std::ostream&, const Matrix2& );
std::ostream& operator<<( std::ostream&, const Matrix3& );
std::ostream& operator<<( std::ostream&, const Matrix4& );
std::ostream& operator<<(std::ostream&, const Quaternion&);


// std::istream& operator>>( std::istream&, Vector4& );
// std::istream& operator>>( std::istream&, Vector3& );
// std::istream& operator>>( std::istream&, Vector2& );
