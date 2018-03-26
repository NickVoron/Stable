// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <type_traits>

namespace stream
{


class istream
{
protected:
	virtual ~istream();

public:

	
	
	
	virtual std::size_t read(void* buf, std::size_t size) = 0;

	virtual void	skip(std::size_t bytes)		   = 0;	
	virtual void	rollback(std::size_t bytes)	   = 0;	
	virtual bool	good() const				   = 0; 
	virtual std::size_t size() const			   = 0; 
	virtual std::size_t left() const			   = 0; 

	template<class T>
	istream& operator >>(T& v)
	{


		read(&v, sizeof(T));
		return *this;
	}
};




class ostream
{
protected:
	virtual ~ostream();

public:

	virtual void write(const void* buf, std::size_t size)	= 0;
	virtual std::size_t size() const						= 0;

	template<class T>
	ostream& operator <<(const T& v)
	{
		
		write( &v, sizeof(v) );
		return *this;
	}
}; 

}

class serializer
{
public:
	serializer(stream::ostream& os_) :os(&os_) {}
	serializer(stream::istream& is_) :is(&is_) {}

	template<class Value>
	serializer& operator<<(Value& value)
	{
		if (os)
			*os << value;
		else
			*is >> value;

		return *this;
	}

private:
	serializer() {}

	stream::ostream* os = 0;
	stream::istream* is = 0;
};




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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