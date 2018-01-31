// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "containers/intrusiveList.h"
#include "newmath/library.include.h"

namespace light
{
	template<class SourceType>	struct SourcesCollection;
	template<class SourceType> 	struct SourcesList;

	template<class SourceType>
	struct Source : public Base::IntrusiveList< Source<SourceType> >::Node
	{
		typedef SourcesCollection<SourceType> SrcCollection;
		typedef SourcesList<SourceType> SrcList;

		nm::Vector3 energy;
		
		void enable(bool enabled)
		{
			SrcCollection& c = collection();
			parent = enabled ? &c.enabled : &c.disabled;
			parent->add(this);
		}

		void toggle()
		{
			SrcCollection& c = collection();
			ENFORCE(parent == &c.enabled || parent == &c.disabled);

			parent = (parent == &c.enabled) ? &c.disabled : &c.enabled;
			parent->add(this);
		}

		static SrcCollection& collection()
		{
			static SrcCollection c;
			return c;
		}

		static SourceType* create(bool enabled = true)
		{
			SourceType* src = new SourceType;
			src->enable(enabled);
			return src;
		}

		static void destroy(SourceType*& source)
		{
			ENFORCE(source);
			source->remove();
			delete source;
		}

	protected:
		Source() { energy = SourceType::defaultEnergy(); }

		virtual ~Source(){}

	private:
		SrcList* parent;
	};

	template<class SourceType>
	struct SourcesList : public Base::IntrusiveList< Source<SourceType> > 
	{
		inline void add(Source<SourceType>* source)
		{
			this->push_back( *(SourceType*)source );
		}
	};	 

	template<class SourceType>
	struct SourcesCollection 
	{
		typedef SourcesList<SourceType> SrcList;

		SrcList enabled;
		SrcList disabled;
	};


	struct Directional : public Source<Directional>
	{
		nm::Vector3 direction;

		static nm::Vector3 defaultEnergy() { return nm::Vector3(1.0f, 1.0f, 1.0f); }
	};

	struct Point : public Source<Point>
	{
		nm::Vector3 position;

		static nm::Vector3 defaultEnergy() { return nm::Vector3(1.0f, 1.0f, 1.0f); }
	};

	struct Spot : public Source<Spot>
	{
		static nm::Vector3 defaultEnergy() { return nm::Vector3(1.0f, 1.0f, 1.0f); }
	};

	struct Ambient : public Source<Ambient>
	{
		static nm::Vector3 defaultEnergy() { return nm::Vector3(0.4f, 0.4f, 0.4f); }
	};
}




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