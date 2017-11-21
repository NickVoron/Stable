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