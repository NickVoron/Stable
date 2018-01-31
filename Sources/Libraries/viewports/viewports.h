// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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




// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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