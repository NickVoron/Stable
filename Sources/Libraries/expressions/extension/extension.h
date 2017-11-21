#pragma once
#include "../reference.h"
#include <string>



namespace Expressions
{
	
	class VariableRequster
	{
	public:

		virtual ~VariableRequster(){}

		virtual Reference* addProxy() = 0;
		virtual Reference* addProxy(const std::string& targetName)=0;
		virtual Reference* addProxy(const std::string& targetName, int index)=0;
		virtual Reference::PathElement* addProxyArrayPath(Expression* expr)=0;
		virtual Reference::PathElement* addProxyComponentsPath(const std::string& name)=0;
		virtual Reference::PathElement* addProxyPropertyPath(const std::string& name)=0;

	};


	class Extension
	{
	public:
		virtual ~Extension(){}

		virtual std::string name() = 0;
		virtual void parse(const std::string& extensionText, VariableRequster* requster)=0;
	};


	template<const char* name_>
	class NamedExtension: public Extension
	{
	public:
		virtual std::string name() { return name_; }
		
	};


}//