// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

namespace dx9
{
	template<D3DFORMAT format>
	struct CubemapRenderTarget
	{
		LPDIRECT3DCUBETEXTURE9 texture;

		void init(int size)
		{
			dx9::device->CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &texture, NULL);
		}

		void clear()
		{
			SAFE_RELEASE(texture);
		}
	};

	struct CubemapTransform
	{
		enum FACE
		{
			CUBE_POS_X = D3DCUBEMAP_FACE_POSITIVE_X,
			CUBE_NEG_X = D3DCUBEMAP_FACE_NEGATIVE_X,
			CUBE_POS_Y = D3DCUBEMAP_FACE_POSITIVE_Y,
			CUBE_NEG_Y = D3DCUBEMAP_FACE_NEGATIVE_Y,
			CUBE_POS_Z = D3DCUBEMAP_FACE_POSITIVE_Z,
			CUBE_NEG_Z = D3DCUBEMAP_FACE_NEGATIVE_Z,
		};

		struct Matrices
		{
			D3DXMATRIX projection;
			D3DXMATRIX view[6];
		};

		template<FACE faceIndex>
		static void getLookAtAndUpVectors(Vector3& lookAt, Vector3& up);

		template<> static void getLookAtAndUpVectors<CUBE_POS_X>(Vector3& lookAt, Vector3& up) { lookAt = Vector3( 1.0f, 0.0f, 0.0f); up = Vector3(0.0f, 1.0f, 0.0f); }
		template<> static void getLookAtAndUpVectors<CUBE_NEG_X>(Vector3& lookAt, Vector3& up) { lookAt = Vector3(-1.0f, 0.0f, 0.0f); up = Vector3(0.0f, 1.0f, 0.0f); }
		template<> static void getLookAtAndUpVectors<CUBE_POS_Y>(Vector3& lookAt, Vector3& up) { lookAt = Vector3( 0.0f, 1.0f, 0.0f); up = Vector3(0.0f, 0.0f,-1.0f); }
		template<> static void getLookAtAndUpVectors<CUBE_NEG_Y>(Vector3& lookAt, Vector3& up) { lookAt = Vector3( 0.0f,-1.0f, 0.0f); up = Vector3(0.0f, 0.0f, 1.0f); }
		template<> static void getLookAtAndUpVectors<CUBE_POS_Z>(Vector3& lookAt, Vector3& up) { lookAt = Vector3( 0.0f, 0.0f, 1.0f); up = Vector3(0.0f, 1.0f, 0.0f); }
		template<> static void getLookAtAndUpVectors<CUBE_NEG_Z>(Vector3& lookAt, Vector3& up) { lookAt = Vector3( 0.0f, 0.0f,-1.0f); up = Vector3(0.0f, 1.0f, 0.0f); }

		template<FACE faceIndex>
		static void getViewMatrix(const Vector3& pos, D3DXMATRIX& view)
		{
			Vector3 lookAt, up;
			getLookAtAndUpVectors<faceIndex>(lookAt, up);

			Vector3 dir = pos + lookAt;
			D3DXMatrixLookAtLH( &view, (D3DXVECTOR3*)&pos, (D3DXVECTOR3*)&dir, (D3DXVECTOR3*)&up );
		}

		static void getProjMatrix(D3DXMATRIX& projection)
		{
			static D3DXMATRIX proj;
			static bool inited = false;
			if(!inited)
			{
				D3DXMatrixPerspectiveFovLH(&proj, nm::PI_2, 1.0f, 0.1f, 512.0f);
				inited = true;
			}

			projection = proj;
		}

		static void getMatrices(const Vector3& pos, Matrices& mtx)
		{
			getProjMatrix(mtx.projection);
			getViewMatrix<CUBE_POS_X>(pos, mtx.view[CUBE_POS_X]);
			getViewMatrix<CUBE_NEG_X>(pos, mtx.view[CUBE_NEG_X]);
			getViewMatrix<CUBE_POS_Y>(pos, mtx.view[CUBE_POS_Y]);
			getViewMatrix<CUBE_NEG_Y>(pos, mtx.view[CUBE_NEG_Y]);
			getViewMatrix<CUBE_POS_Z>(pos, mtx.view[CUBE_POS_Z]);
			getViewMatrix<CUBE_NEG_Z>(pos, mtx.view[CUBE_NEG_Z]);
		}
	};

}



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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