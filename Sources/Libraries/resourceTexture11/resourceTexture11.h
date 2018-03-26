// Copyright (C) 2013-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#ifdef USE_WINDOWS


#include "dx11/library.include.h"
#include "imageD3D/library.include.h"

#include "resourceManager/library.include.h"

namespace Resources
{ 
	struct TextureAPI11
	{
		void load(const char* fileName);
		void load(const char* fileName, int mipsCount);

		void loadCUBE(const dx11::CubemapDesc& desc);
		void clear();

		void createSampler();

		void load(stream::istream& is);
		void save(stream::ostream& os);

		void size(int& width, int& height);

		void append(dx11::DrawableObjectsList& drawables);

		dx11::Sampler sampler;
		dx11::Texture texture;
		dx11::ShaderResource resource;
	};


	struct TextureParams
	{
		int mipsCount = 1;
	};

	class Texture11 : public ResourceT<Texture11, TextureAPI11>
	{
	public:
		Texture11();
		inline static const char* basePath() { return "textures/"; }
		inline static const char* TypeName() { return "Texture"; }

		virtual void Clear();

		void Size(int& width, int& height);

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
	};
}
#endif




// Copyright (C) 2013-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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