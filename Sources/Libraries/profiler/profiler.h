#pragma once

#include <string>
#include <chrono>
#include "containers/fixedArray.h"
#include "containers/intrusiveList.h"

namespace profiler
{
	class Timings
	{
	public:
		void start();
		void end();

		float dt();

	private:
		decltype(std::chrono::high_resolution_clock::now()) startCounter;
		decltype(std::chrono::high_resolution_clock::now()) endCounter;
	};

	class History
	{
	public:
		static const int elementsCount = 256;

		History();

		void addTime(float t);
		float averageTime() const;

		void reset();
		int count() const { return cnt; }

	private:
		float time[elementsCount];
		int cursor;
		int cnt;
	};

	class SampleBase : public History, public Base::IntrusiveList<SampleBase>::Node
	{
	public:
		SampleBase();
		void start();
		void end();

		std::string name;
		Timings timings;
		SampleBase* parent;
		Base::IntrusiveList<SampleBase> children;
	};


	class Kernel
	{
	public:
		static const int totalEntries = 128;
		static Kernel* instance;

		Kernel();
		virtual ~Kernel();

		virtual void init(){}
		virtual void release(){}
		virtual void start(const char* sampleName) = 0;
		virtual void end() = 0;

		SampleBase root;
		float scale;

	protected:
		SampleBase* findSample(const char* sampleName);
		SampleBase* getSample(const char* sampleName);

		SampleBase* current;
		Base::FixedArray<SampleBase*, totalEntries> samples;
	};

	struct SimpleKernelImpl 
	{
		inline void init(){}
		inline void release(){}
		inline void start(const char* sampleName){}
		inline void end(){}
	};

	template<class AdditionalKernel = SimpleKernelImpl>
	class KernelImplementation : public Kernel
	{
	public:
		virtual void init(){ additionalKernel.init(); }
		virtual void release(){ additionalKernel.release(); };

		void start(const char* sampleName)
		{
			SampleBase* sample = getSample(sampleName);
			current = sample;
			
			additionalKernel.start(sampleName);
			//SampleBase* parent = current->parent;

			current->start();			
		}

		void end()
		{
			additionalKernel.end();
			current->end();

			SampleBase* parent = current->parent;
			if(parent)
			{	
				current = parent;
			}
		}

		AdditionalKernel additionalKernel;
	};

	//
	//
	//
	struct ScopedObject
	{
		ScopedObject(const char* sampleName);
		~ScopedObject();
	};

	void start(const char* sampleName);
	void start(const std::string& sampleName);
	void end();
	void log();
	void report();
	void enable(bool enabled);
}

#define PROFILE_START(NAME) {profiler::start(NAME);}
#define PROFILE_END {profiler::end();}
#define PROFILE_SCOPE(NAME) profiler::ScopedObject profile_scope_helper_object(NAME);
#define PROFILE_LOG_RESULT {profiler::log();}
#define PROFILE_REPORT_RESULT {profiler::report();}
#define PROFILE_FUNCTION PROFILE_SCOPE(__FUNCTION__);

