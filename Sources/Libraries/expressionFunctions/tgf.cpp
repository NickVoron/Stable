#include "standard.h"
#include "TGFParser/library.include.h"


namespace ExpressionFunctions
{
	tgf::TGFGraph load_tgf(const std::string& fileName)
	{
		return tgf::load(fileName.c_str());
	}

	Vector3 node_pos(int count)
	{
		return Vector3(rand() % 100 - 50, 0, rand() % 100 - 50);
	}
}