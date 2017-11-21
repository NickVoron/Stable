#ifdef WIN32

#define DIRECTINPUT_VERSION 0x0800
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include "common/delete.h"
#include "directInput.h"

#include <wbemidl.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dinput.h>
#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>


namespace Input
{
	BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	HRESULT SetupForIsXInputDevice();
	bool IsXInputDevice( const GUID* pGuidProductFromDirectInput );
	void CleanupForIsXInputDevice();

	struct XINPUT_DEVICE_NODE
	{
		DWORD dwVidPid;
		XINPUT_DEVICE_NODE* pNext;
	};

	struct DI_ENUM_CONTEXT
	{
		DIJOYCONFIG* pPreferredJoyCfg;
		bool bPreferredJoyCfgValid;
	};

	bool                    g_bFilterOutXinputDevices = false;
	XINPUT_DEVICE_NODE*     g_pXInputDeviceList = NULL;
	LPDIRECTINPUT8          g_pDI = NULL;
	LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;

	// Called once for each enumerated joystick. If we find one, create a device interface on it so we can play with it.
	BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
	{
		DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
		HRESULT hr;

		if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
			return DIENUM_CONTINUE;

		// Skip anything other than the perferred joystick device as defined by the control panel.  
		// Instead you could store all the enumerated joysticks and let the user pick.
		if( pEnumContext->bPreferredJoyCfgValid &&  IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
			return DIENUM_CONTINUE;

		// Obtain an interface to the enumerated joystick.
		hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );	 		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if( FAILED( hr ) )
			return DIENUM_CONTINUE;

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;
	}

	// Initialize the DirectInput variables.
	HRESULT InitDirectInput()
	{
		//return S_FALSE;

		HRESULT hr;

		// Register with the DirectInput subsystem and get a pointer
		// to a IDirectInput interface we can use.
		// Create a DInput object
		if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )
			return hr;


		if( g_bFilterOutXinputDevices )
			SetupForIsXInputDevice();

		DIJOYCONFIG PreferredJoyCfg = {0};
		DI_ENUM_CONTEXT enumContext;
		enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
		enumContext.bPreferredJoyCfgValid = false;

		IDirectInputJoyConfig8* pJoyConfig = NULL;
		if( FAILED( hr = g_pDI->QueryInterface( IID_IDirectInputJoyConfig8, ( void** )&pJoyConfig ) ) )
			return hr;

		PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );
		if( SUCCEEDED( pJoyConfig->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
			enumContext.bPreferredJoyCfgValid = false;
		SAFE_RELEASE( pJoyConfig );

		// Look for a simple joystick we can use for this sample program.
		if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &enumContext, DIEDFL_ATTACHEDONLY ) ) )
			return hr;

		if( g_bFilterOutXinputDevices )
			CleanupForIsXInputDevice();

		// Make sure we got a joystick
		if( NULL == g_pJoystick )
		{
			return S_OK;
		}

		LOG_MSG("Joystick found.");

		// Set the data format to "simple joystick" - a predefined data format 
		//
		// A data format specifies which controls on a device we are interested in,
		// and how they should be reported. This tells DInput that we will be
		// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
		if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
			return hr;

		// Set the cooperative level to let DInput know how this device should
		// interact with the system and with other DInput applications.
