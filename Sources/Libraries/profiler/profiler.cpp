// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "profiler.h"

#include "memory/library.include.h"
#include "stuff/enforce.h"
#include "defaultLogs/library.include.h"

namespace profiler
{
	void Timings::start()
	{
		startCounter = std::chrono::high_resolution_clock::now();
	}

	void Timings::end()
	{
		endCounter = std::chrono::high_resolution_clock::now();
	}

	float Timings::dt()
	{
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(endCounter - startCounter);
		
		return delta.count() / 1000.0f;
	}




History::History()
{
	reset();
}

void History::reset()
{
 	cursor = 0;
	cnt = 0;
 	mem::memzero(time);
}

void History::addTime(float t)
{
	time[cursor] = t;

	int nc = cursor + 1;
	cursor = (nc < elementsCount) ? nc : 0;
	
	if(cnt < elementsCount)
		++cnt;	
}

float History::averageTime() const
{
	float res = 0.0f;
	for (int i = 0; i < cnt; ++i)
	{
		res += time[i];
	}

	return res / cnt;
}





SampleBase::SampleBase()
:parent(0)
{
}

void SampleBase::start()
{
	timings.start();	
}

void SampleBase::end()
{
	timings.end();
	addTime( timings.dt() );
}




Kernel* Kernel::instance = 0;

Kernel::Kernel()
{
	ENFORCE(!instance);
	instance = this;

	current = &root;
	root.name = "Total";
	scale = 0.01f;
}

Kernel::~Kernel()
{
	instance = 0;
}

SampleBase* Kernel::findSample(const char* sampleName)
{
	for(int i = 0; i < samples.size(); ++i)
	{
		if(samples[i]->name == sampleName)
			return samples[i];
	}

	return 0;
}

SampleBase* Kernel::getSample(const char* sampleName)
{
	SampleBase* sample = findSample(sampleName);
	if(!sample)
	{
		sample = new SampleBase();
		sample->name = sampleName;
		samples.addEntry(sample);

		sample->parent = current;
		current->children.push_back(*sample);
	}

	return sample;
}




ScopedObject::ScopedObject(const char* sampleName)
{
	start(sampleName);	
}

ScopedObject::~ScopedObject()
{
	end();
}




static bool enabled = true;
void start(const char* sampleName)
{
	if(enabled && Kernel::instance)
		Kernel::instance->start(sampleName);
}

void start(const std::string& sampleName)
{
	start(sampleName.c_str());
}

void end()
{
	if(enabled && Kernel::instance)
		Kernel::instance->end();
}

void enable(bool e)
{
	enabled = e;
}


void logSample(const profiler::SampleBase& sample, int level, float valuesScale)
{
	if(level >= 0 && sample.count() > 0)
	{
		float time = sample.averageTime()*1000.0f;
		if(time > valuesScale)
		{
			LOG_ERROR(logs::tabs(level) << sample.name << ": " << time);
		}		
	}

	for (auto s = sample.children.begin(); s != sample.children.end(); ++s)
	{
		logSample(*s, level + 1, valuesScale);
	}	
}

void reportSample(const profiler::SampleBase& sample, int level, float valuesScale)
{
	if(level >= 0 && sample.count() > 0)
	{
		float time = sample.averageTime()*1000.0f;
		if(time > valuesScale)
		{
			LOG_REPORT(logs::tabs(level) << sample.name << ": " << time);
		}		
	}

	for (auto s = sample.children.begin(); s != sample.children.end(); ++s)
	{
		reportSample(*s, level + 1, valuesScale);
	}	
}

void logKernel(const profiler::Kernel& kernel)
{
	logSample(kernel.root, -1, kernel.scale);	
};

void reportKernel(const profiler::Kernel& kernel)
{
	reportSample(kernel.root, -1, kernel.scale);	
};

void log()
{
	if(Kernel::instance)
		logKernel(*Kernel::instance);
}

void report()
{
	if(Kernel::instance)
		reportKernel(*Kernel::instance);
}

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