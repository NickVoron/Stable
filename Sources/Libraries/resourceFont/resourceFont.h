#pragma once
#include "resourceManager/library.include.h"
#include "font/library.include.h"
#include "fontDraw/library.include.h"

namespace Resources
{
	class Font : public ResourceT<Font, unigui::font::FontDriver>
	{
	public:
		inline static const char* basePath() { return "font/"; }
		inline static const char* TypeName() { return "Font"; }

		virtual void Clear();

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFileName, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
	};
}