#pragma once

#include "descParser/library.include.h"
#include "componentModel/library.include.h"

namespace ComponentModel
{
	using ExternalComponentLinks = std::map<const ObjectParser::ComponentHandle*, ComponentExternalLink>;

	void descriptionTest(const char* filename);
	void descriptionCompile(const char* filename);

	EntitiesList& descriptionLoad(const char* filename, EntitiesList& entities);

	template<class StrType>
	EntitiesList& descriptionLoad(const StrType& filename, EntitiesList& entities)
	{
		return descriptionLoad(str::c_str(filename), entities);
	}

	template<class StrType, class Prologue, class Epilogue>
	void descriptionTest(const StrType& filename, Prologue&& prologue, Epilogue&& epilogue)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		entities.execute();
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			entities.execute();
		}		
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue, class Iteration>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue, Iteration&& iteration)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);
		
		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			entities.execute();
			iteration(i, entities);
		}
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue, class IterationBegin, class IterationEnd>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue, IterationBegin&& iterationBegin, IterationEnd&& iterationEnd)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			iterationBegin(i, entities);
			entities.execute();
			iterationEnd(i, entities);
		}
		epilogue(entities);
	}
}

