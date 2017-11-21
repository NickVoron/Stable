#pragma once
#include "../types.h"

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

	//
	template<class IState>
	class InternalState
	{
	public:
		InternalState(IState* p) : parent(p){};
		IState* parent;
	};

	//
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

}//