// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "quietConfigurator.h"

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	
	void QuietConfigurator::beginCreateInstance(const std::string& name, const std::string& type, bool noIstance)
	{
		currentInstanceName = name;
	}

	InstanceHandle* QuietConfigurator::endCreateInstance()
	{
		return Expressions::add<InstanceHandle>(currentInstanceName);
	}

	ObjectParser::ComponentHandle* QuietConfigurator::preCreateComponent(const std::string& type, const std::string& name)
	{
		return Expressions::add<ComponentHandle>();
	}

	void QuietConfigurator::beginCreateComponent(ObjectParser::ComponentHandle* handle, const std::string& type, const std::string& name)
	{
	}

	void QuietConfigurator::bindRuntimeProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const std::string& value)
	{

	}

	void QuietConfigurator::bindLink(ObjectParser::ComponentHandle* handle, const std::string& name, ComponentHandle* value)
	{
	}

	void QuietConfigurator::bindComponentProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const Expressions::Expression* value, Expressions::ScopeNames& scopeNames)
	{

	}

	void QuietConfigurator::endCreateComponent(ObjectParser::ComponentHandle* handle)
	{
		
	}

}//



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
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