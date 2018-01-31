// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../base/Vector3.h"
#include "../base/Quaternion.h"

#include "newmath/plane.h"

class State 
{
public:
	static State Simple( float x, float y, float z ) { return Simple( Vector3(x,y,z) ); }
	static State Simple( const Vector3& v ) { State s; s.position = v; s.orientation.Identity(); return s; }
	static State Identity() { State s; s.SetIdentity(); return s; }

	Vector3 position;
	Quaternion orientation;

	
	inline void SetIdentity()			{ position = Vector3(0,0,0); orientation.Identity(); }
	
	
	void GetTransform( Matrix4& ) const ;
	void SetTransform( const Matrix4& );
	Vector3 GetLocalVector( const Vector3& ) const;		
	Vector3 GetGlobalShift( const Vector3& ) const;
	Vector3 GetReverseVector( const Vector3& ) const;	

	float GetHeading() const;
	float GetPitch() const;
	float GetBank() const;
	Vector3 GetHPB() const;

	void DragByPoint( const Vector3& localPoint, const Vector3& shift );
	void RotateByPoint( const Vector3& localPoint, const Vector3& shift );

	
	void LookAt( const Vector3& nose, const Vector3& top  );
	void LookToPoint( const Vector3& );
	void SetHPB( const Vector3& );
	void MultHPB( const Vector3& );

	enum AxesFlags
	{
		X = 0x0001,
		Y = 0x0010,
		Z = 0x0100,
		XY = X|Y,
		XZ = X|Z,
		YZ = Y|Z,
		XYZ = X|Y|Z,
	};

	void Move(int flags, float dist);
	void Move(int flags, const Quaternion& o, float dist);

	
	void GetPlaneXY(nm::plane& plane) const;
	void GetPlaneXZ(nm::plane& plane) const;
	void GetPlaneYZ(nm::plane& plane) const;
	
	State operator* ( const State& ) const;
	State operator+ ( const State& ) const;
	State operator- ( const State& ) const;

	void operator+= (const State&);
	void operator-= (const State&);


	State GetReverseState( const State& ) const;

	
	bool operator == (const State& state) const;
	bool operator != (const State& state) const;
};








// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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