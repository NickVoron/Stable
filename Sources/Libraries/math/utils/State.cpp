#include "State.h"
#include "unsorted.h"


void State::LookAt( const Vector3& targ, const Vector3& top )
{
	
	// Hi, SigMan!
	Vector3 zv = targ - position;
	Vector3	yv = top - position;
	Vector3 xv = zv.SCross( yv );
	yv = zv.SCross( xv );

	orientation.ToAxes(	xv, yv, zv );

	//orientation.Identity();
}

void State::Move(int flags, float dist)
{
	return Move(flags, orientation, dist);
}

void State::Move(int flags, const Quaternion& o, float dist)
{
	if(flags != 0 )
	{
		Vector3 dir(0,0,0);
		if(flags & X) dir += o.GetXAxis();
		if(flags & Y) dir += o.GetYAxis();
		if(flags & Z) dir += o.GetZAxis();

		dir.Normalize();
		position += dir * dist;
	}	
}





//Vector3 State::GetNormal() const 
//{
//	return Vector3( 0, 1, 0 ); // How are you? ;)
//}



void
State::GetTransform( Matrix4& tr ) const
{
	tr.Identity();
	orientation.GetTransformation( tr, false );
	tr(0,3) = position.x;
	tr(1,3) = position.y;
	tr(2,3) = position.z;
}


Vector3
State::GetLocalVector( const Vector3& v ) const
{
	Vector3 res;
	res = orientation.Transform( v );
	res += position;
	return res;
	
}

Vector3
State::GetGlobalShift( const Vector3& v ) const
{
	return orientation.Transform( v );
}

Vector3
State::GetReverseVector( const Vector3& v ) const
{
	Vector3 res;
	res = orientation.InverseTransform( v - position );
	return res;
}


float
State::GetHeading() const 
{
	return Base::MathUtils::QuaternionHeading(orientation);
}

float
State::GetPitch() const
{
	return Base::MathUtils::QuaternionPitch(orientation);
}

float
State::GetBank() const
{
	return Base::MathUtils::QuaternionBank(orientation);
}



Vector3
State::GetHPB() const 
{
	return Base::MathUtils::QuaterionToHPB(orientation);;
}


void
State::SetHPB( const Vector3& hpb  )
{
// 	Matrix3 trans;
// 	trans.Identity();
// 	
// 	trans.Rotate( 0, 0, -hpb.z );
// 	trans.Rotate( hpb.y, 0, 0);
// 	trans.Rotate( 0, -hpb.x, 0);

	Quaternion x; x.SetXAxis(-hpb.y);	
	Quaternion y; y.SetYAxis(hpb.x);
	Quaternion z; z.SetZAxis(hpb.z);
	orientation = y * x * z;//.Set( trans );
}


void
State::MultHPB( const Vector3& hpb )
{
	State s;
	s.SetHPB( hpb );
	orientation *= s.orientation;
}


void
State::SetTransform( const Matrix4& m )
{
	// Sets w/o scale:
	Matrix3 m3;
	m3.Identity();
	m.GetMatrix3( m3 );
	m3.Transpose();
	orientation.Set( m3 );
	position.x = m(0,3);
	position.y = m(1,3);
	position.z = m(2,3);
}

State
State::GetReverseState( const State& st ) const
{
	State res;
	
	Matrix4 mloc, mson;
	mloc.Identity();
	mson.Identity();
	
	GetTransform( mloc );
	st.GetTransform( mson );
		
	mloc.Invert();
	mloc.Multiply( mson );
	res.SetTransform( mloc );

	/*
	GetTransform( mloc );
	st.GetTransform( mson );
		
	mson.Invert();
	mson.Multiply( mloc );
	res.SetTransform( mson );
    */	
	return res;
}

State State::operator *( const State& b ) const 
{
	State c(*this);
	Matrix4 m;
	GetTransform( m );
	c.position = m * b.position;
	c.orientation = orientation * b.orientation;
	c.orientation.Normalize();
	return c;
}


State State::operator +( const State& b ) const
{
	State res(*this); res += b;	return res;
}

State State::operator -( const State& b ) const
{
	State res(*this); res -= b;	return res;
}

void State::operator+= (const State& b)
{
	position += b.position;
	orientation = b.orientation * orientation;
	orientation.Normalize();
}

void State::operator-= (const State& b)
{
	position -= b.position;
	orientation *= b.orientation.GetInverse();
	orientation.Normalize();
}

void
State::LookToPoint( const Vector3& point )
{
	float az = Base::MathUtils::GetAzimuthPos( position, point );
	float vert = Base::MathUtils::GetVertical( position, point );
	SetHPB( Vector3( az, -vert, 0 ) );
}


void
State::DragByPoint( const Vector3& localPoint, const Vector3& shift )
{
	// Fake:
	//position += shift;
	//return;
	
	// Drags (with rotation) object by it's local point, by global shift.
	Vector3 locShift = orientation.InverseTransform( shift );
	Vector3 resultPointLocal = localPoint + locShift;
	
	float localPointMag = localPoint.Magnitude();
	float resPointLocalMag = resultPointLocal.Magnitude();
	
	if ( ( localPointMag <= 0.00001f ) || (resPointLocalMag <= 0.00001f ) )
	//if ( true )
		position += shift;
	else
	{
		//position += shift;
		
		// Rotate and move body:
		Vector3 rotor = localPoint.SCross( resultPointLocal );
		if ( rotor.MagnitudeSquared() > 0.00000001f )
		{
			float rotAngle = rotor.Magnitude() / ( localPointMag * resPointLocalMag ); 

			Quaternion rot;
			rot.Set( rotAngle, rotor, true );
            
			Vector3 centerShift = resultPointLocal - rot.Transform( localPoint );
			//position += orientation.Transform( centerShift );
			position += shift * cosf( rotAngle );

			orientation *= rot;
		}
		else
			position += shift;
    }
}

void
State::RotateByPoint( const Vector3& localPoint, const Vector3& shift )
{
	// Drags (with rotation) object by it's local point, by global shift.

	Vector3 locShift = orientation.InverseTransform( shift );
	Vector3 resultPointLocal = localPoint + locShift;

	float localPointMag = localPoint.Magnitude();
	float resPointLocalMag = resultPointLocal.Magnitude();
  
	if ( ( localPointMag <= 0.00001f ) || (resPointLocalMag <= 0.00001f ) )
	//if ( true )
	{}//position += shift;
	else
	{
		//position += shift;
		
		// Rotate and move body:
		Vector3 rotor = localPoint.SCross( resultPointLocal );
		if ( rotor.MagnitudeSquared() > 0.00000001f )
		{
			float rotAngle = rotor.Magnitude() / ( localPointMag * resPointLocalMag ); 

			Quaternion rot;
			rot.Set( rotAngle, rotor, true );
            
			Vector3 centerShift = resultPointLocal - rot.Transform( localPoint );
			//position += shift * cosf( rotAngle );

			orientation *= rot;

			
		}
    }
}

void State::GetPlaneXY(nm::plane& plane) const
{
	plane.n = orientation.GetZAxis();
	plane.d = -nm::dot( plane.n, position );
}

void State::GetPlaneXZ(nm::plane& plane) const
{
	plane.n = orientation.GetYAxis();
	plane.d = -nm::dot( plane.n, position );
}

void State::GetPlaneYZ(nm::plane& plane) const
{
	plane.n = orientation.GetXAxis();
	plane.d = -nm::dot( plane.n, position );
}

bool State::operator == (const State& state) const
{
	return position == state.position && orientation == state.orientation;
}

bool State::operator != (const State& state) const
{
	return !operator==(state);
}

    