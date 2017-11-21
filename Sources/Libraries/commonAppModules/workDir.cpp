#include "workDir.h"

namespace AppModules
{
	void DllDir::init_impl(const std::vector<std::wstring>& dll_directories)
	{
		for (auto& dir : dll_directories)
		{
			AddDllDirectory(dir.c_str());
		}			
	}
}