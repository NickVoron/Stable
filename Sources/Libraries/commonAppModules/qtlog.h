#pragma once

#include "appModule/library.include.h"
#include "logs.h"

#if __has_include("QtWidgets/QApplication")
#undef realloc
#include "QtWidgets/QApplication"
#define QT_IS_PRESENT
#endif

namespace AppModules
{
	class QtLog : public ModuleBase<QtLog, Loki::MakeTypelist<Logs>>
	{
	public:
		template<class Params> void init(Params& p)		{ init_impl(p.commandLine); }
		template<class Params> void release(Params& p)  { release_impl(); }

		void init_impl(const std::vector<std::string>& commandLine);
		void release_impl();
#ifdef QT_IS_PRESENT
		std::unique_ptr<QApplication> qtapplication;
#endif
	};
}