#include "imgui.h"
//#include "renderTargetsManager.h"
#include "debugDrawingFlusher.h"
//#include "renderPipeline.h"

namespace AppModules
{
	class UniguiFlusherBase
	{
	public:
		void process();
	};

	template<class Prologue, class Epilogue>
	class UniguiFlusherBaseT : public UniguiFlusherBase, public ModuleBase<UniguiFlusherBaseT<Prologue, Epilogue>, Prologue, Epilogue>
	{
	public:
		template<class Params>  void process(Params& p)	{ UniguiFlusherBase::process(); }
	};

	//class UniguiFlusher9 : public UniguiFlusherBaseT<Loki::MakeTypelist<RenderTargetsManager9, Imgui9, DebugDrawingFlusher>, Loki::MakeTypelist<Direct3D9FrameEnd>>
	class UniguiFlusher9 : public UniguiFlusherBaseT<Loki::MakeTypelist<Imgui9, DebugDrawingFlusher>, Loki::MakeTypelist<Direct3D9FrameEnd>>
	{		
	};

	//class UniguiFlusher11 : public UniguiFlusherBaseT<Loki::MakeTypelist<RenderTargetsManager11, Imgui11, DebugDrawingFlusher, RenderPipeline>, Loki::MakeTypelist<Direct3D11FrameEnd>>
	class UniguiFlusher11 : public UniguiFlusherBaseT<Loki::MakeTypelist<Imgui11, DebugDrawingFlusher>, Loki::MakeTypelist<Direct3D11FrameEnd>>
	{
		
	};
}
