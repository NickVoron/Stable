// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "thirdPerson.h"

namespace CameraSystem{
namespace Affectors{

ThirdPerson::ThirdPerson()
{


}

void ThirdPerson::apply(float dt, Affector::Context& ctx)
{
	if(ctx.target)
	{
		RangedValue<float>	rise;
		RangedValue<float>	asimuth;
		

		asimuth.SetCycleMode(true); 
		asimuth.SetMin(0);
		asimuth.SetMax(nm::PI * 2);
		rise.SetMin(5 * nm::PI_180); 
		rise.SetMax(nm::PI - 5 * nm::PI_180);
		rise = nm::PI_2; 



		float inv = 1.0f / 270.0f;
		
		distance -= ctx.input.pos().z * inv; 
		rise += ctx.input.hpb().y * inv;

		
		float radius = ctx.target->radius();
		if(distance < radius)
			distance = radius;

		


		Vector3 dir(0, 0, distance);
		Quaternion  xRot; xRot.Set(rise - nm::PI_2, Vector3::xAxis);
		
		
		float cur_asimuth = ctx.target->state().orientation.GetZAxis().GetSphereAsimuthAngle();

		{
			asimuth = cur_asimuth;
		}

		Quaternion  yRot; yRot.Set(asimuth, Vector3::yAxis);
		Quaternion q(yRot * xRot);

		
		dir = Base::MathUtils::postRotate(dir, q);

		State& s = ctx.params.getStateRef();
		s.position = ctx.target->state().position + Vector3(0, height_offset, 0) - dir;
		s.orientation = q;
	}
}

}
}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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