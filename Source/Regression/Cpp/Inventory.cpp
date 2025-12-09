// Inventory.cpp : Verifies inventory related parameters and functionality
//
#include "Common.h"

extern struct RFID3DATA api3Data;	// RFID3 Data
extern MYTAGDATA MyTags[ 2 ];		// Tag Data for automation

// Semaphore for linux support and Events in linux. For windows we can use regular windows supported events.
//
#ifdef linux
static sem_t InventoryStopSemaphore;
static void MonitorRFIDLinuxEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&InventoryStopSemaphore);
}
#endif

// Inventory with post filter in autonomous mode( tag events )
//
void TestRFID_ReadAUTO( READ_ACCESS_PARAMS rParams, POST_FILTER postfilter )					
{
	RFID_EVENT_TYPE rfideventType = INVENTORY_STOP_EVENT;		// the Event we looking for
	TRIGGER_INFO triggerInfo;									// trigger info
	memset(&triggerInfo,0,sizeof(triggerInfo));
	triggerInfo.tagReportTrigger	= 1;						// Report after the Stop 
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;	// Imm Start
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;	// Imm Stop
#ifndef PIXIE
	triggerInfo.lpExtraTriggerInfo = NULL;
#endif // !PIXIE
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000}; // Tag event info initialization
	UINT32 tagCount =0;											// Reset Tag Count
	triggerInfo.lpTagEventReportInfo = &tagEventInfo;		
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	
	// linux mechanism of call back and eventing
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDLinuxEvent,NULL,NULL);
	sem_init(&InventoryStopSemaphore, 0, 0);		
#else
	// Inventory Stop event( windows ), and registering the event that we looking for
	HANDLE hStopInventory = CreateEvent (NULL, FALSE, FALSE, NULL );	
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,rfideventType,hStopInventory );
#endif

	for( int matchPattern = A_AND_B; matchPattern <= A_AND_NOTB; matchPattern++ )
	{
		tagCount = 0;
		switch( matchPattern )
		{

			case A_AND_B:
				fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_PerformInventory()\n  Expected OutPut :Tags match with Pattern A and Pattern B Should be Read(No Tags)\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
				fprintf( api3Data.fpCurLog,"<br>");
				fprintf( api3Data.fpCurLog, "\n Result is : \n ");
				fprintf( api3Data.fpCurLog,"<br>");
				break;
			case NOTA_AND_B:
				fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_PerformInventory()\n  Expected OutPut :Tags match with Pattern NOT A and B Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
				fprintf( api3Data.fpCurLog,"<br>");
				fprintf( api3Data.fpCurLog, "\n Result is : \n ");
				fprintf( api3Data.fpCurLog,"<br>");
				break;
			case NOTA_AND_NOTB:
				fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_PerformInventory()\n  Expected OutPut :Tags match with NOT A and NOT B Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
				fprintf( api3Data.fpCurLog,"<br>");
				fprintf( api3Data.fpCurLog, "\n Result is : \n ");
				fprintf( api3Data.fpCurLog,"<br>");
				break;
			case A_AND_NOTB:
				fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_PerformInventory()\n  Expected OutPut :Tags match with Pattern A and NOT B Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
				fprintf( api3Data.fpCurLog,"<br>");
				fprintf( api3Data.fpCurLog, "\n Result is : \n ");
				fprintf( api3Data.fpCurLog,"<br>");
			break;	
			default:

				fprintf( api3Data.fpLog,"\n Invalid Match Pattern\n");
				break;
		}

		postfilter.matchPattern = ( MATCH_PATTERN )matchPattern;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5)
		{
			postfilter.lpRSSIRangeFilter = NULL;
		}

		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postfilter,&api3Data.pInfo,&triggerInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		// Wait for the event to be signalled and time out if not signalled.
		// A time out is taken as error condition or failure
		//
#ifdef linux
		wprintf(L"\n waiting for the semaphore event to be triggered\n");
		sem_wait(&InventoryStopSemaphore);
#else
		WaitForSingleObject( hStopInventory, 7000 );
