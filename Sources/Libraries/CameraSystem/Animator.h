// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "affector.h"

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