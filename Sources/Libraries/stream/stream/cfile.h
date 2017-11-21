#pragma once

#include <stdio.h>
#include <string>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace stream
{

#ifdef __ANDROID__
	extern AAssetManager* manager;
#endif

// обертка для С функций работы с файлом
class cfile
{
public:
	cfile();
	cfile(const char* fileName, bool readOnly, bool useAssetsManager = false);
	cfile(const std::string& fileName, bool readOnly, bool useAssetsManager = false);
	virtual ~cfile(); 

	std::size_t fread(void* buf, std::size_t size);
	std::size_t fwrite(const void* buf, std::size_t size);
	void fskip(std::size_t bytes);
	void fopen(const char* fileName, bool readOnly);
	void fclose();
	void fflush();
	std::size_t fgetpos() const;
	std::size_t fsize() const;
	std::size_t fleft() const;
	bool fgood() const;

	const std::string& getName() const{return fileName;}

private:
#ifdef __ANDROID__
	AAssetManager *mgr = nullptr;
#endif
	void* file = nullptr;
	std::string fileName;
};


}
