// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <d3d11.h>

#include "device.h"
#include "viewports/library.include.h"

namespace dx11
{
	struct DrawableObjectsList;

	struct DrawableObject : public Base::IntrusiveList<DrawableObject>::Node
	{
		virtual void execute(int deviceIndex) = 0;
	};

	struct DrawableObjectsList : public Base::IntrusiveList<DrawableObject>
	{
		~DrawableObjectsList() { remove_all(); }
		
		
		
		void execute(int deviceIndex)
		{
			for (auto& obj : *this)
			{
				obj.execute(deviceIndex);
			}
		}

		template<class ObjectType> 
		ObjectType& add(ObjectType& obj)
		{
			push_back(obj);
			return obj;
		}
	};

	void execute(const Viewports::Viewport& viewport, DrawableObjectsList& objects);
	void execute(const Viewports::Viewport& viewport, DrawableObject& object);

	template<class D3D11Interface>
	inline void CheckedRelease(D3D11Interface*& ptr)
	{
		SAFE_RELEASE(ptr);
	}

	template<>
	inline void CheckedRelease<ID3D11InputLayout>(ID3D11InputLayout*& ptr)
	{
		
	}

	template<class D3D11Interface, class Result>
	struct DeviceResource
	{
		Device* device;
		D3D11Interface* ptr;

		DeviceResource() : device(0), ptr(0) {}

		virtual ~DeviceResource() { release(); }

		void release() { CheckedRelease(ptr); }
		bool good() const { return ptr != 0; }

		struct DeviceData 
		{
			DeviceData() { for (int i = 0; i < Devices::MAX_COUNT; ++i) { data[i].device = &(*devices)[i]; } }

			bool empty() const { return data[0].ptr == 0; }
			bool good() const { return !empty(); }

			Result data[Devices::MAX_COUNT];
		};

		struct ResultBase : public DeviceData, public DrawableObject
		{			
			virtual void execute(int deviceIndex)	{	}
		};

		struct Set : public ResultBase
		{				 			

			template<class InputData>	void create(const InputData& indata)				{ for (int i = 0; i < devices->count(); ++i)	{ data[i].create(indata);		} }
			template<class TmplArg>		void create()										{ for (int i = 0; i < devices->count(); ++i)	{ data[i].create<TmplArg>(i);	} }
			void release()																	{ for (int i = 0; i < devices->count(); ++i)	{ data[i].release(); } }

			void set(int deviceIndex) const													{ data[deviceIndex].set(); }
			
			template<class Param>
			void set(int deviceIndex, Param param) const									{ data[deviceIndex].set(param); }

			template<class DataSet>
			void upload(const DataSet& indata)												{ for (int i = 0; i < devices->count(); ++i)	{ data[i].upload(indata); } }
		
		};

		struct StateSet : public Set
		{
			virtual void execute(int deviceIndex)										{ set(deviceIndex); }
		};

		struct ShaderSet : public Set
		{
			ShaderSet() : flags(0)
			{
				mem::memzero(shaderSlots);
			}

			void vertex(int slot) const		{ shaderSlots[0] = slot; flags |= gapi::VERTEX_SHADER; }
			void pixel(int slot) const		{ shaderSlots[1] = slot; flags |= gapi::PIXEL_SHADER; }
			void geometry(int slot) const	{ shaderSlots[2] = slot; flags |= gapi::GEOMETRY_SHADER; }
			void hull(int slot) const		{ shaderSlots[3] = slot; flags |= gapi::HULL_SHADER; }
			void domain(int slot) const		{ shaderSlots[4] = slot; flags |= gapi::DOMAIN_SHADER; }
			void compute(int slot) const	{ shaderSlots[5] = slot; flags |= gapi::COMPUTE_SHADER; }

			mutable char shaderSlots[7];
			mutable int flags;

			virtual void execute(int deviceIndex)										
			{ 
				if (flags & gapi::VERTEX_SHADER) data[deviceIndex].vertex(shaderSlots[0]);
				if (flags & gapi::PIXEL_SHADER) data[deviceIndex].pixel(shaderSlots[1]);
				if (flags & gapi::GEOMETRY_SHADER) data[deviceIndex].geometry(shaderSlots[2]);
				if (flags & gapi::HULL_SHADER) data[deviceIndex].hull(shaderSlots[3]);
				if (flags & gapi::DOMAIN_SHADER) data[deviceIndex].domain(shaderSlots[4]);
				if (flags & gapi::COMPUTE_SHADER) data[deviceIndex].compute(shaderSlots[5]);
			}
		};

		struct BufferSet : public Set
		{
			template<class DataStream>	void create(const DataStream& vs)	{ create(&vs[0], vs.size()); }
			template<class DataStream>	void upload(const DataStream& vs)	{ upload(&vs[0], vs.size()); }

			template<class DataType>	void upload(const DataType* indata, int count)	{ for (int i = 0; i < devices->count(); ++i)	{ data[i].upload(indata, count); } }

			virtual void execute(int deviceIndex)										{ set(deviceIndex); }
		};
	};
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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