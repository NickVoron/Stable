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