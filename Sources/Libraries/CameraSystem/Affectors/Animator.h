#pragma once

#include "../affector.h"

#include "tweener/library.include.h"

namespace CameraSystem{
namespace Affectors{

	class Animator : public Affector
	{
	public:
		Animator();
		virtual void apply(float dt, Affector::Context& ctx);
		virtual void init(Affector::Context& ctx);

		void position(const Vector3& pos);
		void hpb(const Vector3& hpb);

		void heading(float heading);
		void pitch(float pitch);
		void bank(float bank);

		void positionAdd(const Vector3& pos);

		void headingAdd(float heading);
		void pitchAdd(float pitch);
		void bankAdd(float bank);

		void lookTo(const Vector3& point);

	private:
		typedef tween::Tweener<Vector3, 1> TweenerV3;
		typedef tween::Tweener<Quaternion, 1> TweenerQ;

		TweenerV3 tweenerV3;
		TweenerQ tweenerQ;
		TweenerV3::Param animatorPOS; 
		TweenerQ::Param animatorHPB;
	};

}
}