#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "unitTest/library.include.h"

#ifdef __ANDROID__
#define TEST_LOG_OUT(message) do { __android_log_write(ANDROID_LOG_ERROR, "SharedTec", message); } while(false);
#else
#define TEST_LOG_OUT(message) do { std::cout << message << std::endl; } while(false);
#endif



inline boost::unit_test::test_suite* init_unit_test(int argc, char* argv[])
{
	boost::unit_test::framework::master_test_suite().argc = argc;
	boost::unit_test::framework::master_test_suite().argv = argv;
	return 0;
}

template<bool enabled> struct UnitTestEngine;

template<> 
struct UnitTestEngine<true>
{
	static void run(int argc, char* argv [])
	{
		try
		{
			boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
		}
		catch (const std::exception& e)
		{
			TEST_LOG_OUT(e.what());
		}
	}
};

template<>
struct UnitTestEngine<false>
{
	static void run(int argc, char* argv []){}
};

template<typename char_t> struct string_type;
template<> struct string_type<char>		{ typedef std::string type; };
template<> struct string_type<wchar_t>	{ typedef std::wstring type; };

template<class InternalParams, class SetupParamsType, class char_t, class SetupReturnType>
SetupParamsType setup_params(SetupReturnType(*setup)(SetupParamsType&), InternalParams& params, int argc, char_t* argv [])
{
	SetupParamsType result(params);
	result.commandLine = std::vector< typename string_type<char_t>::type >(argv, argv + argc);
	setup(result);

	return result;
}

template<class ModulesList, std::size_t iterationsCount, bool unitTestsRun, class SetupParamsType, class char_t, class SetupReturnType>
int appmain(SetupReturnType (*setup)(SetupParamsType&), int argc, char_t* argv[])
{
	bool restart = true;
	while (restart)
	{
		restart = false;
		try
		{
			typedef typename AppModules::UniqueModules<ModulesList>::ModulesList UniqueModules;
			typedef typename AppModules::Modules<UniqueModules> Modules;
			typedef typename Modules::Params Params;

			{
				Params internalParams;
				setup_params(setup, internalParams, argc, argv);
				AppModules::check_modules<iterationsCount, UniqueModules>(internalParams);
			}			

			Params internalParams;
			Modules modules(internalParams);
			setup_params(setup, internalParams, argc, argv);

			modules.init();
			UnitTestEngine<unitTestsRun>::run(argc, argv);
			modules.process();
			modules.release();
		}
		catch (const std::exception& e)
		{
			TEST_LOG_OUT(e.what());
		}
		catch (...)
		{
			TEST_LOG_OUT("unknown exception");
		}
	}

	return 0;
}