#endif
		bool bSuccess = TRUE;
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			tagCount++;
			printTagDataWithResults( pTagData );
			switch( matchPattern )
			{
				case NOTA_AND_B:
					if (rParams.memoryBank == MEMORY_BANK_EPC || rParams.memoryBank == MEMORY_BANK_USER)
					{
						if (pTagData->pTagID[0] != 0xB2 && pTagData->pTagID[1] != 0x2F)
						{
							bSuccess = FALSE;
						}
					}
					break;
				case A_AND_NOTB:
					if (rParams.memoryBank == MEMORY_BANK_EPC || rParams.memoryBank == MEMORY_BANK_USER)
					{
						if (pTagData->pTagID[0] != 0xA2 && pTagData->pTagID[1] != 0x2F)
						{
							bSuccess = FALSE;
						}
					}
					if (rParams.memoryBank == MEMORY_BANK_TID )
					{
						if (pTagData->pTagID[0] != 0xDA && pTagData->pTagID[1] != 0xDA)
						{
							bSuccess = FALSE;
						}
					}
					break;
				default:
					bSuccess = TRUE;
					break;
			}
			if(bSuccess == FALSE )
			{
				break;
			}
		}

		if(tagCount == 0 && matchPattern != A_AND_B )
		{
			bSuccess = FALSE;
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-POSTFILTERS");
	}
#ifdef linux
	sem_destroy(&InventoryStopSemaphore);
#else
	RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT );
	CloseHandle( hStopInventory );
#endif
	RFID_PurgeTags( api3Data.hReader,0);
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
}

// Access with post filter
//
void TestRFID_Read( READ_ACCESS_PARAMS rParams, ACCESS_FILTER aFilter )					
{
	UINT32 tagCount = 0;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	for( int matchPattern = A_AND_B; matchPattern <= A_AND_NOTB; matchPattern++ )
	{
		switch( matchPattern )
		{
		case A_AND_B:
			fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_Read()\n  Expected OutPut :Tags match with Pattern A and Pattern B Should be Read(NO Tags)\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
			fprintf( api3Data.fpCurLog,"<br>");
			fprintf( api3Data.fpCurLog, "\n Result is : \n ");
			fprintf( api3Data.fpCurLog,"<br>");
				break;
		case NOTA_AND_B:
			fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_Read()\n  Expected OutPut :Tags match with Pattern NOT A and Only B Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
			fprintf( api3Data.fpCurLog,"<br>");
			fprintf( api3Data.fpCurLog, "\n Result is : \n ");
			fprintf( api3Data.fpCurLog,"<br>");
				break;
		case NOTA_AND_NOTB:
			fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_Read()\n  Expected OutPut :Tags match with NOT A and NOT B Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
			fprintf( api3Data.fpCurLog,"<br>");
			fprintf( api3Data.fpCurLog, "\n Result is : \n ");
			fprintf( api3Data.fpCurLog,"<br>");
				break;
		case A_AND_NOTB:
			fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-POSTFILTERS-%d></a> CAPI-POSTFILTERS-%d:\n\n<br>Description: Testing Post Filters with RFID_Read()\n  Expected OutPut :Tags match with Pattern only A and NOT B Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
			fprintf( api3Data.fpCurLog,"<br>");
			fprintf( api3Data.fpCurLog, "\n Result is : \n ");
			fprintf( api3Data.fpCurLog,"<br>");
				break;	
			default:
				fprintf( api3Data.fpLog,"\n Invalid Match Pattern\n");
				break;
		}
		
		aFilter.matchPattern = ( MATCH_PATTERN )matchPattern;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}
		api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		api3Data.rfidStatus = RFID_Read( api3Data.hReader,0,0,&rParams,&aFilter,&api3Data.pInfo,pTagData,0);
		rfid3Sleep( 3000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
		bool bSuccess = FALSE;
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			tagCount++;
			printTagDataWithResults( pTagData );
			switch( matchPattern )
			{
				case A_AND_B:
					if( pTagData->opStatus != ACCESS_SUCCESS )
					{
						bSuccess = TRUE;
					}
					break;
				case NOTA_AND_B:
				case A_AND_NOTB:
						if(pTagData->opStatus == ACCESS_SUCCESS)
						{
							bSuccess = TRUE;
						}
					break;
				default:
					if( pTagData->opStatus == ACCESS_SUCCESS )
					{
						bSuccess = TRUE;
					}
					break;
			}

			if(bSuccess == TRUE )
			{
				break;
			}
		}

		if(tagCount == 0 && matchPattern == A_AND_B )
		{
			bSuccess = TRUE;
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-POSTFILTERS");
	}
	
	RFID_PurgeTags( api3Data.hReader,0);
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
}

