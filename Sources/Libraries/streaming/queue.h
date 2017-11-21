#pragma once

#include "common/waitable_queue.h"

namespace Streaming
{

class LoaderBase
{
public:
	virtual ~LoaderBase(){}

	virtual void load() = 0;
	virtual void remove() = 0;
	virtual std::string desc() const = 0;
	virtual bool isSame(LoaderBase* loader) const = 0;
};

struct FilesQueue :public mt::waitable_queue<std::unique_ptr<LoaderBase>>
{
};

}
