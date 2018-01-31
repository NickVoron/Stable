// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

template<template <class> class ActorInterface, class... Links>
struct ActorBase
{
	template<class ComponentType, class... Params>
	static void developmentACTION(ComponentType& component, Params&&... params)
	{
		ActorInterface<ComponentType>::developmentACTION(component, params...);
	}

	template<class ComponentType>
	static void developmentDESC(std::string& result, ComponentType& component)
	{
		result += str::stringize(ActorInterface<ComponentType>::developmentDESC(component), "\n").str();
	}

	template<class ComponentType, class... Params> static void action(ComponentType* client, Params&&... params)
	{
		auto seq = { (developmentACTION(client->template component<Links>(), params...), 0)... };
	}

	template<class ComponentType> static std::string description(ComponentType* client)
	{
		std::string result;
		auto seq = { (developmentDESC(result, client->template component<Links>()), 0)... };
		return result;
	}
};



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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