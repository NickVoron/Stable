// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <tuple>
#include <type_traits>

template < typename... Types >
struct typelist {};

template < typename TElement, typename TList >
struct exists_in  : public std::false_type
{
};

template < typename Type, typename... List >
struct exists_in < Type, typelist < Type, List... > > : public std::true_type
{
};

template < typename Type, typename ListHead, typename... ListTail >
struct exists_in < Type, typelist <ListHead, ListTail... > > : public exists_in < Type, typelist < ListTail... > >
{
};

template < typename Type, typename List, bool is_duplicate = exists_in < Type, List >::type::value>
struct add_unique;

template < typename Type, typename... List >
struct add_unique < Type, typelist < List... >, true >
{
	typedef typelist < List... > type;
};

template < typename Type, typename... List >
struct add_unique < Type, typelist < List... >, false >
{
	typedef typelist < Type, List... > type;
};


template < typename... Args >
struct process_arguments;

template < >
struct process_arguments < >
{
	typedef typelist < > type;
};

template < typename THead, typename... TTail >
struct process_arguments < THead, TTail... >
{
	typedef typename add_unique < THead, typename process_arguments < TTail... >::type>::type type;
};



template<class... T> 
struct make_uniue_tuple : make_uniue_tuple<typename process_arguments<T...>::type>
{
};

template<class... T>
struct make_uniue_tuple<typelist<T...>>
{
	typedef std::tuple<T...> tuple;
};

namespace fsm
{
	template<class State0T, class State1T, class EventT>
	struct Transition 
	{
		typedef State0T	State0;
		typedef State1T	State1;
		typedef EventT	Event;
		
		template<class Adapter, class FSM, class BaseState>
		static BaseState* try_execute(FSM& fsm, BaseState*& current, State1& newstate, const Event& event)
		{
			static_assert(std::is_base_of<BaseState, State0>::value);
			static_assert(std::is_base_of<BaseState, State1>::value);
			
			if(auto state = dynamic_cast<State0*>(current))
			{
                current = &newstate;
				Adapter::transition(fsm, *state, newstate, event);
				return &newstate;
			}

			return nullptr;			
		}
	};

	template<class Event, bool acceptThisEvent, class Transition, class... TransitionsList>
	struct TransitionWalkerImpl;

	template<class Event, class Transition>
	struct TransitionWalkerImpl<Event, true, Transition>
	{
		template<class Adapter, class FSM, class BaseState, class... PossibleStates>
		static BaseState* try_execute(FSM& fsm, BaseState*& current, std::tuple<PossibleStates...>& states, const Event& event)
		{
			return Transition::template try_execute<Adapter>(fsm, current, std::get<typename Transition::State1>(states), event);
		}
	};

	template<class Event, class Transition>
	struct TransitionWalkerImpl<Event, false, Transition>
	{
		template<class Adapter, class FSM, class BaseState, class... PossibleStates>
		static BaseState* try_execute(FSM& fsm, BaseState*& current, std::tuple<PossibleStates...>& states, const Event& event)
		{
			return nullptr;
		}
	};

	template<class Event, class Transition, class... TransitionsList>
	struct TransitionWalker;

	template<class Event, class Transition, class... TransitionsList>
	struct TransitionWalkerImpl<Event, true, Transition, TransitionsList...>
	{		
		template<class Adapter, class FSM, class BaseState, class... PossibleStates>
		static BaseState* try_execute(FSM& fsm, BaseState*& current, std::tuple<PossibleStates...>& states, const Event& event)
		{ 
			auto result = Transition::template try_execute<Adapter>(fsm, current, std::get<typename Transition::State1>(states), event);
			if(!result)
			{
				return TransitionWalker<Event, TransitionsList...>::template try_execute<Adapter>(fsm, current, states, event);
			}

			return result;
		}
	};

	template<class Event, class Transition, class... TransitionsList>
	struct TransitionWalkerImpl<Event, false, Transition, TransitionsList...>
	{
		template<class Adapter, class EventT, class FSM, class BaseState, class... PossibleStates>
		static BaseState* try_execute(FSM& fsm, BaseState*& current, std::tuple<PossibleStates...>& states, const EventT& event)
		{
			return TransitionWalker<Event, TransitionsList...>::template try_execute<Adapter>(fsm, current, states, event);
		}
	};

	template<class Event, class Transition, class... TransitionsList>
    struct TransitionWalker : TransitionWalkerImpl<Event, std::is_same<Event, typename Transition::Event>::value, Transition, TransitionsList...>
	{
	};

	template<class... TransitionsList>
	struct TransitionsTable 
	{
		typename make_uniue_tuple<typename TransitionsList::State0..., typename TransitionsList::State1...>::tuple states;

		template<class Adapter, class FSM, class BaseState, class Event>
		BaseState* try_transition(FSM& fsm, BaseState*& current, const Event& event)
		{
			return TransitionWalker<Event, TransitionsList...>::template try_execute<Adapter>(fsm, current, states, event);
		}
	};

	template<class ClientFSM, class BaseState, class Adapter, class... TransitionsList>
	struct FSM
	{
		using FSMType = FSM;

		TransitionsTable<TransitionsList...>  transitions;
		
		FSM()
		{
			current = &std::get<std::tuple_size_v<decltype(transitions.states)> - 1>(transitions.states);
		}

		template<class Event>
		bool post(const Event& event)
		{
			auto result = transitions.template try_transition<Adapter>(static_cast<ClientFSM&>(*this), current, event);
			return result != nullptr;
		}

		template<class State>
		decltype(auto) get()
		{
			return std::get<State>(transitions.states);
		}

		BaseState* current = nullptr;
	};
}




// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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