// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "stdCommands.h"
#include <fstream>
#include <iostream>

namespace CmdProc
{


void CommandExec::execute()
{
	CommandArg<std::string>* zeroArg = static_cast< CommandArg<std::string>* >(arguments[0]);
	const std::string& fileName = zeroArg->getValue();

	std::ifstream file( fileName.c_str() );
	if( !file.good() )
	{
		throw std::runtime_error("can't open file: " + fileName);		
	}

	char buffer[4096];
	while(file.good())
	{
		file.getline(buffer, 4096);
		executeCommand(buffer);
	}
}

void CommandExec::fillArgs()
{
	addArgument<std::string>("fn", "file with commands list name");
}


void CommandHelp::execute()
{

	CommandArg<std::string>* zeroArg = static_cast< CommandArg<std::string>* >(arguments[0]);
	const std::string& cmdName = zeroArg->getValue();

	if(cmdName.empty())
	{
		std::cout << std::endl;
		CommandProcessor::get().commandsList(std::string() );
	}
	else
	{
		CommandProcessor::get().outHelp(cmdName);
	}
}

void CommandHelp::fillArgs()
{
	addArgument<std::string>("fn", "alphabetical filter", "");
}


void CommandExit::execute()
{
	exit(0);
}

void CommandExit::fillArgs()
{

}






void registerStdCommands()
{
	registerCommand<CommandExec>();
	registerCommand<CommandHelp>();
	registerCommand<CommandExit>();
}

}




// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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