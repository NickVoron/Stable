#pragma once

#include "errors.h"

#include "memory/library.include.h"

namespace dataop
{
	template<int outputSize>
	struct data_storage
	{	
		enum { OUTPUT_SIZE = outputSize };
		typedef mem::membuf<OUTPUT_SIZE> OutBuffer;

		data_storage() { mem::memzero(upl);	}

		void setBuffers(OutBuffer& b1, OutBuffer& b2){ fb = &b1; bb = &b2; }

	protected:
		mem::mem_desc processData  ( const mem::mem_desc& d) {return d;}
		mem::mem_desc unprocessData( const mem::mem_desc& d) {return d;}
		mem::mem_desc getData(){return fb->getDataDesc();}

		mem::mem_desc getOutBuf(const mem::mem_desc& inputData, std::size_t reqLen)
		{
			if(!fb ||!bb)operationError<buffer_error>("data_storage::swapBuffers(): buffers not initialized");
			if(reqLen > OutBuffer::getLength()) operationError<buffer_error>("data_storage::getOutBuf(): too more memory requested ");
			
			if(inputData.data == (void*)front().data ) swapBuffers();
			mem::mem_desc res = front();
			res.len = reqLen; 
			return res;
		}

		std::size_t upl[10];
		std::size_t uplBeg = 0;

	private: 
		mem::mem_desc front(){return fb->getDataDesc();}
		mem::mem_desc back() {return bb->getDataDesc();}

		void swapBuffers() { std::swap(fb, bb);	}
	
		OutBuffer* fb = nullptr;
		OutBuffer* bb = nullptr;
	};

	template<class MemBuf>
	struct data_size
	{
		enum { OUTPUT_SIZE = MemBuf::LENGTH };
	};


	
	template<class C>
	struct null{};

	//
	template
	<class MemBuf, 
	template<class> class C1 = null, 
	template<class> class C2 = null, 
	template<class> class C3 = null, 
	template<class> class C4 = null>
	struct data_operator;

	//
	//
	template <class MemBuf>
	struct data_operator<MemBuf>
	{
		typedef MemBuf InputBuffer;
	};

	template <class MemBuf, template<class> class C1>
	struct data_operator<MemBuf, C1>
	{
		typedef MemBuf InputBuffer;
		typedef C1< data_size<MemBuf> > Size;
		typedef C1< data_storage< Size::OUTPUT_SIZE > > Op;
		typedef typename Op::OutBuffer OutputBuffer;
	};

	template <class MemBuf, template<class> class C1, template<class> class C2>
	struct data_operator<MemBuf, C1, C2>
	{
		typedef MemBuf InputBuffer;
		typedef C1< C2< data_size<MemBuf> > > Size;
		typedef C1< C2< data_storage< Size::OUTPUT_SIZE > > > Op;
		typedef typename Op::OutBuffer OutputBuffer;
	};

	
	template <class MemBuf, template<class> class C1, template<class> class C2, template<class> class C3>
	struct data_operator<MemBuf, C1, C2, C3>
	{
		typedef MemBuf InputBuffer;
		typedef C1< C2< C3< data_size<MemBuf> > > > Size;
		typedef C1< C2< C3< data_storage< Size::OUTPUT_SIZE > > > >Op;
		typedef typename Op::OutBuffer OutputBuffer;
	};

	template <class MemBuf, template<class> class C1, template<class> class C2, template<class> class C3, template<class> class C4>
	struct data_operator
	{
		typedef MemBuf InputBuffer;
		typedef C1< C2< C3< C4< data_size<MemBuf> > > > > Size;
		typedef C1< C2< C3< C4< data_storage< Size::OUTPUT_SIZE > > > > > Op;
		typedef typename Op::OutBuffer OutputBuffer;
	};


}

