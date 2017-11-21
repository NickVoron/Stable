#include "settingsRegistry.h"

#include "stuff/library.include.h"
#include "defaultLogs/library.include.h"

static const char* fileName = "settings.bin";
#ifdef __ANDROID__
static const char*applicationDir = "/storage/emulated/0/Android/data/com.example.nikolayvoronetskiy.myapplication2/";
#else
static const char*applicationDir = "";
#endif

void SettingsRegistry::save()
{
	try
	{
        auto filename = boost::filesystem::path(applicationDir) / fileName;
		stream::ofstream<dataop::zlib> os(filename.string().c_str());
		os << Base::ExplicitMeyersSingleton<SettingsRegistry>::get().data;
	}
	catch (std::exception& e)
	{
		LOG_ERROR(e);
	}
}

void SettingsRegistry::load()
{
    auto filename = boost::filesystem::path(applicationDir) / fileName;

	if(Base::FileUtils::FileExists(filename.string().c_str()))
	{
		try
		{
			stream::ifstream<dataop::zlib> is(filename.string().c_str());
			is >> Base::ExplicitMeyersSingleton<SettingsRegistry>::get().data;
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}
}
