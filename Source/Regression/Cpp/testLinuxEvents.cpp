#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;

void TestTagDataEvent( RFID_HANDLE32 hReader )
{
	// Create and Register tag Data event handle
	//
	HANDLE htagDataEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,TAG_DATA_EVENT,htagDataEvent );

	// Test Tag Data Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Tag Data Event \n <br> Expected OutPut :Tag Data Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is :");
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );

	if( WAIT_OBJECT_0 == WaitForSingleObject( htagDataEvent, 10000 ) )
	{
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Tag Data Event is Signalled-------<br>");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "<br> TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);
		}
		SetLogResult(TRUE );
		RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Tag Data Event is NOT Signalled-------<br>");	
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_DATA_EVENT );
}

void TestTagReadEvent( RFID_HANDLE32 hReader )
{
	// Create and Register tag read event handle
	//
	HANDLE htagReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,TAG_READ_EVENT,htagReadEvent);

	// Test Tag Read Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Tag Read Event \n <br> Expected OutPut :Tag Read Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );

	if( WAIT_OBJECT_0 == WaitForSingleObject( htagReadEvent, 10000 ) )
	{
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Tag Read Event is Signalled-------<br>");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "<br> TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);
		}
		SetLogResult(TRUE );
		RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Tag Read Event is NOT Signalled-------<br>");	
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_READ_EVENT );
}
void TestInvStartEvent( RFID_HANDLE32 hReader )
{
	// Create and Register Inv Start
	//
	HANDLE hInvStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,INVENTORY_START_EVENT,hInvStartEvent);

	// Test Invstart event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Invenetory start Event \n <br> Expected OutPut :Inentory Start Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );

	if( WAIT_OBJECT_0 == WaitForSingleObject( hInvStartEvent, 10000 ) )
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inv Start Event is Signalled-------<br>");	
		RFID_StopInventory( hReader );
		SetLogResult(TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inv Start Event is Not Signalled-------<br>");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,INVENTORY_START_EVENT );
}

void  TestInvStopEvent( RFID_HANDLE32 hReader )
{
	// Create and Register Inv Stop
	//
	HANDLE hInvStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,INVENTORY_STOP_EVENT,hInvStopEvent);

	// Test Invstop event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Invenetory Stop Event \n <br> Expected OutPut :Inentory Stop Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");

	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
	api3Data.rfidStatus = RFID_StopInventory( hReader );
	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hInvStopEvent, 10000 ) )
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inv Stop Event is Signalled-------<br>");	
		SetLogResult(TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inv Stop Event is Not Signalled-------<br>");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,INVENTORY_STOP_EVENT );
}

void TestAccessStartEvent( RFID_HANDLE32 hReader )
{
	UINT32 opIndex;
	// Create and Register Inv Stop
	//
	HANDLE hAccessStart = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_START_EVENT,hAccessStart);
	
	// Test Invstop event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test RFID3 Access Start Event \n <br> Expected OutPut :Access Start Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");

	TRIGGER_INFO trInfo;
	//
	trInfo.tagReportTrigger	= 0; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	trInfo.stopTrigger.value.tagObservation.n = 5; 
	trInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	
	if( api3Data.apiVersion == RFID_API3_5_1 )
	{
		trInfo.lpTagEventReportInfo = NULL;
	}
	
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,0,0,0 }; 
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,0 };
			
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,&api3Data.pInfo,&trInfo,0);

	if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStart, 10000 ) )
	{
		fprintf(api3Data.fpCurLog,"<br>-------Access Start Event is Signalled-------<br>");	
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		SetLogResult( TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Access Start Event is NOT Signalled-------<br>");	
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );

	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader, ACCESS_START_EVENT );
}

void TestAccessStopEvent( RFID_HANDLE32 hReader )
{
	UINT32 opIndex;
	// Create and Register Inv Stop
	//
	HANDLE hAccessStop = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hAccessStop);

	// Test Invstop event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test RFID3 Access Stop Event \n <br> Expected OutPut :Access Stop Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");

	TRIGGER_INFO trInfo;
	//
	trInfo.tagReportTrigger	= 0; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	trInfo.stopTrigger.value.tagObservation.n = 5; 
	trInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	
	if( api3Data.apiVersion == RFID_API3_5_1 )
	{
		trInfo.lpTagEventReportInfo = NULL;
	}
	
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,0,0,0 }; 
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,0 };
			
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,&api3Data.pInfo,&trInfo,0);
	//Sleep( 3000 );	// wait for the stop to complete	
	api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
	{
		fprintf(api3Data.fpCurLog,"<br>-------Access Stop Event is Signalled-------<br>");	
		SetLogResult( TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Access Stop Event is NOT Signalled-------<br>");	
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );

	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,ACCESS_STOP_EVENT );
}

