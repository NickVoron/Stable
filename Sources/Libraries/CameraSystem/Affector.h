// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


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



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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