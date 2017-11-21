#include "device.h"
#include "dx9/interfaces.h"
#include "common/delete.h"

namespace dx9 
{

	bool deviceIsNullReference = false;

	void CreateNULLRefDevice()
	{
		CreateNULLRefDevice(GetConsoleWindow());	
	}

	void CreateNULLRefDevice(HWND hwnd)
	{
		if(device)	return;

		HRESULT hr;
		d3d = Direct3DCreate9( D3D_SDK_VERSION );
		ENFORCE(d3d);

		D3DDISPLAYMODE Mode;
		d3d->GetAdapterDisplayMode( 0, &Mode );

		D3DPRESENT_PARAMETERS pp;
		ZeroMemory( &pp, sizeof( D3DPRESENT_PARAMETERS ) );
		pp.BackBufferWidth = 1;
		pp.BackBufferHeight = 1;
		pp.BackBufferFormat = Mode.Format;
		pp.BackBufferCount = 1;
		pp.SwapEffect = D3DSWAPEFFECT_COPY;
		pp.Windowed = TRUE;

		hr = d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &pp, &device);
		ENFORCE(device);
	}

	void ReleaseD3D()
	{
		SAFE_RELEASE(device);
		SAFE_RELEASE(d3d);
	}
} 