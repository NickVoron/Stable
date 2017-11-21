#pragma once
#include "resourceManager/library.include.h"

namespace Resources
{
	class Text : public Resources::ResourceT<Text, std::string>
	{
	public:
		inline static const char* basePath() { return "text/"; }
		inline static const char* TypeName() { return "Text"; }

		virtual void Clear();

	protected:
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void Compile(stream::ostream& os) override;		
		virtual void LoadCompiled(stream::istream& is) override;
	};
}
