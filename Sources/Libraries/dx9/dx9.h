#pragma once

#include "interfaces.h"
#include "viewports/library.include.h"

namespace dx9
{
	LPDIRECT3D9 DeviceCreate(D3DFORMAT fmt);
	LPDIRECT3DDEVICE9 DeviceInit(HWND hwnd, bool windowed, unsigned int adapterIdx, unsigned int modeIdx);
	void DeviceRelease();
	bool DeviceIsValid();

	class Viewport : public Viewports::Viewport
	{
	public:
		Viewport();

		virtual int width() const;
		virtual int height() const;
		virtual void set() const;
		virtual int deviceIndex() const;
	};
}

