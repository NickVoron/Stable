// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "dxmath.h"

#if defined(USE_WINDOWS)

#include <d3dx9.h>

namespace dxmath
{
	
	void FillDXMatrix(nm::matrix4& target, const State& state, float scale )
	{
		FillDXMatrix(target, state, Vector3(scale, scale, scale) );
	}

	
	void FillDXMatrix(nm::matrix4& target, const State& state, const Vector3& scale)
	{
		D3DXMATRIX scaleMat, stateMat;
		D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
		D3DXMatrixAffineTransformation(&stateMat, 1.0f, 0,	(const D3DXQUATERNION*)(&state.orientation), (const D3DXVECTOR3*)(&state.position) );
		D3DXMatrixMultiply((D3DXMATRIX*)&target, &scaleMat, &stateMat);
	}

	void FillDXMatrixTranslate(nm::matrix4& target, const Vector3& translate)
	{
		D3DXMatrixTranslation((D3DXMATRIX*) &target, translate.x, translate.y, translate.z);
	}

	void FillDXMatrixRotateXYZ(nm::matrix4& target, const Vector3& rotate)
	{
		D3DXMATRIX rotateX, rotateY, rotateZ;
		D3DXMatrixRotationX(&rotateX, rotate.x);
		D3DXMatrixRotationY(&rotateY, rotate.y);
		D3DXMatrixRotationZ(&rotateZ, rotate.z);
		((D3DXMATRIX&) target) = rotateX * rotateY * rotateZ;
	}

	void FillDXMatrixScale(nm::matrix4& target, const Vector3& scale)
	{
		D3DXMatrixScaling((D3DXMATRIX*) &target, scale.x, scale.y, scale.z);
	}
}

#endif



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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