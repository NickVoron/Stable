#pragma once
#include "../paramsHolder/include.h"

namespace CameraComponents
{
	class AffectorBase : public SimpleComponent<AffectorBase>
	{
	public:
		CM_DEFINE_BASE_CLSID(AffectorBase);

		typedef Loki::MakeTypelist<CameraParamHolder> Dependencies;

		inline AffectorBase(CameraSystem::Affector& a):affector(a){};

		void linker();

		virtual void finalize();
		
	protected:				  
		CameraParamHolder* paramHolder;
		CameraSystem::Affector& affector;
	};

	template<class CamAffector>
	class Affector : public AffectorBase
	{
	public:
		inline Affector() : AffectorBase(camAffector){};

	protected:
		CamAffector camAffector;
	};
}
