// RFID3EventsAutoMode.Cpp : Check RFID3 Events in Autonomous Mode( Tag Eventing Enabled )
#include "Common.h"

extern struct RFID3DATA api3Data;
extern struct HANDLE_LIST hList;
#ifdef linux
static sem_t AccessStartSemaphore;	// Access Start Semaphore
static sem_t AccessStopSemaphore;	// Access Stop Semaphore
static sem_t BufferfulWarningSemaphore;	// BufferFullWarning
static sem_t BufferfullSemaphore;		// Bufferfull

// Access Start event Handler
static void MonitorAccessStartEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&AccessStartSemaphore);
}

// Access Stop event Handler
static void MonitorAccessSopEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&AccessStopSemaphore);
}
// BufferFullWarning Event Handler
static void MonitorBufferfullwarningEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&BufferfulWarningSemaphore);
}
// BufferFull Event Handler
static void MonitorBufferfullEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&BufferfullSemaphore);
}
#endif

// Verify Access Start Event
void TestAccessStartEventAuto ( RFID_HANDLE32 hReader,BOOLEAN bAutoMode )
{
	RFID_EVENT_TYPE rfideventType = ACCESS_START_EVENT;
	UINT32 opIndex;
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorAccessStartEvent,NULL,NULL);
	sem_init(&AccessStartSemaphore, 0, 0);		
#else

	// Create and Register Inv Stop
	//
	HANDLE hAccessStart = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,rfideventType,hAccessStart);
#endif	
	// Test Invstop event
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test RFID3 Access Start Event \n <br> Expected OutPut :Access Start Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	TRIGGER_INFO trInfo;
	memset(&trInfo,0,sizeof(trInfo));
	// Initialize Trigger Infomation for Tag Events
	//
	trInfo.tagReportTrigger	= 0; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	trInfo.stopTrigger.value.tagObservation.n = 5; 
	trInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
#if defined PIXIE
	trInfo.lpTagEventReportInfo = NULL;
#else
	trInfo.lpTagEventReportInfo = &tagEventInfo;
#endif
	
	// OpSequence Initialization
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} }; 
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,{0} };
			
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
	if( bAutoMode == TRUE )
	{
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,&api3Data.pInfo,&trInfo,0);
	}
	else
	{
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,&api3Data.pInfo,NULL,0);
	}
#ifdef linux
	if( sem_wait(&AccessStartSemaphore) == 0 )
	{
#else

	if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStart, 10000 ) )
	{
#endif
		fprintf(api3Data.fpCurLog,"\n-------Access Start Event is Signalled-------");	
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			wprintf(L"\n TAG ID : %ls  ACCESSSTATUS: %ls",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength ),wACCESSSTATUS[pTagData->opStatus] );
			fprintf(api3Data.fpCurLog,"\n TAG ID : %s  ACCESSSTATUS: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),ACCESSSTATUS[pTagData->opStatus] );
		}
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
		SetLogResult( TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"\n-------Access Start Event is NOT Signalled-------");	
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
	
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
#ifdef linux
	sem_destroy(&AccessStartSemaphore);
#else
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader, ACCESS_START_EVENT );
	CloseHandle( hAccessStart );
#endif
}

void TestAccessStopEventAuto( RFID_HANDLE32 hReader,BOOLEAN bAutoMode )
{
	RFID_EVENT_TYPE rfideventType = ACCESS_STOP_EVENT;
	UINT32 opIndex;

#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorAccessSopEvent,NULL,NULL);
	sem_init(&AccessStopSemaphore, 0, 0);		
#else
	// Create and Register Inv Stop
	//
	HANDLE hAccessStop = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hAccessStop);
#endif
	// Test Invstop event
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test RFID3 Access Stop Event \n <br> Expected OutPut :Access Stop Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	TRIGGER_INFO trInfo;
	memset(&trInfo,0,sizeof(trInfo));
	//
	trInfo.tagReportTrigger	= 0; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	trInfo.stopTrigger.value.tagObservation.n = 5; 
	trInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	trInfo.lpTagEventReportInfo = &tagEventInfo;
	
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} }; 
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,{0} };
			
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
	if( bAutoMode == TRUE )
	{
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,&api3Data.pInfo,&trInfo,0);
	}
	else
	{
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,&api3Data.pInfo,NULL,0);
	}
	//rfid3Sleep( 3000 );	// wait for the stop to complete	
	api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
#ifdef linux
	if( sem_wait(&AccessStopSemaphore) == 0 )
	{
#else	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
	{
#endif
		fprintf(api3Data.fpCurLog,"\n-------Access Stop Event is Signalled-------");	
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			wprintf(L"\n TAG ID : %ls  ACCESSSTATUS: %ls",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength ),wACCESSSTATUS[pTagData->opStatus] );
			fprintf(api3Data.fpCurLog,"\n TAG ID : %s  ACCESSSTATUS: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),ACCESSSTATUS[pTagData->opStatus] );
		}
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
		SetLogResult( TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"\n-------Access Stop Event is NOT Signalled-------");	
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
	
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
#ifdef linux
	sem_destroy(&AccessStopSemaphore);
#else
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,ACCESS_STOP_EVENT );
	CloseHandle(hAccessStop); 
