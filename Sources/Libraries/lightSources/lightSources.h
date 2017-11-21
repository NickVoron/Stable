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
