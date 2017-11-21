#include "Action.h"

namespace EditorInfrastructure
{

	//
	//
	//
	void ActionsManager::undo()
	{
		if(undoAvailable())
		{
			Action* action = actions.back();
			action->undo();
			undoActions.push_back(action);
			actions.pop_back();
		}
	}

	void ActionsManager::redo()
	{
		if(redoAvailable())
		{
			Action* action = undoActions.back();
			action->redo();
			actions.push_back(action);
			undoActions.pop_back();
		}
	}

	void ActionsManager::undo(std::size_t steps)
	{
		for (int i = 0; i < steps; ++i)
		{
			undo();
		}
	}

	void ActionsManager::redo(std::size_t steps)
	{
		for (int i = 0; i < steps; ++i)
		{
			redo();
		}
	}

	bool ActionsManager::undoAvailable()
	{
		return !actions.empty();
	}

	bool ActionsManager::redoAvailable()
	{
		return !undoActions.empty();
	}

	void ActionsManager::description(std::vector<std::string>& names, std::size_t& executedActionsCount, std::size_t& undoActionsCount) const
	{
		executedActionsCount = actions.size();
		undoActionsCount = undoActions.size();

		for(const Action* action : actions)
		{
			names.push_back(action->className());
		}

		for(const Action* action : undoActions)
		{
			names.push_back(action->className());
		}
	}

	void ActionsManager::step(std::size_t s)
	{
		auto executedActionsCount = actions.size();
		auto undoActionsCount = undoActions.size();

		if(s > executedActionsCount) redo(s - executedActionsCount);
		else 
		if(s < executedActionsCount) undo(executedActionsCount - s);
	}

	//
	//
	//

	void undo()	{	ActionsManager::get().undo();	}
	void redo()	{	ActionsManager::get().redo();	}

	void undo(std::size_t steps) { ActionsManager::get().undo(steps); }
	void redo(std::size_t steps) { ActionsManager::get().redo(steps); }

	void description(std::vector<std::string>& names, std::size_t& executedActionsCount, std::size_t& undoActionsCount)
	{
		ActionsManager::get().description(names, executedActionsCount, undoActionsCount);
	}

	void step(std::size_t s)
	{
		ActionsManager::get().step(s);
	}

}
