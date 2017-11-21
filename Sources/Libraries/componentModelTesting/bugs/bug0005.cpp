#include "bug0005.h"
#include "../tests/utils.h"
#include "descParser/library.include.h"

namespace ComponentModelTesting
{
	Bug0005::Bug0005()
	{
		std::string path = Resources::resourceRelativePath("desc/bugs/bug0005.desc");
		ComponentModel::descriptionCompile(path.c_str());
		ComponentModel::descriptionTest(path.c_str());
	}
}