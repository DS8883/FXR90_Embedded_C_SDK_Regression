#include "ManualTests.h"

extern struct RFID3DATA api3Data;
struct packageInfo
{
	CHAR packagename[128];
	CHAR packagePath[512];
};

void InstallUserAppA(RFID_HANDLE32 readerHandle, CHAR* path)
{
	api3Data.rfidStatus = RFID_InstallUserAppA(readerHandle, path);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n RFID_InstallUserApp: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
	}
}

void UnInstallUserAppA(RFID_HANDLE32 readerHandle, CHAR* appname)
{
	api3Data.rfidStatus = RFID_UninstallUserAppA(readerHandle, appname);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n RFID_UninstallUserApp: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
	}

}
void ListuserAppsA(RFID_HANDLE32 readerHandle)
{
	/*USERAPP_LISTA appList;
	memset(&appList, 0, sizeof(appList));
	api3Data.rfidStatus = RFID_ListUserAppsA(readerHandle, &appList);
	wprintf(L"\n App Count %ld", appList.numMaxApps);
	for (int appCount = 1; appCount < appList.numMaxApps + 1; appCount++)
	{
		wprintf(L"\n app %ld.%ls\trunStatus:%ld\tautoStart:%ld", appCount, appList.pUserAppData->pAppName, appList.pUserAppData->runStatus, appList.pUserAppData->autoStart);
		appList.pUserAppData++;
	}*/
}

void StartorStopUserAppA(RFID_HANDLE32 readerHandle, CHAR* appname, BOOLEAN action)
{
	if (action == FALSE)
	{
		//stop
		api3Data.rfidStatus = RFID_StopUserAppA(readerHandle, appname);
		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			DisplayLastErrorInfo();
		}
		else
		{
			wprintf(L"\n RFID_StopUserApp: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		}
	}
	else
	{
		api3Data.rfidStatus = RFID_StartUserAppA(readerHandle, appname);
		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			DisplayLastErrorInfo();
		}
		else
		{
			wprintf(L"\n RFID_StartUserApp: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		}
	}
}
void ShowUserAppRunStatusA(RFID_HANDLE32 readerHandle, CHAR* appname)
{
	BOOLEAN bStatus;
	api3Data.rfidStatus = RFID_GetUserAppRunStatusA(readerHandle, appname, &bStatus);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n user app: %ls \t is %ls", appname, bStatus ? L"Running" : L"Stopped");
	}
}

void setUserappAutoStartA(RFID_HANDLE32 readerHandle, CHAR* appname, BOOLEAN status)
{
	api3Data.rfidStatus = RFID_SetUserAppAutoStartA(readerHandle, appname, status);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n RFID_SetUserAppAutoStart: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
	}
}

void InstallUSerAppTestA(RFID_VERSION rfidVersion, BOOLEAN bMode, int iterCount)
{
	LOGIN_INFO loginfo;
	int totalAppCount = 10;

	packageInfo myPackageList[10] = {
		{ "package-1", "./package-1_3.6_all.deb" },
		{ "package-2", "./package-2_1.0_all.deb" },
		{ "package-3", "./package-3_1.0_all.deb" },
		{ "package-4", "./package-4_1.0_all.deb" },
		{ "package-5", "./package-5_1.0_all.deb" },
		{ "package-6", "./package-6_1.0_all.deb" },
		{ "package-7", "./package-7_1.0_all.deb" },
		{ "package-8", "./package-8_1.0_all.deb" },
		{ "package-9", "./package-9_1.0_all.deb" },
		{ "package-10","./package-10_1.0_all.deb" }
	};


	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;

	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, bMode, 0);

	// repeat the test based on the number of iterations as per the use request
	//
	for (int i = 0; i<iterCount; i++)
	{
		wprintf(L"\n Listing User Apps : \t Iteration Count %ld\n", i);
		ListuserAppsA(api3Data.hReaderMgmt);
		for (int appCount = 0; appCount < totalAppCount; appCount++)
		{
			ListuserAppsA(api3Data.hReaderMgmt);
			InstallUserAppA(api3Data.hReaderMgmt, myPackageList[appCount].packagePath);
			setUserappAutoStartA(api3Data.hReaderMgmt, myPackageList[appCount].packagename, TRUE);
			StartorStopUserAppA(api3Data.hReaderMgmt, myPackageList[appCount].packagename, TRUE);
			ListuserAppsA(api3Data.hReaderMgmt);
			ShowUserAppRunStatusA(api3Data.hReaderMgmt, myPackageList[appCount].packagename);
			setUserappAutoStartA(api3Data.hReaderMgmt, myPackageList[appCount].packagename, FALSE);
			StartorStopUserAppA(api3Data.hReaderMgmt, myPackageList[appCount].packagename, FALSE);
			ShowUserAppRunStatusA(api3Data.hReaderMgmt, myPackageList[appCount].packagename);
			ListuserAppsA(api3Data.hReaderMgmt);
		}

		for (int appCount = 0; appCount < totalAppCount; appCount++)
		{
			UnInstallUserAppA(api3Data.hReaderMgmt, myPackageList[appCount].packagename);
			//ListuserApps( api3Data.hReaderMgmt);
		}
	}
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
}

