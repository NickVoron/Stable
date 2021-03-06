// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include <vector>
#include <map>

#include "stuff/library.include.h"

namespace CmdProc
{
	typedef std::vector<std::string> StringList;


	
	class BaseArgument
	{
	public:
		virtual ~BaseArgument(){}
		virtual void load(const std::string& str)  = 0;
		virtual const std::string& getName() const = 0;
		virtual const std::string& getDesc() const = 0;
		virtual bool isRequired() const			   = 0;
		virtual void setDefault()				   = 0;
	};

	template<typename T>
	class CommandArg : public BaseArgument
	{
	public:
		CommandArg(const std::string& n, const std::string& d)
		:name(n), desc(d), req(true){}

		CommandArg(const std::string& n, const std::string& d, const T& defaultVal)
		:name(n), desc(d), defaultValue(defaultVal), req(false){}

		virtual void load(const std::string& str)
		{
			try 
			{
				val = Base::StrUtils::fromString<T>(str);	
			}
			catch (std::exception& e) 
			{
				throw std::runtime_error( std::string("can't create CommandArg from string: ") + e.what() );
			}
		}

		const T& getValue() const {return val;}
		virtual const std::string& getName() const {return name;}
		virtual const std::string& getDesc() const {return desc;}
		virtual bool isRequired() const {return req;}
		virtual void setDefault(){if(!req) val = defaultValue;}

	private:
		T val;
		
		std::string name;
		std::string desc;
		bool req;
		T defaultValue;
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const CommandArg<T>& cmdArg )
	{
		return stream << cmdArg.getValue();
	}


	
	class StringProcessor
	{
	public:
		StringProcessor(const std::string& str){ process(str); }
		
		const std::string& getCommandName() const;
		StringList::const_iterator getArgsBegin() const;
		StringList::const_iterator getArgsEnd() const;

	private:
		void process(const std::string& str);
		StringList strList;
	};

	
	class Command
	{
	friend class CommandProcessor;
	public:
		virtual ~Command();
		virtual void execute() = 0;
		virtual void fillArgs() = 0;
		virtual std::string getName() const = 0;
		virtual std::string getDesc() const = 0;

		template<class T>
		void addArgument(const std::string& n, const std::string& d, const T& defaultVal)
		{
			arguments.push_back( new CmdProc::CommandArg<T>(n, d, defaultVal) );
		};

		template<class T>
		void addArgument(const std::string& n, const std::string& d)
		{
			arguments.push_back( new CmdProc::CommandArg<T>(n, d) );
		};

	protected:
		void parseArgs(StringList::const_iterator argsBeg, StringList::const_iterator argsEnd);
		void outHelp() const;
		void validateArumentsSequence() const;
		void afterExecClear();

		template<class ValType>
		const ValType& getArgVal(int index)
		{
			CmdProc::CommandArg<ValType>* arg = static_cast< CmdProc::CommandArg<ValType>* >(arguments[index]);
			return arg->getValue();
		}


		typedef std::vector<BaseArgument*> ArgumentList;
		ArgumentList arguments;
	};

	
	
	class CommandProcessor : public Base::ExplicitMeyersSingleton<CommandProcessor>
	{
	public:
		~CommandProcessor();
		void executeCommand(const std::string& cmdString);

		template<class Cmd>
		void registerCommand()
		{
			registerCommand(new Cmd());
		}

		void commandsList(const std::string& filter);
		void outHelp(const std::string& cmdName);

	private:
		void registerCommand(Command* command);
		void executeCommand(const std::string& cmdName, StringList::const_iterator argsBeg, StringList::const_iterator argsEnd);
		Command* findCommand(const std::string& cmdName);

		typedef std::map<std::string, std::unique_ptr<Command>> CommandsMap;
		CommandsMap commands;
	};

	class CommandsManager
	{
	public:
		static void init();
		static void process();
		static void release();

	private:
		static void executeCommand(const std::string& cmd);
	};

	template<class Cmd>
	void registerCommand()
	{
		CommandProcessor::get().registerCommand<Cmd>();
	}

	inline void executeCommand(const std::string& cmd)
	{
		CommandProcessor::get().executeCommand(cmd);
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