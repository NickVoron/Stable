#include "vector3.io.h"


// Output:
std::ostream& operator<<( std::ostream& s, const Vector4& v ){	return s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";	}
std::ostream& operator<<( std::ostream& s, const Vector3& v ){	return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";}
std::ostream& operator<<( std::ostream& s, const Vector2& v ){	return s << "(" << v.x << ", " << v.y << ")";}

std::ostream &operator<<(std::ostream& s, const std::vector<Vector3> & vector)
{
	return s;
}


// std::istream& operator>>( std::istream& s, Vector4& v )
// {
// 	s >> Base::Parsers::Token("(");
// 	s >> v.x;
// 	s >> Base::Parsers::Token(",");
// 	s >> v.y;
// 	s >> Base::Parsers::Token(",");
// 	s >> v.z;
// 	s >> Base::Parsers::Token(",");
// 	s >> v.w;
// 	s >> Base::Parsers::Token(")");
// 	return s;
// }
// 
// std::istream& operator>>( std::istream& s, Vector3& v )
// {
// 	s >> Base::Parsers::Token("(");
// 	s >> v.x;
// 	s >> Base::Parsers::Token(",");
// 	s >> v.y;
// 	s >> Base::Parsers::Token(",");
// 	s >> v.z;
// 	s >> Base::Parsers::Token(")");
// 	return s;
// }
// 
// std::istream& operator>>( std::istream& s, Vector2& v )
// {
// 
// 	s >> Base::Parsers::Token("(");
// 	s >> v.x;
// 	s >> Base::Parsers::Token(",");
// 	s >> v.y;
// 	s >> Base::Parsers::Token(")");
// 	return s;
// 
// }