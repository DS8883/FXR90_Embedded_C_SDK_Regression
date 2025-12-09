#include "Common.h"
#include "APITest.h"
#include "rfid3Events.h"

extern struct RFID3DATA api3Data;
static BOOLEAN bRFSurveyStartEvent;
static BOOLEAN bRFSurveyStopEvent;
static BOOLEAN bRFSurveyDataEvent;

#ifdef linux
static sem_t TagEventSemaphore;
static sem_t TagReadEventSemaphore;
static sem_t InvStartSemaphore;
static sem_t InvStopEventSemaphore;
static void MonitorRFIDTagEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&TagEventSemaphore);
}
static void MonitorRFIDTagReadEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&TagReadEventSemaphore);
}
static void MonitorRFIDInvStart(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&InvStartSemaphore);
}
static void MonitorRFIDInvStop(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&InvStopEventSemaphore);
}
#endif

void TestTagDataEvent( RFID_HANDLE32 hReader )
{
	RFID_EVENT_TYPE rfideventType = TAG_DATA_EVENT;
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDTagEvent,NULL,NULL);
	sem_init(&TagEventSemaphore, 0, 0);		
#else
	// Create and Register tag Data event handle
	//
	HANDLE htagDataEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,rfideventType,htagDataEvent );
#endif

	// Test Tag Data Event
	//
	wprintf(L"\n Test : %d Test Tag Data Event\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Tag Data Event \n <br> Expected OutPut :Tag Data Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is :");
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
#ifdef linux
	if( sem_wait(&TagEventSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( htagDataEvent, 10000 ) )
	{
#endif
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Tag Data Event is Signalled-------<br>");	
		RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			/*fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			wprintf(L"\n TAG ID : %ls  TAGEVENT: %ls",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength ),wTAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "<br> TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);*/
			printTagDataWithResults( pTagData);
		}
		fflush(api3Data.fpCurLog);
		SetLogResult(TRUE );
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Tag Data Event is NOT Signalled-------<br>");	
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_DATA_EVENT );
	CloseHandle(htagDataEvent);
#endif
}

void TestTagReadEvent( RFID_HANDLE32 hReader )
{
	RFID_EVENT_TYPE rfideventType = TAG_READ_EVENT;
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDTagReadEvent,NULL,NULL);
	sem_init(&TagReadEventSemaphore, 0, 0);		
#else
	// Create and Register tag read event handle
	//
	HANDLE htagReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,rfideventType,htagReadEvent);
#endif
	wprintf(L"\n Test : %d Test Tag Read Event\n",api3Data.dwTestCaseNo);

	// Test Tag Read Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Tag Read Event \n <br> Expected OutPut :Tag Read Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
#ifdef linux
	if( sem_wait(&TagReadEventSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( htagReadEvent, 10000 ) )
	{
#endif
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		fprintf(api3Data.fpCurLog,"<br>-------Tag Read Event is Signalled-------<br>");	
		api3Data.rfidStatus = RFID_StopInventory( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			/*fprintf( api3Data.fpCurLog, "<br> TAG ID : %s  TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),TAGEVENTS[pTagData->tagEvent] );
			wprintf(L"\n TAG ID : %ls  TAGEVENT: %ls",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength ),wTAGEVENTS[pTagData->tagEvent] );
			fprintf(api3Data.fpCurLog, "<br> TAG EVENT TIME Day:%d,HOUR:%d,MIN:%d,SEC:%d \n", pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond);*/
			printTagDataWithResults( pTagData);
		}
		SetLogResult(TRUE );
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Tag Read Event is NOT Signalled-------<br>");	
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_READ_EVENT );
	CloseHandle(htagReadEvent);
#endif
}
void TestInvStartEvent( RFID_HANDLE32 hReader )
{
	RFID_EVENT_TYPE rfideventType = INVENTORY_START_EVENT;
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDInvStart,NULL,NULL);
	sem_init(&InvStartSemaphore, 0, 0);		
#else
	// Create and Register Inv Start
	//
	HANDLE hInvStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,rfideventType,hInvStartEvent);
#endif
	wprintf(L"\n Test : %d Inventory Start Event\n",api3Data.dwTestCaseNo);

	// Test Invstart event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Invenetory start Event \n <br> Expected OutPut :Inentory Start Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
