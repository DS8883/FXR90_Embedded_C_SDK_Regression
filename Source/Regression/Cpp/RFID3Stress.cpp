#ifndef linux
#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;
extern struct HANDLE_LIST hList;

DWORD CallCount = 0;
DWORD AccessStartCount = 0;
DWORD AccessStopCount = 0;
DWORD DisconnectCount = 0;
DWORD TagCount = 0;


DWORD WINAPI ProcessOnlyTags( LPVOID hStopEvent )
{
	DWORD dwStatus;
	while( api3Data.bContinue );
	{
		dwStatus = WaitForSingleObject( (HANDLE)hStopEvent, INFINITE );
		switch( dwStatus )
		{
		case WAIT_OBJECT_0:
		{
			LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
			fprintf( api3Data.fpStressLog,"\nInventory Call Count, TIME:%s Count:%d ",PrintTime(), CallCount++ );
			 while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
			 {				
				Print2Log( L"Tag Data",pTagData->antennaID, pTagData->CRC,pTagData->PC, pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength );
				 //Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);							
			 }

			if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
		}
		break;
		default:
			;
		}
	}
		
	return TRUE;

}

DWORD WINAPI ProcessTags( LPVOID handleList )
{
	HANDLE_LIST* pList = ( HANDLE_LIST*)handleList;
	DWORD dwStatus;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	
	while( api3Data.bContinue )
	{
		dwStatus = WaitForMultipleObjects( pList->nTotalHandles, pList->handles, FALSE, INFINITE );

		switch( dwStatus )
		{
		case WAIT_OBJECT_0: // access Start event
			{
				fprintf(api3Data.fpStressLog,"\n------------------Access Start Event:TIME:%s Count: %d-----------\n",PrintTime(),AccessStartCount++ );					
			}
			break;
		case WAIT_OBJECT_0+1: // Tag read eventp
		{
			//UINT8 Data2Write[4]  = { 0x12,0x34,0x56,0x78 };
			//if( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
			//{
			//	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,0,0,0};
			//	//fprintf( api3Data.fpStressLog,"\n Tag Read Count Write Count:%d ",TagCount );
			//	printTagMemoryBankData( pTagData->PC,pTagData->CRC,pTagData->pTagID,pTagData->tagIDLength,pTagData->pMemoryBankData,pTagData->memoryBankDataLength,pTagData->memoryBank );
			//	Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);				
			//	api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&rParams,0,0,pTagData,0 );
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_Read() API RESULT:%d ",api3Data.rfidStatus );
			//	WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write, 4, 0,0 };
			//	api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,NULL,0);
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_Write() API RESULT:%d ",api3Data.rfidStatus );
			//	WRITE_SPECIFIC_FIELD_ACCESS_PARAMS wSpecificParams = { pTagData->pTagID,pTagData->tagIDLength,0 };
			//	api3Data.rfidStatus = RFID_WriteTagID( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wSpecificParams,NULL,0);
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_WriteTagID() API RESULT:%d ",api3Data.rfidStatus );
			//	wSpecificParams.pWriteData = Data2Write;
			//	wSpecificParams.writeDataLength = 4;
			//	api3Data.rfidStatus = RFID_WriteAccessPassword( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wSpecificParams,NULL,0);
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_WriteAccessPassword() API RESULT:%d ",api3Data.rfidStatus );
			//	
			//	api3Data.rfidStatus = RFID_WriteKillPassword( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wSpecificParams,NULL,0);
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_WriteKillPassword() API RESULT:%d ",api3Data.rfidStatus );
			//	BLOCK_ERASE_ACCESS_PARAMS blkParams = { MEMORY_BANK_USER,0,16,0,0 };
			//	
			//	api3Data.rfidStatus = RFID_BlockErase( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&blkParams,NULL,0,0);
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_BlockErase() API RESULT:%d ",api3Data.rfidStatus );
			//	
			//	api3Data.rfidStatus = RFID_BlockWrite( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,NULL,0);
			//	rfid3Sleep(500 );
			//	fprintf( api3Data.fpStressLog,"\n RFID_BlockWrite() API RESULT:%d ",api3Data.rfidStatus );
			//	RFID_PurgeTags( api3Data.hReader, 0 );
			//	//TagCount++;
			//}		
		}
		break;
		case WAIT_OBJECT_0+2: // access Stop event
			{
				fprintf(api3Data.fpStressLog,"\n------------------Access Stop Event: TIME: %s Count: %d-----------\n",PrintTime(),AccessStopCount++ );
				UINT8 Data2Write[4]  = { 0x12,0x34,0x56,0x78 };
				UINT8 SpecificData[8] = { 0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11 };
				if( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
				{
					READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,12,0,0};
					//fprintf( api3Data.fpStressLog,"\n Tag Read Count Write Count:%d ",TagCount );
					api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&rParams,0,&api3Data.pInfo,pTagData,0 );
					printTagDataWithResults( pTagData );
					
					fprintf( api3Data.fpStressLog,"\n RFID_Read() API RESULT:%d ",api3Data.rfidStatus );
					WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write, 4, 0,0 };
					api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,&api3Data.pInfo,0);
				
					fprintf( api3Data.fpStressLog,"\n RFID_Write() API RESULT:%d ",api3Data.rfidStatus );
					
					BLOCK_ERASE_ACCESS_PARAMS blkParams = { MEMORY_BANK_USER,0,16,0,0 };
					api3Data.rfidStatus = RFID_BlockErase( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&blkParams,NULL,&api3Data.pInfo,0);
					fprintf( api3Data.fpStressLog,"\n RFID_BlockErase() API RESULT:%d ",api3Data.rfidStatus );
					
					api3Data.rfidStatus = RFID_BlockWrite( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,&api3Data.pInfo,0);
					fprintf( api3Data.fpStressLog,"\n RFID_BlockWrite() API RESULT:%d ",api3Data.rfidStatus );
					
				}	
			}
			break;
		case WAIT_OBJECT_0+3: // Reader DisConnect event
		{
			DISCONNECTION_EVENT_DATA DisConnectEvent;
			RFID_GetEventData( api3Data.hReader,DISCONNECTION_EVENT,&DisConnectEvent);
			fprintf( api3Data.fpStressLog,"\nReader Disconnect Event, TIME:%s Data: %d ",PrintTime(),DisConnectEvent.eventInfo);			
		}
		break;
		default:
			;
		}
	}	
	
	if( pTagData != NULL ) RFID_DeallocateTag(api3Data.hReader,pTagData );

	return TRUE;
}

