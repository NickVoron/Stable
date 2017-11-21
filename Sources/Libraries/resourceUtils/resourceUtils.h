#pragma once

#include "resourceManager/library.include.h"
#include "rectAtlasBuilder/library.include.h"

namespace Resources
{
	void configureAtlasBuilder(TextureAtlasComposer& builder, const std::string& fileName, Resources::Dependencies& dependencies);

	
	void open_filestream(const char* fileName, std::ifstream& is, bool binary = false);
	void open_filestream(const char* fileName, stream::ifstream<>& is);
	std::string resourceRelativePath(const char* fileName);

	std::string editor_path();

	bool edit(const char* fileName);

	template<class Lines>
	void line_by_line(const char* fileName, Lines& lines)
	{
		std::ifstream infile(fileName);
		for (std::string line; std::getline(infile, line);)
		{
			lines.push_back(line);
		}
	}

	template<class Function>
	void call_line_by_line(const char* fileName, Function&& func)
	{
		std::ifstream infile(fileName);
		for (std::string line; std::getline(infile, line);)
		{
			func(line);
		}
	}

	void setupManager();

	struct Action
	{
		template<class ResourceType, class ActionImpl>
		static bool execute(ResourceType* res, ActionImpl& action)
		{
			if (res && res->GetResource())
			{
				action(res->GetResource());
				return true;
			}

			return false;
		}
	};

	class ActionOnce 
	{
	public:
		template<class ResourceType, class OnceActionImpl>
		static bool execute(ResourceType* res, OnceActionImpl& action)
		{
			if (res && res->GetResource() && (action.value != (int*) res->GetResource()))
			{
				action(res->GetResource());
				action.value = (int*) res->GetResource();
				return true;
			}

			return false;
		}

		template<class ResourceType, class OnceActionImpl, class StepActionImpl>
		static bool execute(ResourceType* res, OnceActionImpl& action, StepActionImpl& step)
		{
			if (res && res->GetResource() && (action.value != (int*)res->GetResource()))
			{
				action(res->GetResource());
				action.value = (int*) res->GetResource();
			}

			return Action::execute(res, step);
		}

	private:
		int* value = 0;	 
	};
}