#ifdef linux
	if( sem_wait(&InvStartSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( hInvStartEvent, 10000 ) )
	{
#endif
		fprintf(api3Data.fpCurLog,"<br>-------Inv Start Event is Signalled-------<br>");	
		wprintf(L"\n-------Inv Start Event is Signalled-------\n");	
		RFID_StopInventory( hReader );
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
		}
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
		
		SetLogResult(TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inv Start Event is Not Signalled-------<br>");	
		wprintf(L"\n-------Inv Start Event is Not Signalled-------\n");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,INVENTORY_START_EVENT );
	CloseHandle(hInvStartEvent);
#endif
}

void  TestInvStopEvent( RFID_HANDLE32 hReader )
{
	RFID_EVENT_TYPE rfideventType = INVENTORY_STOP_EVENT;
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDInvStop,NULL,NULL);
	sem_init(&InvStopEventSemaphore, 0, 0);		
#else
	// Create and Register Inv Stop
	//
	HANDLE hInvStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,INVENTORY_STOP_EVENT,hInvStopEvent);
#endif
	wprintf(L"\n Test : %d Inventory Stop Event\n",api3Data.dwTestCaseNo);

	// Test Invstop event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Invenetory Stop Event \n <br> Expected OutPut :Inentory Stop Event Should be Signalled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");

	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,NULL,NULL );
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_StopInventory( hReader );
#ifdef linux
	if( sem_wait(&InvStopEventSemaphore) == 0 )
	{
#else	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hInvStopEvent, 10000 ) )
	{
#endif
		fprintf(api3Data.fpCurLog,"<br>-------Inv Stop Event is Signalled-------<br>");	
		wprintf(L"\n-------Inv Stop Event is Signalled-------\n");	
		SetLogResult(TRUE );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inv Stop Event is Not Signalled-------<br>");	
		wprintf(L"\n-------Inv Stop Event is Not Signalled-------\n");	
		RFID_StopInventory( hReader );
		SetLogResult( FALSE );
	}

	logToResultFile(L"CAPI-EVENTS");
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,INVENTORY_STOP_EVENT );
	CloseHandle(hInvStopEvent);
#endif
}

void TestTagEvents( RFID_HANDLE32 hReader, TRIGGER_INFO* ptriggerInfo )
{
	RFID_EVENT_TYPE rfideventType = INVENTORY_STOP_EVENT;
	fprintf(api3Data.fpCurLog, "<br><b>REPORT NEW TAG: %s TAG BACK2VISIBILITY: %s TAG INVISIBLE: %s</b>",TAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportNewTagEvent],
																		  TAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportTagBackToVisibilityEvent],
																		  TAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportTagInvisibleEvent]);
	wprintf(L"\n REPORT NEW TAG: %ls \t TAG BACK2VISIBILITY: %ls \t TAG INVISIBLE: %ls\n",wTAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportNewTagEvent],wTAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportTagBackToVisibilityEvent],
																		  wTAGEVENTREPORTTRIGGER[ptriggerInfo->lpTagEventReportInfo->reportTagInvisibleEvent]);
	
	fprintf( api3Data.fpCurLog, "\n Result is: ");
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	sem_init(&InvStopEventSemaphore, 0, 0);		
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDInvStop,NULL,NULL);
#else
	// Create and Register tag read event handle
	//
	HANDLE hInventoryDone = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,rfideventType,hInventoryDone);
#endif

	// Perform inventory to Start access and inventory
	api3Data.rfidStatus = RFID_PerformInventory( hReader,NULL,&api3Data.pInfo,ptriggerInfo,NULL);
#ifdef linux
	if( sem_wait(&InvStopEventSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( hInventoryDone, 20000 ) )
	{
#endif
		LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		//fprintf(api3Data.fpCurLog,"<br>-------Inventory Stop Event is Signalled-------<br>");	
		//wprintf(L"\n -------Inventory Stop Event is Signalled-------");	
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			/*fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
			wprintf(L"\n TAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );*/

		}

		SetLogResult(TRUE );
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	}
	else
	{
		fprintf(api3Data.fpCurLog,"<br>-------Inventory Stop is NOT Signalled-------<br>");
		wprintf(L"\n -------Inventory Stop Event is Not Signalled-------\n");	
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
	rfid3Sleep( 2000 ); //wait for the inventory to stop.
	RFID_PurgeTags( hReader , 0);

	logToResultFile(L"CAPI-AUTOEVENTS");
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_READ_EVENT );
	CloseHandle(hInventoryDone);
