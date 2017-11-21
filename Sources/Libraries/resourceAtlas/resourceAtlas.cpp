#include "resourceAtlas.h"

#include "rectAtlasBuilder/library.include.h"
#include "imageD3D/library.include.h"

#include "resourceUtils/library.include.h"


namespace Resources
{
	//
	//
	//
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
