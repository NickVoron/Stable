#pragma once


namespace Resources {
namespace Errors {

class Unknown : public Base::Errors::Unknown {
public:
	Unknown() {}
	Unknown( const std::string& e ) : Base::Errors::Unknown( "resource error: " + e ) {}
};

class Unnamed : public Unknown {
public:
	Unnamed() : Unknown("has no name") {}
};

class NameRedefinition : public Unknown {
public:
	NameRedefinition( const std::string& newName, const std::string& oldName ) : Unknown(str::stringize("trying to change resource name from", std::quoted(oldName), " to ",std::quoted(newName) )) {}
};

class NotFound : public Unknown {
public:
	NotFound( const std::string& fileName ) : Unknown(str::stringize("file not found: ", std::quoted( fileName )) ) {}
};

class NotCompiled : public Unknown {
public:
	NotCompiled( const std::string& fileName, const std::string& code  ) : Unknown(str::stringize("failed to compile ", std::quoted(fileName), " reason: ", code )) {}
};

}
}