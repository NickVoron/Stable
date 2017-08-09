// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "stuff/enforce.h"
#include "defaultLogs/library.include.h"

#include <string>
#include <list>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace Base 
{
namespace FileUtils 
{
	
	void setModuleDirAsRoot();
	std::string getModuleDir();
	std::string getModuleName();

	
	std::string ChangeFileExt(const std::string& filename, const std::string& ext);

	std::string GetDirectoryFrom(const std::string& fullFileName);
	std::string GetFileNameFrom(const std::string& fullFileName);
	std::string GetFileNameMinusLastExtention(const std::string& fullFileName);
	std::string GetFileNameMinusExtentions(const std::string& fullFileName);
	std::string GetFileNameOnly(const std::string& fullFileName);
	std::string GetLastExtention(const std::string& fullFileName);

	
    bool FileExists(const std::string& filename);
	std::size_t GetFileSize(const std::string& fileName);

	void create_dir(const boost::filesystem::path& dir_ph);
	void create_dir_for_file(const boost::filesystem::path& fileName);
	void copy_file(const boost::filesystem::path& src, const boost::filesystem::path& dst, bool overwrite);
	void copy_file_to_dir(const boost::filesystem::path& src, const boost::filesystem::path& dir, bool overwrite);
	void copy_files_to_dir(const std::list<boost::filesystem::path>& src, const boost::filesystem::path& dir, bool overwrite);
	boost::filesystem::path extract_dir(const boost::filesystem::path& fileName);
	
	
	std::string CutPath(const char* filename, const char* path);
	std::string CutPath(const std::string& filename, const std::string& path);

	template<class Operation>
	void apply_to_files(const boost::filesystem::path& path, const std::string& ext, bool recursive, Operation&& op)
	{
		namespace fs = boost::filesystem;

		if (fs::exists(path))
		{
			const auto& inext = (!ext.empty() && ext[0] != '.') ? ("." + ext) : ext;

			auto operation = [&op](auto inpath)
			{
				try
				{
					op(inpath);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR(e.what());
				}
			};
			
			if (fs::is_directory(path))
			{
				fs::directory_iterator end;
				for (fs::directory_iterator dit(path); dit != end; ++dit)
				{
					try
					{
						if (recursive && fs::is_directory(dit->status()))
						{
							apply_to_files(dit->path(), inext, true, op);
						}
						else if (fs::is_regular_file(dit->status()))
						{
							const auto& ditpath = dit->path();
							auto pext = ditpath.extension().string();
							if (boost::iequals(pext, inext))
							{
								operation(ditpath.lexically_normal());
							}							
						}
					}
					catch (const std::exception& e)
					{
						LOG_ERROR(e.what());
						throw;
					}
				}
			}
			else if (fs::is_regular_file(path))
			{
				operation(path.lexically_normal());
			}
		}
	}
	
	std::vector<boost::filesystem::path> GetFileNamesByMask(const boost::filesystem::path& folder, const std::string& ext, bool recursive);
}
}



// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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