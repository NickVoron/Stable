// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	
	class ActionsManager : public Base::MeyersSingleton<ActionsManager>
	{
	public:
		template<class ActionType, class ParamsType> 
		ActionType* execute(ParamsType& params);

		void undo();
		void redo();

		void undo(int steps);
		void redo(int steps);

		bool undoAvailable();
		bool redoAvailable();

		void step(int s);

		void description(std::vector<std::string>& names, int& executedActionsCount, int& undoActionsCount) const;

	private:
		std::list<Action*> actions;
		std::list<Action*> undoActions;
	};

	
	
	
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

	void undo(int steps);
	void redo(int steps);

	void step(int s);

	void description(std::vector<std::string>& names, int& executedActionsCount, int& undoActionsCount);
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