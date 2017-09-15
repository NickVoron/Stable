// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <string>
#include <memory>

#include "../classDescEtc/classDesc.h"
#include "instanceHandle.h"
#include "prototypeHandle.h"

namespace ObjectParser
{
	struct ComponentModelConfigurator
	{
		
		virtual void beginCreateInstance(const std::string& name, const std::string& type) {}
		virtual void endCreateInstance(InstanceHandle* instance) {}

		
		virtual void beginCreatePrototype(const InstanceDefinitionExpression& prototype) {}
		virtual void endCreatePrototype(PrototypeHandle* handle) {}

		
		
		
		
		
		
		
		
		
		
		

		virtual void preCreateComponent(const std::string& type, const std::string& name) {}
		virtual void beginCreateComponent(ComponentHandle* handle, const std::string& type, const std::string& name) {}
		virtual void bindRuntimeProperty(ComponentHandle* handle, const std::string& name, const std::string& value) {}
		virtual void bindLink(ComponentHandle* handle, const std::string& name, const ComponentHandle* value) {}
		virtual void bindComponentProperty(ComponentHandle* handle, const std::string& name, const Expressions::Expression* value) {}
		virtual void endCreateComponent(ComponentHandle* handle) {}
	};


	
	struct Logger
	{
		Logger(bool enable = false):enable(enable) {}
		template<class T>
		Logger& operator<<(const T& obj);

		bool enable = false;
	};

	template<class T>
	Logger& Logger::operator<<(const T& obj)
	{
		if (enable)
		{
			logs::log() << obj;
		}

		return *this;
	}

}



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