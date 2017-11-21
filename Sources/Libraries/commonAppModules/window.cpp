#include "window.h"

#include "dragNdrop/library.include.h"

namespace AppModules
{

Window::Params::Params()
{
	needExit = false;
	windowsParams.add(L"default");
}

void Window::process_impl()
{
	while(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::release_impl(Params& params)
{
	params.windowProcs.reset();
}

}