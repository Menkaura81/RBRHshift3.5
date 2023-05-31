#include <dinput.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define SAMPLE_BUFFER_SIZE 16

LPDIRECTINPUT8       g_pDI       = NULL;        
LPDIRECTINPUTDEVICE8 g_pKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pMouse	 = NULL;  
LPDIRECTINPUTDEVICE8 g_pJoystick = NULL; 
LPDIRECTINPUTDEVICE8 g_pJoystickNum = NULL; 

    BYTE diks[256];		  // DirectInput keyboard state buffer 
	DIMOUSESTATE2 dims2;      // DirectInput mouse state structure
	DIJOYSTATE2 js;           // DInput joystick state 
	int jindex = NULL;
	int iindex = NULL;
	int numj = NULL;
	char jname[255];

void FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pKeyboard ) 
        g_pKeyboard->Unacquire();
	if( g_pMouse ) 
        g_pMouse->Unacquire();
    if( g_pJoystick ) 
        g_pJoystick->Unacquire();

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pKeyboard );
    SAFE_RELEASE( g_pMouse );
	SAFE_RELEASE( g_pJoystick );
    SAFE_RELEASE( g_pDI );
	iindex = NULL;
}
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{

	if (iindex == jindex)
	{
		iindex = 0;
		g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );
		strcpy(jname,pdidInstance->tszProductName);
		return DIENUM_STOP;
	}
	else
	{
		++iindex;
		return DIENUM_CONTINUE;
	}

}

BOOL CALLBACK EnumJoysticksNumCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
	//HRESULT hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystickNum, NULL );
	//if (!FAILED(hr)) ++numj;
	++numj;
	return DIENUM_CONTINUE;

}

BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
        if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;
         
    }

    return DIENUM_CONTINUE;
}

