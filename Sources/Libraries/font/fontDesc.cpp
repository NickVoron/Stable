#include "fontDesc.h"


namespace unigui
{
namespace font
{



FontDesc::FontDesc():
	size(14),
	bold(false),
	italic(false),
	underline(false),
	central({1, 6}),
	inner({0,0}),
	outer({0,0})
{
}


FontDesc::~FontDesc()
{
}

}//
}//