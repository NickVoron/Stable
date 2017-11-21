#pragma once

#include "vector.h"
#include "quaternion.h"


namespace nm
{
	class locator 
	{
	public:
		Vector3 position;
		Quaternion orientation;

		// Initialization:
		inline void identity()			{ position = Vector3(0,0,0); orientation.Identity(); }

		float heading() const;
		float pitch() const;
		float bank() const;
		Vector3 hpb() const;

		// Modifiers:
		void lookAt( const Vector3& direction, const Vector3& up);
		void lookTo( const Vector3& point);
		void setHPB( const Vector3& hpb);
		void multHPB( const Vector3& hpb);
	};

}



