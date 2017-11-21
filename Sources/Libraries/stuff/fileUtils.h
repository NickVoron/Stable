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
	// сделать текущей папку с исполняемой программой
	void setModuleDirAsRoot();
	std::string getModuleDir();
	std::string getModuleName();

	// изменить расширение файла (или добавить если нет) на другое
	std::string ChangeFileExt(const std::string& filename, const std::string& ext);

	std::string GetDirectoryFrom(const std::string& fullFileName);
	std::string GetFileNameFrom(const std::string& fullFileName);
	std::string GetFileNameMinusLastExtention(const std::string& fullFileName);
	std::string GetFileNameMinusExtentions(const std::string& fullFileName);
	std::string GetFileNameOnly(const std::string& fullFileName);
	std::string GetLastExtention(const std::string& fullFileName);

	// проверить что файл существует
    bool FileExists(const std::string& filename);
	std::size_t GetFileSize(const std::string& fileName);

	void create_dir(const boost::filesystem::path& dir_ph);
	void create_dir_for_file(const boost::filesystem::path& fileName);
	void copy_file(const boost::filesystem::path& src, const boost::filesystem::path& dst, bool overwrite);
	void copy_file_to_dir(const boost::filesystem::path& src, const boost::filesystem::path& dir, bool overwrite);
	void copy_files_to_dir(const std::list<boost::filesystem::path>& src, const boost::filesystem::path& dir, bool overwrite);
	boost::filesystem::path extract_dir(const boost::filesystem::path& fileName);
	
	// отрезать путь у файла (например "D:\program\levels\filename.ext" и "D:\program" - будет "levels\filename.ext")
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
					LOG_ERROR(e);
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