#include "locator.h"
#include "angles.h"

namespace nm {

void locator::lookAt( const Vector3& targ, const Vector3& top )
{
	Vector3	xv;
	Vector3 zv = targ - position;
	Vector3	yv = top - position;
	cross(zv, yv, xv);
	cross(zv, xv, yv);
	 
// 	orientation.ToAxes(	xv, yv, zv );
}


float locator::heading() const 
{
	return nm::heading(orientation);
}


float locator::pitch() const
{
	return nm::pitch(orientation);
}


float locator::bank() const
{
	return nm::bank(orientation);
}


Vector3 locator::hpb() const 
{
	Vector3 result;	
	nm::hpb(orientation, result);
	return result;
} 


void locator::setHPB( const Vector3& hpb  )
{
	Quaternion x; x.SetXAxis(-hpb.y);	
	Quaternion y; y.SetYAxis(hpb.x);
	Quaternion z; z.SetZAxis(hpb.z);
	orientation = y * x * z;
} 


void locator::multHPB( const Vector3& hpb )
{
	locator s;
	s.setHPB( hpb );
	orientation *= s.orientation;
}


void locator::lookTo( const Vector3& point )
{
	setHPB( Vector3( azimuth( position, point ), -vertical( position, point ), 0 ) );
}
    
}