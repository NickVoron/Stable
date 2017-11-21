#pragma once

#include "resourceManager/library.include.h"

#include "componentModel/library.include.h"

namespace Resources
{
	class Module : public ResourceT<Module, EntitiesList>
	{
	public:
		inline static const char* basePath() { return "desc/"; }
		inline static const char* TypeName() { return "Module"; }

		virtual void Clear();

		void execute();

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFileName, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
		virtual void OnLoadCompiledComplete();
	};
}