// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.