#pragma once

#include <stdexcept>

namespace dataop
{

class encryption_error : public std::runtime_error
{  
public:
	encryption_error(const char* err):std::runtime_error(err){} 
	virtual ~encryption_error() throw() {}
};

class decryption_error : public std::runtime_error
{  
public:
	decryption_error(const char* err):std::runtime_error(err){} 
	virtual ~decryption_error() throw() {}
};

class compression_error : public std::runtime_error
{  
public:
	compression_error(const char* err):std::runtime_error(err){} 
	virtual ~compression_error() throw() {}
};

class decompression_error : public std::runtime_error
{  
public:
	decompression_error(const char* err):std::runtime_error(err){} 
	virtual ~decompression_error() throw() {}
};

class buffer_error : public std::runtime_error
{  
public:
	buffer_error(const char* err):std::runtime_error(err){} 
	virtual ~buffer_error() throw() {}
};

template<class E>
void operationError(const char* err)
{
	throw E(err);
}

}