// 		if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
// 			return hr;

		// Enumerate the joystick objects. The callback function enabled user
		// interface elements for objects that are found, and sets the min/max
		// values property for discovered axes.
		if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback, 0, DIDFT_ALL ) ) )
			return hr;

		return S_OK;
	}

	// Enum each PNP device using WMI and check each device ID to see if it contains 
	// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it’s an XInput device
	// Unfortunately this information can not be found by just using DirectInput.
	// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
	// XInput devices.
	// This function stores the list of xinput devices in a linked list 
	// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
	HRESULT SetupForIsXInputDevice()
	{
		IWbemServices* pIWbemServices = NULL;
		IEnumWbemClassObject* pEnumDevices = NULL;
		IWbemLocator* pIWbemLocator = NULL;
		IWbemClassObject* pDevices[20] = {0};
		BSTR bstrDeviceID = NULL;
		BSTR bstrClassName = NULL;
		BSTR bstrNamespace = NULL;
		DWORD uReturned = 0;
		bool bCleanupCOM = false;
		UINT iDevice = 0;
		VARIANT var;
		HRESULT hr;

		// CoInit if needed
		hr = CoInitialize( NULL );
		bCleanupCOM = SUCCEEDED( hr );

		// Create WMI
		hr = CoCreateInstance( __uuidof( WbemLocator ),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof( IWbemLocator ),
			( LPVOID* )&pIWbemLocator );
		if( FAILED( hr ) || pIWbemLocator == NULL )
			goto LCleanup;

		// Create BSTRs for WMI
		bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
		bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
		bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

		// Connect to WMI 
		hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 0L, NULL, NULL, &pIWbemServices );
		if( FAILED( hr ) || pIWbemServices == NULL )
			goto LCleanup;

		// Switch security level to IMPERSONATE
		CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

		// Get list of Win32_PNPEntity devices
		hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
		if( FAILED( hr ) || pEnumDevices == NULL )
			goto LCleanup;

		// Loop over all devices
		for(; ; )
		{
			// Get 20 at a time
			hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
			if( FAILED( hr ) )
				goto LCleanup;
			if( uReturned == 0 )
				break;

			for( iDevice = 0; iDevice < uReturned; iDevice++ )
			{
				// For each device, get its device ID
				hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
				if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
				{
					// Check if the device ID contains "IG_".  If it does, then it’s an XInput device
					// Unfortunately this information can not be found by just using DirectInput 
					if( wcsstr( var.bstrVal, L"IG_" ) )
					{
						// If it does, then get the VID/PID from var.bstrVal
						DWORD dwPid = 0, dwVid = 0;
						WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
						if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
							dwVid = 0;
						WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
						if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
							dwPid = 0;

						DWORD dwVidPid = MAKELONG( dwVid, dwPid );

						// Add the VID/PID to a linked list
						XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
						if( pNewNode )
						{
							pNewNode->dwVidPid = dwVidPid;
							pNewNode->pNext = g_pXInputDeviceList;
							g_pXInputDeviceList = pNewNode;
						}
					}
				}
				SAFE_RELEASE( pDevices[iDevice] );
			}
		}

