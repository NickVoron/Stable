#pragma once

#include "commandProcessor.h"

namespace CmdProc
{

//
//Комманда зачитывающая файл и последовательно выполняющая содержащиеся в нем комманды
//
class CommandExec : public Command
{
public:
	virtual void execute();
	virtual void fillArgs();
	virtual std::string getName() const {return "exec";}
	virtual std::string getDesc() const {return "execute command file";}
};

//
//Комманда печатающая на экран список доступных комманд 
//
class CommandHelp : public Command
{
public:
	virtual void execute();
	virtual void fillArgs();
	virtual std::string getName() const {return "help";}
	virtual std::string getDesc() const {return "show help";}
};


//
//Выход из программы
//
class CommandExit : public Command
{
public:
	virtual void execute();
	virtual void fillArgs();
	virtual std::string getName() const {return "exit";}
	virtual std::string getDesc() const {return "exit from program with exitcode 0";}
};

//
//Регистрация стандартных комманд
//
void registerStdCommands();

}
