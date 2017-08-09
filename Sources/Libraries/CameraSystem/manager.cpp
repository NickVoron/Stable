// Copyright (C) 2013-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "manager.h"

#include "common/clearPtrContainer.h"

namespace CameraSystem
{
	UserCameraBase::UserCameraBase(AffectorsSet& affSet) : context(params, input), affectorsSet(affSet)
	{
		controller = 0;
	}

	void UserCameraBase::calculate(float dt)
	{
		if(controller)
		{
			controller->update(*this, dt);
		}

		affectorsSet.apply(dt, context);
	}

	void UserCameraBase::lookTo(const Vector3& point)
	{
		params.lookTo(point);
	}

	UserCameras& cameras()			
	{ 
		static UserCameras cams;
		return cams;
	}

	
	
	
	AroundCamera::AroundCamera()
	{
		context.target = &focus;
	}

	void AroundCamera::focusTo(const Vector3& from, const Vector3& point)
	{
		focus.point = point;

		Affectors::Around& around = affectors.affectors.get<Affectors::Around>();
		around.distance = nm::magnitude(from - point);
		params.getStateRef().position = from;
		params.lookTo(point);
	}

	void AroundCamera::focusTo(const Vector3& point)
	{
		focusTo(params.getState().position, point);
	}

	
	
	
	CameraSwitcher::CameraSwitcher(UserCameraBase& defCam) : defaultCamera(defCam)
	{
		animator.init(0.3f, tween::QUAD_EASE_OUT);
		animator.bind(0, interpolatedCamera.params.getStateRef());

		currentCamera = &defaultCamera;
	}

	bool CameraSwitcher::animation()
	{
		bool completed = animator.completed(0);
		if (completed && animator.isInList())
		{
			animator.finish();
		}

		return !completed;
	}

	UserCameraBase& CameraSwitcher::camera()				
	{
		ENFORCE(currentCamera);
		if(!animation())
		{
			return currentCamera ? *currentCamera : defaultCamera;
		}
		else
		{
			return interpolatedCamera;
		}		
	}

	const UserCameraBase& CameraSwitcher::camera() const 	
	{ 
		return ((CameraSwitcher*)this)->camera();
	}

	const Params& CameraSwitcher::defaultParams() const		{ return defaultCamera.params;	}
	Params& CameraSwitcher::defaultParams()					{ return defaultCamera.params;	}

	void CameraSwitcher::activate(UserCameraBase* userCam, bool interpolate)
	{
		ENFORCE(userCam);
		ENFORCE(currentCamera);

		if (interpolate)
		{
			animate(camera().params, userCam->params);

			currentCamera->remove();
			currentCamera = userCam;
			cameras().insert(*currentCamera);
		}
		else
		{
			
			currentCamera->params.getStateRef() = userCam->params.getState();
		}
	}

	void CameraSwitcher::update(float dt)
	{
		ENFORCE(currentCamera);
		bool completed = !animation();
		tweener.process(dt);

		if (!completed && !animation())
		{
			camera().params = interpolatedCamera.params;
		}

		if (!animation())
		{
			currentCamera->calculate(dt);
		}
	}

	void CameraSwitcher::defaults()
	{
		if (currentCamera)
		{
			animate(currentCamera->params, defaultCamera.params);
			currentCamera->remove();
		}	

		currentCamera = &defaultCamera;

		cameras().insert(defaultCamera);
	}

	void CameraSwitcher::animate(const Params& from, const Params& to)
	{
		interpolatedCamera.params = from;

		animator.target(0, to.getState());
		animator.start(tweener);
	}

	
	
	
	void FreeCamera::position(const Vector3& pos)	{ affectors.affectors.get<Affectors::Animator>().position(pos);	}
	void FreeCamera::hpb(const Vector3& val)		{ affectors.affectors.get<Affectors::Animator>().hpb(val); }

	void FreeCamera::heading(float h)				{ affectors.affectors.get<Affectors::Animator>().heading(h); }
	void FreeCamera::pitch(float p)					{ affectors.affectors.get<Affectors::Animator>().pitch(p); }
	void FreeCamera::bank(float b)					{ affectors.affectors.get<Affectors::Animator>().bank(b); }

	void FreeCamera::headingAdd(float h)			{ affectors.affectors.get<Affectors::Animator>().headingAdd(h); }
	void FreeCamera::pitchAdd(float p)				{ affectors.affectors.get<Affectors::Animator>().pitchAdd(p); }
	void FreeCamera::bankAdd(float b)				{ affectors.affectors.get<Affectors::Animator>().bankAdd(b); }

	void FreeCamera::lookTo(const Vector3& point)	{ affectors.affectors.get<Affectors::Animator>().lookTo(point); }

	void FreeCamera::positionAdd(const Vector3& pos){ affectors.affectors.get<Affectors::Animator>().positionAdd(pos); }

	
	
	
	UserCameras::~UserCameras()
	{
		Base::clearPtrContainer(created);
	}

	UserCameraBase& UserCameras::insert(UserCameraBase& camera)
	{
		push_back(camera);
		return camera;
	}

	void UserCameras::update(float dt)
	{
 		for(auto& camera : *this)
 		{
 			camera.calculate(dt);
 		}
	}
}



// Copyright (C) 2013-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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