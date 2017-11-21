#pragma once

#include <vector>

#include "stuff/meyersSingleton.h"
#include "defaultLogs/library.include.h"

namespace EditorInfrastructure
{
	class Action
	{
	public:
		virtual void execute(){};
		virtual void undo(){};
		virtual void redo(){};
		virtual const char* className() const = 0;
	
	protected:
		virtual void presentResult(){};
	};

	//
	class ActionsManager : public Base::MeyersSingleton<ActionsManager>
	{
	public:
		template<class ActionType, class ParamsType> 
		ActionType* execute(ParamsType& params);

		void undo();
		void redo();

		void undo(std::size_t steps);
		void redo(std::size_t steps);

		bool undoAvailable();
		bool redoAvailable();

		void step(std::size_t s);

		void description(std::vector<std::string>& names, std::size_t& executedActionsCount, std::size_t& undoActionsCount) const;

	private:
		std::list<Action*> actions;
		std::list<Action*> undoActions;
	};

	//
	//
	//
	template<class ActionType, class ParamsType>
	ActionType* ActionsManager::execute(ParamsType& params)
	{
		try
		{
		 	ActionType* action = new ActionType(params);
			action->execute();
			actions.push_back(action);
			LOG_MSG(action->className())
			return action;
		}
		catch(std::exception& e)
		{
			LOG_ERROR(e);		
		}

		return 0;
	}

	template<class ActionType, class ParamsType>
	inline ActionType* execute(ParamsType& params)
	{
		return EditorInfrastructure::ActionsManager::get().execute<ActionType>(params);
	}

	void undo();
	void redo();

	void undo(std::size_t steps);
	void redo(std::size_t steps);

	void step(std::size_t s);

	void description(std::vector<std::string>& names, std::size_t& executedActionsCount, std::size_t& undoActionsCount);
}