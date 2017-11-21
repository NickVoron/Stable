#include "autorun.h"

#include "defaultLogs/library.include.h"
#include "resourceUtils/library.include.h"

#ifdef WIN32
#include "imgui/library.include.h"
#include "imguiModules/library.include.h"
#endif

namespace AppModules
{
	std::string autoexecName()
	{
		auto name = Base::FileUtils::ChangeFileExt(Base::FileUtils::getModuleName(), "scm");
		return Resources::resourceRelativePath(("autoruns/" + name).c_str());
	}

	void autoexec(const std::string& fileName)
	{
		try
		{
//			LOG_MSG("execute autorun: " << fileName);
			{std::ofstream(fileName.c_str(), std::ios::app).close(); }
			scheme::exec_file(fileName, scheme::global_env());
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}		
	}

	void deault_autoexec()
	{
		autoexec(autoexecName());
	}

#ifdef WIN32
	struct AutorunEditor : public imgui::modules::Module
	{
		AutorunEditor()
		{
			switcher = Input::KEY_A;
		}

		virtual void process()
		{
			if (imgui::button("Edit Autorun"))
			{
				Resources::edit(autoexecName().c_str());
			}

			if (imgui::button("Execute Autorun"))
			{
				deault_autoexec();
			}
		}

		const char* name() const { return "Autorun"; }
		const wchar_t* viewportName() const { return L"default"; }
	};
#endif

// 	bool openPredicate()
// 	{
// 		return (GetAsyncKeyState(VK_F2) & 0x8000);
// 	}

	void Autorun::after_init()
	{	
#ifdef WIN32
		static AutorunEditor editor;
#endif

// 		std::thread([fullname = autoexecName()]
// 		{
// 			while (true)
// 			{
// 				Sleep(100);
// 
// 				if (openPredicate())
// 				{
// 					Resources::edit(fullname.c_str());
// 				}
// 			}
// 		}).detach();

		scheme::defun("edit", &Resources::edit);
		scheme::defun("autoexec", &deault_autoexec);

		deault_autoexec();
	}
}
