// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "resourceManager/library.include.h"
#include "rectAtlasBuilder/library.include.h"

namespace Resources
{
	void configureAtlasBuilder(TextureAtlasComposer& builder, const std::string& fileName, Resources::Dependencies& dependencies);

	
	void open_filestream(const char* fileName, std::ifstream& is, bool binary = false);
	void open_filestream(const char* fileName, stream::ifstream<>& is);
	std::string resourceRelativePath(const char* fileName);

	std::string editor_path();

	bool edit(const char* fileName);

	template<class Lines>
	void line_by_line(const char* fileName, Lines& lines)
	{
		std::ifstream infile(fileName);
		for (std::string line; std::getline(infile, line);)
		{
			lines.push_back(line);
		}
	}

	template<class Function>
	void call_line_by_line(const char* fileName, Function&& func)
	{
		std::ifstream infile(fileName);
		for (std::string line; std::getline(infile, line);)
		{
			func(line);
		}
	}

	void setupManager();

	struct Action
	{
		template<class ResourceType, class ActionImpl>
		static bool execute(ResourceType* res, ActionImpl& action)
		{
			if (res && res->GetResource())
			{
				action(res->GetResource());
				return true;
			}

			return false;
		}
	};

	class ActionOnce 
	{
	public:
		template<class ResourceType, class OnceActionImpl>
		static bool execute(ResourceType* res, OnceActionImpl& action)
		{
			if (res && res->GetResource() && (action.value != (int*) res->GetResource()))
			{
				action(res->GetResource());
				action.value = (int*) res->GetResource();
				return true;
			}

			return false;
		}

		template<class ResourceType, class OnceActionImpl, class StepActionImpl>
		static bool execute(ResourceType* res, OnceActionImpl& action, StepActionImpl& step)
		{
			if (res && res->GetResource() && (action.value != (int*)res->GetResource()))
			{
				action(res->GetResource());
				action.value = (int*) res->GetResource();
			}

			return Action::execute(res, step);
		}

	private:
		int* value = 0;	 
	};
}



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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