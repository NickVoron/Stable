// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "resourceFont.h"
#include "imageD3D/library.include.h"
#include "fontExport/library.include.h"

namespace Resources
{		   
	void Font::Clear()
	{
		deactivate();
		nativeResource.clear();
	}

	void Font::Compile(stream::ostream& os)
	{
		nativeResource.save(os);
	}

	void Font::LoadSource(const char* sourceFile, const UserData* userData)
	{
		int size;
		if (userData)
		{
			userData->store(size);
		}

		unigui::font::FontExport fontExport;
		fontExport.addRange(0x0000, 0x00FF);
		TextureAtlasComposer::AtlasImage img; 
		fontExport.compile(nativeResource.face, img, "testFont", sourceFile, size);

		nativeResource.texture.create(img);
	}

	void Font::LoadCompiled(stream::istream& is)
	{
		nativeResource.load(is);
		nativeResource.parent = this;
		activate();
	}

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