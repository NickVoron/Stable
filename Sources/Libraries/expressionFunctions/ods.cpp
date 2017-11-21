#include "standard.h"
#include "odsfile/library.include.h"


namespace ExpressionFunctions
{
	std::vector<std::vector<std::string>> load_ods(const std::string& fileName)
	{
		std::vector<std::vector<std::string>> result;
		odsfile::table(fileName, result);
		return result;
	}
}