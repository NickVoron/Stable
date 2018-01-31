// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	
	BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
	{
		DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
		HRESULT hr;

		if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
			return DIENUM_CONTINUE;

		
		
		if( pEnumContext->bPreferredJoyCfgValid &&  IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
			return DIENUM_CONTINUE;

		
		hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );	 		
		
		if( FAILED( hr ) )
			return DIENUM_CONTINUE;

		
		
		return DIENUM_STOP;
	}

	
	HRESULT InitDirectInput()
	{
		

		HRESULT hr;

		
		
		
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
		if( SUCCEEDED( pJoyConfig->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) 
			enumContext.bPreferredJoyCfgValid = false;
		SAFE_RELEASE( pJoyConfig );

		
		if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &enumContext, DIEDFL_ATTACHEDONLY ) ) )
			return hr;

		if( g_bFilterOutXinputDevices )
			CleanupForIsXInputDevice();

		
		if( NULL == g_pJoystick )
		{
			return S_OK;
		}

		LOG_MSG("Joystick found.");

		
		
		
		
		
		if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
			return hr;

		
		



		
		
		
		if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback, 0, DIDFT_ALL ) ) )
			return hr;

		return S_OK;
	}

	
	
	
	
	
	
	
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

		
		hr = CoInitialize( NULL );
		bCleanupCOM = SUCCEEDED( hr );

		
		hr = CoCreateInstance( __uuidof( WbemLocator ),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof( IWbemLocator ),
			( LPVOID* )&pIWbemLocator );
		if( FAILED( hr ) || pIWbemLocator == NULL )
			goto LCleanup;

		
		bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
		bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
		bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

		
		hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 0L, NULL, NULL, &pIWbemServices );
		if( FAILED( hr ) || pIWbemServices == NULL )
			goto LCleanup;

		
		CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

		
		hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
		if( FAILED( hr ) || pEnumDevices == NULL )
			goto LCleanup;

		
		for(; ; )
		{
			
			hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
			if( FAILED( hr ) )
				goto LCleanup;
			if( uReturned == 0 )
				break;

			for( iDevice = 0; iDevice < uReturned; iDevice++ )
			{
				
				hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
				if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
				{
					
					
					if( wcsstr( var.bstrVal, L"IG_" ) )
					{
						
						DWORD dwPid = 0, dwVid = 0;
						WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
						if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
							dwVid = 0;
						WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
						if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
							dwPid = 0;

						DWORD dwVidPid = MAKELONG( dwVid, dwPid );

						
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

	
	HRESULT UpdateInputState(WinInput::DirectInput2Buffer& buffer)
	{
		HRESULT hr;
		TCHAR strText[512] = {0}; 
		DIJOYSTATE2 js;           

		if( NULL == g_pJoystick )
			return S_OK;

		
		hr = g_pJoystick->Poll();
		if( FAILED( hr ) )
		{
			
			
			
			
			hr = g_pJoystick->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = g_pJoystick->Acquire();

			
			
			
			return S_OK;
		}

		
		if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) )
			return hr; 

		

		
		
		
		

		
		
		

		
		
		
		
		
		
		

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

	
	BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
	{
		HWND hDlg = ( HWND )pContext;

		static int nSliderCount = 0;  
		static int nPOVCount = 0;     

		
		
		if( pdidoi->dwType & DIDFT_AXIS )
		{
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof( DIPROPRANGE );
			diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType; 
			diprg.lMin = -1000;
			diprg.lMax = +1000;

			
			if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
				return DIENUM_STOP;

		}

		return DIENUM_CONTINUE;
	}

	
	bool IsXInputDevice( const GUID* pGuidProductFromDirectInput )
	{
		
		XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
		while( pNode )
		{
			if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
				return true;
			pNode = pNode->pNext;
		}

		return false;
	}


	
	void CleanupForIsXInputDevice()
	{
		
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



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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