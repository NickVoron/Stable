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

	// Initialization:
	inline void SetIdentity()			{ position = Vector3(0,0,0); orientation.Identity(); }
	
	//Selectors:
	void GetTransform( Matrix4& ) const ;
	void SetTransform( const Matrix4& );
	Vector3 GetLocalVector( const Vector3& ) const;		// преобразование из локальной координатной системы в глобальную 
	Vector3 GetGlobalShift( const Vector3& ) const;
	Vector3 GetReverseVector( const Vector3& ) const;	// преобразование из глобальной системы координат в локальную 

	float GetHeading() const;
	float GetPitch() const;
	float GetBank() const;
	Vector3 GetHPB() const;

	void DragByPoint( const Vector3& localPoint, const Vector3& shift );
	void RotateByPoint( const Vector3& localPoint, const Vector3& shift );

	// Modifiers:
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

	// получение плоскостей проходящих через position
	void GetPlaneXY(nm::plane& plane) const;
	void GetPlaneXZ(nm::plane& plane) const;
	void GetPlaneYZ(nm::plane& plane) const;
	
	State operator* ( const State& ) const;
	State operator+ ( const State& ) const;
	State operator- ( const State& ) const;

	void operator+= (const State&);
	void operator-= (const State&);


	State GetReverseState( const State& ) const;

	// Binary compare
	bool operator == (const State& state) const;
	bool operator != (const State& state) const;
};




