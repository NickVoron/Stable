#pragma once

#include "commandProcessor.h"

namespace CmdProc
{

//
//�������� ������������ ���� � ��������������� ����������� ������������ � ��� ��������
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
//�������� ���������� �� ����� ������ ��������� ������� 
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
//����� �� ���������
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
//����������� ����������� �������
//
void registerStdCommands();

}
