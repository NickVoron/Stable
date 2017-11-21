#include "expressionFunctions.h"

#include "expressions/library.include.h"

#include "positionGenerators/library.include.h"

#include "filesystem.h"
#include "standard.h"
#include "tgf.h"
#include "ods.h"
#include "componentModel.h"

namespace ExpressionFunctions
{
	void InitFunctionsLibrary()
	{
		{
			using namespace Positions;
			BIND_EXPRESSION_FUNCTION(regularXY);
			BIND_EXPRESSION_FUNCTION(regularXZ);
			BIND_EXPRESSION_FUNCTION(regularYZ);

			BIND_EXPRESSION_FUNCTION(circleXY);
			BIND_EXPRESSION_FUNCTION(circleXZ);
			BIND_EXPRESSION_FUNCTION(circleYZ);
		}		

		{
			BIND_EXPRESSION_FUNCTION(filenames);
			BIND_EXPRESSION_FUNCTION(resource_filenames);
			BIND_EXPRESSION_FUNCTION(resource_filename);
		}

		{
			BIND_EXPRESSION_FUNCTION(load_tgf);
			BIND_EXPRESSION_FUNCTION(node_pos);
		}

		{
			BIND_EXPRESSION_FUNCTION(component_info);
		}

		{
			BIND_EXPRESSION_FUNCTION(load_ods);
		}
	}
}