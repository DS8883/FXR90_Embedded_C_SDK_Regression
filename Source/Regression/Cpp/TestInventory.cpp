#include "Common.h"
#include "APITest.h"
#include <stdio.h>
#include <math.h>

extern struct RFID3DATA api3Data;
extern struct HANDLE_LIST hList;


double getTimeStampDifference( LPTAG_DATA pTagData )
 {
	 double timediff = 0.0;
	 timediff = ( pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour * 3600 +  pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute * 60 +  pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond +  (pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds / 1000.0) ) - 
		 ( pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour * 3600 +  pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute * 60 +  pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond +  (pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds / 1000.0) );
	 return fabs(timediff);
 }

#ifdef linux
sem_t InventoryStopSemaphore;	// Access Stop Semaphore
// Access Stop event Handler
static void MonitorAccessSopEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	wprintf(L"\n monitor stop event function- post event");
	sem_post(&InventoryStopSemaphore);
}
#else
HANDLE rfidhandle2StopEvent;
#endif

/*********Inventory with Triggers**
STOP_TRIGGER_TYPE_IMMEDIATE = 0, Stop immediately when RFID_StopInventory or RFID_StopAccessSequence is called
STOP_TRIGGER_TYPE_DURATION,  Stop after a specified duration: STOP_TRIGGER::triggerValue
STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT, Stop after a specified GPI Timeout: STOP_TRIGGER::triggerValue
STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT, Stop after a specified number of Tags are read: STOP_TRIGGER::triggerValue
STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT  Stop after a specified number of attempts: STOP_TRIGGER::triggerValue
	**********************************/	
int TestInventroyWithTriggers( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo )
{
	UINT16 antennaID = 0;
	BOOLEAN bTagsRead = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description: Inventory without Triggers( Trigger Info is NULL) \n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	api3Data.rfidStatus = RFID_PerformInventory( hReader, NULL, pInfo, NULL, NULL);	
	rfid3Sleep( 2000); //wait for the Inventory to finish	
	api3Data.rfidStatus =  RFID_StopInventory( hReader );
#ifdef linux
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 1: Start :Immediate , Stop: Immediate \n");
			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData ))
			{
				//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ));
				printTagDataWithResults( pTagData);
				antennaID = pTagData->antennaID;
				bTagsRead= TRUE;
			}
		}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	

	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description: Obervation Trigger(TagObservation =10) \n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(TRIGGER_INFO));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	tInfo.stopTrigger.value.tagObservation.n = 10; // Test with Two Tags in the field
	tInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	tInfo.tagReportTrigger = 1;
	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		tInfo.lpTagEventReportInfo = NULL;
	}
	api3Data.rfidStatus = RFID_PerformInventory( hReader, NULL, pInfo,&tInfo, NULL);
	UINT16 tagSeenCount = 0;
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 2: Start : Imm, Stop: TagO( 10 )TI( 3000 )\n");
			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData ))
			{
				tagSeenCount +=  pTagData->tagSeenCount;
				//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ));
				printTagDataWithResults( pTagData);
			}
		}

	if( tagSeenCount == 10 )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");

	// inventory with attempts
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:Inventory with Attempts- (Number of attempts = 3)\n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT;
	tInfo.stopTrigger.value.numAttempts.n = 3;/*Attempt 2 times*/
	tInfo.stopTrigger.value.numAttempts.timeoutMilliseconds = 3000;
	tInfo.tagReportTrigger = 0;
	
	UINT16 ant[] = { antennaID };
	ANTENNA_INFO pantInfo;
	memset(&pantInfo,0,sizeof(ANTENNA_INFO));
	pantInfo.pAntennaList = ant;
	pantInfo.length = 1;
	pantInfo.pAntennaOpList = NULL ;
	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, &pantInfo,&tInfo, NULL);
	UINT16 tagsRead = 0;
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 3: Start : Imm, Stop: NAttemt( 3 )TI( 3000 )\n");
			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
			{
				printTagDataWithResults( pTagData);
				if( pTagData->tagSeenCount == 3 )
				{
					SetLogResult( TRUE );
				}
				else
				{
					SetLogResult( FALSE );
				}

				//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ) );
			}
		}

	logToResultFile(L"CAPI-TRIGGERS");
	RFID_PurgeTags(hReader,0);
	
	// Start --> immediate ; Stop---> duration
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:Inventory with Duration (Duration = 3Seconds)\n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 3000;
	tInfo.tagReportTrigger = 0;	
	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		tInfo.lpTagEventReportInfo = NULL;
	}
	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 4: Start : Imm, Stop: Duration( 3000 )\n");

			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
			{
				bTagsRead = TRUE;
				printTagDataWithResults( pTagData);
				wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
						pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
						pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
				if( getTimeStampDifference( pTagData ) > 4 )
				{
					bTagsRead = FALSE;
					break;
				}
			}
		}
	
	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");

	// Start --> immediate ; Stop---> GPI with time out
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:GPI Event( No GPI Event , So time out in 3Sec)\n <br> Expected OutPut :Inv Stop upon GPI event or Time Out\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	// Fill stop trigger info
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT;
	tInfo.stopTrigger.value.gpi.gpiEvent= TRUE;
	tInfo.stopTrigger.value.gpi.portNumber = 1;
	tInfo.stopTrigger.value.gpi.timeoutMilliseconds = 3000;
	
	api3Data.rfidStatus = RFID_EnableGPIPort( hReader, 2,TRUE );

	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 5: Start : Imm, Stop: GPI ,TO( 3000 )\n");				
			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
			{
				bTagsRead = TRUE;
				printTagDataWithResults( pTagData);
				wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
						pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
						pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
				if( getTimeStampDifference( pTagData ) > 4 )
				{
					bTagsRead = FALSE;
					break;
				}
			}
		}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	
	// Start --> immediate ; Stop---> HH PRESS or timeout
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:GPI Event( No GPI Event , So time out in 3Sec)\n <br> Expected OutPut :Tags should be Read upon GPI event\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	// Fill stop trigger info
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT;
	tInfo.stopTrigger.value.handheld.handheldEvent = HANDHELD_TRIGGER_PRESSED;
	tInfo.stopTrigger.value.handheld.timeoutMilliseconds = 3000;

	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 6: Start : Imm, Stop: HH ,TO( 3000 )\n");				
			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
			{
				bTagsRead = TRUE;
				printTagDataWithResults( pTagData);
				wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
						pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
						pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
				if( getTimeStampDifference( pTagData ) > 4 )
				{
					bTagsRead = FALSE;
					break;
				}
			}
		}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	
	// Start --> immediate ; Stop---> HH PRESS or timeout
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:GPI Event( No GPI Event , So time out in 3Sec)\n <br> Expected OutPut :Tags should be Read upon GPI event\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	// Fill stop trigger info
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT;
	tInfo.stopTrigger.value.handheld.handheldEvent = HANDHELD_TRIGGER_RELEASED;
	tInfo.stopTrigger.value.handheld.timeoutMilliseconds = 3000;

	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