void StressConnectDisConnectRead( DWORD CycleCount )
{
	fprintf( api3Data.fpStressLog, "\n\n ===========================================================================\n");
	fprintf( api3Data.fpStressLog, "\n\n ========================StressConnectDisConnectRead() =====================\n");
	fprintf( api3Data.fpStressLog, "\n\n ===========================================================================\n");
	fprintf( api3Data.fpStressLog, " START TIME: %s",PrintTime() );
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );

	int Count = 0;
	DWORD dwThreadID;
	api3Data.bContinue = TRUE;
	// Opcode Related Data 
	UINT32 opIndex;
	TRIGGER_INFO triggerInfo;
	memset(&triggerInfo,0,sizeof(triggerInfo));
	// create an access stop event
	//
	hList.handles[ 0 ] = CreateEvent( NULL,0,0,NULL );	// start
	hList.handles[ 1 ] = CreateEvent( NULL,0,0,NULL ); // Read event
	hList.handles[ 2 ] = CreateEvent( NULL,0,0,NULL ); // stop access
	hList.handles[ 3 ] = CreateEvent( NULL,0,0,NULL ); // disconnect access
	 
	hList.nTotalHandles = 4;

	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,ACCESS_START_EVENT,hList.handles[0] );
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,TAG_READ_EVENT,hList.handles[1] );
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT,hList.handles[2]);
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,DISCONNECTION_EVENT,hList.handles[3]);

	// Do the processing in sepearate thread
	//
	HANDLE hEventThread = CreateThread( NULL, 0, ProcessTags, (LPVOID)(&hList), 0, &dwThreadID);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );

	//Access read for all memory banks
	//
	triggerInfo.tagReportTrigger	= 0; 
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;//START_TRIGGER_TYPE_PERIODIC
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	triggerInfo.stopTrigger.value.tagObservation.n = 5; 
	triggerInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	if( api3Data.apiVersion ==  RFID_API3_5_1  || api3Data.apiVersion ==  RFID_API3_5_5)
	{
		triggerInfo.lpTagEventReportInfo = NULL;
	}
	while( api3Data.bContinue )
	{
		fprintf( api3Data.fpStressLog,"\n------ CALL %d --------------------------------\n",Count);
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,0}; 
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,0};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,0,&api3Data.pInfo,&triggerInfo,0);
		rfid3Sleep( 15000);
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
		
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
		Count++;

		if( Count == CycleCount ) api3Data.bContinue = FALSE;
	}
		
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );

	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT );
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,TAG_READ_EVENT );
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_START_EVENT );
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,DISCONNECTION_EVENT ); 

	CallCount = 0;
	api3Data.bContinue = TRUE;
		
	for( CallCount =0; CallCount <= CycleCount; CallCount )
	{		
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress, api3Data.dwPort,0,0 );
		fprintf( api3Data.fpStressLog, "\n -------Connect Disconnect Call Count : %d \n", CallCount++ );
		if( CallCount == CycleCount ) api3Data.bContinue = FALSE;
	}

	fprintf( api3Data.fpStressLog, " END TIME: %s",PrintTime() );
}
	
