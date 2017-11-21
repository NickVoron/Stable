#include "dxmath.h"

namespace dxmath
{
	//
	void FillDXMatrix( D3DXMATRIX& target, const State& state, float scale )
	{
		FillDXMatrix(target, state, Vector3(scale, scale, scale) );
	}

	//
	void FillDXMatrix( D3DXMATRIX& target, const State& state, const Vector3& scale)
	{
		D3DXMATRIX scaleMat, stateMat;
		D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
		D3DXMatrixAffineTransformation(&stateMat, 1.0f, 0,	(const D3DXQUATERNION*)(&state.orientation), (const D3DXVECTOR3*)(&state.position) );
		D3DXMatrixMultiply(&target, &scaleMat, &stateMat);
	}

	void FillDXMatrixTranslate( D3DXMATRIX& target, const Vector3& translate)
	{
		D3DXMatrixTranslation(&target, translate.x, translate.y, translate.z);
	}

	void FillDXMatrixRotateXYZ( D3DXMATRIX& target, const Vector3& rotate)
	{
		D3DXMATRIX rotateX, rotateY, rotateZ;
		D3DXMatrixRotationX(&rotateX, rotate.x);
		D3DXMatrixRotationY(&rotateY, rotate.y);
		D3DXMatrixRotationZ(&rotateZ, rotate.z);
		target = rotateX * rotateY * rotateZ;
	}

	void FillDXMatrixScale( D3DXMATRIX& target, const Vector3& scale)
	{
		D3DXMatrixScaling(&target, scale.x, scale.y, scale.z);
	}
}