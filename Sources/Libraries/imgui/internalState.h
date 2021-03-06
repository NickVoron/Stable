// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "types.h"

namespace imgui
{
	template<class IState, int count>
	struct StateStack 
	{
		StateStack():cursor(0){}

		IState* add()
		{
			IState* p = (IState*)&data[cursor * sizeof(IState)];
			++cursor;
			return p;
		}

		void pop() 
		{ 
			--cursor;
			((IState*)&data[cursor * sizeof(IState)])->~IState();
		}

		int cursor;
		char data[ sizeof(IState) * count ];
	};

	
	template<class IState>
	class InternalState
	{
	public:
		InternalState(IState* p) : parent(p){};
		IState* parent;
	};

	
	template<class StateHolder, class IState>
	class InternalStateHolder
	{
	public:
		InternalStateHolder() : current(0){};

		IState* start()
		{
			current = new (states.add()) IState(current);
			return current;
		}

		void finish()
		{
			current = current->parent;
			states.pop();
		}

		IState* current;

		StateStack<IState, 64> states;		
	};

}




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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