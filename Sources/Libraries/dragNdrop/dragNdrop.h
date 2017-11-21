#pragma once

#include "containers/library.include.h"

namespace DragNDrop
{
	struct FileList : public std::vector<std::wstring> {};

	class Module : public Base::IntrusiveList<Module>::Node
	{
	public:
		Module();

		virtual const char* extension() const = 0;
		virtual void process(const FileList& fileList) = 0;
	};

	class Dispatcher : public Base::IntrusiveList<Module>
	{
	friend class Module;
	public:
		static void process(const FileList& fileList);

	private:
		Dispatcher(){};
		static Dispatcher& get();
		static void add(Module& module);

		void processImpl(const FileList& fileList);
	};
}