#endif
}

void TestRFID3TagEvents( RFID_HANDLE32 hReader )
{
	TAG_STORAGE_SETTINGS tgSettings;
	tgSettings.maxMemoryBankByteCount = 512;
	tgSettings.maxTagCount = 100;
	tgSettings.maxTagIDByteCount=512;
	tgSettings.enableAccessReports = TRUE;
	tgSettings.tagFields = ANTENNA_ID | FIRST_SEEN_TIME_STAMP| LAST_SEEN_TIME_STAMP | PEAK_RSSI | PC | CRC | TAG_SEEN_COUNT;
	
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings );
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	tInfo.lpTagEventReportInfo = &tagEventInfo;
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 5000;
	tInfo.tagReportTrigger = 0;
	// Test Tag Read Event
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Tag reporting IMMEDIATE for tag events \n <br> Expected OutPut :Tag Data events should report immediatly\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	TestTagEvents( hReader, &tInfo );	
	
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Tag reporting MODERATED for tag events \n <br> Expected OutPut :Tag Data events should be reported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	tagEventInfo.reportNewTagEvent = MODERATED;
	tagEventInfo.reportTagBackToVisibilityEvent = MODERATED;
	tagEventInfo.reportTagInvisibleEvent = MODERATED;
	tInfo.lpTagEventReportInfo->newTagEventModeratedTimeoutMilliseconds = tInfo.lpTagEventReportInfo->tagBackToVisibilityModeratedTimeoutMilliseconds=tInfo.lpTagEventReportInfo->tagInvisibleEventModeratedTimeoutMilliseconds = 1000;
	TestTagEvents( hReader, &tInfo );	
	
	// with NEVER
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Test Tag reporting NEVER for tag events \n <br> Expected OutPut :Dont report any tag events\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);
	tagEventInfo.reportNewTagEvent = tagEventInfo.reportTagBackToVisibilityEvent = tagEventInfo.reportTagInvisibleEvent = NEVER;

	TestTagEvents( hReader, &tInfo );	
	
	// with NEVER 
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a>CAPI-EVENTS-%d:\n\n<br>Description:Test Tag reporting NEVER for reportTagInvisibleEvent \n <br> Expected OutPut :report only new tag, back to visibility tags\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
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
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Verify different event reporting for \n <br> Expected OutPut :Check Log File\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		TestTagEvents( hReader, &tInfo );	
	}

	
	for( int events =0; events<3;events++ )
	{
		// with NEVER 
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Verify different event reporting for \n <br> Expected OutPut :Check Log File\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
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
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Verify different event reporting for \n <br> Expected OutPut :Check Log File\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
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
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-EVENTS-%d></a> CAPI-EVENTS-%d:\n\n<br>Description:Verify different event reporting tag which is not visible now\n <br> Expected OutPut :Tag reporting for invisible status tags\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");

	TestTagEvents( hReader, &tInfo );	
	
	// Clean up the queue.
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0);

}

// Interested Events are 
// RF_SURVEY_START_EVENT
// RF_SURVEY_STOP_EVENT
// RF_SURVEY_DATA_READ_EVENT
//
static void myRFIDEventsCallBackforRFSurveyEvents(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	switch (eventType)
	{
	case RF_SURVEY_START_EVENT:
		bRFSurveyStartEvent = TRUE;
		fprintf(api3Data.fpCurLog, "\n RF_SURVEY_START_EVENT: Triggered <br>\n");
		wprintf(L"\nRF_SURVEY_START_EVENT Triggered");
		break;
	case RF_SURVEY_STOP_EVENT:
		bRFSurveyStopEvent = TRUE;
		fprintf(api3Data.fpCurLog, "\n RF_SURVEY_STOP_EVENT: Triggered <br>\n");
		wprintf(L"\nRF_SURVEY_STOP_EVENT Triggered");
		break;
	case RF_SURVEY_DATA_READ_EVENT:
	{
		bRFSurveyDataEvent = TRUE;
		wprintf(L"\nRF_SURVEY_DATA_READ_EVENT Triggered");
		fprintf(api3Data.fpCurLog, "\n RF_SURVEY_DATA_READ_EVENT: Triggered <br>\n");
		LPRFSURVEY_DATA pSurveyData = RFID_AllocateRFSurvey(api3Data.hReader);
		api3Data.rfidStatus = RFID_GetEventData(api3Data.hReader, RF_SURVEY_DATA_READ_EVENT, (STRUCT_HANDLE)pSurveyData);
		wprintf(L"\n Event Data : AVGpeakRSSI: %ld peakRSSI: %ld bandWidth:%ld frequency:%ld", pSurveyData->averagePeakRSSI, pSurveyData->peakRSSI, pSurveyData->bandWidth, pSurveyData->frequency);
		fprintf(api3Data.fpCurLog, "Event Data : AVGpeakRSSI: %ld peakRSSI: %ld bandWidth:%ld frequency:%ld", pSurveyData->averagePeakRSSI, pSurveyData->peakRSSI, pSurveyData->bandWidth, pSurveyData->frequency);
		api3Data.rfidStatus = RFID_DeallocateRFSurvey(api3Data.hReader, pSurveyData);
	}
	break;
	
	default:
		break;
	}
}

