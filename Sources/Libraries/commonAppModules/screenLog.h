#pragma once
#include "appModule/library.include.h"

#include "d3d9/direct3d9.h"
#include "logsUpdater.h"
#include "imgui.h"
#include "settings.h"

namespace AppModules
{
	class ScreenLogBase
	{
	public:
		ScreenLogBase();
		void process();
		bool showScreenLog;
	};

	template<class Prologue, class Epilogue>
	class ScreenLogBaseT : public ScreenLogBase, public ModuleBase<ScreenLogBaseT<Prologue, Epilogue>, Prologue, Epilogue>
	{
	public:
		template<class Params>  void process(Params& p) { ScreenLogBase::process(); }
	};

	class ScreenLog9 : public ScreenLogBaseT<Loki::MakeTypelist<Settings, Imgui9>, Loki::MakeTypelist<LogsUpdater, Direct3D9FrameEnd>>
	{
	};

	class ScreenLog11 : public ScreenLogBaseT<Loki::MakeTypelist<Settings, Imgui11>, Loki::MakeTypelist<LogsUpdater, Direct3D11FrameEnd>>
	{
	};

}