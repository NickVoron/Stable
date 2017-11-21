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