// VERIFY TRUNCATE ACTION( This verfiy the check if truncate is supported as returned from capabilites)
// 
void TestG2Truncate( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestG2Truncate");
	logCurrentExecution( "TestG2Truncate");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 41;		// Test Case Count
	api3Data.dwTestCaseNo = 01;		// Initial Count
	BOOLEAN bSuccess = TRUE;
	BOOLEAN bPCSevenTag = FALSE;
	PRE_FILTER pf;
	UINT8 pattern[14];				// Pattern lenght 14 bytes: This is to include 12 EPC  + 2 PC Bytes
	UINT32 index;					// pre filter index
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	SINGULATION_CONTROL s;

	// Get the tag if there is one with larger length than 96 bit.
	//
	//api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);

	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_AB_FLIP, SL_ALL);
	api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);

	LPTAG_DATA pcSevenTag = RFID_AllocateTag(api3Data.hReader);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	while ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
	{
		if (pTagData->PC >= 0x7000)
		{
			pcSevenTag->pTagID[0] = HIBYTE(pTagData->PC);	// modify the tag data to take care of PC
			pcSevenTag->pTagID[1] = LOBYTE(pTagData->PC);	//
			pcSevenTag->tagIDLength = pTagData->tagIDLength;
			memcpy(&(pcSevenTag->pTagID[2]), pTagData->pTagID, pTagData->tagIDLength);
			bPCSevenTag = TRUE;
			break;
		}
	}

	// Filling the pattern with Known tags in the field.
	//
	pattern[ 0 ] = HIBYTE( MyTags[ 0 ].PC );
	pattern[ 1 ] = LOBYTE( MyTags[ 0 ].PC );
	pattern[ 2 ]= MyTags[0].epc[ 0 ];
	pattern[ 3 ]= MyTags[0].epc[ 1 ];
	
	
	
	// Adding pre filter with truncate action unspecified
	//
	pf.bitOffset = 16 ;pf.filterAction = FILTER_ACTION_STATE_AWARE;
	pf.filterActionParams.stateAwareParams.target = TARGET_SL;
	pf.filterActionParams.stateAwareParams.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
	pf.memoryBank = MEMORY_BANK_EPC;pf.pTagPattern = pattern;pf.tagPatternBitCount = 32;
	pf.truncateAction = TRUNCATE_ACTION_UNSPECIFIED;
	api3Data.rfidStatus = RFID_AddPreFilter( api3Data.hReader, 0,&pf,&index );
	
	// Initialize singulation Control Parameters
	//
	
	InitSingulationControl( &s,SESSION_S0,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );

	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d -TEST TRUNCATE_ACTION_UNSPECIFIED\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_UNSPECIFIED \n <br> Expected OutPut :Tags should not be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 3000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
	rfid3Sleep( 1000 );
	bSuccess = FALSE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		// If Truncate action is TRUNCATE_ACTION_UNSPECIFIED the PC should not be ZERO
		//
		if(pTagData->PC !=  0x00 )	
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
			break;
		}
		printTagDataWithResults(pTagData);
	}
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-TRUNC");

	RFID_DeletePreFilter( api3Data.hReader,0,0 );

	// If Truncate action is TRUNCATE_ACTION_DO_NOT_TRUNCATE the PC should not be ZERO
	//
	wprintf(L"\n %d -TEST TRUNCATE_ACTION_DO_NOT_TRUNCATE\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_DO_NOT_TRUNCATE \n <br> Expected OutPut :Tags should not be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	pf.truncateAction = TRUNCATE_ACTION_DO_NOT_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( api3Data.hReader, 0,&pf,&index );
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 3000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
	rfid3Sleep( 1000 );
	bSuccess = FALSE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		if(pTagData->PC !=  0x00 )
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
			break;
		}
		printTagDataWithResults(pTagData);
	}
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-TRUNC");
	RFID_DeletePreFilter( api3Data.hReader,0,0 );

	// If Truncate action is TRUNCATE_ACTION_TRUNCATE the PC should  be ZERO
	//
	wprintf(L"\n %d -TEST TRUNCATE_ACTION_TRUNCATE\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_TRUNCATE \n <br> Expected OutPut :Tags should be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	pf.truncateAction = TRUNCATE_ACTION_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( api3Data.hReader, 0,&pf,&index );
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 3000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	rfid3Sleep( 1000 );
	bSuccess = FALSE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		printTagDataWithResults(pTagData);
		if(pTagData->PC == 0x00)
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
			break;
		}
	}
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-TRUNC");	

	for(int Session = 1;Session<=3;Session++)
	{
		wprintf(L"\n %d -TEST TRUNCATE_ACTION_TRUNCATE in %ls\n",api3Data.dwTestCaseNo,wSESSIONS[Session]);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: Truncate in %s \n <br> Expected OutPut :Tags should be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,SESSIONS[Session]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		s.session = (SESSION)Session;
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
		rfid3Sleep( 3000 );	
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
		rfid3Sleep( 1000 );

		bSuccess = FALSE;
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
		{		
			printTagDataWithResults(pTagData);
			if(pTagData->PC == 0x00)
			{
				bSuccess = TRUE;
			}
			else
			{
				bSuccess = FALSE;
				break;
			}
		}

		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		if ( bSuccess )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-TRUNC");	
	}
	
	// Truncate with varying the pattern lenghts( Pattern bit Count 64 bits)
	//
	pattern[4] = MyTags[0].epc[2];pattern[5] = MyTags[0].epc[3];
	pattern[6] = MyTags[0].epc[4];pattern[7] = MyTags[0].epc[5];
	RFID_DeletePreFilter( api3Data.hReader,0,0 );
	wprintf(L"\n %d -TEST TRUNCATE_ACTION_TRUNCATE\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_TRUNCATE \n <br> Expected OutPut :Tags should be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	pf.bitOffset = 16 ;pf.filterAction = FILTER_ACTION_STATE_AWARE;
	pf.filterActionParams.stateAwareParams.target = TARGET_SL;
	pf.filterActionParams.stateAwareParams.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
	pf.memoryBank = MEMORY_BANK_EPC;pf.pTagPattern = pattern;pf.tagPatternBitCount = 64;
	pf.truncateAction = TRUNCATE_ACTION_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( api3Data.hReader, 0,&pf,&index );
	s.session = SESSION_S0;
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 3000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
	rfid3Sleep( 1000 );
	bSuccess = FALSE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		printTagDataWithResults(pTagData);
		if(pTagData->PC == 0x00)
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
			break;
		}
	}
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-TRUNC");
	
	// Truncate with varying the pattern lenghts( Pattern bit Count 96 bits)
	//
	pattern[8] = MyTags[0].epc[6];pattern[9] = MyTags[0].epc[7];
	pattern[10] = MyTags[0].epc[8];pattern[11] = MyTags[0].epc[9];
	
	RFID_DeletePreFilter( api3Data.hReader,0,0 );
	wprintf(L"\n %d -TEST TRUNCATE_ACTION_TRUNCATE\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_TRUNCATE \n <br> Expected OutPut :Tags should be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	pf.bitOffset = 16 ;pf.filterAction = FILTER_ACTION_STATE_AWARE;
	pf.filterActionParams.stateAwareParams.target = TARGET_SL;
	pf.filterActionParams.stateAwareParams.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
	pf.memoryBank = MEMORY_BANK_EPC;pf.pTagPattern = pattern;pf.tagPatternBitCount = 96;
	pf.truncateAction = TRUNCATE_ACTION_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( api3Data.hReader, 0,&pf,&index );
	s.session = SESSION_S0;
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 3000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	rfid3Sleep( 1000 );
	bSuccess = FALSE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		printTagDataWithResults(pTagData);
		if(pTagData->PC == 0x00)
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
			break;
		}
	}
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-TRUNC");
	RFID_DeletePreFilter(api3Data.hReader, 0, 0);


	// Truncate with varying the pattern lenghts( Pattern bit Count 112 bits( PC 16 + EPC 96 ))
	// This test case matches the Mask exaclty ending on the last but of EPC
	//
	pattern[12] = MyTags[0].epc[10];pattern[13] = MyTags[0].epc[11];
	
	wprintf(L"\n %d -TEST TRUNCATE_ACTION_TRUNCATE\n",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_TRUNCATE \n <br> Expected OutPut :Tags should be truncated \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	pf.bitOffset = 16 ;pf.filterAction = FILTER_ACTION_STATE_AWARE;
	pf.filterActionParams.stateAwareParams.target = TARGET_SL;
	pf.filterActionParams.stateAwareParams.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
	pf.memoryBank = MEMORY_BANK_EPC;pf.pTagPattern = pattern;pf.tagPatternBitCount = 112;
	pf.truncateAction = TRUNCATE_ACTION_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( api3Data.hReader, 0,&pf,&index );
	s.session = SESSION_S0;
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 3000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
	bSuccess = FALSE;

	// A Truncate reply always come with PC as Zero, accompanied with the truncated reply.
	// The verification below does the check on PC field only. Any furthur check on truncated reply
	// to be verified manually.
	//
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		printTagDataWithResults(pTagData);
		if(pTagData->PC == 0x00)
		{
			bSuccess = TRUE;
		}
		else
		{
			bSuccess = FALSE;
			break;
		}
	}
	
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-TRUNC");

	api3Data.rfidStatus = RFID_DeletePreFilter( api3Data.hReader,0,0 );
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);
	
	if (!bPCSevenTag)
	{
		wprintf(L"\n No Tag Read with PC > 7000. Skipping the Rest of the TestCases\n");
		return;
	}

	//iterate through different mask lengths and check the reply from the reader
	//
	for (UINT8 Session = 0; Session <= 1; Session++)
	{
		for (int bitcount = 16; bitcount <= pcSevenTag->tagIDLength * 8 + 16; bitcount = bitcount + 16)
		{
			wprintf(L"\n %d -TEST TRUNCATE_ACTION_TRUNCATE in %ls maskBitCnt %ld \n\n", api3Data.dwTestCaseNo, wSESSIONS[Session],bitcount);
			fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-TRUNC-%d></a> CAPI-TRUNC-%d:\n\n<br>Description: TRUNCATE_ACTION_TRUNCATE with Session-->%ld\n <br> Expected OutPut :Tags should be truncated \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, Session);
			fprintf(api3Data.fpCurLog, "<br>");
			fprintf(api3Data.fpCurLog, "\n Result is : \n ");

			// Adding pre filter with truncate action unspecified
			//
			pf.bitOffset = 16; pf.filterAction = FILTER_ACTION_STATE_AWARE;
			pf.filterActionParams.stateAwareParams.target = TARGET_SL;
			pf.filterActionParams.stateAwareParams.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
			pf.memoryBank = MEMORY_BANK_EPC; pf.pTagPattern = pcSevenTag->pTagID; pf.tagPatternBitCount = bitcount;
			pf.truncateAction = TRUNCATE_ACTION_TRUNCATE;
			//pf.truncateAction = TRUNCATE_ACTION_DO_NOT_TRUNCATE;


			api3Data.rfidStatus = RFID_AddPreFilter(api3Data.hReader, 0, &pf, &index);

			s.session = (SESSION)Session; s.stateAwareSingulationAction.inventoryState = INVENTORY_STATE_A;
			s.stateAwareSingulationAction.slFlag = SL_FLAG_ASSERTED;
			s.stateAwareSingulationAction.perform = TRUE;

			api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
			api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
			rfid3Sleep(5000);
			api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
			bSuccess = FALSE;

			// A Truncate reply always come with PC as Zero, accompanied with the truncated reply.
			// The verification below does the check on PC field only. Any furthur check on truncated reply
			// to be verified manually.
			//
			while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
			{
				printTagDataWithResults(pTagData);
				if (pTagData->PC == 0x00)
				{
					bSuccess = TRUE;
					break;
				}
				else
				{
					bSuccess = FALSE;
					
				}
			}

			api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);

			if (bSuccess)
			{
				SetLogResult(TRUE);
			}

			logToResultFile(L"CAPI-TRUNC");

			RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		}
	}

	if (pTagData->memoryBankDataAllocated)
	{
		RFID_DeallocateTag(api3Data.hReader, pTagData);
		pTagData = NULL;
	}
	if (pcSevenTag->memoryBankDataAllocated)
	{
		RFID_DeallocateTag(api3Data.hReader, pTagData);
		pcSevenTag = NULL;
	}

	fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount + api3Data.dwFailCount));
	LogPassFailCount();
	wprintf(L"\n Executing %ls Complete",MultiByte2WideChar( __FUNCTION__));
}

