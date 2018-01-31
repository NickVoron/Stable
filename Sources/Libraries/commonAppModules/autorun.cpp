// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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






	void Autorun::after_init()
	{	
#ifdef WIN32
		static AutorunEditor editor;
#endif














		scheme::defun("edit", &Resources::edit);
		scheme::defun("autoexec", &deault_autoexec);

		deault_autoexec();
	}
}




// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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