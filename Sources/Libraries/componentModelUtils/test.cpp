#include "test.h"
#include "io.h"

//#include "componentsInitScript/library.include.h"

namespace ComponentsTest
{
	void test(EntitiesList& content, int iterationsCount)
	{
		try
		{
			for (auto& cls : content.classes.classes)
			{
				//content.classes[i].spawn(0);
			};

			content.activate(true);
			content.activate(false);
			content.activate(true);

			for (int i = 0; i < iterationsCount; ++i)
			{
				content.execute();
			};

			LOG_MSG("aftercycle");
			content.execute();
			LOG_MSG("completed");
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

	void test(const char* fileName, int iterationsCount)
	{
		LOG_MSG("Test: " << fileName << " iterations count = " << iterationsCount);

		try
		{
			EntitiesList content;
			ComponentsIO::load_script(fileName, content, "", true, false);

			test(content, iterationsCount);
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

	void test(const char* fileName)
	{
		test(fileName, 0);
	}
}