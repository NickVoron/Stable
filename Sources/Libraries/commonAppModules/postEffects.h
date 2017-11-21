#pragma once
//#include "renderTargetsManager.h"
#include "d3d11/direct3d11.h"

namespace AppModules
{
	struct PostEffectsBase
	{
		void init();
		void release();
	};

/*
	template<class Prologue>
	struct PostEffectsBaseT : public PostEffectsBase, public ModuleBase<PostEffectsBaseT<Prologue>, Prologue>
	{
		template<class Params>  void init(Params& p)		{ PostEffectsBase::init();		}
		template<class Params>  void release(Params& p)	{ PostEffectsBase::release();	}
	};

	struct PostEffects9 : public PostEffectsBaseT<Loki::MakeTypelist<RenderTargetsManager9>>
	{
	};

	struct PostEffects11 : public PostEffectsBaseT<Loki::MakeTypelist<Direct3D11>>
	{
	};*/

}