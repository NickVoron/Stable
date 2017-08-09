// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "stream.h"
#include "stuff/enforce.h"

namespace stream
{


template<std::size_t internalBufferSize>
class simemstream : public istream
{
public:
	static const std::size_t BUFFER_SIZE = internalBufferSize;

	simemstream() : readCursor(0), sz(0) {}
	virtual ~simemstream() {}

	virtual std::size_t size()	const { return sz; }
	virtual std::size_t left() const { return sz - readCursor; }

	virtual std::size_t read(void* buf, std::size_t size)
	{
		ENFORCE(readCursor + size <= sz);
		mem::memcpy(buf, &_buf[readCursor], size);
		readCursor += size;
		return size;
	}

	virtual void skip(std::size_t size)
	{
		ENFORCE(readCursor + size <= sz);
		readCursor += size;
	}

	virtual void rollback(std::size_t bytes)
	{
		ENFORCE((int)readCursor - (int) bytes >= 0);
		readCursor -= bytes;
	}

	virtual bool	good() const { return left() > 0; }
	
	void resetReadCursor() { readCursor = 0; }

	char* bufptr() { return &_buf[0]; }
	std::size_t& sizeref() { return sz; }
	std::size_t capacity() const { return BUFFER_SIZE; }
	void reserve(unsigned int len) { ENFORCE(len <= BUFFER_SIZE); }

protected:
	std::size_t sz;
	std::size_t readCursor;
	char _buf[internalBufferSize];
};


template<std::size_t internalBufferSize>
class smemstream : public ostream, public istream
{
template<int ibs> friend  stream::ostream& operator<<(stream::ostream& os, const smemstream<ibs>& ms);
template<int ibs> friend  stream::istream& operator>>(stream::istream& is, smemstream<ibs>& ms);
public:
	static const std::size_t BUFFER_SIZE = internalBufferSize;

	smemstream() : writeCursor(0), readCursor(0)	{};
	virtual ~smemstream(){};

	
	virtual void write(const void* buf, std::size_t size)
	{
		ENFORCE(writeCursor+size <= internalBufferSize);
		mem::memcpy(&_buf[writeCursor], buf, size);
		writeCursor += size;
	}

	virtual std::size_t size()	const	{	return writeCursor;	}
	virtual std::size_t size()	{	return writeCursor;	}


	
	virtual std::size_t read(void* buf, std::size_t size)
	{
		ENFORCE(readCursor+size <= writeCursor);
		mem::memcpy(buf, &_buf[readCursor], size);
		readCursor += size;
		return size;
	}

	virtual void skip(std::size_t bytes)
	{
		ENFORCE(readCursor + bytes <= writeCursor);
		readCursor += bytes;
	}

	virtual void rollback(std::size_t bytes)
	{
		ENFORCE((int)readCursor - (int) bytes >= 0);
		readCursor -= bytes;
	}

	virtual bool	good() const	{	return left() > 0;	}
	virtual std::size_t left() const	{	return writeCursor - readCursor;	}

	void resetReadCursor(){readCursor = 0;}
	void resetWriteCursor(){writeCursor = 0;}
	void reset() { readCursor = writeCursor = 0; }

	mem::mem_desc data() const { return mem::mem_desc(((char*) _buf + readCursor), size()); }
	char* bufptr() { return &_buf[0]; }
	std::size_t capacity() const { return BUFFER_SIZE; }
	void reserve(std::size_t len) { ENFORCE(len <= BUFFER_SIZE); }

protected:
	std::size_t writeCursor, readCursor;
	char _buf[internalBufferSize];
};

template<int ibs>
stream::ostream& operator<<(stream::ostream& os, const smemstream<ibs>& ms)
{
	os << ms.writeCursor << ms.readCursor;
	os.write(ms._buf, ms.writeCursor * sizeof(char));
	return os;
}

template<int ibs>
stream::istream& operator>>(stream::istream& is, smemstream<ibs>& ms)
{
	is >> ms.writeCursor >> ms.readCursor;
	is.read(ms._buf, ms.writeCursor * sizeof(char));
	return is;
}




class dmemstream : public ostream, public istream
{
	friend stream::ostream& operator<<(stream::ostream& os, const dmemstream& ms);
	friend stream::istream& operator>>(stream::istream& is, dmemstream& ms);
public:
	dmemstream() : writeCursor(0), readCursor(0) {}
	dmemstream(const dmemstream& s){ operator=(s); }
	virtual ~dmemstream(){ mem::deallocate(_buf); }

	
	virtual void write(const void* buf, std::size_t size) override
	{
		prepareToWrite(size);
		mem::memcpy(&((char*)_buf.data)[writeCursor], buf, size);
		writeCursor += size;
	}

