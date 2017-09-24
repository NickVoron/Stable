// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "interfaces.h"
#include "shaderResource.h"

namespace dx11
{
	
	
	
	struct Validator
	{
	protected:
		virtual bool validateFormat(DXGI_FORMAT format) = 0;
		virtual bool validateBindFlag(D3D11_BIND_FLAG bindFlag) = 0;
	};
	
	
	
	
	template<class ResultType, class ViewType>
	struct DevicePlainData : public Validator, public DeviceResource<ID3D11Texture2D, ResultType>
	{
		DevicePlainData():view(0){}

		void create(int sX, int sY, DXGI_FORMAT fmt, DWORD usage, int MultisampleCount, int MultisampleQuality)
		{
			ENFORCE(validateFormat(fmt));

			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(texDesc));
			texDesc.Width = sX;
			texDesc.Height = sY;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = fmt;
			texDesc.SampleDesc.Count = MultisampleCount;
			texDesc.SampleDesc.Quality = MultisampleQuality;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = usage;
			ENFORCE(SUCCEEDED(this->device->device->CreateTexture2D(&texDesc, 0, &this->ptr)));
		}

		void size(int& w, int&h) const
		{
			D3D11_TEXTURE2D_DESC desc;
			this->ptr->GetDesc(&desc);

			w = desc.Width;
			h = desc.Height;
		}

		DXGI_FORMAT format() const
		{
			D3D11_TEXTURE2D_DESC desc;
			this->ptr->GetDesc(&desc);

			return desc.Format;
		}

		void texelSize(float& w, float&h) const
		{
			int sizeX, sizeY;
			size(sizeX, sizeY);

			w = 1.0f / sizeX;
			h = 1.0f / sizeY;
		}