#endif

}

void TestBufferFullWarningEventAuto( RFID_HANDLE32 hReader,BOOLEAN bAutoMode )
{
	RFID_EVENT_TYPE rfideventType = BUFFER_FULL_WARNING_EVENT;
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorBufferfullwarningEvent,NULL,NULL);
	sem_init(&BufferfulWarningSemaphore, 0, 0);		
#else
	// Create and Register Bufferfull Warning event handle
	//
	HANDLE hBufferfullWarning = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,BUFFER_FULL_WARNING_EVENT,hBufferfullWarning );
#endif

	// Test Bufferfull Warning Event
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test BUfferFullWarning Event \n <br> Expected OutPut :BUfferFullWarning Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	TAG_STORAGE_SETTINGS tgSettings;
	tgSettings.maxMemoryBankByteCount = 512;
	tgSettings.maxTagCount = 5;
	tgSettings.maxTagIDByteCount = 96;
	RFID_SetTagStorageSettings(api3Data.hReader,&tgSettings);
	TRIGGER_INFO trInfo;
	memset(&trInfo,0,sizeof(trInfo));
	trInfo.tagReportTrigger	= 0; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	trInfo.lpTagEventReportInfo = &tagEventInfo;
	if( bAutoMode == TRUE )
	{
		api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,&trInfo,NULL );
	}
	else
	{
		api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
	}
#ifdef linux
	if( sem_wait(&BufferfulWarningSemaphore) == 0 )
	{
#else		
	if( WAIT_OBJECT_0 == WaitForSingleObject( hBufferfullWarning, 10000 ) )
	{
#endif

		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"\n-------Bufferfull WarningEvent is Signalled-------");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "\n TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);
		}
		SetLogResult(TRUE );
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"\n-------Bufferfull Warning Event is NOT Signalled-------");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
#ifdef linux
	sem_destroy(&BufferfulWarningSemaphore);
#else	
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,BUFFER_FULL_WARNING_EVENT );
	CloseHandle( hBufferfullWarning );
#endif

}

void TestBufferFullEventAuto( RFID_HANDLE32 hReader,BOOLEAN bAutoMode )
{
	RFID_EVENT_TYPE rfideventType = BUFFER_FULL_EVENT;
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorBufferfullEvent,NULL,NULL);
	sem_init(&BufferfullSemaphore, 0, 0);		
#else// Create and Register Bufferfull  event handle
	//
	HANDLE hBufferfull = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,rfideventType,hBufferfull );
#endif
	// Test Bufferfull Warning Event
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test BUfferFull Event \n <br> Expected OutPut :BUfferFull Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	TAG_STORAGE_SETTINGS tgSettings;
	tgSettings.maxMemoryBankByteCount = 512;
	tgSettings.maxTagCount = 5;
	tgSettings.maxTagIDByteCount = 96;
	
	TRIGGER_INFO trInfo;
	memset(&trInfo,0,sizeof(trInfo));
	trInfo.tagReportTrigger	= 0; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	trInfo.lpTagEventReportInfo = &tagEventInfo;
	if( bAutoMode == TRUE )
	{
		api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,&trInfo,NULL );
	}
	else
	{
		api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
	}
#ifdef linux
	if( sem_wait(&BufferfullSemaphore) == 0 )
	{
#else		
	if( WAIT_OBJECT_0 == WaitForSingleObject( hBufferfull, 10000 ) )
	{
#endif
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"\n-------Bufferfull Event is Signalled-------");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "\n TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);
		}
		SetLogResult(TRUE );
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"\n-------Bufferfull  Event is NOT Signalled-------");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");	
#ifdef linux
	sem_destroy(&BufferfullSemaphore);
#else	
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,BUFFER_FULL_EVENT );
	CloseHandle(hBufferfull);
#endif
}

void TestRFID3EventsAutoMode( RFID_HANDLE32 hReader )
{
	if (!api3Data.rCaps.tagEventReportingSupported)
	{
		wprintf(L"\n autonomous mode events are not supported\n");
		return;
	}

	TestAccessStartEventAuto( hReader,TRUE );wprintf(L"\n TestAccessStartEvent - Done");
	TestAccessStopEventAuto( hReader,TRUE );wprintf(L"\n TestAccessStopEvent - Done");
	TestBufferFullWarningEventAuto( hReader,TRUE );wprintf(L"\n TestBufferFullWarningEvent - Done");
	TestBufferFullEventAuto( hReader,TRUE );wprintf(L"\n TestBufferFullEvent - Done");
}