void StressRMAPI( DWORD CycleCount )
{
	fprintf( api3Data.fpStressLog, "\n\n ================================================================\n");
	fprintf( api3Data.fpStressLog, "\n   ========================StressRMAPI() ==========================\n");
	fprintf( api3Data.fpStressLog, "\n   ================================================================\n");
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
	fprintf( api3Data.fpStressLog, " START TIME: %s",PrintTime() );
	ANTENNA_MODE antMode;
	BOOLEAN pStatus;
	UPDATE_STATUS upStatus;
	READER_SYSTEM_INFO readerSystemInfo;
	PROFILE_LIST lpProfileList;
	
	for( CallCount = 0; CallCount < CycleCount; CallCount++ )
	{
		LOGIN_INFO loginfo;
		wcscpy( loginfo.hostName,api3Data.szIPAddress );
		wcscpy( loginfo.userName,L"admin" );
		wcscpy( loginfo.password,L"change" );
		
		fprintf( api3Data.fpStressLog," \n Call Count API %d:\n",CallCount );
		api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,TRUE,0 );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_EnableReadPoint() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_EnableReadPoint( api3Data.hReaderMgmt, 1,TRUE );
		api3Data.rfidStatus = RFID_EnableReadPoint( api3Data.hReaderMgmt, 1,FALSE );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetReadPointStatus() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetReadPointStatus(api3Data.hReaderMgmt, 1, &pStatus);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetAntennaMode() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetAntennaMode( api3Data.hReaderMgmt,&antMode);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetAntennaMode() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_SetAntennaMode( api3Data.hReaderMgmt, ANTENNA_MODE_MONOSTATIC );
		api3Data.rfidStatus = RFID_SetAntennaMode( api3Data.hReaderMgmt, ANTENNA_MODE_BISTATIC );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetUpdateStatus() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetUpdateStatus(api3Data.hReaderMgmt, &upStatus);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetSystemInfo() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetSystemInfo( api3Data.hReaderMgmt, &readerSystemInfo);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetRadioPowerState() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReaderMgmt,TRUE);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetProfileList() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &lpProfileList);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_Logout() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt );
	}

	fprintf( api3Data.fpStressLog, " END TIME: %s",PrintTime() );
}

