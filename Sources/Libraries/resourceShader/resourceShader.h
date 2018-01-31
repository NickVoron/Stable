// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#if defined(USE_WINDOWS)

#include "resourceManager/library.include.h"
#include "viewports/library.include.h"

#include "dx11/library.include.h"

namespace Resources
{
	class ShaderInterface
	{
	public: 
		virtual bool set(Viewports::Viewport& viewport)=0;
	};

	template<int shaders>
	class Shader : public ResourceT<Shader<shaders>, dx11::ShaderSet<shaders>>, public ShaderInterface
	{
	public:
		inline static const char* basePath() { return "shaders/"; }
		inline static const char* TypeName() { return "Shader"; }

		virtual void Clear();

		virtual bool set(Viewports::Viewport& viewport);

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFileName, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
	};

	
	
	
	template<int shaders>
	void Shader<shaders>::Clear()
	{
		this->deactivate();
		this->nativeResource.clear();
	}

	template<int shaders>
	void Shader<shaders>::Compile(stream::ostream& os)
	{
		this->nativeResource.save(os);
	}

	template<int shaders>
	void Shader<shaders>::LoadSource(const char* sourceFile, const UserData* userData)
	{
		if(this->nativeResource.compile(Base::StrUtils::Convert( sourceFile ).c_str(), userData))
		{
			this->activate();
		}
		else
		{
			this->deactivate();
		} 		
	}

	template<int shaders>
	void Shader<shaders>::LoadCompiled(stream::istream& is)
	{
		this->nativeResource.load(is);
		this->activate();
	}

	template<int shaders>
	bool Shader<shaders>::set(Viewports::Viewport& viewport)
	{
		return this->resource([&viewport](auto& shader) { dx11::execute(viewport, shader); });
	}

	typedef Shader<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER> VertexPixelShader;


}

#endif



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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