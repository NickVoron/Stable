// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "common/delete.h"
#include "stuff/nonCopyable.h"
#include "math/library.include.h"
#include "viewports/library.include.h"

#include <d3dx11.h>
#include <vector>

namespace dx11
{
	struct Adapter : public Base::NonCopyable
	{
		Adapter();
		~Adapter();

		void release();

		IDXGIAdapter1* adapter;
	};

	struct Adapters : public Base::NonCopyable
	{
		static const int MAX_COUNT = 4;

		Adapters();

		Adapter& operator[](int idx) { return adapters[idx]; }

		Adapter adapters[MAX_COUNT];
		int count;
	};

	struct Device : public Base::NonCopyable
	{
		Device();
		~Device();
		void create(Adapter& adapter);
		void release();

		std::wstring adapterDesc;
		ID3D11Device* device;
		ID3D11DeviceContext* context;
	};

	struct Devices : public Base::NonCopyable
	{
		static const int MAX_COUNT = Adapters::MAX_COUNT;

		enum CreationMode
		{
			FOR_EACH_ADAPTER,
			FIRST_ADAPTER,
			MANUAL_CREATE
		};

		Devices(CreationMode mode);

		Device& operator[](int idx) { return devices_[idx]; }
		int count() const { return cnt; }

	private:
		void create(Adapters& adapters);
		Device devices_[MAX_COUNT];
		int cnt;
	};

	std::wstring DeviceDescription(int deviceIndex);

	struct OutputSurfaceBase : public Base::NonCopyable
	{
		OutputSurfaceBase();
		virtual ~OutputSurfaceBase();
		
		virtual void release();
		void set();
		void clearColor(const Vector4& color);
		void clearDepthStencil(UINT flags, float zValue, UINT8 stencil);
		bool good() const;

		int deviceIndex;
		Device* parent;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11DepthStencilView* depthStencilView;
		unsigned int width;
		unsigned int height;
	};

	struct OutputSurface : public OutputSurfaceBase
	{
		OutputSurface();
		virtual ~OutputSurface();

		void init(int deviceIndex, HWND hwnd, bool hasDepthStencil, bool fullscreen);
		void present();
		virtual void release();
	
		IDXGISwapChain* swapChain;
	};

	struct OutputSurfaceClone : public OutputSurfaceBase
	{
		OutputSurfaceClone();
		void init(OutputSurfaceBase& source);
		virtual void release();
		
		ID3D11Texture2D* buffer;
		ID3D11ShaderResourceView* shaderResViewRT;
	};


	class Viewport : public Viewports::Viewport
	{
	public:
		Viewport();
		void init(const std::wstring& name, OutputSurface& outputSurface);

		virtual int width() const;
		virtual int height() const;
		virtual void set() const;
		virtual int deviceIndex() const;

		void setPrimaryOutputSurface();
		void setSecondaryOutputSurface();
		void clearSecondary();

		ID3D11Device* device();
		ID3D11DeviceContext* context();
		ID3D11ShaderResourceView* renderTarget();

	private:
		OutputSurface* surface;
		OutputSurfaceClone secondary;
	};

	extern std::unique_ptr<Devices> devices;
}



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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