#pragma once
#include "appModule/library.include.h"
#include "stuff/library.include.h"

namespace AppModules
{
	class WorkDir : public ModuleBase<WorkDir>
	{
	public:
		template<class Params>  void before_init(Params& p) { Base::FileUtils::setModuleDirAsRoot(); }
	};

	class DllDir : public ModuleBase<DllDir>
	{
	public:
		struct Params
		{
			std::vector<std::wstring> dll_path_list{L"./bin"};
		};

		template<class Params>  
		void before_init(Params& p) { init_impl(p.dll_path_list); }

		void init_impl(const std::vector<std::wstring>& dll_directories);
	};	
}