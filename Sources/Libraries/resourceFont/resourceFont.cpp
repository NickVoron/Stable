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