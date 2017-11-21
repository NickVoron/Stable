#include "stdCommands.h"
#include <fstream>
#include <iostream>

namespace CmdProc
{

//
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

//
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

//
void CommandExit::execute()
{
	exit(0);
}

void CommandExit::fillArgs()
{

}



//
//
//
void registerStdCommands()
{
	registerCommand<CommandExec>();
	registerCommand<CommandHelp>();
	registerCommand<CommandExit>();
}

}