void TestBufferFullWarningEvent( RFID_HANDLE32 hReader )
{
	TAG_STORAGE_SETTINGS tgSettings;
	// Create and Register Bufferfull Warning event handle
	//
	HANDLE hBufferfullWarning = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,BUFFER_FULL_WARNING_EVENT,hBufferfullWarning );

	// Test Bufferfull Warning Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test BUfferFullWarning Event \n <br> Expected OutPut :BUfferFullWarning Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader, &tgSettings );
	tgSettings.maxTagCount = 5;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hBufferfullWarning, 10000 ) )
	{
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Bufferfull WarningEvent is Signalled-------<br>");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "<br> TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);
		}
		SetLogResult(TRUE );
		RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Bufferfull Warning Event is NOT Signalled-------<br>");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}
	RFID_PurgeTags(api3Data.hReader,0);
	logToResultFile(TEXT("EVENT-FUN"));
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,BUFFER_FULL_WARNING_EVENT );

}

void TestBufferFullEvent( RFID_HANDLE32 hReader )
{
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);

	// Create and Register Bufferfull  event handle
	//
	HANDLE hBufferfull = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,BUFFER_FULL_EVENT,hBufferfull );

	// Test Bufferfull Warning Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test BUfferFull Event \n <br> Expected OutPut :Warning and followed by FULL Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");

	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hBufferfull, 10000 ) )
	{
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Bufferfull Event is Signalled-------<br>");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "<br> TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);
		}
		SetLogResult(TRUE );
		RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Bufferfull  Event is NOT Signalled-------<br>");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(TEXT("EVENT-FUN"));
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,BUFFER_FULL_EVENT );
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
}

void TestTagEvents( RFID_HANDLE32 hReader, TRIGGER_INFO* ptriggerInfo )
{
	fprintf(api3Data.fpCurLog, "<br><b>REPORT NEW TAG: %s TAG BACK2VISIBILITY: %s TAG INVISIBLE: %s</b>",TAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportNewTagEvent],
																		  TAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportTagBackToVisibilityEvent],
																		  TAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportTagInvisibleEvent]);

	fprintf( api3Data.fpCurLog, "\n Result is: ");

	// Create and Register tag read event handle
	//
	HANDLE hInventoryDone = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,INVENTORY_STOP_EVENT,hInventoryDone);

	// Perform inventory to Start access and inventory
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,ptriggerInfo,NULL);
	if( WAIT_OBJECT_0 == WaitForSingleObject( hInventoryDone, 20000 ) )
	{
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Inventory Stop Event is Signalled-------<br>");	
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		}

		SetLogResult(TRUE );
		RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inventory Stop is NOT Signalled-------<br>");
		if(ptriggerInfo->lpTagEventReportInfo->reportNewTagEvent == NEVER &&
			ptriggerInfo->lpTagEventReportInfo->reportTagBackToVisibilityEvent == NEVER &&
			ptriggerInfo->lpTagEventReportInfo->reportTagInvisibleEvent == NEVER )
		{
			SetLogResult( TRUE );
		}
		else
		{
			SetLogResult( FALSE );
		}
	}
	api3Data.rfidStatus = RFID_StopInventory( hReader );
	Sleep( 2000 ); //wait for the inventory to stop.
	RFID_PurgeTags( hReader , 0);

	logToResultFile(TEXT("EVENT-FUN"));
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_READ_EVENT );
}