void PollDevices()
{
    HRESULT hr;   

	//KEYBOARD
    if( g_pKeyboard ) 
	{
		ZeroMemory( diks, sizeof(diks) );
		hr = g_pKeyboard->GetDeviceState( sizeof(diks), diks );
		if( FAILED(hr) ) 
		{
			g_pKeyboard->Acquire();
		}
	}

	//MOUSE
    if( g_pMouse ) 
	{
		ZeroMemory( &dims2, sizeof(dims2) );
		hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

		if( FAILED(hr) ) 
			g_pMouse->Acquire();
	}

	//JOYSTICK
    if( g_pJoystick ) 
	{
		hr = g_pJoystick->Poll(); 
		if( FAILED(hr) )  
		{
			g_pJoystick->Acquire();

		}
		else
		{
			g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js );
		}
	}
}
bool KeyDown(DWORD KeyID)
{
	//Check keyboard
	if (KeyID < 256)
	{
		if( g_pKeyboard ) 
			if ( diks[KeyID] & 0x80 )
				return true;
	}
	//Check mouse
	if (KeyID > 256 & KeyID < 265)
	{
		if( g_pMouse ) 
			if ( dims2.rgbButtons[KeyID - 257] & 0x80 )
				return true;
	}
	//Check joystick
	if (KeyID > 264)
	{
		if( g_pJoystick ) 
			if ( js.rgbButtons[KeyID - 265] & 0x80 )
				return true;
	}
	//Check Joystick POVs
	if (KeyID > 296)
	{
		if( g_pJoystick ) 
		{
			switch (KeyID - 297)
			{
	
			case 0:
				{
					if (js.rgdwPOV[0] == 0)
					{
						return true;
					}
					break;
				}
			case 1:
				{
					if (js.rgdwPOV[0] > 4400 & js.rgdwPOV[0] < 4600)
					{
						return true;
					}
					break;
				}
			case 2:
				{
					if (js.rgdwPOV[0] > 8900 & js.rgdwPOV[0] < 9100)
					{
						return true;
					}
					break;
				}
			case 3:
				{
					
					if (js.rgdwPOV[0] > 13400 & js.rgdwPOV[0] < 13600)
					{
						return true;
					}
					break;
				}
			case 4:
				{
					if (js.rgdwPOV[0] > 17900 & js.rgdwPOV[0] < 18100)
					{
						return true;
					}
					break;
				}
			case 5:
				{
					if (js.rgdwPOV[0] > 22400 & js.rgdwPOV[0] < 22600)
					{
						return true;
					}
					break;
				}
			case 6:
				{
					if (js.rgdwPOV[0] > 26900 & js.rgdwPOV[0] < 27100)
					{
						return true;
					}
					break;
				}
			case 7:
				{
					if (js.rgdwPOV[0] > 31400 & js.rgdwPOV[0] < 31600)
					{
						return true;
					}
					break;
				}
			case 8:
				{
					if (js.rgdwPOV[0] > 35900 & js.rgdwPOV[0] < 36100)
					{
						return true;
					}
					break;
				}
			case 9:
				{
					if (js.rgdwPOV[1] == 0)
					{
						return true;
					}
					break;
				}
			case 10:
				{
					if (js.rgdwPOV[1] > 4400 & js.rgdwPOV[1] < 4600)
					{
						return true;
					}
					break;
				}
			case 11:
				{
					if (js.rgdwPOV[1] > 8900 & js.rgdwPOV[1] < 9100)
					{
						return true;
					}
					break;
				}
			case 12:
				{
					if (js.rgdwPOV[1] > 13400 & js.rgdwPOV[1] < 13600)
					{
						return true;
					}
					break;
				}
			case 13:
				{
					if (js.rgdwPOV[1] > 17900 & js.rgdwPOV[1] < 18100)
					{
						return true;
					}
					break;
				}
			case 14:
				{
					if (js.rgdwPOV[1] > 22400 & js.rgdwPOV[1] < 22600)
					{
						return true;
					}
					break;
				}
			case 15:
				{
					if (js.rgdwPOV[1] > 26900 & js.rgdwPOV[1] < 27100)
					{
						return true;
					}
					break;
				}
			case 16:
				{
					if (js.rgdwPOV[1] > 31400 & js.rgdwPOV[1] < 31600)
					{
						return true;
					}
					break;
				}
			case 17:
				{
					if (js.rgdwPOV[1] > 35900 & js.rgdwPOV[1] < 36100)
					{
						return true;
					}
					break;
				}
			case 18:
				{
					if (js.rgdwPOV[2] == 0)
					{
						return true;
					}
					break;
				}
			case 19:
				{
					if (js.rgdwPOV[2] > 4400 & js.rgdwPOV[2] < 4600)
					{
						return true;
					}
					break;
				}
			case 20:
				{
					if (js.rgdwPOV[2] > 8900 & js.rgdwPOV[2] < 9100)
					{
						return true;
					}
					break;
				}
			case 21:
				{
					if (js.rgdwPOV[2] > 13400 & js.rgdwPOV[2] < 13600)
					{
						return true;
					}
					break;
				}
			case 22:
				{
					if (js.rgdwPOV[2] > 17900 & js.rgdwPOV[2] < 18100)
					{
						return true;
					}
					break;
				}
			case 23:
				{
					if (js.rgdwPOV[2] > 22400 & js.rgdwPOV[2] < 22600)
					{
						return true;
					}
					break;
				}
			case 24:
				{
					if (js.rgdwPOV[2] > 26900 & js.rgdwPOV[2] < 27100)
					{
						return true;
					}
					break;
				}
			case 25:
				{
					if (js.rgdwPOV[2] > 31400 & js.rgdwPOV[2] < 31600)
					{
						return true;
					}
					break;
				}
			case 26:
				{
					if (js.rgdwPOV[2] > 35900 & js.rgdwPOV[2] < 36100)
					{
						return true;
					}
					break;
				}
			case 27:
				{
					if (js.rgdwPOV[3] == 0)
					{
						return true;
					}
					break;
				}
			case 28:
				{
					if (js.rgdwPOV[3] > 4400 & js.rgdwPOV[3] < 4600)
					{
						return true;
					}
					break;
				}
			case 29:
				{
					if (js.rgdwPOV[3] > 8900 & js.rgdwPOV[3] < 9100)
					{
						return true;
					}
					break;
				}
			case 30:
				{
					if (js.rgdwPOV[3] > 13400 & js.rgdwPOV[3] < 13600)
					{
						return true;
					}
					break;
				}
			case 31:
				{
					if (js.rgdwPOV[3] > 17900 & js.rgdwPOV[3] < 18100)
					{
						return true;
					}
					break;
				}
			case 32:
				{
					if (js.rgdwPOV[3] > 22400 & js.rgdwPOV[3] < 22600)
					{
						return true;
					}
					break;
				}
			case 33:
				{
					if (js.rgdwPOV[3] > 26900 & js.rgdwPOV[3] < 27100)
					{
						return true;
					}
					break;
				}
			case 34:
				{
					if (js.rgdwPOV[3] > 31400 & js.rgdwPOV[3] < 31600)
					{
						return true;
					}
					break;
				}
			case 35:
				{
					if (js.rgdwPOV[3] > 35900 & js.rgdwPOV[3] < 36100)
					{
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
	//max 296
}

HRESULT InitDirectInput( HWND hDlg, int joyindex )
{
    HRESULT hr; 
  
    FreeDirectInput();


    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;

	//Create Keyboard
    g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL );
	if( g_pKeyboard )
	{
		g_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
		g_pKeyboard->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND );
		g_pKeyboard->Acquire();
	}

	//Create Mouse
    g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL );
	if( g_pMouse )
	{
		g_pMouse->SetDataFormat( &c_dfDIMouse2 );
		g_pMouse->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND );
		g_pMouse->Acquire();
	}

	//Create Joystick
	numj = 0;
	g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksNumCallback, NULL, DIEDFL_ATTACHEDONLY );
	jindex = joyindex;
	g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY );
    if( g_pJoystick )
	{
		g_pJoystick->SetDataFormat( &c_dfDIJoystick2 );
		g_pJoystick->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND );
		g_pJoystick->EnumObjects( EnumObjectsCallback, (VOID*)hDlg, DIDFT_ALL );
		g_pJoystick->Acquire();
	}

	return S_OK;

}

void SwitchJoystick(HWND hDlg, int jid)
{
	if (!g_pDI) return;
	if (g_pJoystick) g_pJoystick->Unacquire();
	jindex = jid;
	
	g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY );
    if( g_pJoystick )
	{
		g_pJoystick->SetDataFormat( &c_dfDIJoystick2 );
		g_pJoystick->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND );
		g_pJoystick->EnumObjects( EnumObjectsCallback, (VOID*)hDlg, DIDFT_ALL );
		g_pJoystick->Acquire();
	}
}