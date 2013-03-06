#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

LPCSTR g_mirrorDeviceString = "sandia mirror driver";
LPCSTR g_displayDriverName = "sandia";

//LPCSTR g_mirrorDeviceString = "Microsoft Mirror Driver";
//LPCSTR g_displayDriverName = "mirror";

LPSTR dispCode[7] = {
	"change successful",
	"must restart",
	"bad flags",
	"bad parameters",
	"failed",
	"bad mode",
	"not updated"
};

LPSTR getDispCode(INT code)
{
	switch (code)
	{
	case DISP_CHANGE_SUCCESSFUL:
		return dispCode[0];
	case DISP_CHANGE_RESTART:
		return dispCode[1];
	case DISP_CHANGE_BADFLAGS:
		return dispCode[2];
	case DISP_CHANGE_BADPARAM:
		return dispCode[3];
	case DISP_CHANGE_FAILED:
		return dispCode[4];
	case DISP_CHANGE_BADMODE:
		return dispCode[5];
	case DISP_CHANGE_NOTUPDATED:
		return dispCode[6];
	default:
		{
			static CHAR tmp[MAX_PATH];
			sprintf_s(tmp, MAX_PATH, "unknown code: %08x\n", code);
			return tmp;
		}
	}
}

BOOL getPrimaryDeviceSetting(DWORD& pelsWidth, DWORD& pelsHeight)
{
	DEVMODEA devMode = {0};
	devMode.dmSize = sizeof(devMode);
	devMode.dmDriverExtra = 0;

	DISPLAY_DEVICEA dispDevice = {0};
	dispDevice.cb = sizeof(dispDevice);

	INT devNum = 0;
	printf("get primary device setting.\n");
	while (EnumDisplayDevicesA(NULL, devNum, &dispDevice, 0))
	{
		printf("device name: '%s' ", dispDevice.DeviceName);

		if (dispDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
		{
			EnumDisplaySettingsA(dispDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);
			pelsWidth = devMode.dmPelsWidth;
			pelsHeight = devMode.dmPelsHeight;

			printf("primary.\n");

			return TRUE;
		}

		printf("\n");
	}
	printf("get primary device setting done.\n");

	return FALSE;
}

BOOL getMirrorDeviceName(LPCSTR devString, LPSTR devName)
{
	DISPLAY_DEVICEA dispDevice = {0};
	dispDevice.cb = sizeof(dispDevice);

	INT devNum = 0;
	printf("get mirror device.\n");
	while (EnumDisplayDevicesA(NULL, devNum, &dispDevice, 0))
	{
		printf("device name: '%s' ", dispDevice.DeviceString);

		if (strcmp(dispDevice.DeviceString, devString) == 0)
		{
			printf("match.\n");
			strcpy_s(devName, 32, dispDevice.DeviceName);
			return TRUE;
		}

		printf("\n");

		devNum++;
	}
	printf("get mirror device done.\n");

	return FALSE;
}

VOID startMirrorDriver()
{
	DEVMODEA devMode = {0};
	devMode.dmSize = sizeof(devMode);
	devMode.dmDriverExtra = 0;
	devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH |
		DM_PELSHEIGHT | DM_POSITION;
	if (!getPrimaryDeviceSetting(devMode.dmPelsWidth, devMode.dmPelsHeight))
	{
		return;
	}

	CHAR devName[32];
	if (!getMirrorDeviceName(g_mirrorDeviceString, devName))
	{
		return;
	}

	sprintf_s((LPSTR)devMode.dmDeviceName, sizeof(devMode.dmDeviceName), g_mirrorDeviceString);

	INT code = ChangeDisplaySettingsExA(devName, &devMode,
		NULL, CDS_UPDATEREGISTRY | CDS_NORESET, NULL);
	printf("update registry setting: %s.\n", getDispCode(code));

	code = ChangeDisplaySettingsExA(NULL, NULL, NULL, 0, NULL);
	printf("make registry setting work: %s.\n", getDispCode(code));
}

VOID stopMirrorDriver()
{
	DEVMODEA devMode = {0};
	devMode.dmSize = sizeof(devMode);
	devMode.dmDriverExtra = 0;
	devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH |
		DM_PELSHEIGHT | DM_POSITION;

	CHAR devName[32];
	if (!getMirrorDeviceName(g_mirrorDeviceString, devName))
	{
		return;
	}

	sprintf_s((LPSTR)devMode.dmDeviceName, sizeof(devMode.dmDeviceName), g_mirrorDeviceString);

	INT code = ChangeDisplaySettingsExA(devName, &devMode,
		NULL, CDS_UPDATEREGISTRY | CDS_NORESET, NULL);
	printf("update registry setting: %s.\n", getDispCode(code));

	code = ChangeDisplaySettingsExA(NULL, NULL, NULL, 0, NULL);
	printf("make registry setting work: %s.\n", getDispCode(code));
}

VOID enumMirrorDriver()
{
	DISPLAY_DEVICEA dispDevice = {0};
	dispDevice.cb = sizeof(dispDevice);

	INT devNum = 0;
	printf("enum display device.\n");
	while (EnumDisplayDevicesA(NULL, devNum, &dispDevice, 0))
	{
		printf("[%d] device name: '%s' \n", devNum, dispDevice.DeviceString);

		devNum++;
	}
	printf("enum display device done.\n");
}

INT __cdecl main(INT argc, CHAR* argv[])
{
	MessageBox(0,0,0,0);

	for (INT i = 1; i < argc; i++)
	{
		if (_stricmp(argv[i], "start") == 0)
		{
			startMirrorDriver();
		}
		else if (_stricmp(argv[i], "stop") == 0)
		{
			stopMirrorDriver();
		}
		else if (_stricmp(argv[i], "enum") == 0)
		{
			enumMirrorDriver();
		};
	}

	return 0;
}
