// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "resourceAtlas.h"
#ifdef USE_WINDOWS
#include "rectAtlasBuilder/library.include.h"
#include "imageD3D/library.include.h"

#include "resourceUtils/library.include.h"


namespace Resources
{
	
	
	
	void Atlas::Clear()
	{
		deactivate();
		nativeResource.clear();
	}

	void Atlas::Compile(stream::ostream& os)
	{
		nativeResource.save(os);
	}

	void Atlas::LoadSource(const char* sourceFile, const UserData* userData)
	{
		TextureAtlasComposer builder;

		configureAtlasBuilder(builder, sourceFile, dependencies);

		TextureAtlasComposer::AtlasImage img;
		builder.build(nativeResource.atlas, img);
		nativeResource.texture.create(img);
	}


	void Atlas::LoadCompiled(stream::istream& is)
	{
		nativeResource.load(is);
		activate();
	}

	std::vector<std::string> Atlas::help() const
	{
		std::vector<std::string> result;
		resource([&result](auto& res)
		{ 
			for (int i = 0; i < res.atlas.count(); ++i)
			{
				result.push_back(res.atlas.name(i).c_str());
			}
		});

		return result;
	}
}
#endif




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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