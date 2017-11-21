#pragma once

#include <vector>
#include <ostream>
#include "../base/StdFloat.h"

namespace Base {

           
template < class T >  // T is vector
class PWPath {
public:
	PWPath() : setted( false ) {}
	~PWPath() {}

	// selectors:
	inline T operator()( float thetha ) const;
	inline T AdvancedInterpolation( float thetha, float smoothingStrength ) const;
	inline float GetLength() const;
	inline int GetFragmentCount() const { return static_cast<int>( fragments.size() ); }
	inline bool IsEmpty() const { return fragments.empty(); }

	// operations:
	inline void PushBackPoint( const T& newPoint );
	inline void ValidateCashe() const;
	//void SetOrigin( const T& ori ) { origin = ori; }
	void Clear();
	void Test( std::ostream& s );
	

private:
	// int. structures:
	template < class T >
	struct Fragment {
		T end;
		float length;
		float beginsThetha;
		float endsThetha;
	};

	// int. utulities:
	inline T GetStartFor( int n ) const {
		if ( n == 0 ) return origin;
		else return fragments[ n-1 ].end;
	}
	
	mutable std::vector< Fragment<T> > fragments;
	T origin;
	bool setted;
	mutable bool casheIsValid;

};




////////////////////////////////////////////////////////////////////////
// realization:

template< class T >
void
PWPath<T>::Clear()
{
	setted = false;
	casheIsValid = false;
	fragments.clear();
}



template < class T >
void
PWPath<T>::PushBackPoint( const T& newPoint )
{
	// if applied first time, setup origin
	int currCount = static_cast<int>( fragments.size() );

	if ( ! setted )
	{
		setted = true;
		origin = newPoint;
	}
	else
	{
		Fragment<T> newFragment;
		newFragment.end = newPoint;
		newFragment.length = (GetStartFor( currCount ) - newPoint).Magnitude();
		fragments.push_back( newFragment );
	}

	// tell, that cashe is invalid:
	casheIsValid = false;
}

template< class T >
void
PWPath<T>::ValidateCashe() const
{	
	float totalLength = GetLength();
	int fragmentCount = static_cast<int>( fragments.size() );
    
	if ( fragmentCount <= 0 )
		return;

	// setup start thetha:
	fragments[0].beginsThetha = 0.0f;
	for ( int xx = 1; xx < fragmentCount; xx++ )
	{
		T a = GetStartFor( xx - 1 );
		float prevLen = ( fragments[ xx - 1 ].end - a ).Magnitude();
		fragments[ xx-1 ].endsThetha = prevLen / totalLength + fragments[ xx-1 ].beginsThetha;
		fragments[ xx ].beginsThetha = fragments[ xx-1 ].endsThetha; 
	}
	fragments[ fragmentCount-1 ].endsThetha = 1.0f;

	casheIsValid = true;
	
}



template< class T >
float
PWPath<T>::GetLength() const
{
	int ct = static_cast<int>( fragments.size() );
	float result = 0.0f;

	if ( ct == 0 ) return 0;
	else
	{
		for ( int xx = 0; xx < ct; xx++ )
		{
			result += ( GetStartFor( xx ) - fragments[xx].end ).Magnitude();
		}
	}
	return result;
}


template< class T >
typename T
PWPath<T>::operator()( float thetha ) const
{
	if ( ! casheIsValid )
		ValidateCashe();

	// input parameter has only [0..1] range
	thetha = Base::Clamp( thetha, 0.0f, 1.0f );
	if ( thetha <= 0.00001f ) return origin;

	const int frCount = static_cast<int>( fragments.size() );
	T result;
	// locate segment:
	for ( int xx = frCount-1; xx >= 0 ; xx-- )
	{
		if ( fragments[xx].beginsThetha < thetha )
		{
			float tale = thetha - fragments[xx].beginsThetha;
			T start = GetStartFor( xx );

			float taleNorm = tale / (fragments[xx].endsThetha - fragments[xx].beginsThetha );
			result = start + (fragments[xx].end - start) * taleNorm;

			break;
		}
	}
	return result;
}

template< class T >
typename T
PWPath<T>::AdvancedInterpolation( float thetha, float sms ) const
{
	T clearResult = operator()( thetha );

	// for exteremally low smoothing use simple interpolation:
	if ( sms <= 0.0001f )
		return clearResult;

	float bordDist = min( thetha, 1.0f - thetha );
	float smoothEffect = max( 0.0f, min( 1.0f, (bordDist-sms) / sms ) );
	
    T onSmoothing = ( operator()( thetha-sms ) + operator()( thetha+sms ) ) * 0.5f;

	// result is between clear and interpolated:
	return onSmoothing * smoothEffect + clearResult * ( 1.0f - smoothEffect );
}


template< class T >
void
PWPath< T >::Test( std::ostream& s )
{
	// report content to stream:
	s << "Testing path:" << std::endl;
	if ( ! casheIsValid )
	{
		s << "ERROR! cashe is not vaild!" << std::endl;
		return;
	}


	s << "full Length = " << GetLength() << " origin = " << origin << std::endl;
	for ( int xx = 0; xx < static_cast<int>( fragments.size() ); xx++ )
	{
		s << "[" << xx << "] - ends:"
			<< fragments[xx].end
			<< " thetha rng: (" << fragments[ xx ].beginsThetha << " ; " << fragments[xx].endsThetha << ")"
			<< std::endl;
	}

	// use function on [0..1]:
	s << std::endl;
	s << "Texting function" << std::endl;
	for ( float a = 0.0f; a <= 1.0f; a += 0.05f )
		s << "in " << a << " value == " << operator()( a ) << std::endl;

	s << "done..." << std::endl;

}


} // namespace Base