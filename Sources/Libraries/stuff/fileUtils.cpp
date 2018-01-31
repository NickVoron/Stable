// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "fileUtils.h"

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

namespace Base
{
namespace FileUtils
{

	void create_dir( const boost::filesystem::path& dir_ph )
	{
		boost::filesystem::create_directories(dir_ph);
	}

	void create_dir_for_file( const boost::filesystem::path& fileName )
	{
		create_dir( extract_dir(fileName) );
	}

	void copy_file(const boost::filesystem::path& src, const boost::filesystem::path& dst, bool overwrite)
	{
		create_dir_for_file(dst);
		boost::filesystem::copy_file(src, dst, overwrite ? boost::filesystem::copy_option::overwrite_if_exists : boost::filesystem::copy_option::fail_if_exists );
	}

	void copy_file_to_dir(const boost::filesystem::path& src, const boost::filesystem::path& dir, bool overwrite)
	{
		copy_file(src, dir/src.filename(), overwrite); 
	}

	void copy_files_to_dir(const std::list<boost::filesystem::path>& src, const boost::filesystem::path& dir, bool overwrite)
	{
		for(auto& file : src)
		{
			copy_file_to_dir(file, dir, overwrite);
		}	
	}

	boost::filesystem::path extract_dir(const boost::filesystem::path& fileName)
	{
		return boost::filesystem::path(fileName).remove_filename();
	}

	void setModuleDirAsRoot()
	{
		chdir(getModuleDir().c_str());	
	}

	std::string getModuleDir()
	{
#ifdef WIN32
		CHAR buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return GetDirectoryFrom(buffer);
#endif
		return "";
	}

	std::string getModuleName()
	{
#ifdef WIN32
		CHAR buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return GetFileNameOnly(buffer);
#endif
		return "";
	}

	inline bool IsPathSlash(char ch)
	{
		return (ch == '/') || (ch == '\\');
	}

	
	inline char SkipSlashes(const char*& path)
	{
		if (!*path) return 0;
		const bool fcIsSlash = IsPathSlash(*path);
		if (fcIsSlash)
		{
			while(*path && IsPathSlash(*path + 1)) ++path;
			return '\\';
		}
		else
		{
			return toupper(*path);
		}
	}

	
	std::string CutPath(const char* filename, const char* path)
	{
		if (!filename || !path || !*filename || !*path) return "";

		
		int offset = 0;

		const char* restFileName = filename;
		while (*restFileName && *path)
		{
			char fc = SkipSlashes(restFileName);
			char pc = SkipSlashes(path);

			if (toupper(fc) != toupper(pc)) break;

			++restFileName;
			++path;
			++offset;
		}

		
		return (!*path) ? restFileName : filename;
	}        

	std::string CutPath(const std::string& filename, const std::string& path)
	{
		return CutPath(filename.c_str(), path.c_str());
	}

	
	std::string ChangeFileExt(const std::string& filename, const std::string& ext)
	{
		return GetFileNameMinusLastExtention(filename) + "." + ext;
	}

	bool FileExists(const std::string& filename)
	{
		return boost::filesystem::exists(filename);
	}

	std::size_t GetFileSize(const std::string& fileName)
	{
#ifdef WIN32
		struct stat st;
		auto res = ::stat(fileName.c_str(), &st);
		return res == 0 ? st.st_size : 0;
#endif
		return 0;
	}

	std::string	GetDirectoryFrom(const std::string& fileName)
	{
		return extract_dir(fileName).string() + "/";
	}

	std::string GetFileNameFrom(const std::string& fileName)
	{
		return boost::filesystem::path(fileName).filename().string();
	}

	std::string	GetFileNameMinusLastExtention(const std::string& fileName)
	{
		auto npos = fileName.rfind('.');
		return (npos != std::string::npos) ? fileName.substr(0, npos) : fileName;
	}

	std::string GetFileNameMinusExtentions(const std::string& fileName)
	{
		auto npos = fileName.find('.');
		return (npos != std::string::npos) ? fileName.substr(0, npos) : fileName;
	}

	std::string GetFileNameOnly(const std::string& fullFileName)
	{
		std::string res = fullFileName;
		res = GetFileNameFrom(res);
		res = GetFileNameMinusExtentions(res);
		return res;
	}

	std::string	GetLastExtention(const std::string& fileName)
	{
		auto npos = fileName.rfind('.');
		return (npos != std::string::npos) ? fileName.substr(npos + 1, fileName.length() - npos) : "";
	}

	std::vector<boost::filesystem::path> GetFileNamesByMask(const boost::filesystem::path& folder, const std::string& ext, bool recursive)
	{
		std::vector<boost::filesystem::path> result;
		apply_to_files(folder, ext, recursive, [&result](const auto& file) { result.push_back(file); });
		return result;
	}
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