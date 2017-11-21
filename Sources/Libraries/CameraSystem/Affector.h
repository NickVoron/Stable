#pragma once
// аффектор камеры занимается изменением определённых параметров камеры

#include <string>
#include "Params.h"
#include "Target.h"
#include "InputCollector.h"

#include "containers/intrusiveList.h"
#include "newmath/library.include.h"
#include "Loki/library.include.h"

namespace CameraSystem
{
	class Affector : public Base::IntrusiveList<Affector>::Node
	{
	public:
		// контекст аффектора
		struct Context
		{
 			Context(Params& p, const InputCollector& i) :params(p), input(i), target(0)	{}
			Context(Params& p, const InputCollector& i, const Target& t) :params(p), input(i), target(&t){}
			
			State& state() { return params.getStateRef(); }
			Params&					params;
			const InputCollector&	input;
			const Target*			target;
		};

		virtual ~Affector(){};
		virtual void apply(float dt, Affector::Context& ctx) = 0;
		virtual void init(Affector::Context& ctx) {}
	};

	//
	//
	//
	struct AffectorsSet : public Base::IntrusiveList<Affector>
	{
		void apply(float dt, Affector::Context& ctx);
		void init(Affector::Context& ctx);
	};

	template<class... AffectorsList>
	struct AffectorsSetT : public AffectorsSet
	{
		Loki::TypeTuple<Loki::MakeTypelist<AffectorsList...>> affectors;

		static void add(Affector& affector, AffectorsSet& affectors)
		{
			affectors.push_back(affector);
		}
		
		AffectorsSetT(Affector::Context& ctx)
		{
			Loki::TupleFunc::exec(affectors, &add, *this);
			init(ctx);
		}
	};

	template<>
	struct AffectorsSetT<Loki::NullType> : public AffectorsSet
	{
	};




}; // end namespace CameraSystem