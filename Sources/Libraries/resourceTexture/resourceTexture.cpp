#include "resourceTexture.h"
#include "texutils/library.include.h"
#include "common/delete.h"

namespace Resources
{

	Texture::Texture():sizeX(0), sizeY(0)
	{

	}

	void Texture::Clear()
	{
		INCOMPLETE;
		//deactivate([](auto res) { SAFE_RELEASE(res); });
	}

	void Texture::Compile(stream::ostream& os)
	{
		INCOMPLETE;
		//ENFORCE(res);
		//texutils::dx9::SaveTextureToStream(os, res);		
	}

	void Texture::LoadSource(const char* sourceFile, const UserData* userData)
	{
		INCOMPLETE;
// 		ENFORCE(!active());
// 		res = texutils::dx9::LoadTexture( sourceFileName.c_str(), true, true);
// 		D3DXFilterTexture(res, 0, 0, D3DX_FILTER_BOX);
// 		ENFORCE(res);
	}

	void Texture::LoadCompiled(stream::istream& is)
	{
		INCOMPLETE;
// 		ClientResource res;
// 		texutils::dx9::LoadTextureFromStream(is, 0, &res, &sizeX, &sizeY);
// 		activate(res);
	}

	void Texture::OnUpdateDetailLevel(float newLevel)
	{
		INCOMPLETE;
// 		if(currentMaxLevel != determineMaxLevel(newLevel))
// 		{
// 			ForceStreamCompiled();
// 		}	
	}
}