#include "ManualTests.h"

extern struct RFID3DATA api3Data;
struct packageInfo
{
	wchar_t packagename[128];
	wchar_t packagePath[512];
};

void InstallUserApp( RFID_HANDLE32 readerHandle,wchar_t* path)
{
	api3Data.rfidStatus = RFID_InstallUserApp( readerHandle,path);
	if( api3Data.rfidStatus !=RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n RFID_InstallUserApp: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	}
}

void UnInstallUserApp( RFID_HANDLE32 readerHandle,wchar_t* appname)
{
	api3Data.rfidStatus = RFID_UninstallUserApp( readerHandle,appname);
	if( api3Data.rfidStatus !=RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n RFID_UninstallUserApp: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	}

}
void ListuserApps( RFID_HANDLE32 readerHandle )
{
	USERAPP_LIST appList;
	memset(&appList,0,sizeof(appList));
	api3Data.rfidStatus = RFID_ListUserApps( readerHandle,&appList); 
	wprintf(L"\n App Count %ld", appList.numMaxApps);
	for(int appCount =1;appCount < appList.numMaxApps+1;appCount++)
	{
		wprintf(L"\n app %ld.%ls\trunStatus:%ld\tautoStart:%ld",appCount,appList.pUserAppData->pAppName,appList.pUserAppData->runStatus,appList.pUserAppData->autoStart);
		appList.pUserAppData++;
	}
}

void StartorStopUserApp( RFID_HANDLE32 readerHandle,wchar_t* appname,BOOLEAN action)
{
	if( action == FALSE )
	{
		//stop
		api3Data.rfidStatus = RFID_StopUserApp( readerHandle,appname);
		if( api3Data.rfidStatus !=RFID_API_SUCCESS)
		{
			DisplayLastErrorInfo();
		}
		else
		{
			wprintf(L"\n RFID_StopUserApp: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		}
	}
	else
	{
		api3Data.rfidStatus = RFID_StartUserApp( readerHandle,appname);
		if( api3Data.rfidStatus !=RFID_API_SUCCESS)
		{
			DisplayLastErrorInfo();
		}
		else
		{
			wprintf(L"\n RFID_StartUserApp: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		}
	}
}
void ShowUserAppRunStatus( RFID_HANDLE32 readerHandle,wchar_t* appname )
{
	BOOLEAN bStatus;
	api3Data.rfidStatus = RFID_GetUserAppRunStatus( readerHandle,appname,&bStatus);
	if( api3Data.rfidStatus !=RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n user app: %ls \t is %ls",appname,bStatus?L"Running":L"Stopped" );
	}
}

void setUserappAutoStart( RFID_HANDLE32 readerHandle,wchar_t* appname,BOOLEAN status )
{
	api3Data.rfidStatus = RFID_SetUserAppAutoStart( readerHandle,appname,status);
	if( api3Data.rfidStatus !=RFID_API_SUCCESS)
	{
		DisplayLastErrorInfo();
	}
	else
	{
		wprintf(L"\n RFID_SetUserAppAutoStart: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	}
}

void InstallUSerAppTest( RFID_VERSION rfidVersion,BOOLEAN bMode,int iterCount )
{
	LOGIN_INFO loginfo;
	int totalAppCount = 10;
#ifndef linux
	packageInfo myPackageList[10] = {
										{L"package-1",L"C:\\FX7500\\package-1_3.6_all.deb"},
										{L"package-2",L"C:\\FX7500\\package-2_1.0_all.deb"},
										{L"package-3",L"C:\\FX7500\\package-3_1.0_all.deb"},
										{L"package-4",L"C:\\FX7500\\package-4_1.0_all.deb"},
										{L"package-5",L"C:\\FX7500\\package-5_1.0_all.deb"},
										{L"package-6",L"C:\\FX7500\\package-6_1.0_all.deb"},
										{L"package-7",L"C:\\FX7500\\package-7_1.0_all.deb"},
										{L"package-8",L"C:\\FX7500\\package-8_1.0_all.deb"},
										{L"package-9",L"C:\\FX7500\\package-9_1.0_all.deb"},
										{L"package-10",L"C:\\FX7500\\package-10_1.0_all.deb"}
									};
#else
/*
	packageInfo myPackageList[10] = {
										{L"package-1",L"./package-1_3.6_all.deb"},
										{L"package-2",L"./package-2_1.0_all.deb"},
										{L"package-3",L"./package-3_1.0_all.deb"},
										{L"package-4",L"./package-4_1.0_all.deb"},
										{L"package-5",L"./package-5_1.0_all.deb"},
										{L"package-6",L"./package-6_1.0_all.deb"},
										{L"package-7",L"./package-7_1.0_all.deb"},
										{L"package-8",L"./package-8_1.0_all.deb"},
										{L"package-9",L"./package-9_1.0_all.deb"},
										{L"package-10",L"./package-10_1.0_all.deb"}
									};
									*/
	packageInfo myPackageList[1] = {
										{L"package-1",L"./RFIDSample4App_3.6.21.deb"}
										
									};
									
#endif

	wcscpy( loginfo.hostName,api3Data.szIPAddress);
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;

	api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,bMode,0 );
	
	// repeat the test based on the number of iterations as per the use request
	//
	for( int i=0;i<iterCount;i++)
	{
		wprintf(L"\n Listing User Apps : \t Iteration Count %ld\n",i);
		ListuserApps( api3Data.hReaderMgmt );
		for( int appCount =0;appCount < totalAppCount; appCount++)
		{
			ListuserApps( api3Data.hReaderMgmt );
			InstallUserApp( api3Data.hReaderMgmt, myPackageList[appCount].packagePath);
			setUserappAutoStart(api3Data.hReaderMgmt, myPackageList[appCount].packagename,TRUE);
			StartorStopUserApp( api3Data.hReaderMgmt,myPackageList[appCount].packagename,TRUE);
			ListuserApps( api3Data.hReaderMgmt);
			ShowUserAppRunStatus( api3Data.hReaderMgmt,myPackageList[appCount].packagename);
			setUserappAutoStart(api3Data.hReaderMgmt, myPackageList[appCount].packagename,FALSE);
			StartorStopUserApp( api3Data.hReaderMgmt,myPackageList[appCount].packagename,FALSE);
			ShowUserAppRunStatus( api3Data.hReaderMgmt,myPackageList[appCount].packagename);
			ListuserApps( api3Data.hReaderMgmt);
		}

		for( int appCount =0;appCount < totalAppCount; appCount++)
		{
			UnInstallUserApp( api3Data.hReaderMgmt,myPackageList[appCount].packagename);
			//ListuserApps( api3Data.hReaderMgmt);
		}
	}
	api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt);
}
void GetSetRadioPowerState( RFID_VERSION rfidVersion )
{
	BOOLEAN bPowerstate = 0;
	LOGIN_INFO loginfo;
	wcscpy( loginfo.hostName,api3Data.szIPAddress);
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	for( ;;)
	{
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
		
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
		api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
		INT32 timeout;
		api3Data.rfidStatus = RFID_TurnOffRadioWhenIdle( api3Data.hReaderMgmt,10 );
		//rfid3Sleep( 10000 );
		api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,FALSE);
		rfid3Sleep( 2000 );
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
		api3Data.rfidStatus		= RFID_PerformInventory(api3Data.hReader, NULL,NULL,NULL, 0);
		wprintf(L"\n RFID_PerformInventory - %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		rfid3Sleep( 4000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
		{
			printTagDataWithResults(pTagData);
		}
		RFID_DeallocateTag( api3Data.hReader, pTagData );
		api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt);
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	}
}
void TestRadioIdleTimeout( RFID_VERSION rfidVersion )
{
	BOOLEAN bPowerstate = 0;
	LOGIN_INFO loginfo;
	wcscpy( loginfo.hostName,api3Data.szIPAddress);
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	INT32* pTime = NULL;
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
	wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
	api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
	INT32 timeout =0;
	api3Data.rfidStatus = RFID_GetRadioIdleTimeout( api3Data.hReaderMgmt,&timeout);
	wprintf(L"\n idle time out - before setting ---> 10 %ld",timeout);
	api3Data.rfidStatus = RFID_TurnOffRadioWhenIdle( api3Data.hReaderMgmt,10 );
	api3Data.rfidStatus = RFID_GetRadioIdleTimeout( api3Data.hReaderMgmt,&timeout);
	wprintf(L"\n idle time out %ld",timeout);
	rfid3Sleep(11000);
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
	wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
	api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader, TRUE);
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
	wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
	api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader, FALSE);
	api3Data.rfidStatus = RFID_TurnOffRadioWhenIdle( api3Data.hReaderMgmt,0 );
	api3Data.rfidStatus = RFID_GetRadioIdleTimeout( api3Data.hReaderMgmt,&timeout);
	wprintf(L"\n idle time out %ld",timeout);
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
	wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
	api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader, TRUE);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
	wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	api3Data.rfidStatus		= RFID_PerformInventory(api3Data.hReader, NULL,NULL,NULL, 0);
	wprintf(L"\n RFID_PerformInventory: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	rfid3Sleep( 10000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	wprintf(L"\n RFID_StopInventory: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	rfid3Sleep(2000);
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		printTagDataWithResults(pTagData);
	}

	api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt);
	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}

