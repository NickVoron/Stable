#pragma once

#include <vector>
#include "camera.h"

namespace Viewports
{
	class ViewportHandle;

	struct ViewportHandles : public Base::IntrusiveList<ViewportHandle>
	{
		virtual ~ViewportHandles();
	};

	class Viewport :  public Base::IntrusiveList<Viewport>::Node, public ViewportHandles
	{
	public:
		Viewport();
		virtual ~Viewport();

		void init(const std::wstring& name, int width, int height);
		virtual int deviceIndex() const = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual void set() const = 0;

		CalculatedCamera calculateCamera() const;

		std::wstring name;
		
		CameraSystem::ViewportCameras camera;
	};

	class ViewportHandle : public Base::IntrusiveList<ViewportHandle>::Node
	{
	public:
		ViewportHandle();
		explicit ViewportHandle(Viewport* v);
		~ViewportHandle();
		
		void bind(Viewport* v);
		Viewport* viewport();

	private:
		ViewportHandle(const ViewportHandle& vh);
		
		Viewport* vp;
	};

	struct Viewports : public Base::IntrusiveList<Viewport>
	{
		Viewport* find(const std::wstring& name);
	};


	bool equal(const Viewport* f, const Viewport* s);

	extern Viewports viewports;
}
