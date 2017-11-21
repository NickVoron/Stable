#pragma once

#include "math/library.include.h"

namespace CameraSystem
{
	class InputCollector
	{
	public:
		struct Params 
		{
			Params();

			void defaults();
			Vector3 hpb;
			Vector3 pos;
		};
				
		void update(float dt);
	
		void heading(float delta);
		void pitch(float delta);		
		void bank(float delta);

		void forward(float delta);
		void vertical(float delta);
		void horizontal(float delta);

		inline const Vector3& hpb() const { return current.hpb; }
		inline const Vector3& pos() const { return current.pos; }

	protected:
		Params accumulated;
		Params current;
	};
}