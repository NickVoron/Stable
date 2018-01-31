// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "stats.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexDeclaration.h"
#include "dip.h"
#include "query.h"

#include "defaultLogs/library.include.h"

namespace dx9{

int BufferBase<VertexBuffer>::changeCounter = 0;
int BufferBase<IndexBuffer>::changeCounter = 0;

void Statistic::reset()
{
	DrawingStats::resetCounters();
	VertexBuffer::resetCounter();
	IndexBuffer::resetCounter();
	VertexDeclaration::resetCounter();
	OcclusionQuery::resetCounter();
}

void Statistic::report()
{
	LOG_REPORT("dip count: " << DrawingStats::callCounter);
	LOG_REPORT("poly count: " << DrawingStats::primitivesCounter);
	LOG_REPORT("IB change: " << IndexBuffer::changeCounter);
	LOG_REPORT("VB change: " << VertexBuffer::changeCounter);
	LOG_REPORT("VD change: " << VertexDeclaration::changeCounter);
	LOG_REPORT("HW occluded: " << OcclusionQuery::occludedCounter);
}

}





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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