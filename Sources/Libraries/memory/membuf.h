#pragma once

#include <stdexcept>

#include "memory.h"

namespace mem
{
	template<std::size_t length>
	class membuf
	{
	public:

		typedef membuf type;

		enum { LENGTH = length };

		membuf();
		membuf(void* data, std::size_t len);
		~membuf();

		//помещение данных в буффер, 
		//после помещения виртуальный размер буффера увеличивается,
		//при переполнении буффера выбрасывается исключение
		void insert(const void* d, std::size_t len);
		template<class V> void insert(const V& value);
		template<std::size_t len> void insert(const membuf<len>& buf);


		//взятие данных из буфера, 
		//после взятия данных виртуальный размер буффера уменьшается,
		//при попытке взять больше данных чем есть выбрасывается исключение
		template<class V> V* take();

		virtual void takeCopy(void* d, std::size_t len );
		template<class V> void takeCopy(V& v);
		template<std::size_t len> void takeCopy( membuf<len>& buf );
		
		//очистка и заполнение буффера
		void clear();
		void fill(char val);


		//задание и получение виртуального размера
		//при задании размера который больше чем максимально возможный 
		//выбрасывается исключение
		std::size_t getSize() const;
	 	void setSize(std::size_t size);
		static std::size_t getLength() {return LENGTH;}

		void* getEndPtr() {return (char*)data() + cursor;}

		//получить количество использованного и свободного места в буффере
		std::size_t getUsed() const;
		std::size_t getFree() const;

		//получить описатель данных в буфере
		mem::mem_desc getDataDesc();

		//проверк на возможность помещения объекта и проверка на объем содержимого
		bool canContain(std::size_t size) const;
		bool isContain(std::size_t size) const;
		bool isEmpty() const;

	private:
 		void* takeData(std::size_t len);
		void* data();
		const void* data() const;

		size_t cursor;

		char* dataPtr__;
		char data__[LENGTH];
	};



template<std::size_t length>
membuf<length>::membuf():cursor(0), dataPtr__(&data__[0]){};

template<std::size_t length>
membuf<length>::membuf(void* data, std::size_t len):cursor(0), dataPtr__(&data__[0]){ insert(data, len); }

template<std::size_t length>
membuf<length>::~membuf(){};

//
template<std::size_t length>
void membuf<length>::insert(const void* d, std::size_t len)
{
	if( canContain(len) ) 
	{
		memcpy(dataPtr__ + cursor, d, len);
		cursor += len;
	}
	else
	{
		throw std::runtime_error("membuf::insert(): not enough space");
	}
}

template<std::size_t length>
template<class V>
void membuf<length>::insert(const V& value)
{
	insert( (void*)&value, sizeof(V));
}

template<std::size_t length>
template<std::size_t len>
void membuf<length>::insert(const membuf<len>& buf)
{
	insert(buf.data(), buf.getUsed());
}


template<std::size_t length>
template<class V>
V* membuf<length>::take()
{
	return takeData(sizeof(V));
}


template<std::size_t length>
template< class V>
void membuf<length>::takeCopy(V& v)
{
	void* cdp = takeData(sizeof(V));
	if(cdp)
	{
		mem::memcpy(&v, cdp, sizeof(V));
	}
}


template<std::size_t length>
template<std::size_t len>
void membuf<length>::takeCopy( membuf<len>& buf )
{
	void* cdp = takeData(len);
	if(cdp)
	{
		buf.clear();
		buf.insert(cdp, len);
	}
}


template<std::size_t length>
void* membuf<length>::data() 
{
	return dataPtr__;
}

template<std::size_t length>
const void* membuf<length>::data() const 
{
	return dataPtr__;
}

template<std::size_t length>
void membuf<length>::clear()
{
	cursor  = 0;
	dataPtr__ = data__;
}

template<std::size_t length>
void membuf<length>::fill(char val)
{
	memset(data(), val, LENGTH);
}

template<std::size_t length>
std::size_t membuf<length>::getSize() const {return cursor;}

template<std::size_t length>
void membuf<length>::setSize(std::size_t size)
{
	if( size <= LENGTH ) 
	{
		cursor = size;
	}
	else
	{
		throw std::runtime_error("membuf::setSize(): not enough space");
	}
}

template<std::size_t length>
std::size_t membuf<length>::getUsed() const
{
	return cursor;
}

template<std::size_t length>
std::size_t membuf<length>::getFree() const
{
	return LENGTH - cursor;
}

template<std::size_t length>
mem::mem_desc membuf<length>::getDataDesc() 
{
	return mem::mem_desc(data(), cursor); 
}

template<std::size_t length>
bool membuf<length>::canContain(std::size_t size) const
{
	return getFree() >= size;
}

template<std::size_t length>
bool membuf<length>::isContain(std::size_t size)  const
{
	return getUsed() >= size;
}

template<std::size_t length>
bool membuf<length>::isEmpty() const
{
	return cursor == 0;
}
	

template<std::size_t length>
void* membuf<length>::takeData(std::size_t len)
{
	if(isContain(len))
	{
		cursor -= len;				
		return (char*)data() + cursor;
	}
	else
	{
		throw std::runtime_error("membuf::take(): not enough data");
	}

	return 0;			
}

template<std::size_t length>
void membuf<length>::takeCopy(void* d, std::size_t len )
{
	void* ptr = takeData(len);
	mem::memcpy(d, ptr, len);
}



}

