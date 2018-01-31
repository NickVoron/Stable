// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "containers/intrusiveList.h"
#include "inputSysWin/library.include.h"

namespace imgui
{
namespace modules
{

	class Module : public Base::IntrusiveList<Module>::Node
	{
	friend struct Modules;
	friend class Application;
	public:
		Module();
		virtual ~Module(){};
		virtual void process() = 0;
		virtual const char* name() const = 0;
		virtual const wchar_t* viewportName() const;

		bool active;

	protected:
		Input::KeyCode switcher;

	private:
		void release();
		void processInternal();
		Input::Action switchAction;
	};

	struct Modules : public Base::IntrusiveList<Module> 
	{
		void process();
		void release();
		void save();
		void load();
	};

	class Application
	{
	friend class Module;
	public:
		static const int maxViewportsCount = 16;

		static void process();
		static void save();
		static void load();
		static void release();
		
		static bool showActivationFlags;
		static bool hideOnMouseR;

	private:
		Application();
		static Application& get();
		static void add(Module& module);
		void activateSwitcher(Input::KeyCode& switcher);

		bool enabledSwitcher;
		bool switchers[Input::KEYBOARD_TOTAL_COUNT];

		
		Modules uiModules;
	};

	class Editor : public Module
	{
	public:
		typedef void (*EditFunction)();
		
		Editor(const char* name, EditFunction func);
		Editor(EditFunction func);

		virtual void process();
		virtual const char* name() const;


		std::string caption;
		EditFunction function;
	};
}
}



// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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