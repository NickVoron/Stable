// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.