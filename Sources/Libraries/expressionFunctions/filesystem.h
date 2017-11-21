#pragma once

#include "stuff/library.include.h"

namespace ExpressionFunctions
{
	UserStruct::StringList filenames(const std::string& dir, const std::string& ext);
	UserStruct::StringList resource_filenames(const std::string& dir, const std::string& ext);
	std::string resource_filename(const std::string& filename);
}