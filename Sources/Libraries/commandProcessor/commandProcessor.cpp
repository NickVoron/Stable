// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "commandProcessor.h"
#include "stdCommands.h"
#include "defaultLogs/library.include.h"
#include "console/library.include.h"
#include <boost/tokenizer.hpp>
#include "common/clearPtrContainer.h"


namespace CmdProc
{




void StringProcessor::process(const std::string& str)
{
	if(str.empty())
	{
		throw std::runtime_error("can't process empty command string");
	}

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	strList.clear();

	tokenizer tok(str, sep);

	for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
	{
		strList.push_back(*beg);
	}
}

const std::string& StringProcessor::getCommandName() const
{
	return strList.front();
}

StringList::const_iterator StringProcessor::getArgsBegin() const
{
	return ++strList.begin();
}

StringList::const_iterator StringProcessor::getArgsEnd() const
{
	return strList.end();
}




Command::~Command()
{
	Base::clearPtrContainer(arguments);
}

void Command::parseArgs(StringList::const_iterator argsBeg, StringList::const_iterator argsEnd)
{
	size_t argsCount		= arguments.size();
	size_t inputArgsCount	= std::distance(argsBeg, argsEnd);

	if(argsCount < inputArgsCount)
	{
		throw std::runtime_error( str::stringize("incorrect arguments count: needed = ", argsCount, " passed = ", inputArgsCount));
	}

	for(size_t i = 0; i < inputArgsCount; ++i)
	{
		arguments[i]->load( *(argsBeg++) );
	}
}

void Command::afterExecClear()
{
	for(BaseArgument* arg : arguments)
	{
		arg->setDefault();
	}
}

void Command::outHelp() const
{
	LOG_INFO("command name: " << getName());
	LOG_INFO("command desc: " << getDesc());
	
	ArgumentList::const_iterator it  = arguments.begin();
	ArgumentList::const_iterator end = arguments.end();
	if(it != end)
	{
		LOG_INFO("parameters: ");
		
		BaseArgument* arg = *it;

		bool req = true;
		if(arg->isRequired())
			LOG_INFO("\trequired: ");
		
		for(; it != end; ++it)
		{
			if(req && !arg->isRequired())
			{
				req = false;
				LOG_INFO("\toptional: ");
			}
			LOG_INFO("\t" <<arg->getName() << " - " << arg->getDesc());
		}
	}
}

void Command::validateArumentsSequence() const
{
	ArgumentList::const_iterator it  = arguments.begin();
	ArgumentList::const_iterator end = arguments.end();

	for(; it != end; ++it)
	{
		
	}
}





CommandProcessor::~CommandProcessor()
{
	
}


void CommandProcessor::executeCommand(const std::string& cmdName, StringList::const_iterator argsBeg, StringList::const_iterator argsEnd)
{
	Command* cmd = findCommand(cmdName);
	try
	{
		if(!cmd) 
		{
			throw std::runtime_error("not found command with name: " + cmdName);
		}

		cmd->parseArgs(argsBeg, argsEnd);
		cmd->execute();
		cmd->afterExecClear();
	}
	catch(std::exception& e)
	{
		LOG_WARNING("command " << cmdName << " execution error: " << e.what() << ".");
		
		if(cmd)
		{
			LOG_WARNING("Use ""help " << cmdName << """");
		}
		else
		{
			LOG_WARNING("Use ""help"" command to show available commands list.");
		}
	}
}


void CommandProcessor::executeCommand(const std::string& cmdString)
{
	if(!cmdString.empty())
	{
		StringProcessor sp(cmdString);
		executeCommand(sp.getCommandName(), sp.getArgsBegin(), sp.getArgsEnd());
	}
}


void CommandProcessor::outHelp(const std::string& cmdName)
{
	Command* cmd = findCommand(cmdName);
	if(cmd)
	{
		cmd->outHelp();
	}
	else
	{
		commandsList(cmdName);
	}

}


void CommandProcessor::registerCommand(Command* command)
{
	std::string cmdName = command->getName();
	Command* cmd = findCommand(cmdName);
	if(cmd)	return;

	command->fillArgs();
	commands.insert( std::make_pair(cmdName, command) );
}



Command* CommandProcessor::findCommand(const std::string& cmdName)
{
	CommandsMap::iterator it = commands.find(cmdName);
	return it == commands.end() ? 0 : it->second.get();
}


void CommandProcessor::commandsList(const std::string& filter)
{
	CommandsMap::iterator it  = commands.begin();
	CommandsMap::iterator end =	commands.end();

	if(filter.empty())
	{
		for(; it != end; ++it)
			LOG_INFO(it->first);
	}
	else
	{
		for(; it != end; ++it)
		{
			const std::string& cmdName = it->first;

			size_t filterLen = filter.length();
			bool filtered = filterLen < cmdName.length();
			
			for(size_t i = 0; i < filterLen && filtered; ++i)
				filtered &= (filter[i] == cmdName[i]);

			if(filtered) 
			{
				LOG_INFO(cmdName);
			}
		}
	}	
}





void CommandsManager::init()
{
	CmdProc::CommandProcessor::create();
	CmdProc::registerStdCommands();
}

void CommandsManager::release()
{
	CmdProc::CommandProcessor::destroy();
}

void CommandsManager::process()
{
}

void CommandsManager::executeCommand(const std::string& cmd)
{
	try
	{
		CmdProc::CommandProcessor::get().executeCommand(cmd);
	}
	catch(std::exception& e)
	{
		LOG_ERROR(e.what());
	}
}




}




// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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