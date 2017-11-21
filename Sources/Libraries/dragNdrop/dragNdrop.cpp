#include "dragNdrop.h"
#include "defaultLogs/library.include.h"
#include "stuff/library.include.h"

namespace DragNDrop
{
	//
	//
	//
	Module::Module()
	{
		Dispatcher::add(*this);
	}

	//
	//
	//
	Dispatcher& Dispatcher::get()
	{
		static Dispatcher disp;
		return disp;
	}

	void Dispatcher::add(Module& module)
	{
		get().push_back(module);	
	}

	void Dispatcher::process(const FileList& fileList)
	{
		get().processImpl(fileList);
	}

	void Dispatcher::processImpl(const FileList& fileList)
	{
		for (auto module = begin(); module != end(); ++module)
		{
			try
			{
				FileList moduleInput;
				std::string ext = module->extension();

				for (const std::wstring& fileName : fileList)
				{
					std::string fext = Base::FileUtils::GetLastExtention(Base::StrUtils::Convert(fileName));
					Base::StrUtils::toLower(fext);
					Base::StrUtils::toLower(ext);
					if (ext == fext)
					{
						moduleInput.push_back(fileName);
					}
				}

				if (!moduleInput.empty())
				{
					module->process(moduleInput);
				}
			}
			catch (std::exception& e)
			{
				LOG_ERROR(e);
			}			
		}
	}
}
