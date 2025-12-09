#include "ManualTests.h"

extern struct RFID3DATA api3Data;

void StressRMAPI( DWORD CycleCount,RFID_VERSION version )
{
	wprintf(L"\n Stress RMAPI");
	ANTENNA_MODE antMode;
	READER_STATS rdrstats;
	BOOLEAN pStatus;
	UPDATE_STATUS upStatus;
	READER_SYSTEM_INFO readerSystemInfo;
	PROFILE_LIST lpProfileList;
	CONNECTION_INFO ConInfo = { version, 0 };
	ERROR_INFO error;
	HEALTH_STATUS sts;
	READER_SYSTEM_INFO sysInfo;
	LLRP_CONNECTION_CONFIG llrpConnConfig;
	SEC_LLRP_CONFIG SecureConfig;
	
	for( int CallCount = 0; CallCount < CycleCount; CallCount++ )
	{
		wprintf(L" \n\n Call Count API %d:\n",CallCount );
		
		
		LOGIN_INFO loginfo;
		wcscpy( loginfo.hostName,api3Data.szIPAddress );
		wcscpy( loginfo.userName,L"admin" );
		wcscpy( loginfo.password,L"change" );
		loginfo.forceLogin = TRUE;
		loginfo.version = version;
	
		api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
		wprintf(L"\n RFID_Login: %ls p",RFID_GetErrorDescription(api3Data.rfidStatus));
	
	/*	api3Data.rfidStatus = RFID_GetReaderStats( api3Data.hReaderMgmt, 1,&rdrstats );
		api3Data.rfidStatus = RFID_GetAntennaMode( api3Data.hReaderMgmt,&antMode);
		wprintf(L"\nRFID_GetAntennaMode: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_SetAntennaMode( api3Data.hReaderMgmt, ANTENNA_MODE_MONOSTATIC );
		wprintf(L"\n RFID_SetAntennaMode: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();
		api3Data.rfidStatus = RFID_SetAntennaMode( api3Data.hReaderMgmt, ANTENNA_MODE_BISTATIC );
		wprintf(L"\n RFID_SetAntennaMode: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();
		api3Data.rfidStatus = RFID_GetUpdateStatus(api3Data.hReaderMgmt, &upStatus);
		wprintf(L"\n RFID_GetUpdateStatus: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetSystemInfo( api3Data.hReaderMgmt, &readerSystemInfo);
		wprintf(L"\n RFID_GetSystemInfo: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));*/
		
		/*api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &lpProfileList);
		
		wprintf(L"\n RFID_GetProfileList: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		if( lpProfileList.pProfileName == NULL )
		{
			wprintf(L"\n Profile List is NULL");
		}
		if( lpProfileList.activeProfileIndex == 0) lpProfileList.activeProfileIndex++;
		api3Data.rfidStatus = RFID_SetActiveProfile( api3Data.hReaderMgmt, lpProfileList.pProfileName[lpProfileList.activeProfileIndex-1]);
		wprintf(L"\n RFID_SetActiveProfile: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();
		api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &lpProfileList);
		for ( int index=0;index<lpProfileList.numProfiles;index++)
		{
			wprintf( L"\n Profile INDEX --> NAME: %d --> %ls\n",index, lpProfileList.pProfileName[index] );
		}*/

		//api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,LLRP_SERVER,&sts,0);
		//wprintf(L"\n LLRP Status %ls",sts?L"YES":L"NO");
		//api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,RM,&sts,0);
		//wprintf(L"\n RM Status %ls",sts?L"YES":L"NO");
		//api3Data.rfidStatus = RFID_GetSystemInfo( api3Data.hReaderMgmt,&sysInfo);
		//wprintf(L"\n CPU-system %ld  CPU-user %ld",sysInfo.cpuUsageForSystemProcesses,sysInfo.cpuUsageForUserProcesses);
		//memset(&llrpConnConfig,0,sizeof(LLRP_CONNECTION_CONFIG));
		//api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&llrpConnConfig);
		//wprintf(L"\n ServerIP:%ls \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ",llrpConnConfig.hostServerIP,llrpConnConfig.isClient,llrpConnConfig.port,llrpConnConfig.pSecureModeConfig->secureMode,llrpConnConfig.pSecureModeConfig->validatePeerCert);
		/*api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,2000,&ConInfo );
		wprintf(L"\n Connect: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

		api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps );
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,0,0);
		wprintf(L"\n RFID_PerformInventory: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		rfid3Sleep(2000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		while( (api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData))== RFID_API_SUCCESS)
		{		
			printTagDataWithResults(pTagData);
		}
		RFID_DeallocateTag( api3Data.hReader,pTagData );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );*/
		api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt );
	}
}