void Test_PostFiltersRead( void )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestRFID3PostFilters-With RFID_Read");
	logCurrentExecution( "TestRFID3PostFilters-With RFID_Read");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	UINT16 dataLen;	// MAsk length

	// Initialize mask( bigger mask to account for larger lengths ).
	// Since mask can be varied by giving the address with in the larger mask
	//
	UINT8 TagMask[512] = {0xFF,};	
	api3Data.itemTestCount = 112;	// No of Tests
	api3Data.dwTestCaseNo = 01;		// Init the test Count
	
	// Iterate through from lenght varying from 16 bits to 96 bits with the increment length of 16 bits.
	//
	for( dataLen = 16;dataLen <= 96; dataLen = dataLen+16 )
	{
		// Test Read with EPC Mem Bank  with FIRST TWO BYTES with OFF Set 0;
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with TagEPC Pattern-A %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[0].epc,0,dataLen/8 ),dataLen/8 );
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with TagEPC Pattern-B %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[1].epc,0,dataLen/8 ),dataLen/8 );
		
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} }; 
		TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,MyTags[0].epc,dataLen,TagMask,dataLen,{0} };
		TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,MyTags[1].epc,dataLen,TagMask,dataLen,{0} };
		ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };
		TestRFID_Read( rParams,accessFilter );		
	}

	for (UINT16 dataLen = 16; dataLen <= 96; dataLen = dataLen + 16)
	{
		// Test Read with TID Mem Bank  with Varying offset with length Fixed
		fprintf(api3Data.fpCurLog, "\n\nTesting PostFilters with TID Pattern-A %s with OFFSET 0 and Datalength :%d", Array2Char(api3Data.pG2V2Tag->pMemoryBankData, 0, dataLen / 8), dataLen / 8);
		fprintf(api3Data.fpCurLog, "\n\nTesting PostFilters with TID Pattern-B %s with OFFSET 0 and Datalength :%d", Array2Char(MyTags[1].tid, 0, dataLen / 8), dataLen / 8);

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,12,0,{0} };
		TAG_PATTERN tpA = { MEMORY_BANK_TID,0,api3Data.pG2V2Tag->pMemoryBankData,dataLen,TagMask,dataLen,0 };
		TAG_PATTERN tpB = { MEMORY_BANK_TID,0,MyTags[1].tid,dataLen,TagMask,dataLen,0 };
		ACCESS_FILTER accessFilter; accessFilter.lpTagPatternA = &tpA; accessFilter.lpTagPatternB = &tpB;

		TestRFID_Read(rParams, accessFilter);
	}

	// Iterate through from lenght varying from 32 bits to 512 bits with the increment length of 32 bits.
	//
	for( UINT16 dataLen = 32;dataLen <= 512;dataLen = dataLen+32 )
	{
		 //Test Read with USER Mem Bank  with Varying offset with length Fixed
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with USER MEMORY Pattern-A %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[0].user,0,dataLen/8 ),dataLen/8 );
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with USER MEMORY Pattern-B %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[1].user,0,dataLen/8 ),dataLen/8 );

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,0,64,0,{0} }; 
		TAG_PATTERN tpA = { MEMORY_BANK_USER,0,MyTags[0].user,dataLen,TagMask,dataLen,{0} };
		TAG_PATTERN tpB = { MEMORY_BANK_USER,0,MyTags[1].user,dataLen,TagMask,dataLen,{0} };
		ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };
		TestRFID_Read( rParams,accessFilter );		
	}
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

