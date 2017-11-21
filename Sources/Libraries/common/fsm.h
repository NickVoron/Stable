#pragma once

#include "Loki/library.include.h"
#include "stuff/enforce.h"

namespace fsm
{
	template<class EnumType, EnumType val>
	struct Enum2Type
	{
		static const EnumType value = val;
	};

	template<class State0T, class State1T, class EventType, EventType transitionEvent>
	struct Transition 
	{
		static const EventType event = transitionEvent;
		typedef State0T			State0;
		typedef State1T			State1;
		typedef EventType		Event;
	};

	template<class InputResultList, class TransitionsList, class InputEventsList>
	struct ExtractStatesExtractor
	{
		typedef typename TransitionsList::Head Transition;
		typedef typename Transition::Event  Event;

		typedef Loki::MakeTypelist<typename Transition::State0, typename Transition::State1> TransitionStates;
		typedef typename Loki::TL::Append<InputResultList, TransitionStates>::Result ResultList;
		typedef typename Loki::TL::Append<InputEventsList, Enum2Type<Event, Transition::event> >::Result ResultEventsList;


		typedef ExtractStatesExtractor<ResultList, typename TransitionsList::Tail, ResultEventsList> ResultData;

		typedef typename ResultData::Result Result;
		typedef typename ResultData::Events Events;

	};

	template<class InputResultList, class InputEventsList>
	struct ExtractStatesExtractor<InputResultList, Loki::NullType, InputEventsList>
	{
		typedef typename Loki::TL::NoDuplicates<InputResultList>::Result Result;
		typedef typename Loki::TL::NoDuplicates<InputEventsList>::Result Events;
	};

	template<class TransitionsList>
	struct ExtractStates
	{
		typedef typename TransitionsList::Head::Event Event;

		typedef ExtractStatesExtractor<Loki::NullType, TransitionsList, Loki::NullType> ResultData;

		typedef typename ResultData::Result Result;
		typedef typename ResultData::Events Events;
	};

	//
	//
	//
	template<class TransitionsList, class TransList>
	struct TransitionsTableInitializer
	{
		typedef typename ExtractStates<TransitionsList>::Result	States;
		typedef typename ExtractStates<TransitionsList>::Events	Events;

		typedef typename TransList::Head Transition;
		typedef typename Transition::State0 State0;
		typedef typename Transition::State1 State1;
		typedef typename Transition::Event  Event;

		template<class Table>
		inline static void init(Table& table)
		{
			int s0 = Loki::TL::IndexOf<States, State0>::value;
			int s1 = Loki::TL::IndexOf<States, State1>::value;
			int e = Loki::TL::IndexOf<Events, Enum2Type<Event, Transition::event> >::value;

			//LOG_MSG(str::stringize(typeid(State0).name(), "[", s0, "]", "->", Transition::event, "(", e, ")", "->", typeid(State1).name(), "[", s1, "]").str());
						
			ENFORCE(s0 >= 0 && s0 < Table::statesCount && e >= 0 && e < Table::eventsCount);

			auto& state = table.data[s0][e];
			state = table.template state<State1>();

			TransitionsTableInitializer<TransitionsList, typename TransList::Tail>::init(table);
		}
	};

	template<class TransitionsList>
	struct TransitionsTableInitializer<TransitionsList, Loki::NullType>
	{
		template<class Table>
		inline static void init(Table&) {}
	};

	template<class EventList, class EventListIter>
	struct TransitionsTableEventIndex
	{
		template<class Event>
		inline static int get(Event eventRV)
		{
			return (EventListIter::Head::value == eventRV) ? Loki::TL::IndexOf<EventList, Enum2Type<Event, EventListIter::Head::value> >::value : TransitionsTableEventIndex<EventList, typename EventListIter::Tail>::get(eventRV);
		}
	};

	template<class EventList>
	struct TransitionsTableEventIndex<EventList, Loki::NullType>
	{
		template<class Event>
		inline static int get(Event eventRV) { return -1; }
	};

	template<class BaseStateType, class TransitionsList>
	struct TransitionsTable 
	{
		typedef typename TransitionsList::Head::Event			Event;
		typedef TransitionsList									Transitions;
		typedef typename ExtractStates<Transitions>::Result		States;
		typedef typename ExtractStates<Transitions>::Events		Events;
		typedef Loki::TypeTuple<States>							StatesTuple;
		typedef Loki::TypeTuple<Events>							EventsTuple;
		typedef BaseStateType									BaseState;

		struct StateExt
		{
			inline StateExt():index(-1), state(0){}

			int index;
			BaseStateType* state;
		};

		struct Table
		{
			template<class State>
			StateExt state() const
			{
				StateExt res;
				res.index = Loki::TL::IndexOf<States, State>::value;
				res.state = (BaseStateType*)(&states.template get<State>());
				return res;
			}

			StateExt initial() const
			{
				StateExt res = state<typename TransitionsList::Head::State0>();
				ENFORCE(res.index == 0);
				return res;
			}

			static const int statesCount = StatesTuple::size;
			static const int eventsCount = EventsTuple::size;
			
			StatesTuple	states;
			StateExt data[statesCount][eventsCount];
		};

		TransitionsTable()
		{
			TransitionsTableInitializer<Transitions, Transitions>::init(transitions);
		}


		StateExt transition(int stateIndex, Event event) const
		{
			int e = TransitionsTableEventIndex<Events, Events>::get(event);
			//LOG_MSG("transition" << stateIndex << "->" << event);
			//ENFORCE(stateIndex >= 0 && stateIndex < StatesTuple::size && e >= 0 && e < EventsTuple::size);
			return e >= 0 ? transitions.data[stateIndex][e] : StateExt();
		}

		StateExt initial() const
		{
			return transitions.initial();
		}

		Table transitions;	
	};

	template<class ClientFSM, class BaseStateType, class BaseStateAdapter, class TransitionsList>
	struct FSM
	{
		typedef TransitionsTable<BaseStateType, TransitionsList> Transitions;
		typedef typename Transitions::Event Event;
		Transitions transitions;

		inline FSM()
		{		
			defaults();
		}

		virtual ~FSM() {}

		inline void defaults()
		{
			current = transitions.initial();
		}

		inline bool isInInitialState() const
		{
			return current.state == initial();
		}

		inline bool post(Event event)
		{
			auto stateExt = transitions.transition(current.index, event);
			if(stateExt.state)
			{
				auto& refthis = *(ClientFSM*)this;
								
				ENFORCE(current.state);
				BaseStateAdapter::exit(refthis, *current.state);
				
				current = stateExt;
				ENFORCE(current.state);
				BaseStateAdapter::enter(refthis, *current.state);
				
				return true;
			}

			return false;
		}
		
		inline BaseStateType* next(Event event) const
		{
			return transitions.transition(current.index, event).state;
		}

		inline BaseStateType* initial() const
		{
			return transitions.initial().state;
		}

		typename Transitions::StateExt current;
	};
}

#define FSM_TRANSITION(SOURCE_0, SOURCE_1, EVENT_VALUE) fsm::Transition<SOURCE_0, SOURCE_1, decltype(EVENT_VALUE), EVENT_VALUE>
