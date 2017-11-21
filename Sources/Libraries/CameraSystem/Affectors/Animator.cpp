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