// Test Newly Added Events such as 
void TestRFID3EventsNewAdditions(RFID_HANDLE32 hReader)
{
	RFID_EVENT_TYPE rfidEvents[3] = { RF_SURVEY_DATA_READ_EVENT,RF_SURVEY_START_EVENT,RF_SURVEY_STOP_EVENT };
	RFSURVEY_STOP_TRIGGER rfSurveyStopTrigger;
	rfSurveyStopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	rfSurveyStopTrigger.value.duration = 10000;
	
	api3Data.rfidStatus = RFID_RegisterEventNotificationCallback(api3Data.hReader, rfidEvents, 3, (RfidEventCallbackFunction)myRFIDEventsCallBackforRFSurveyEvents, NULL, NULL);

	for (UINT16 antenna = 1; antenna <= api3Data.rCaps.numAntennas; antenna++)
	{
		bRFSurveyStartEvent = bRFSurveyStopEvent = bRFSurveyDataEvent = FALSE;
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-EVENTS-API-%d></a>CAPI-EVENTS-API-%d:\n\n<br>RFSurvery on Antenna %ld \n <br> Expected OutPut : Start , Stop , DATA EVents should e triggered. \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, antenna);
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : \n ");

		api3Data.rfidStatus = RFID_StartRFSurvey(api3Data.hReader, antenna, &rfSurveyStopTrigger, 860000, 928000, NULL);
		rfid3Sleep(12000);
		api3Data.rfidStatus = RFID_StopRFSurvey(api3Data.hReader);

		if(bRFSurveyStartEvent && bRFSurveyStopEvent && bRFSurveyDataEvent)
		{
			SetLogResult(TRUE);
		}
		else
		{
			SetLogResult(FALSE);
		}

		logToResultFile(L"CAPI-EVENTS");
	}

	//  De-Register Events one by one. as there is no API to deregister all at once.
	// 
	for (int eventIndex = 0; eventIndex < 3; eventIndex++)
	{
		api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, rfidEvents[eventIndex]);
	}
}


void TestRFID3EventsAutomated( RFID_HANDLE32 hReader )
{	
	TestTagDataEvent( hReader );wprintf(L"\n TestTagDataEvent - Done");
	TestTagReadEvent( hReader );wprintf(L"\n TestTagReadEvent - Done");
	TestInvStartEvent( hReader );wprintf(L"\n TestInvStartEvent - Done");
	TestInvStopEvent( hReader );wprintf(L"\n TestInvStopEvent - Done");
	TestAccessStartEventAuto( hReader,FALSE );wprintf(L"\n TestAccessStartEvent - Done");
	TestAccessStopEventAuto ( hReader,FALSE );wprintf(L"\n TestAccessStopEvent - Done");
	TestBufferFullWarningEventAuto( hReader,FALSE );wprintf(L"\n TestBufferFullWarningEvent - Done");
	TestBufferFullEventAuto( hReader,FALSE );wprintf(L"\n TestBufferFullEvent - Done");
	TestRFID3TagEvents( hReader );wprintf(L"\n TestRFID3TagEvents - Done");
}

void TestingEvents( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestingEvents");
	logCurrentExecution("Testing Events");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 26;
	TestRFID3EventsAutomated( api3Data.hReader );
	TestRFID3EventsAutoMode ( api3Data.hReader );
	//TestRFID3EventsNewAdditions(api3Data.hReader);
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
	
