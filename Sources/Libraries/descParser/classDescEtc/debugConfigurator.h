// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "componentModelConfigurator.h"

namespace ObjectParser
{
	struct DebugComponentModelConfigurator : public ComponentModelConfigurator
	{
		virtual void beginCreateInstance(const std::string& name, const std::string& type)override;
		virtual InstanceHandle* endCreateInstance() override;

		virtual ComponentHandle* preCreateComponent(const std::string& type, const std::string& name) override;
		virtual void beginCreateComponent(ComponentHandle* handle, const std::string& type, const std::string& name) override;
		virtual void bindRuntimeProperty(ComponentHandle* handle, const std::string& name, const std::string& value) override;
		virtual void bindLink(ComponentHandle* handle, const std::string& name, const ComponentHandle* value) override;
		virtual void bindComponentProperty(ComponentHandle* handle, const std::string& name, const Expressions::Expression* value) override;
		virtual void endCreateComponent(ComponentHandle* handle) override;

	private:
		void printTabs();
		int instanceTabs = -1;
		std::string currentInstanceName;
	};
}



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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