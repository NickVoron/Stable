#pragma once

#include "resourceManager/library.include.h"
#include "openal/library.include.h"

namespace Resources
{
	class Sound : public ResourceT<Sound, openal::source>
	{
	public:
		inline static const char* basePath() { return "sounds/"; }
		inline static const char* TypeName() { return "Sound"; }

		virtual void Clear();

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;

	private:
		openal::buffer buf;
		Wave wave;
	};
}