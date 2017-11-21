#pragma once

#include <d3d9.h>

#include "common/delete.h"

namespace dx9 
{
	void CreateNULLRefDevice();
	void CreateNULLRefDevice(HWND hwnd);
	void ReleaseD3D();
}