void StressConfigAPI( DWORD CycleCount )
{
	BOOLEAN bStatus;
	GPI_PORT_STATE pState;
	UINT32 index;
	SINGULATION_CONTROL singControl;
	TAG_STORAGE_SETTINGS tgSettings;
	fprintf( api3Data.fpStressLog, "\n\n ===============================================================\n");
	fprintf( api3Data.fpStressLog, "\n\n ========================StressConfigAPI() =====================\n");
	fprintf( api3Data.fpStressLog, "\n\n ===============================================================\n");
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );

	fprintf( api3Data.fpStressLog, " START TIME: %s",PrintTime() );
	HANDLE hReadEvent = CreateEvent( NULL,0,0,NULL ); // Read event
	
	for( CallCount = 0; CallCount < CycleCount;CallCount++ )
	{
		fprintf( api3Data.fpStressLog," \n Call Count API %d:\n",CallCount );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetReaderCaps() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_ResetConfigToFactoryDefaults() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetGPOState() API Count %d:\n",CallCount );
		api3Data.rfidStatus =  RFID_SetGPOState( api3Data.hReader, 1 ,TRUE);
		api3Data.rfidStatus =  RFID_SetGPOState( api3Data.hReader, 2 ,TRUE);
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetAntennaConfig() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetAntennaConfig( api3Data.hReader,1,(UINT16*)&index,(UINT16*)&index,(UINT16*)&index );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetAntennaConfig() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_SetAntennaConfig( api3Data.hReader,1,api3Data.rCaps.receiveSensitivtyTable.numValues-1,api3Data.rCaps.transmitPowerLevelTable.numValues,api3Data.rCaps.fixedFreqInfo.numFreq );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetGPOState() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetGPOState( api3Data.hReader, 1, &bStatus );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetGPIState() API Count %d:\n",CallCount );		
		api3Data.rfidStatus = RFID_GetGPIState( api3Data.hReader,1,&bStatus, &pState );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_EnableGPIPort() API Count %d:\n",CallCount );		
		api3Data.rfidStatus = RFID_EnableGPIPort( api3Data.hReader,1,TRUE );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetSingulationControl() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetSingulationControl( api3Data.hReader,1,&singControl );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetSingulationControl() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader,1,&singControl );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_GetRFMode() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_GetRFMode( api3Data.hReader,1,&index,&index );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetRFMode() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_SetRFMode( api3Data.hReader,1,index,index );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_RegisterEventNotification() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,TAG_READ_EVENT,hReadEvent );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_DeregisterEventNotification() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,TAG_READ_EVENT ); 
		//fprintf( api3Data.fpStressLog," \n Calling RFID_DeletePreFilter() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_DeletePreFilter(api3Data.hReader,0,0 );
		
		//fprintf( api3Data.fpStressLog," \n Calling RFID_SetTagStorageSettings() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_AllocateTag() API Count %d:\n",CallCount );
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
		//fprintf( api3Data.fpStressLog," \n Calling RFID_DeallocateTag() API Count %d:\n",CallCount );
		api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader, pTagData );		
	}

	fprintf( api3Data.fpStressLog, " END TIME: %s",PrintTime() );
}

void StressInventory(  )
{
	fprintf( api3Data.fpStressLog, "\n\n ===============================================================\n");
	fprintf( api3Data.fpStressLog, "\n\n ========================StressInventory() =====================\n");
	fprintf( api3Data.fpStressLog, "\n\n ===============================================================\n");
	
	fprintf( api3Data.fpStressLog, " START TIME: %s",PrintTime() );
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	SYSTEMTIME sysTime;
	rfid3SystemTime( &sysTime );
	sysTime.wMilliseconds += 1000;
	tInfo.startTrigger.type = START_TRIGGER_TYPE_PERIODIC;
	tInfo.startTrigger.value.periodic.periodMilliseconds = 10000;	//for every wait of 1 sec do inv for 4 sec
	tInfo.startTrigger.value.periodic.startTime = &sysTime;
	tInfo.tagReportTrigger = 0;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 4000;
	if( api3Data.apiVersion ==  RFID_API3_5_1|| api3Data.apiVersion ==  RFID_API3_5_5 )
	{
		tInfo.lpTagEventReportInfo = NULL;
	}
	api3Data.bContinue = TRUE;
	CallCount = 0;
	DWORD dwThreadID; 
	HANDLE hTagEvent = CreateEvent( NULL,0,0,NULL );	// Tag Event
	HANDLE hStopEvent = CreateEvent( NULL,0,0,NULL );	// inventory stop Event

	//api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,TAG_READ_EVENT,hTagEvent);
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT,hStopEvent);
	//HANDLE hEventThread = CreateThread( NULL, 0, ProcessOnlyTags, (LPVOID)(hStopEvent), 0, &dwThreadID);
	for( int Count =0;Count<=1000;Count++)
	{
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, &api3Data.pInfo,&tInfo, NULL );
		DWORD dwStatus = WaitForSingleObject( (HANDLE)hStopEvent, INFINITE );
		
		fprintf( api3Data.fpLog,"\nInventory Call Count, TIME:%s Count:%d ",PrintTime(), CallCount++ );
		 while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		 {				
			Print2Log( L"Tag Data", pTagData->antennaID,pTagData->CRC,pTagData->PC, pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength );
			 //Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);							
		 }

		fprintf( api3Data.fpLog,"\n Inven Count%d\n\n",Count );
	}
	
	api3Data.bContinue = FALSE;
	RFID_StopInventory( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	//api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,TAG_READ_EVENT );
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT );

	fprintf( api3Data.fpLog, " END TIME: %s",PrintTime() );
}
#endif