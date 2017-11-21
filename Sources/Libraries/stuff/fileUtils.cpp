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

	// пропустить повторяющиеся слеши
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

	// отрезать путь у файла (например "D:\program\levels\filename.ext" и "D:\program" - будет "levels\filename.ext")
	std::string CutPath(const char* filename, const char* path)
	{
		if (!filename || !path || !*filename || !*path) return "";

		// найдем общую часть
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

		// если вырезали весь путь - вырезаем, иначе ничего не делаем
		return (!*path) ? restFileName : filename;
	}        

	std::string CutPath(const std::string& filename, const std::string& path)
	{
		return CutPath(filename.c_str(), path.c_str());
	}

	// изменить расширение файла (или добавить если нет) на другое
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