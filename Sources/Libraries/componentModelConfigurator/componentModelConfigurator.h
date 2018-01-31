// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "descParser/library.include.h"
#include "componentModel/library.include.h"

namespace ComponentModel
{
	using ExternalComponentLinks = std::map<const ObjectParser::ComponentHandle*, ComponentExternalLink>;

	void descriptionTest(const char* filename);
	void descriptionCompile(const char* filename);

	EntitiesList& descriptionLoad(const char* filename, EntitiesList& entities);

	template<class StrType>
	EntitiesList& descriptionLoad(const StrType& filename, EntitiesList& entities)
	{
		return descriptionLoad(str::c_str(filename), entities);
	}

	template<class StrType, class Prologue, class Epilogue>
	void descriptionTest(const StrType& filename, Prologue&& prologue, Epilogue&& epilogue)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);
		
		prologue(entities);
		entities.execute();
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			entities.execute();
		}		
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue, class Iteration>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue, Iteration&& iteration)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);
		
		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			entities.execute();
			iteration(i, entities);
		}
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue, class IterationBegin, class IterationEnd>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue, IterationBegin&& iterationBegin, IterationEnd&& iterationEnd)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			iterationBegin(i, entities);
			entities.execute();
			iterationEnd(i, entities);
		}
		epilogue(entities);
	}
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