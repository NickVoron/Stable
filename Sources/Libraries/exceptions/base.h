#pragma once

#include <iostream>
#include <fstream>
#include <ostream>
#include <stdexcept>

namespace Base {
	
namespace Errors {

class Unknown : public std::runtime_error {
public:
	virtual ~Unknown() throw() {}
	Unknown() : std::runtime_error( "unknown error" ), errorCode("unknown error"){}
	Unknown( const std::string& code ) : std::runtime_error( code.c_str()), errorCode(code){} 
	const std::string& GetMessageText() const { return errorCode; }
protected:
	std::string errorCode;
};


class Simple : public Unknown {
public:
	Simple( const std::string& code ):Unknown(code) {}
	virtual ~Simple() throw() {}
};


std::ostream& operator<<( std::ostream& s, const Base::Errors::Unknown& error );

}
}


