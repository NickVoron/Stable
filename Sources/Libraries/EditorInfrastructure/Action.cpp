// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "Action.h"

namespace EditorInfrastructure
{

	
	
	
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




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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