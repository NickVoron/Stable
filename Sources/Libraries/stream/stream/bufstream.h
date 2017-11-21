#pragma once

#include "stream.h"
#include "../dataop/dataop.h"

namespace stream
{

//
template<int bufferSize, template <class> class O1, template <class> class O2>
class streambuf 
	: //public dataop::data_operator<mem::membuf<bufferSize>, O1, O2>,
	  public dataop::data_operator<mem::membuf<bufferSize>, O1, O2>::Op
{
protected:
	streambuf(){ this->setBuffers(b1, b2); }
	virtual ~streambuf(){};

	mem::membuf<bufferSize> buffer;

private:
	typedef typename dataop::data_operator<mem::membuf<bufferSize>, O1, O2>::OutputBuffer OB;
	OB b1, b2;
};

//
template<int bufferSize>
class streambuf<bufferSize, dataop::null, dataop::null> 
{
protected:
	virtual ~streambuf(){};
	mem::membuf<bufferSize> buffer;
};


}