		ViewType* view;
	};

	
	struct DeviceDefaultRenderTargets
	{
		DeviceDefaultRenderTargets();
		void set();
		void clearColor(const D3DXVECTOR4& color);
		void clearDepthStencil(UINT flags, float zValue, UINT8 stencil);

		void size(int& w, int&h) const;
		void texelSize(float& w, float&h) const;

		Device* device;
		D3D11_VIEWPORT viewport;
		ID3D11RenderTargetView* target;
		ID3D11DepthStencilView* depthStencil;
	};

	struct DefaultRenderTargets
	{
	public:
		static void clearColor(const D3DXVECTOR4& color);
		static void clearDepthStencil(UINT flags, float zValue, UINT8 stencil);

		static bool empty();
		static bool good();

		static void execute(int deviceIndex);

		static void set();
				
		static void create();
		static void init(int deviceIndex, ID3D11RenderTargetView* target, ID3D11DepthStencilView* depthStencil, int width, int height);
		static DeviceDefaultRenderTargets data[Devices::MAX_COUNT];
	};


	
	struct DevicePlainTargetBackup : public DeviceResource<ID3D11RenderTargetView, DevicePlainTargetBackup>
	{
		void backup(unsigned int count);
		void restore(bool setViewport);
		void clearColor(const D3DXVECTOR4& color);
		void clearDepthStencil(UINT flags, float zValue, UINT8 stencil);

		void size(int& w, int&h) const;
		DXGI_FORMAT format() const;
		void texelSize(float& w, float&h) const;
		

	
		D3D11_VIEWPORT viewport;
		ID3D11RenderTargetView* targets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		ID3D11DepthStencilView* depthStencilView;
		unsigned int count;
	};

	
	struct PlainTargetBackup : public DevicePlainTargetBackup::ResultBase
	{
		void backup(unsigned int count);
		void restore(bool setViewport);
		void clearColor(const D3DXVECTOR4& color)						{ for (int i = 0; i < devices->count(); ++i)	{ data[i].clearColor(color); } }
		void clearDepthStencil(UINT flags, float zValue, UINT8 stencil) { for (int i = 0; i < devices->count(); ++i)	{ data[i].clearDepthStencil(flags, zValue, stencil); } }

		void size(int& w, int&h) const							{ data[0].size(w, h); }
		void texelSize(float& w, float&h) const					{ data[0].texelSize(w, h); }
		DXGI_FORMAT format() const								{ return data[0].format(); }
	};

	
	
	
	struct DeviceDepthStencil : public DevicePlainData<DeviceDepthStencil, ID3D11DepthStencilView>
	{
		void create(DXGI_FORMAT format, int multisampleCount, int multisampleQuality);
		void create(int sizeX, int sizeY, DXGI_FORMAT format, int multisampleCount, int multisampleQuality);
		void release();
		void set();
		void set(DevicePlainTargetBackup& renderTarget);
		void clearDepthStencil(UINT flags, float zValue, UINT8 stencil);

		void zero();

	protected:					
		virtual bool validateFormat(DXGI_FORMAT format);
		virtual bool validateBindFlag(D3D11_BIND_FLAG bindFlag);
	};

	
	
	
	struct DevicePlainTarget : public DevicePlainData<DevicePlainTarget, ID3D11RenderTargetView>
	{
		void create(DXGI_FORMAT format, int multisampleCount, int multisampleQuality);
		void create(int sizeX, int sizeY, DXGI_FORMAT format, int multisampleCount, int multisampleQuality);
		void release();
		void set(bool setViewport, DeviceDepthStencil* depthStencil);
		void set(bool setViewport, DevicePlainTargetBackup* depthStencil);
		void set(bool setViewport, ID3D11DepthStencilView* depthStencil);
		void clearColor(const D3DXVECTOR4& color);
	
		D3D11_VIEWPORT viewport;		

		virtual bool validateFormat(DXGI_FORMAT format);
		virtual bool validateBindFlag(D3D11_BIND_FLAG bindFlag);	
	};

	struct DepthStencil : public DeviceDepthStencil::ResultBase
	{
		void create(DXGI_FORMAT format)							{ for (int i = 0; i < devices->count(); ++i)	{ data[i].create(format, multisampleCount, multisampleQuality); } }
		void create(int sizeX, int sizeY, DXGI_FORMAT format)	{ for (int i = 0; i < devices->count(); ++i)	{ data[i].create(sizeX, sizeY, format, multisampleCount, multisampleQuality); } }
		void release()											{ for (int i = 0; i < devices->count(); ++i)	{ data[i].release(); } }
		void set()												{ for (int i = 0; i < devices->count(); ++i)	{ data[i].set(); } }
		void set(PlainTargetBackup& renderTarget)				{ for (int i = 0; i < devices->count(); ++i)	{ data[i].set(renderTarget.data[i]); } }
		void clearDepthStencil(UINT flags, float zValue, UINT8 stencil) { for (int i = 0; i < devices->count(); ++i)	{ data[i].clearDepthStencil(flags, zValue, stencil); } }
		
		void size(int& w, int&h) const							{ data[0].size(w, h); }
		void texelSize(float& w, float&h) const					{ data[0].texelSize(w, h); }
		DXGI_FORMAT format() const								{ return data[0].format(); }

		int multisampleCount = 1;
		int multisampleQuality = 0;
	};

	
	struct PlainTarget : public DevicePlainTarget::ResultBase
	{
		void create(DXGI_FORMAT format)										{ for (int i = 0; i < devices->count(); ++i)	{ data[i].create(format, multisampleCount, multisampleQuality); } }
		void create(int sizeX, int sizeY, DXGI_FORMAT format)				{ for (int i = 0; i < devices->count(); ++i)	{ data[i].create(sizeX, sizeY, format, multisampleCount, multisampleQuality); } }
		void release()														{ for (int i = 0; i < devices->count(); ++i)	{ data[i].release(); } }
		void set(bool setViewport, DepthStencil* depthStencil)				{ for (int i = 0; i < devices->count(); ++i)	{ data[i].set(setViewport, depthStencil ? &depthStencil->data[i] : 0); } }
		void set(bool setViewport, PlainTargetBackup* depthStencil)			{ for (int i = 0; i < devices->count(); ++i)	{ data[i].set(setViewport, &depthStencil->data[i]); } }
		void set(bool setViewport)											{ for (int i = 0; i < devices->count(); ++i)	{ data[i].set(setViewport, DefaultRenderTargets::data[i].depthStencil); } }
		void clearColor(const D3DXVECTOR4& color)							{ for (int i = 0; i < devices->count(); ++i)	{ data[i].clearColor(color); } }

		void size(int& w, int&h) const				{	data[0].size(w, h);			}
		void texelSize(float& w, float&h) const		{	data[0].texelSize(w, h);	}
		DXGI_FORMAT format() const					{	return data[0].format();	}

		int multisampleCount = 1;
		int multisampleQuality = 0;
	};

}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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