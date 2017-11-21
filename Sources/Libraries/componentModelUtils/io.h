#pragma once

#include "componentModel/library.include.h"
#include "resourceModule/library.include.h"

namespace ComponentsIO
{
	void load_script(const std::string& source, EntitiesList& entities, const std::string& category, bool createObjects, bool activate);
	Resources::Module* load_script(const std::string& source);

	void save(const char* fileName, EntitiesList& entities);
	void load(const char* fileName, EntitiesList& entities);
}
