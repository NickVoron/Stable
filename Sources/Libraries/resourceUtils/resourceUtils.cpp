// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "resourceUtils.h"

namespace Resources
{
	void open_filestream(const char* fileName, std::ifstream& is, bool binary)
	{
		boost::filesystem::path devPath = "../../Resources";
		std::string fullName = (devPath / fileName).string();

		if (binary)
		{
			is.open(fullName, std::ios::binary);
		}
		else
		{
			is.open(fullName);
		}

		if (!is.good())
		{
			if (binary)
			{
				is.open(fileName, std::ios::binary);
			}
			else
			{
				is.open(fileName);
			}
		}
	}

	void open_filestream(const char* fileName, stream::ifstream<>& is)
	{
		boost::filesystem::path devPath = "../../Resources";
		std::string fullName = (devPath / fileName).string();

		is.open(fullName.c_str());
	}

	std::string resourceRelativePath(const char* fileName)
	{
#ifdef WIN32
		boost::filesystem::path devPath = "../../Resources";
		return (devPath / fileName).string();
#elif defined(__ANDROID__)
		return fileName;
#elif defined(__APPLE__)
		extern std::string resourceBundlePath(const std::string& fileName, const std::string& ext);
		return resourceBundlePath(Base::FileUtils::GetFileNameFrom(Base::FileUtils::GetFileNameMinusLastExtention(fileName)), Base::FileUtils::GetLastExtention(fileName));
#endif 
	}

	void setupManager()
	{
		str::string256 sdkPath;
		if (const char* res = getenv("SharedTec_SDK_path"))
			sdkPath += res;

		sdkPath += "/Resources";

		Resources::Store::setPath(sdkPath.c_str(), "../../Resources", "compiled");
	}




















	
	
	
	bool isFolder(const boost::filesystem::path& name)
	{
		return false;
	}

	bool isFile(const boost::filesystem::path& name)
	{
		return boost::filesystem::exists(name);
	}

	bool isPsdFile(const boost::filesystem::path& name)
	{
		if( !isFile(name) )	return false;

		return name.extension() == ".psd";
	}

#ifdef WIN32
	void configureAtlasBuilder(TextureAtlasComposer& builder, const std::string& fileName, Resources::Dependencies& dependencies)
	{
		std::string ext = Base::FileUtils::GetLastExtention(fileName);

		if(ext == "psd")
		{
			if(Base::FileUtils::FileExists(fileName))
			{
				builder.addSourcePSD(fileName, true);
			}
			else
			{
				LOG_ERROR("file: " << fileName << " not exists");
			}		 			
		}
		else if(ext == "atlas")
		{
			typedef std::list<boost::filesystem::path> SrcList;

			SrcList files;
			SrcList folders;
			SrcList psdFiles;

			boost::filesystem::path sourceFolder = fileName;
			sourceFolder.remove_filename();

			std::ifstream file( fileName.c_str() );
			ENFORCE(file.good());

			char buffer[4096];
			while(file.good())
			{
				file.getline(buffer, 4096);
				boost::filesystem::path relFolderPath = buffer;
				boost::filesystem::path fullFolderPath = sourceFolder;
				fullFolderPath /= relFolderPath;

				bool is_file = isFile(fullFolderPath);
				bool is_psd_file = isPsdFile(fullFolderPath);
				bool is_folder = isFolder(fullFolderPath);

				if(is_folder)		folders.push_back(fullFolderPath);
				else if(is_psd_file) psdFiles.push_back(fullFolderPath);
				else if(is_file)	files.push_back(fullFolderPath);

				if(is_file || is_psd_file)
				{
					dependencies.addActualDependence(fullFolderPath, relFolderPath);	
				}
			}

			for(const auto& fileName : files)
			{
				builder.addSourceRect(fileName.string());
			}

			for(const auto& folderName : folders)
			{
				builder.addSourceDir(folderName.string(), true, true);
			}

			for(const auto& psdName : psdFiles)
			{
				builder.addSourcePSD(psdName.string(), true);
			}
		}		  
	}
#endif

	std::string editor_path()
	{
		std::string sdkPath;
		if (const char* res = getenv("SharedTec_SDK_path"))
			sdkPath += res;

		sdkPath += "/../Tools/Sublime/subl.exe";

		if (!Base::FileUtils::FileExists(sdkPath.c_str()))
		{
			sdkPath = "notepad";
		}

		return sdkPath;
	}

	bool edit(const char* fileName)
	{
		bool exist = Base::FileUtils::FileExists(fileName);
		if (exist)
		{
			auto editor = editor_path();
			Base::spawn_process(editor.c_str(), ("-a " + resourceRelativePath("")).c_str());
			std::string ext = Base::FileUtils::GetLastExtention(fileName);

			if (ext == "atlas"
				|| ext == "txt"
				|| ext == "fx"
				|| ext == "iml"
				|| ext == "sdl"
				|| ext == "aisd"
				|| ext == "desc"
				|| ext == "lvl"
				|| ext == "bqst"
				|| ext == "scm"
				)
			{
				Base::spawn_process(editor.c_str(), fileName);				
			}
			else
			{
#ifdef WIN32
				ShellExecuteA(0, "open", fileName, NULL, NULL, SW_RESTORE);
#endif
			}

			return false;
		}

		return exist;
	}
}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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