LCleanup:
		if( bstrNamespace )
			SysFreeString( bstrNamespace );
		if( bstrDeviceID )
			SysFreeString( bstrDeviceID );
		if( bstrClassName )
			SysFreeString( bstrClassName );
		for( iDevice = 0; iDevice < 20; iDevice++ )
			SAFE_RELEASE( pDevices[iDevice] );
		SAFE_RELEASE( pEnumDevices );
		SAFE_RELEASE( pIWbemLocator );
		SAFE_RELEASE( pIWbemServices );

		return hr;
	}

	//Get the input device's state and display it.
	HRESULT UpdateInputState(WinInput::DirectInput2Buffer& buffer)
	{
		HRESULT hr;
		TCHAR strText[512] = {0}; // Device state text
		DIJOYSTATE2 js;           // DInput joystick state 

		if( NULL == g_pJoystick )
			return S_OK;

		// Poll the device to read the current state
		hr = g_pJoystick->Poll();
		if( FAILED( hr ) )
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = g_pJoystick->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = g_pJoystick->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			return S_OK;
		}

		// Get the input's device state
		if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) )
			return hr; // The device should have been acquired during the Poll()

		// Display joystick state to dialog

		// Axes
		//LOG_REPORT("axis_X" << js.lX);
		//LOG_REPORT("axis_Y" << js.lY);
		//LOG_REPORT("axis_Z" << js.lZ);

		//LOG_REPORT("rot_X" << js.lRx);
		//LOG_REPORT("rot_Y" << js.lRy);
		//LOG_REPORT("rot_Z" << js.lRz);

		//LOG_REPORT("slider_0" << js.rglSlider[0]);
		//LOG_REPORT("slider_1" << js.rglSlider[1]);
		//
		//LOG_REPORT("point_of_view_0" << js.rgdwPOV[0]);
		//LOG_REPORT("point_of_view_1" << js.rgdwPOV[1]);
		//LOG_REPORT("point_of_view_2" << js.rgdwPOV[2]);
		//LOG_REPORT("point_of_view_3" << js.rgdwPOV[3]);

		buffer.data[0][DIRECT_PAD2_X_POSITION] = (float)js.lX;
		buffer.data[0][DIRECT_PAD2_Y_POSITION] = (float)js.lY;
		buffer.data[0][DIRECT_PAD2_Z_POSITION] = (float)js.lZ;

		buffer.data[0][DIRECT_PAD2_X_ROTATION] = (float) js.lRx;
		buffer.data[0][DIRECT_PAD2_Y_ROTATION] = (float) js.lRy;
		buffer.data[0][DIRECT_PAD2_Z_ROTATION] = (float) js.lRz;

		buffer.data[0][DIRECT_PAD2_SLIDER0_POSITION] = (float) js.rglSlider[0];
		buffer.data[0][DIRECT_PAD2_SLIDER1_POSITION] = (float) js.rglSlider[1];
		
		buffer.data[0][DIRECT_PAD2_POINT_OF_VIEW0] = (float) js.rgdwPOV[0];
		buffer.data[0][DIRECT_PAD2_POINT_OF_VIEW1] = (float) js.rgdwPOV[1];
		buffer.data[0][DIRECT_PAD2_POINT_OF_VIEW2] = (float) js.rgdwPOV[2];
		buffer.data[0][DIRECT_PAD2_POINT_OF_VIEW3] = (float) js.rgdwPOV[3];
		

		// Fill up text with which buttons are pressed
		for( int i = 0; i < 128; i++ )
		{
			if(i == 12)	buffer.data[0][DIRECT_PAD2_BUTTON12] = 0.0f;
			if(i == 13)	buffer.data[0][DIRECT_PAD2_BUTTON13] = 0.0f;
			if( js.rgbButtons[i] & 0x80 )
			{
				LOG_REPORT("button " << i);

				if(i == 12)	buffer.data[0][DIRECT_PAD2_BUTTON12] = 1.0f;
				if(i == 13)	buffer.data[0][DIRECT_PAD2_BUTTON13] = 1.0f;
			}
		}

		return S_OK;
	}

	// Callback function for enumerating objects (axes, buttons, POVs) on a joystick. This function enables user interface elements for objects that are found to exist, and scales axes min/max values.
	BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
	{
		HWND hDlg = ( HWND )pContext;

		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if( pdidoi->dwType & DIDFT_AXIS )
		{
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof( DIPROPRANGE );
			diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin = -1000;
			diprg.lMax = +1000;

			// Set the range for the axis
			if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
				return DIENUM_STOP;

		}

		return DIENUM_CONTINUE;
	}

	// Returns true if the DirectInput device is also an XInput device. Call SetupForIsXInputDevice() before, and CleanupForIsXInputDevice() after
	bool IsXInputDevice( const GUID* pGuidProductFromDirectInput )
	{
		// Check each xinput device to see if this device's vid/pid matches
		XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
		while( pNode )
		{
			if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
				return true;
			pNode = pNode->pNext;
		}

		return false;
	}


	// Cleanup needed for IsXInputDevice()
	void CleanupForIsXInputDevice()
	{
		// Cleanup linked list
		XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
		while( pNode )
		{
			XINPUT_DEVICE_NODE* pDelete = pNode;
			pNode = pNode->pNext;
			SAFE_DELETE( pDelete );
		}
	}
}




#endif