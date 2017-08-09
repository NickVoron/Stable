// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "DXUT/library.include.h"
#include "resourceManager/library.include.h"

namespace Resources
{
	class Font : public Resource
	{
	public:
		typedef CDXUTTextHelper* ConcreteResource;

		Font(const std::string& newName);

		virtual void Clear();
		virtual bool IsValid() const;

		ConcreteResource GetResource() { return text; }

	protected:
		virtual void Compile(stream::ostream& os);
		virtual void LoadSource();
		virtual void LoadCompiled(stream::istream& is);
		virtual void OnLoadCompiledComplete();

	private:
		ConcreteResource text;
		ID3DXFont* font;        
		ID3DXSprite* sprite;
		std::string fontName;
	};
}



// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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