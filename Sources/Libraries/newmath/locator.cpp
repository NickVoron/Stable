// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.