#ifdef linux
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&InventoryStopSemaphore,&ts) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( rfidhandle2StopEvent, 10000 ) )
		{
#endif
			wprintf(L"\n Inventory Stop Event is received for Test Case 7: Start : Imm, Stop: HH ,TO( 3000 )\n");	
			while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
			{
				bTagsRead = TRUE;
				printTagDataWithResults( pTagData);
				wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
						pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
						pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
				if( getTimeStampDifference( pTagData ) > 4 )
				{
					bTagsRead = FALSE;
					break;
				}
			}
		}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	api3Data.rfidStatus = RFID_EnableGPIPort( hReader, 2,FALSE );

	if( pTagData != NULL ) RFID_DeallocateTag( hReader, pTagData );
	return TRUE;	
}



/*********Inventory with Triggers**
STOP_TRIGGER_TYPE_IMMEDIATE = 0, Stop immediately when RFID_StopInventory or RFID_StopAccessSequence is called
STOP_TRIGGER_TYPE_DURATION,  Stop after a specified duration: STOP_TRIGGER::triggerValue
STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT, Stop after a specified GPI Timeout: STOP_TRIGGER::triggerValue
STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT, Stop after a specified number of Tags are read: STOP_TRIGGER::triggerValue
STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT  Stop after a specified number of attempts: STOP_TRIGGER::triggerValue
	**********************************/	
