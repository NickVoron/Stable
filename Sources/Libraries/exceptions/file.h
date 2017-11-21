#pragma once

#include "base.h"
#include <string>

namespace Base {
namespace Errors {
namespace File {


// abstract file error:
class Unknown : public Base::Errors::Unknown {
public:
	virtual ~Unknown() throw (){}
	Unknown( const std::string& userFile )
		: fileName( userFile )
		{
			Base::Errors::Unknown::errorCode = "unknown file error in ";
			AddFileNameTo( Base::Errors::Unknown::errorCode );
		}
protected:
	void AddFileNameTo( std::string& target ) { std::string s = "\"" + fileName + "\""; target += s;}
	std::string fileName;
};

class NotFound : public Base::Errors::File::Unknown {
public:
	//FileNotFound( const std::string& userFileName ) { Unknown::errorCode += "\"" + userFileName + "\" not found"; }
	NotFound( const std::string& userFile )
		: Base::Errors::File::Unknown( userFile )
		{
			Base::Errors::Unknown::errorCode = "file ";
			AddFileNameTo( Base::Errors::Unknown::errorCode );
			Base::Errors::Unknown::errorCode += " not found";
		}

	virtual ~NotFound() throw (){}

};


class Read : public Base::Errors::File::Unknown {
public:
	Read( const std::string& userFile, const std::string& errorTag )
		: Base::Errors::File::Unknown( userFile )
		{
			Base::Errors::Unknown::errorCode = "failed to read ";
			Base::Errors::Unknown::errorCode += errorTag;
			Base::Errors::Unknown::errorCode += " from";
			AddFileNameTo( Base::Errors::Unknown::errorCode );
		}

	virtual ~Read() throw (){}
};


} // namespace File
} // namespace Errors
} // namespace Base

