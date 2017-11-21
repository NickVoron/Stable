#pragma once

#include "stuff/enforce.h"

template<class T, unsigned int maxElements>
class circular_buffer
{
public:
	static const unsigned int BUFFER_SIZE = maxElements;

	T& operator[](unsigned int index) { return buffer[cidx(index)]; }
	const T& operator[](unsigned int index) const { return buffer[cidx(index)]; }

	T& current() { return operator[](cursor); }
	const T& current() const { return operator[](cursor); }

	T& step()
	{
		if (count < BUFFER_SIZE)
			++count;

		return next();
	}

	int size() const { return count; }

	int begin_index() const { return cursor + 1; }
	int end_index() const { return begin_index() + size(); }

private:

	unsigned int cidx(unsigned int index) const
	{
		ENFORCE(count > 0 && count <= BUFFER_SIZE);
		return index % count;
	}

	T& next()
	{
		++cursor;
		if (cursor == count)
			cursor = 0;

		return current();
	}

	int count = 0;
	int cursor = -1;
	T buffer[BUFFER_SIZE];
};