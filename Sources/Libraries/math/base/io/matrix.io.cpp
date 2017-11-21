#include "matrix.io.h"


// Output:

std::ostream& operator<<( std::ostream& s, const Matrix3& mat )
{
	s << "(" << mat(0,0) << ", "<< mat(0,1) << ", " << mat(0,2) << ")\n";
	s << "(" << mat(1,0) << ", "<< mat(1,1) << ", " << mat(1,2) << ")\n";
	s << "(" << mat(2,0) << ", "<< mat(2,1) << ", " << mat(2,2) << ")\n";
	return s;
}

std::ostream& operator<<(std::ostream& s, const Quaternion& q)
{
	return s << "(" << q.v.x << ", " << q.v.y << ", " << q.v.z << ", " << q.w << ")";
}


// std::ostream& operator<<( std::ostream& s, const Vector3& v )
// {
// 	s << "(" << v.x << "," << v.y << "," << v.z<< ")";
// 	return s;
// }
// 
// std::ostream& operator<<( std::ostream& s, const Vector2& v )
// {
// 	s << "(" << v.x << "," << v.y << ")";
// 	return s;
// }