void Test_PostFiltersReadAuto( void )
{
	if (!api3Data.rCaps.tagEventReportingSupported)
	{
		wprintf(L"\n autonomous mode events are not supported\n");
		return;
	}

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("POST FILTER-AUTOMODE");
	logCurrentExecution( "POST FILTER-AUTOMODE");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 112;
	api3Data.dwTestCaseNo = 01;
	UINT16 dataLen;
	UINT8 TagMask[64];
	memset(TagMask,0xFF,64);

	// Reset the config to avoid if any...
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader); 

	// Iterate through from lenght varying from 16 bits to 96 bits with the increment length of 16 bits.
	// EPC Memory Bank
	for( dataLen = 16;dataLen <= 96; dataLen = dataLen+16 )
	{
		// Test Read with EPC Mem Bank  with FIRST TWO BYTES with OFF Set 0;
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with TagEPC Pattern-A %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[0].epc,0,dataLen/8 ),dataLen/8 );
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with TagEPC Pattern-B %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[1].epc,0,dataLen/8 ),dataLen/8 );
		
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} }; 
		TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,MyTags[0].epc,dataLen,TagMask,dataLen,{0} };
		TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,MyTags[1].epc,dataLen,TagMask,dataLen,{0} };
		POST_FILTER aFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };
		TestRFID_ReadAUTO( rParams,aFilter );		
	}

	for( UINT16 dataLen = 16;dataLen<=96;dataLen=dataLen+16 )
	{
		// Test Read with TID Mem Bank  with Varying offset with length Fixed
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with TID Pattern-A %s with OFFSET 0 and Datalength :%d",Array2Char( api3Data.pG2V2Tag->pMemoryBankData,0,dataLen/8 ),dataLen/8 );
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with TID Pattern-B %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[1].tid,0,dataLen/8 ),dataLen/8 );

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,12,0,{0} };
		TAG_PATTERN tpA = { MEMORY_BANK_TID,0,api3Data.pG2V2Tag->pMemoryBankData,dataLen,TagMask,dataLen,0 };
		TAG_PATTERN tpB = { MEMORY_BANK_TID,0,MyTags[1].tid,dataLen,TagMask,dataLen,0 };
		POST_FILTER aFilter; aFilter.lpTagPatternA = &tpA;aFilter.lpTagPatternB = &tpB;
		
		TestRFID_ReadAUTO( rParams,aFilter );		
	}

	// Iterate through from lenght varying from 32 bits to 512 bits with the increment length of 32 bits.
	// USER Memory Bank
	for( UINT16 dataLen = 32;dataLen <= 512;dataLen = dataLen+32 )
	{
		 //Test Read with USER Mem Bank  with Varying offset with length Fixed
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with USER MEMORY Pattern-A %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[0].user,0,dataLen/8 ),dataLen/8 );
		fprintf(api3Data.fpCurLog,"\n\nTesting PostFilters with USER MEMORY Pattern-B %s with OFFSET 0 and Datalength :%d",Array2Char( MyTags[1].user,0,dataLen/8 ),dataLen/8 );

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,0,64,0,{0} }; 
		TAG_PATTERN tpA = { MEMORY_BANK_USER,0,MyTags[0].user,dataLen,TagMask,dataLen,{0} };
		TAG_PATTERN tpB = { MEMORY_BANK_USER,0,MyTags[1].user,dataLen,TagMask,dataLen,{0} };
		POST_FILTER aFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL}; 
		TestRFID_ReadAUTO( rParams,aFilter );		
	}

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf(api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
