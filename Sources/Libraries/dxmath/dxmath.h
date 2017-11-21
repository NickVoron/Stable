#pragma once

#include <d3dx9.h>

#include "math/library.include.h"

namespace dxmath
{
	void FillDXMatrix( D3DXMATRIX& target, const State& state, float scale = 1.0f );
	void FillDXMatrix( D3DXMATRIX& target, const State& state, const Vector3& scale);

	void FillDXMatrixTranslate( D3DXMATRIX& target, const Vector3& translate);
	void FillDXMatrixRotateXYZ( D3DXMATRIX& target, const Vector3& rotate);
	void FillDXMatrixScale( D3DXMATRIX& target, const Vector3& scale);
}