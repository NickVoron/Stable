// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "containers/library.include.h"

#include "Params.h"
#include "InputCollector.h"

#include "Affector.h"
#include "Forward.h"
#include "FreeRotator.h"
#include "HorizVert.h"
#include "Around.h"
#include "Animator.h"
#include "Null.h"

namespace CameraSystem
{
	struct UserCameraBase;
	struct BaseCameraController
	{
		virtual void update(UserCameraBase& camera, float dt) = 0;
	};

	struct UserCameraBase : public Base::IntrusiveList<UserCameraBase>::Node
	{
		UserCameraBase(AffectorsSet& affectorsSet);
		virtual ~UserCameraBase(){}

		void calculate(float dt);
		void lookTo(const Vector3& point);

		Params params;
		InputCollector input;
		Affector::Context context;
		AffectorsSet& affectorsSet;
		BaseCameraController* controller;
	};

	template<class... AffectorsList>
	struct UserCamera : public UserCameraBase
	{
	public:
		UserCamera() : UserCameraBase(affectors), affectors(context){}

		AffectorsSetT<AffectorsList...> affectors;
	};

	struct TargetedFreeCamera   : public UserCamera<Affectors::Around>{};
	struct EmptyCamera			: public UserCamera<Affectors::Null>{};

	struct AroundCamera : public UserCamera<Affectors::Forward, Affectors::Around>
	{
		AroundCamera();

		void focusTo(const Vector3& from, const Vector3& point);
		void focusTo(const Vector3& point);

		CameraSystem::PointTarget focus;
	};

	struct FreeCamera : public UserCamera<Affectors::Forward, Affectors::HorizVert, Affectors::FreeRotator, Affectors::Animator>
	{
		void position(const Vector3& pos);
		void hpb(const Vector3& hpb);

		void heading(float heading);
		void pitch(float pitch);
		void bank(float bank);

		void headingAdd(float heading);
		void pitchAdd(float pitch);
		void bankAdd(float bank);

		void positionAdd(const Vector3& pos);

		void lookTo(const Vector3& point);
	};

	template<class CameraType, class ControllerType>
	struct ControlledCamera : public CameraType
	{
		ControlledCamera() { this->controller = &controllerTYPE; }

		ControllerType controllerTYPE;
	};

	class CameraSwitcher : public Base::IntrusiveList<CameraSwitcher>::Node
	{
	public:
		CameraSwitcher(UserCameraBase& defaultCamera);

		UserCameraBase& camera();
		const UserCameraBase& camera() const;

		void activate(UserCameraBase* camera, bool interpolate);
		void defaults();
		virtual void update(float dt);
		bool animation();

		const Params& defaultParams() const;
		Params& defaultParams();

		protected:
		typedef tween::Tweener<State, 1> Tweener;

		void animate(const Params& from, const Params& to);

		UserCameraBase* currentCamera;
		UserCameraBase& defaultCamera;
		EmptyCamera interpolatedCamera;
		
		Tweener tweener;
		Tweener::Param animator;
	};

	class UserCameras : public Base::IntrusiveList<UserCameraBase>
	{
	public:
		virtual ~UserCameras();

		void update(float dt);
		UserCameraBase& insert(UserCameraBase& camera);

		template<class CameraType>
		CameraType* create()
		{
			return create<CameraType>(State::Identity());
		}

		template<class CameraType>
		CameraType* create(const State& locator)
		{
			CameraType* camera = new CameraType();
			camera->params.getStateRef() = locator;
			insert(*camera);

			created.push_back(camera);

			return camera;
		}

	private:
		std::list<UserCameraBase*> created;
	};
	
	UserCameras& cameras();
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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