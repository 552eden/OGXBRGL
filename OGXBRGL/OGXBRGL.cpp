//-----------------------------------------------------------------------------
//  Edens OgXbox@RgLoaderEnabler (OGXBRGL) - base for plugin: Sunrise 2.0
//
//  Devs:
//			552eden (UbErGaMeR_PiE)			
//			Byrom
// 
//  Credits:
//			Byrom for the xex plugin base, and other general help
//			Xbox 360 Hub #coding-corner - yall are the best
//			craftycodie - Halo hooks and address
//-----------------------------------------------------------------------------

#include <xtl.h>
#include "stdafx.h"
#include "CoreHooks.h"
#include "Utilities.h"



#define SET_PROT_OFF	2
#define SET_PROT_ON		3
BOOL _gProtection_On = false;
HANDLE hXam;
BOOL bRunContinuous = TRUE;
BOOL bLoopHasComplete = FALSE;
BOOL bIsDevkit; // Set on plugin load. Skips doing xnotify on devkits
DWORD LastTitleId;
const char* LastXex;


DWORD XboxEmu = 0xFFFE07D1;

HANDLE protoHandle = nullptr;

const char* OGXBRGLver = "0.0.1";

DWORD __declspec(naked) HvxSetState(DWORD mode)
{ 
	__asm {
		li      r0, 0x7B
		sc
		blr
	}
}

DWORD __declspec(naked) HvxGetVersions(DWORD key, DWORD mode)
{
        __asm
        {
                li r0, 0x0
                sc
                blr
        }
}



void ReloadXex(const char* xexPath) {
	const char* xboxXex = "xbox.xex";
	const char* xefuXex = "xefu.xex";
    // Unload the currently loaded XEX
    HANDLE hModule;
    NTSTATUS status = XexGetModuleHandle((PSZ)xexPath, &hModule);
    if (NT_SUCCESS(XexGetModuleHandle((PSZ)xboxXex, &hModule)) || NT_SUCCESS(XexGetModuleHandle((PSZ)xefuXex, &hModule) )) {
        XexUnloadImage(hModule);
    } else {
        Sunrise_Dbg("Failed to get handle for XEX: %s\n", xexPath);
        return;
    }

    // Load the XEX again
    status = XexLoadImage(xexPath, 0, 0, &hModule);
    if (!NT_SUCCESS(status)) {
        Sunrise_Dbg("Failed to load XEX: %s\n", xexPath);
        return;
    }

    Sunrise_Dbg("Successfully reloaded XEX: %s\n", xexPath);
}


char* xexname = "\\Device\\Harddisk0\\SystemPartition\\Compatibility\\xbox.xex";
char* xex2name = "\\Device\\Harddisk0\\SystemPartition\\Compatibility\\xefu.xex";

CONST CHAR* GetLoadedImageName() { return ExLoadedImageName; }


VOID Initialise()
{

	while (bRunContinuous)
	{
		if(!bIsDevkit)
		{

			Sunrise_Dbg("you should probably not be running this...");
			return;

		}
		
		//Sunrise_Dbg("\n current title id:  %x", XamGetCurrentTitleId());
		//Sunrise_Dbg("\n");
		//DbgPrint("%x", XamGetCurrentTitleId());
		DWORD TitleID = XamGetCurrentTitleId();
		//const char* currentXex = GetLoadedImageName();
		if (TitleID != LastTitleId)
		{
			LastTitleId = TitleID; // Set the last title id  to the current title id so we don't loop rechecking
			if (TitleID == XboxEmu) // XBOX EMU TEST
			{
				const char* loadedImageName = GetLoadedImageName();
				Sleep(50);
				Sunrise_Dbg("Loaded image name: ", loadedImageName);
					if(!_gProtection_On)
					{
						if(loadedImageName == nullptr)
						{
							Sunrise_Dbg("loaded image is nullptr");
						}

						else if(strcmp(xexname,loadedImageName) == 0 || strcmp(xex2name,loadedImageName) == 0 )
						{
							if(XexGetModuleHandle("Proto.xex", &protoHandle) == 0)
							{
								Sunrise_Dbg("\n\nsuccsfully got proto handle\n\n");
								((LDR_DATA_TABLE_ENTRY*)protoHandle)->LoadCount = 1;
								XexUnloadImage(protoHandle);
								ReloadXex(loadedImageName);
							}
							else
								Sunrise_Dbg("\n\ndidnt get proto handle wtf\n\n");
							Sleep(500);
							Sunrise_Dbg("finished sleeping after deload attempt");
							Sunrise_Dbg("\n\n unloaded proto!");
							Sunrise_Dbg("\n\n successfully deteced og xbox launch \n\n");
							Sunrise_Dbg("\n HvxGetVersions:  %x", HvxGetVersions(0x72627472, SET_PROT_ON));
							_gProtection_On = true;
							Sunrise_Dbg("\n\n successfully enabled mem protecc \n\n");
							bRunContinuous = false;
						}

						
					Sunrise_Dbg("\n Finished all actions, Mem protecc should be off and og xbox games should work now.\n");


				}
				Sunrise_Dbg("\n same XEX name. doing nothing.\n");
			}
		}
		Sleep(100); // Add a slight delay to the check loop
	}
	bLoopHasComplete = TRUE;
}



BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Sunrise_Dbg("OGXBRGL activated");
		if (IsTrayOpen())
		{
			Sunrise_Dbg("Plugin load aborted! Disc tray is open");
			HANDLE hSunrise = hModule;
			bLoopHasComplete = TRUE; // Make sure we set this so we don't get stuck in a sleep loop when we unload (DLL_PROCESS_DETACH) in a sec.
			*(WORD*)((DWORD)hSunrise + 64) = 1;
			return FALSE;
		}

		bIsDevkit = *(DWORD*)0x8E038610 & 0x8000 ? FALSE : TRUE; // Simple devkit check
		Sunrise_Dbg("v%s loaded! Running on %s kernel", OGXBRGLver, bIsDevkit ? "Devkit" : "Retail");
		if (bIsDevkit)
		{
			ThreadMe((LPTHREAD_START_ROUTINE)Initialise);
		}
		else
			XNotify(L"this should only be run on RGLoader");
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		bRunContinuous = FALSE; // Exit the continuous loop
		while (!bLoopHasComplete) // Wait for loop to complete
			Sleep(100);

		Sleep(500);
		Sunrise_Dbg("Unloaded!");
		break;

	}
	return TRUE;
}
