#pragma once

#include "resourceManager/library.include.h"
#include "rectAtlas/library.include.h"


namespace Resources
{
	class Atlas : public Resources::ResourceT<Atlas, RectAtlasGAPI>
	{
	public:
		inline static const char* basePath() { return "atlases/"; }
		inline static const char* TypeName() { return "Atlas"; }
		virtual std::vector<std::string> help() const override;

		virtual void Clear();

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
	};
}