void TestRFID3TagEvents( RFID_HANDLE32 hReader )
{
	TAG_STORAGE_SETTINGS tgSettings;
	tgSettings.maxMemoryBankByteCount = 512;
	tgSettings.maxTagCount = 100;
	tgSettings.maxTagIDByteCount=512;
	tgSettings.enableAccessReports = TRUE;
	tgSettings.tagFields = ANTENNA_ID | FIRST_SEEN_TIME_STAMP| LAST_SEEN_TIME_STAMP | PEAK_RSSI | PC | CRC;
	
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings );
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	TRIGGER_INFO tInfo;
	tInfo.lpTagEventReportInfo = &tagEventInfo;
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 5000;
	tInfo.tagReportTrigger = 0;
	// Test Tag Read Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Tag reporting IMMEDIATE for tag events \n <br> Expected OutPut :Tag Data events should report immediatly\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	TestTagEvents( hReader, &tInfo );	
	
	fprintf(api3Data.fpCurLog,"<br><a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Tag reporting MODERATED for tag events \n <br> Expected OutPut :Tag Data events should be reported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	tagEventInfo.reportNewTagEvent = MODERATED;
	tagEventInfo.reportTagBackToVisibilityEvent = MODERATED;
	tagEventInfo.reportTagInvisibleEvent = MODERATED;
	tInfo.lpTagEventReportInfo->newTagEventModeratedTimeoutMilliseconds = tInfo.lpTagEventReportInfo->tagBackToVisibilityModeratedTimeoutMilliseconds=tInfo.lpTagEventReportInfo->tagInvisibleEventModeratedTimeoutMilliseconds = 1000;
	TestTagEvents( hReader, &tInfo );	
	
	// with NEVER
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Tag reporting NEVER for tag events \n <br> Expected OutPut :Dont report any tag events\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	tagEventInfo.reportNewTagEvent = tagEventInfo.reportTagBackToVisibilityEvent = tagEventInfo.reportTagInvisibleEvent = NEVER;

	TestTagEvents( hReader, &tInfo );	
	
	// with NEVER 
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Test Tag reporting NEVER for reportTagInvisibleEvent \n <br> Expected OutPut :report only new tag, back to visibility tags\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	tagEventInfo.reportNewTagEvent = IMMEDIATE;
	tagEventInfo.reportTagBackToVisibilityEvent = MODERATED;
	tagEventInfo.reportTagInvisibleEvent = NEVER;
	
	TestTagEvents( hReader, &tInfo );	

	
	for( int events =0; events<3;events++ )
	{
		
		api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
		tagEventInfo.reportNewTagEvent = (TAG_EVENT_REPORT_TRIGGER)events;
		tagEventInfo.reportTagBackToVisibilityEvent = NEVER;
		tagEventInfo.reportTagInvisibleEvent = NEVER;
		// with NEVER 
		fprintf(api3Data.fpCurLog,"\n\n<a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Verify different event reporting for \n <br> Expected OutPut :Check Log File\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		TestTagEvents( hReader, &tInfo );	
	}

	
	for( int events =0; events<3;events++ )
	{
		// with NEVER 
		fprintf(api3Data.fpCurLog,"\n\n<a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Verify different event reporting for \n <br> Expected OutPut :Check Log File\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
		tagEventInfo.reportNewTagEvent =  NEVER;
		tagEventInfo.reportTagBackToVisibilityEvent = (TAG_EVENT_REPORT_TRIGGER)events;
		tagEventInfo.reportTagInvisibleEvent = NEVER;
		
		TestTagEvents( hReader, &tInfo );	
	}
	
	for( int events =0; events<3;events++ )
	{
		// with NEVER 
		fprintf(api3Data.fpCurLog,"\n\n<a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Verify different event reporting for \n <br> Expected OutPut :Check Log File\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
	
		api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
		tagEventInfo.reportNewTagEvent =  NEVER;
		tagEventInfo.reportTagBackToVisibilityEvent = NEVER;
		tagEventInfo.reportTagInvisibleEvent = (TAG_EVENT_REPORT_TRIGGER)events;
		
		TestTagEvents( hReader, &tInfo );	
	}

	// cleanup the queue
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);

	//register only for immediate event of new tag
	//
	tagEventInfo.reportNewTagEvent = IMMEDIATE;
	tagEventInfo.reportTagBackToVisibilityEvent = NEVER;
	tagEventInfo.reportTagInvisibleEvent = NEVER;
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=EVENT-FUN-%d></a> EVENT-FUN-%d:\n\n<br>Description:Verify different event reporting tag which is not visible now\n <br> Expected OutPut :Tag reporting for invisible status tags\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");

	TestTagEvents( hReader, &tInfo );	
	
	// Clean up the queue.
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);

}

void TestRFID3EventsAutomated( RFID_HANDLE32 hReader )
{	
	TestTagDataEvent( hReader );printf("\n TestTagDataEvent - Done");
	TestTagReadEvent( hReader );printf("\n TestTagReadEvent - Done");
	TestInvStartEvent( hReader );printf("\n TestInvStartEvent - Done");
	TestInvStopEvent( hReader );printf("\n TestInvStopEvent - Done");
	TestAccessStartEvent( hReader );printf("\n TestAccessStartEvent - Done");
	TestAccessStopEvent ( hReader );printf("\n TestAccessStopEvent - Done");
	TestBufferFullWarningEvent( hReader );printf("\n TestBufferFullWarningEvent - Done");
	TestBufferFullEvent( hReader );printf("\n TestBufferFullEvent - Done");
	TestRFID3TagEvents( hReader );printf("\n TestRFID3TagEvents - Done");
}

void TestingTriggers( )
{
	printf("\n Executing %s Started ",__FUNCTION__);
	SetLogText("TestingTriggers");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	logCurrentExecution("Inventory with TRIGGGERS");
	api3Data.itemTestCount = 7;
	TestInventroyWithTriggers( api3Data.hReader, &api3Data.pInfo );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();

	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	printf("\n Executing %s Comleted ",__FUNCTION__);
}

void TestingEvents( )
{
	printf("\n Executing %s Started ",__FUNCTION__);
	SetLogText("TestingEvents");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	logCurrentExecution("Testing Events");

	api3Data.itemTestCount = 26;
	TestRFID3EventsAutomated( api3Data.hReader );
	TestRFID3EventsAutoMode ( api3Data.hReader );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();

	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	printf("\n Executing %s Comleted ",__FUNCTION__);
}
	
