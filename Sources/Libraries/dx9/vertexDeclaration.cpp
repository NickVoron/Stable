#pragma once

#include "vertexDeclaration.h"

namespace dx9{

	Base::FixedArray<LPDIRECT3DVERTEXDECLARATION9*, 1024> VertexDeclaration::declarations;
	int  VertexDeclaration::changeCounter = 0;

}
