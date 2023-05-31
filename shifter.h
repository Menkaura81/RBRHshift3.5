#include "stdio.h"
#include "stdlib.h"
#include "Windowsx.h"
#include "Mmsystem.h"
#include "Winuser.h"
#include "ini.h"
#include "di.h"

#pragma comment(lib, "winmm.lib")  // Needed to compile with Visual Studio 2022. Added by manteka

bool didinit = false;
bool running = true;
bool listening = false;
int listenid = NULL;
bool hbutton = false;
bool dhbutton = false;
bool onoff = false;
long carptr = NULL;
float clutch = 0;
bool clutchon = false;
bool rclutch = true;
int bitepoint = 65; // How much % cluth need to be pressed for gears to change. Added by manteka 
int cindex = 0;
int curgear;
int lastgear;

HWND hWin = NULL;
DWORD PID = NULL;

int Keys[10];
char tmpStr[255];
char tmpStr2[255];

HWND GetWindowHandle(DWORD tPID)
{
	//Get first window handle
	HWND res = FindWindow(NULL,NULL);
	DWORD mPID = NULL;
	while(res != 0)
	{
		if(!GetParent(res))
		{
			GetWindowThreadProcessId(res,&mPID);
			if (mPID == tPID)
				return res;
		}
		res = GetWindow(res, GW_HWNDNEXT);
	}


}

void Load()
{
	char	*pStr, strPath[255];
	GetModuleFileName (NULL, strPath, 255);
	pStr = strrchr(strPath, '\\');
	if (pStr != NULL)
	    *(++pStr)='\0'; 

    sprintf(strMySystemFile,"%s%s",strPath, "\\plugins\\shifter.ini");
	if (!ReadFromFile("MAIN", "ControlIndex") == 0) cindex = atoi(ReadFromFile("MAIN", "ControlIndex"));
	if (!ReadFromFile("MAIN", "Enabled") == 0) onoff = atoi(ReadFromFile("MAIN", "Enabled"));
	if (!ReadFromFile("MAIN", "AutoNeutral") == 0) hbutton = atoi(ReadFromFile("MAIN", "AutoNeutral"));
	if (!ReadFromFile("MAIN", "RequireClutch") == 0) rclutch = atoi(ReadFromFile("MAIN", "RequireClutch"));
	if (!ReadFromFile("MAIN", "Bite point") == 0) bitepoint = atoi(ReadFromFile("MAIN", "Bite point"));  // Added by manteka

	for (int i = 0; i < 7 ; i++ )
	{
		
		sprintf(tmpStr, "Key%d", i);
		if (!ReadFromFile("MAIN",tmpStr) == 0) Keys[i] = strtoul(ReadFromFile("MAIN",tmpStr),(char**) NULL, 16);
	}

}

void Save()
{
	char	*pStr, strPath[255];
	GetModuleFileName (NULL, strPath, 255);
	pStr = strrchr(strPath, '\\');
	if (pStr != NULL)
	    *(++pStr)='\0'; 

    sprintf(strMySystemFile,"%s%s",strPath, "\\plugins\\shifter.ini");
	itoa(cindex, tmpStr2,10);
	WriteToFile("MAIN", "ControlIndex", tmpStr2);
	itoa(onoff, tmpStr2,10);
	WriteToFile("MAIN", "Enabled", tmpStr2);
	itoa(hbutton, tmpStr2,10);
	WriteToFile("MAIN", "AutoNeutral", tmpStr2);
	itoa(rclutch, tmpStr2,10);
	WriteToFile("MAIN", "RequireClutch", tmpStr2);
	itoa(bitepoint, tmpStr2, 10);
	WriteToFile("MAIN", "Bite point", tmpStr2);  // Added by manteka
	for (int i = 0; i < 7 ; i++ )
	{
		sprintf(tmpStr, "Key%d", i);
		itoa(Keys[i], tmpStr2,16);
		WriteToFile("MAIN",tmpStr, tmpStr2);
	}
	


}

void Init()
{
	Load();
	InitDirectInput(hWin, cindex);
	didinit = true;
}


DWORD WINAPI ShifterMain( LPVOID lpParam )
{
	while (hWin == 0)
		hWin = GetWindowHandle(GetCurrentProcessId());


	timeBeginPeriod(1);    
	while (running == true)
	{
		if (!didinit) Init();
		PollDevices();

		carptr = *(long*)0x8EF660;
		if (onoff)
		{
		if (carptr)
		{
			clutch = *(float*)(carptr+0x86c);

			if(clutch>(float(bitepoint)/100))clutchon = true; else clutchon = false;  // Bitepoint use. Added by manteka

			if (rclutch == false) clutchon = true;

			if (*(long*)0x165F10F)
			{

				if (KeyDown(Keys[0]))
				{
					curgear = 2;
					if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 2;
					lastgear = 2;
				}
				else
				{
					if (KeyDown(Keys[1]))
					{
						curgear = 3;
						if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 3;
						lastgear = 3;
					}
					else
					{
						if (KeyDown(Keys[2]))
						{
							curgear = 4;
							if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 4;
							lastgear = 4;
						}
						else
						{
							if (KeyDown(Keys[3]))
							{
								curgear = 5;
								if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 5;
								lastgear = 5;
							}
							else
							{
								if (KeyDown(Keys[4]))
								{
									curgear = 6;
									if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 6;
									lastgear = 6;
								}
								else
								{
									if (KeyDown(Keys[5]))
									{
										curgear = 7;
										if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 7;
										lastgear = 7;
									}
									else
									{
										if (KeyDown(Keys[6]))
										{
											curgear = 0;
											if (curgear != lastgear && clutchon) *(long*)(carptr+0x1100) = 0;
											lastgear = 0;
										}
										else
										{

											if (hbutton)
											{
												curgear = 1;
												if (curgear != lastgear) *(long*)(carptr+0x1100) = 1;
												lastgear = 1;
											}

										}
									}
								}
							}
						}
					}
				}
				}
			}
		}
		
		Sleep(1);
	}

	
	return 1;
}
DWORD WINAPI Main2( LPVOID lpParam )
{



	return 1;
}
void StartShifter()
{


	DWORD dwThreadId, dwThrdParam = -1; 
	HANDLE hThread; 

	hThread = CreateThread( 
		NULL,                        // no security attributes 
		0,                           // use default stack size  
		ShifterMain,                  // thread function 
		&dwThrdParam,                // argument to thread function 
		0,                           // use default creation flags 
		&dwThreadId);                // returns the thread identifier 
		
}