#pragma once
#include <d3d9.h>

#include "resourceManager/library.include.h"

namespace Resources
{
	class Texture : public ResourceT<Texture, LPDIRECT3DTEXTURE9>
	{
	public:
		Texture();
		inline static const char* basePath() { return "textures/"; }
		inline static const char* TypeName() { return "Texture"; }

		virtual void Clear();

		int sizeX, sizeY;

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
		virtual void OnUpdateDetailLevel(float newLevel);
	};
}