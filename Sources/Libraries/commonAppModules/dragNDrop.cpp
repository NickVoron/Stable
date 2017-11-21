#include "dragNDrop.h"

#include "dragNdrop/library.include.h"


namespace AppModules
{

LRESULT DragNDrop::DragNDropWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DROPFILES: 
		{
			::DragNDrop::FileList files;

			//LOG_ERROR("files dropped");
			HDROP hdrop = (HDROP)wParam;
			UINT count = DragQueryFile(hdrop, 0xFFFFFFFF, 0, 0);
			//LOG_ERROR("dropped files count: " << count);
			for (UINT i = 0; i < count; ++i)
			{
				UINT size = DragQueryFile(hdrop, i, 0, 0);
				ENFORCE(size > 0);
				TCHAR* buffer = new TCHAR[size + 1];
				UINT result_size = DragQueryFile(hdrop, i, buffer, size + 1);
				ENFORCE(result_size > 0);

				files.push_back( std::wstring(buffer) );

				delete [] buffer;
			}

			::DragNDrop::Dispatcher::process(files);
		}
	}
	return 0;
}

void DragNDrop::create(Window::Params& windowParams)
{
	windowParams.windowProcs.add(&DragNDropWndProc);
}


}