	virtual std::size_t size()	const override {	return writeCursor - readCursor; }

	dmemstream& operator=(const dmemstream& s)
	{ 
		writeCursor = s.writeCursor;
		readCursor = s.readCursor;
		mem::allocate(_buf, s._buf.len);
		mem::memcpy(_buf.data, s._buf.data, s._buf.len);

		return *this;
	}

	
	virtual std::size_t read(void* buf, std::size_t sz) override
	{
		if (readCursor + sz >= writeCursor)
		{
			sz = size();
		}

		if (sz > 0)
		{
			mem::memcpy(buf, &((char*) _buf.data)[readCursor], sz);
			readCursor += sz;
		}
		
		return sz;
	}

	virtual void skip(std::size_t size) override
	{
		
		ENFORCE(readCursor+size <= writeCursor);
		readCursor += size;
	}

	virtual void rollback(std::size_t bytes) override
	{
		ENFORCE((int) readCursor - (int) bytes >= 0);
		readCursor -= bytes;
	}

	virtual bool	good() const	override {	return left() > 0;	}
	virtual std::size_t left() const	override {	return writeCursor - readCursor;	}

	void resetReadCursor(){readCursor = 0;}
	void resetWriteCursor(){writeCursor = 0;}
	void reset(){readCursor = writeCursor = 0; }

	stream::ostream& save(stream::ostream& os) const
	{
		os << writeCursor << readCursor;
		if(writeCursor > 0)
		{
			os.write(_buf.data, writeCursor * sizeof(char));
		}		

		return os;
	}

	stream::istream& load(stream::istream& is)
	{
		is >> writeCursor >> readCursor;

		if(writeCursor > 0)
		{
			mem::allocate(_buf, writeCursor);
			is.read(_buf.data, writeCursor * sizeof(char));
		}	

		return is;
	}

	mem::mem_desc data() const { return mem::mem_desc( ((char*)_buf.data + readCursor), size()); }
	char* bufptr() { return (char*)_buf.data; }
	std::size_t& sizeref() { return _buf.len; }
	std::size_t capacity() const { return _buf.len; }
	void reserve(std::size_t len) { prepareToWrite(len); }

protected:
	
	void prepareToWrite(std::size_t size)
	{
		if(writeCursor+size >= _buf.len)
			mem::allocate(_buf, (writeCursor+size) * 2 * sizeof(char));
	}

	std::size_t writeCursor = 0;
	std::size_t readCursor = 0;
	mem::mem_desc _buf;
};

inline stream::ostream& operator<<(stream::ostream& os, const dmemstream& ms) { return ms.save(os); }
inline stream::istream& operator>>(stream::istream& is, dmemstream& ms) { return ms.load(is); }


class cycle_idmemstream : public istream
{
public:
	cycle_idmemstream(dmemstream& is) : is(is) { is.resetReadCursor(); }
	virtual ~cycle_idmemstream() { is.resetReadCursor(); }
	
	virtual std::size_t read(void* buf, std::size_t size) override { return is.read(buf, size); }

	virtual void	skip(std::size_t bytes) override { is.skip(bytes); }
	virtual void	rollback(std::size_t bytes) override { is.rollback(bytes); }
	virtual bool	good() const override { return is.good(); }
	virtual std::size_t size() const override { return is.size(); }
	virtual std::size_t left() const override { return is.left(); }

public:
	dmemstream& is;
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