void TestTransmitDelay(RFID_VERSION rfidVersion)
{
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);

	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);

	RADIO_TRANSMIT_DELAY_TYPE Transmittype;
	UINT8 time;
	api3Data.rfidStatus = RFID_GetRadioTransmitDelay(api3Data.hReader, &Transmittype, &time);
	wprintf(L"\n");
	wprintf(L"GetRadioTransmitDelay  Transmittype = %d\n", Transmittype);
	wprintf(L"GetRadioTransmitDelay time = %d\n", time);
	wprintf(L"\n");
	time = 230;
	Transmittype = RADIO_TRANSMIT_DELAY_ON_NO_TAG;
	//Transmittype = RADIO_TRANSMIT_DELAY_OFF;
	api3Data.rfidStatus = RFID_SetRadioTransmitDelay(api3Data.hReader, Transmittype, time);
	api3Data.rfidStatus = RFID_GetRadioTransmitDelay(api3Data.hReader, &Transmittype, &time);
	//api3Data.rfidStatus = RFID_SaveLlrpConfig(api3Data.hReader, NULL);
	wprintf(L"\n");
	wprintf(L"GetRadioTransmitDelay  Transmittype = %d\n", Transmittype);
	wprintf(L"GetRadioTransmitDelay time = %d\n", time);
	wprintf(L"\n");

	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
	rfid3Sleep(60000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	while ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(pTagData);
	}
	RFID_DeallocateTag(api3Data.hReader, pTagData);
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
}