BOOLEAN TestInventroyWithTriggersAutoMode( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo  )
{
	if (!api3Data.rCaps.tagEventReportingSupported)
	{
		wprintf(L"\n autonomous mode events are not supported\n");
		return FALSE;
	}

	UINT16 antennaID = 0;
	BOOLEAN bTagsRead = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
		
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description: Obervation Trigger in AutoMode(TagObservation =10) \n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	RFID_PurgeTags(hReader,0);
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));

	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	tInfo.stopTrigger.value.tagObservation.n = 10; // Test with Two Tags in the field
	tInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 5000;
	tInfo.tagReportTrigger = 1;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	tInfo.lpTagEventReportInfo = &tagEventInfo;
	
	api3Data.rfidStatus = RFID_PerformInventory( hReader, NULL, pInfo,&tInfo, NULL);
	rfid3Sleep(6000);
	UINT16 tagSeenCount = 0;
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData ))
	{
		tagSeenCount +=  pTagData->tagSeenCount;
		//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ));
		printTagDataWithResults( pTagData);
		antennaID = pTagData->antennaID;
	}

	if( tagSeenCount == 10 )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");

	// inventory with attempts
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:Inventory with Attempts-in Autonomous Mode (Number of attempts = 3)\n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT;
	tInfo.stopTrigger.value.numAttempts.n = 3;/*Attempt 2 times*/
	tInfo.stopTrigger.value.numAttempts.timeoutMilliseconds = 3000;
	tInfo.tagReportTrigger = 0;
	
	UINT16 ant[] = { antennaID };
	ANTENNA_INFO pantInfo;
	pantInfo.pAntennaList = ant;
	pantInfo.length = 1;
	pantInfo.pAntennaOpList = NULL ;
	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, &pantInfo,&tInfo, NULL);
	rfid3Sleep(5000);
	
	UINT16 tagsRead = 0;
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		if(pTagData->antennaID == antennaID )
		{
			tagsRead++;
		}

		printTagDataWithResults( pTagData);
		//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ) );
	}

	if( tagsRead >= 3 )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	logToResultFile(L"CAPI-TRIGGERS");
	RFID_PurgeTags(hReader,0);
	
	// Start --> immediate ; Stop---> duration
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:Inventory with Duration in AutonomousMode(Duration = 3Seconds)\n <br> Expected OutPut :Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 3000;
	tInfo.tagReportTrigger = 0;	
	
	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
	rfid3Sleep(5000);
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		bTagsRead = TRUE;
		printTagDataWithResults( pTagData);
		wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
				pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
		if( getTimeStampDifference( pTagData ) > 4 )
		{
			bTagsRead = FALSE;
			break;
		}
	}
	
	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");

	// Start --> immediate ; Stop---> GPI with time out
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:GPI Event in Autonomous Mode( No GPI Event , So time out in 3Sec)\n <br> Expected OutPut :Inv Stop upon GPI event or Time Out\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	// Fill stop trigger info
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT;
	tInfo.stopTrigger.value.gpi.gpiEvent= TRUE;
	tInfo.stopTrigger.value.gpi.portNumber = 1;
	tInfo.stopTrigger.value.gpi.timeoutMilliseconds = 3000;
	
	api3Data.rfidStatus = RFID_EnableGPIPort( hReader, 2,TRUE );

	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
	rfid3Sleep(5000);				
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		bTagsRead = TRUE;
		printTagDataWithResults( pTagData);
		wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
				pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
		if( getTimeStampDifference( pTagData ) > 4 )
		{
			bTagsRead = FALSE;
			break;
		}
	}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	
	// Start --> immediate ; Stop---> HH PRESS or timeout
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:GPI Event in Autonomous Mode( No GPI Event , So time out in 3Sec)\n <br> Expected OutPut :Tags should be Read upon GPI event\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	// Fill stop trigger info
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT;
	tInfo.stopTrigger.value.handheld.handheldEvent = HANDHELD_TRIGGER_PRESSED;
	tInfo.stopTrigger.value.handheld.timeoutMilliseconds = 3000;

	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
	rfid3Sleep(5000);
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		bTagsRead = TRUE;
		printTagDataWithResults( pTagData);
		wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
				pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
		if( getTimeStampDifference( pTagData ) > 4 )
		{
			bTagsRead = FALSE;
			break;
		}
	}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	
	// Start --> immediate ; Stop---> HH PRESS or timeout
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRIGGERS-%d></a> CAPI-TRIGGERS-%d:\n\n<br>Description:GPI Event in Autonomous Mode( No GPI Event , So time out in 3Sec)\n <br> Expected OutPut :Tags should be Read upon GPI event\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	// Fill stop trigger info
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_HANDHELD_WITH_TIMEOUT;
	tInfo.stopTrigger.value.handheld.handheldEvent = HANDHELD_TRIGGER_RELEASED;
	tInfo.stopTrigger.value.handheld.timeoutMilliseconds = 3000;

	api3Data.rfidStatus = RFID_PerformInventory(hReader, NULL, pInfo,&tInfo, NULL);
	rfid3Sleep(5000);
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		bTagsRead = TRUE;
		printTagDataWithResults( pTagData);
		wprintf(L"\nFST - %2d/%2d/%2d %02d:%02d:%2d:%03d LST -%2d/%2d/%2d %02d:%02d:%2d:%03d",
				pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
				pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth, pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds);		
		if( getTimeStampDifference( pTagData ) > 4 )
		{
			bTagsRead = FALSE;
			break;
		}
	}

	if( bTagsRead )
	{
		SetLogResult( TRUE);
		bTagsRead = FALSE;
	}	
	RFID_PurgeTags(hReader,0);
	logToResultFile(L"CAPI-TRIGGERS");
	api3Data.rfidStatus = RFID_EnableGPIPort( hReader, 2,FALSE );

	if( pTagData != NULL ) RFID_DeallocateTag( hReader, pTagData );
	return TRUE;	
}

void TestingTriggers( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestingTriggers");
	logCurrentExecution("Inventory with TRIGGGERS");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 13;
	RFID_EVENT_TYPE rfideventType = INVENTORY_STOP_EVENT;
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorAccessSopEvent,NULL,NULL);
	sem_init(&InventoryStopSemaphore, 0, 0);		
#else
	// Create and Register Inv Stop
	//
	rfidhandle2StopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT,rfidhandle2StopEvent);
#endif

	TestInventroyWithTriggers( api3Data.hReader, &api3Data.pInfo );
	TestInventroyWithTriggersAutoMode(api3Data.hReader, &api3Data.pInfo );

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
#ifdef linux
	sem_destroy(&InventoryStopSemaphore);	// Destroy Semaphore
#else
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT); 
	CloseHandle(rfidhandle2StopEvent);
#endif
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