void StressConfigAPI( DWORD CycleCount ,RFID_VERSION version)
{
	BOOLEAN bStatus;
	GPI_PORT_STATE pState;
	UINT32 index;
	SINGULATION_CONTROL singControl;
	TAG_STORAGE_SETTINGS tgSettings;
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	
	
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
	
	for( int CallCount = 0; CallCount < CycleCount;CallCount++ )
	{
		api3Data.rfidStatus = RFID_SetTraceLevel(api3Data.hReader,TRACE_LEVEL_ALL);
		wprintf(L" \n Call Count API %d:\n",CallCount );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus =  RFID_SetGPOState( api3Data.hReader, 1 ,TRUE);
		api3Data.rfidStatus =  RFID_SetGPOState( api3Data.hReader, 2 ,TRUE);
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetAntennaConfig( api3Data.hReader,1,(UINT16*)&index,(UINT16*)&index,(UINT16*)&index );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_SetAntennaConfig( api3Data.hReader,1,api3Data.rCaps.receiveSensitivtyTable.numValues-1,api3Data.rCaps.transmitPowerLevelTable.numValues,api3Data.rCaps.fixedFreqInfo.numFreq );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetGPOState( api3Data.hReader, 1, &bStatus );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetGPIState( api3Data.hReader,1,&bStatus, &pState );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_EnableGPIPort( api3Data.hReader,1,TRUE );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetSingulationControl( api3Data.hReader,1,&singControl );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader,1,&singControl );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_GetRFMode( api3Data.hReader,1,&index,&index );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_SetRFMode( api3Data.hReader,1,index,index );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_DeletePreFilter(api3Data.hReader,0,0 );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
		wprintf(L"\n API Result: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader, pTagData );		
	}
}
void StressZoneInfo( DWORD CycleCount ,RFID_VERSION version )
{
	LOGIN_INFO loginfo;
	wcscpy( loginfo.hostName,api3Data.szIPAddress );
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = version;
	TIME_ZONE_LIST timeZoneList;

	api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );	// login
	wprintf(L"\n RFID_Login: %ls p",RFID_GetErrorDescription(api3Data.rfidStatus));

	for( int CallCount = 0; CallCount < CycleCount;CallCount++ )
	{
		wprintf(L" \n Call Count API %d:\n",CallCount );
		memset(&timeZoneList,0,sizeof(TIME_ZONE_LIST));
		api3Data.rfidStatus = RFID_GetTimeZoneList(api3Data.hReaderMgmt,&timeZoneList);
		
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			wprintf(L"\nNumber of TimeZones: %ld",timeZoneList.numTimeZones);
			wprintf(L"\nActiveTimeZone %ld -- %ls",timeZoneList.activeTimeZoneIndex,timeZoneList.pTimeZone[timeZoneList.activeTimeZoneIndex]);
			for (int i=0; i<timeZoneList.numTimeZones ; i++ )
			{
				wprintf( L"\nTime Zone %d -- %ls ", i, timeZoneList.pTimeZone[i]);
			}
		}
		api3Data.rfidStatus = RFID_SetTimeZone( api3Data.hReaderMgmt,(1 + rand( ) % 92));
		rfid3Sleep(1000);
	}

	api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt); //logout after the test
}