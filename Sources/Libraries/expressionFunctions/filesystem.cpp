#include "filesystem.h"

#include "resourceUtils/library.include.h"

namespace ExpressionFunctions
{
	UserStruct::StringList filenames(const std::string& dir, const std::string& ext)
	{
		UserStruct::StringList files;
		Base::FileUtils::apply_to_files(dir, ext, true, [&files, bpath = boost::filesystem::path(dir).lexically_normal()](auto name)
		{
			files.push_back(name.lexically_relative(bpath).string());
		});

		return files;
	}

	UserStruct::StringList resource_filenames(const std::string& dir, const std::string& ext)
	{
		UserStruct::StringList result = filenames(Resources::resourceRelativePath(dir.c_str()), ext);

		for (auto& file : result)
		{
			file = Resources::resourceRelativePath((dir + "/" + file).c_str());
		}
		return result;
	}

	std::string resource_filename(const std::string& filename)
	{
		return Resources::resourceRelativePath(filename.c_str());
	}
}