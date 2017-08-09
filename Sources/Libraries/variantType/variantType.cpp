// Copyright (C) 2014-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "variantType.h"

namespace Variant
{
	ValueList::ValueList(){}

	ValueList::ValueList(Value* id0)
	{
		push_back(id0);
	}

	ValueList::ValueList(Value* id0, Value* id1)
	{
		reserve(2);
		push_back(id0);
		push_back(id1);
	}
	ValueList::ValueList(Value* id0, Value* id1, Value* id2)
	{
		reserve(3);
		push_back(id0);
		push_back(id1);
		push_back(id2);
	}

	ValueList::ValueList(Value* id0, Value* id1, Value* id2, Value* id3)
	{
		reserve(4);
		push_back(id0);
		push_back(id1);
		push_back(id2);
		push_back(id3);
	}

	ValueList::ValueList(Value* id0, Value* id1, Value* id2, Value* id3, Value* id4)
	{
		reserve(5);
		push_back(id0);
		push_back(id1);
		push_back(id2);
		push_back(id3);
		push_back(id4);
	}
}



// Copyright (C) 2014-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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