#pragma once

#include "stream.h"
#include "../dataop/data_op.h"

namespace stream
{

template<int internalBufferSize = 1024*1024 >
class mstream
{
public:

	template<class T>
	mstream& operator<<(const T& v)
	{
		buffer.insert(v);
		return *this;
	}

	template<class T>
	mstream& operator>>(T& v)
	{
		buffer.takeCopy(v);
		return *this;
	}
	unsigned int size()	const {	return buffer.getSize(); }
	void clear() { buffer.clear(); }

private:
	typedef	mem::membuf<internalBufferSize> BufferType;
	BufferType buffer;
};


}
