// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "font.h"
#include "DXUT/library.include.h"
#include "utilsD3D/utils.h"
#include "driverD3D/device.h"

namespace Resources
{

Font::Font(const std::string& newName )
:Resource(newName)
{
	font = 0;
	sprite = 0;
	text = 0;
}

void Font::Clear()
{
	fontName.clear();
	if(font)font->OnLostDevice();
	if(sprite)sprite->OnLostDevice();
	SAFE_DELETE(text);
	SAFE_RELEASE(font);
	SAFE_RELEASE(sprite);
}

bool Font::IsValid() const
{
	return text != 0;
}

void Font::Compile(stream::ostream& os)
{
	os << fontName;
}

void Font::LoadSource()
{
	std::ifstream is(fullSourcePath.string().c_str());
	ENFORCE(is.good());
	is >> fontName;
}

void Font::LoadCompiled(stream::istream& is)
{
	is >> fontName;
	D3DXCreateFont( MicroDrv::Globals::GetDevice(), 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		Base::StrUtils::Convert( fontName ).c_str(), &font );

	D3DXCreateSprite( MicroDrv::Globals::GetDevice(), &sprite );
	text = new CDXUTTextHelper( font, sprite, NULL, NULL, 15 );
}

void Font::OnLoadCompiledComplete()
{

}

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