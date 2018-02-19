// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "animator.h"

namespace CameraSystem{
namespace Affectors{

Animator::Animator()
{
	animatorPOS.init(0.5f, tween::QUAD_EASE_OUT);
	animatorHPB.init(0.5f, tween::QUAD_EASE_OUT);
}

void Animator::init(Affector::Context& ctx)
{
	animatorPOS.bind(0, ctx.params.getStateRef().position);
	animatorHPB.bind(0, ctx.params.getStateRef().orientation);
}

void Animator::apply(float dt, Affector::Context& ctx)
{
	tweenerV3.process(dt);
	tweenerQ.process(dt);
}

void Animator::position(const Vector3& pos)
{
 	animatorPOS.target(0, pos);
	animatorPOS.start(tweenerV3);
}

void Animator::positionAdd(const Vector3& pos)
{
	animatorPOS.target(0, animatorPOS.value(0) + pos);
	animatorPOS.start(tweenerV3);
}

void Animator::lookTo(const Vector3& point)
{
	State state;
	state.position = animatorPOS.value(0);
	state.orientation = animatorHPB.value(0);

	state.LookToPoint(point);
	animatorHPB.target(0, state.orientation);
	animatorHPB.start(tweenerQ);
}

void Animator::hpb(const Vector3& val)
{
	animatorHPB.target(0, Base::MathUtils::HPBtoQuaternion(val));
	animatorHPB.start(tweenerQ);
}

void Animator::heading(float heading)
{
	Vector3 HPB = Base::MathUtils::QuaterionToHPB(animatorHPB.value(0));
	HPB.x = heading;
	animatorHPB.target(0, Base::MathUtils::HPBtoQuaternion(HPB));
	animatorHPB.start(tweenerQ);
}

void Animator::pitch(float pitch)
{

}

void Animator::bank(float bank)
{

}

void Animator::headingAdd(float heading)
{
	Vector3 HPB = Base::MathUtils::QuaterionToHPB(animatorHPB.value(0));
	HPB.x += heading;
	animatorHPB.target(0, Base::MathUtils::HPBtoQuaternion(HPB));
	animatorHPB.start(tweenerQ);
}

void Animator::pitchAdd(float pitch)
{

}

void Animator::bankAdd(float bank)
{

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