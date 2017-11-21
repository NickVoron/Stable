#include "randomVar.h"
#include "../base/Vector3.h"



namespace Base {
namespace Math {


template<>
float RandomVarBasic< float >::operator()() const
{
	if ( minValue >= maxValue - 0.000001f)
		return minValue;

	return Base::MathUtils::GetRandomFromRange( minValue, maxValue );
}

template<>
int RandomVarBasic< int >::operator()() const
{
	if ( minValue >= maxValue )
		return minValue;
	
	return ( rand() % maxValue-minValue ) + minValue;
}

// ��������: ���� ��� ������� ������� ��������� ������ - ��������� ��� ��������.
//
//

    
template<>
Vector3 RandomVarBasic< Vector3 >::operator()() const
{
		//it is 3D random
	//if ( minValue.x >= maxValue.x )
	//	return minValue;
	//if ( minValue.y >= maxValue.y )
	//	return minValue;
	//if ( minValue.z >= maxValue.z )
	//	return minValue;

	return Vector3(
		MathUtils::GetRandomFromRange( minValue.x, maxValue.x ),
		MathUtils::GetRandomFromRange( minValue.y, maxValue.y ),
		MathUtils::GetRandomFromRange( minValue.z, maxValue.z )
	);
	
	//float rs = MathUtils::GetRandomFromRange( 0.0f, 1.0f );
	//return ( minValue * ( 1.0f - rs ) + maxValue * rs );
}







} //namespace Math
} // namespace Base