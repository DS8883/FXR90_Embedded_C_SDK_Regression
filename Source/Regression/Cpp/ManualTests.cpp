//#include "Common.h"
//#include <stdio.h>
//#include "APITEST.h"
//#include "rfid2Helper.h"
//
//extern struct RFID3DATA api3Data;
//extern struct RFID2DATA api2Data;
//extern struct HANDLE_LIST hList;
//extern MYTAGDATA MyTags[2];
//
//#ifndef linux
//RFID_HANDLE32 hMyEvent;  // event to signally local operation
//#else
//sem_t hMyEvent;			// Semaphore to signal stop of the activity in linux
//#endif
//
//int logToResultFile( void )
//{
//	char szTemp[512];
//	char szMsg[256];
//	if( api3Data.logResult == TRUE )
//	{
//		strcpy(szMsg,"PASS");
//		api3Data.dwPassCount++;
//	}
//	else if( api3Data.logResult == FALSE )
//	{
//		strcpy(szMsg,"FAIL");
//		api3Data.dwFailCount++;
//	}
//	else
//	{
//		strcpy(szMsg,"ANALYZE THE LOG FILE FOR RESULT");
//		api3Data.dwPassCount++;
//	}
//	api3Data.logResult = FALSE;
//	
//	if( !api3Data.api2Logging )
//	{
//		sprintf( szTemp,"\n%-45s- %d: %s \tERRORCODE:%d ERRORDISCR:%S",api3Data.logText,api3Data.dwTestCaseNo++,szMsg,api3Data.rfidStatus,RFID_GetErrorDescription( api3Data.rfidStatus )); 
//	}
//	else
//	{
//		sprintf( szTemp,"\n%-45s- %d: %s \tERRORCODE:%d ERRORDISCR:%S",api3Data.logText,api3Data.dwTestCaseNo++,szMsg,api2Data.rfidStatus,RFID_GetCommandStatusText(api2Data.hReader, api2Data.rfidStatus )); 
//	}
//
//	fprintf( api3Data.fpResult,szTemp );
//
//	fflush(api3Data.fpLog);
//	fflush( api3Data.fpResult );
//   return TRUE;
//}
//static DWORD WINAPI ProcessMyEvents( LPVOID lpParam )
//{
//	HANDLE_LIST *pHandleList = (HANDLE_LIST*)lpParam;
//	DWORD dwStatus;
//	
//	while( api3Data.bContinue )
//	{
//		dwStatus = WaitForMultipleObjects( pHandleList->nTotalHandles,pHandleList->handles,0,INFINITE );
//
//		switch(dwStatus)
//		{
//		case WAIT_OBJECT_0: // gpi event
//			{
//				fprintf(api3Data.fpLog,"\n>>>>>gpi Event is Signalled:get the event Data<<<<<<<<<");	
//				GPI_EVENT_DATA gpiEventData;
//
//				while( RFID_API_SUCCESS == RFID_GetEventData(api3Data.hReader, GPI_EVENT, (STRUCT_HANDLE)&gpiEventData))
//				{
//					SetLogResult( TRUE );
//					fprintf(api3Data.fpLog," GPI State:%d on GPI Port:%d",gpiEventData.eventInfo,gpiEventData.port );
//				//	logToResultFile();
//				}
//				
//			}
//			break;
//		
//		case WAIT_OBJECT_0 + 1:	// antenna Connect or DisConnect Event
//			{
//				ANTENNA_EVENT_DATA antEvntData;
//				fprintf(api3Data.fpLog,"\n>>>>>antenna Event is Signalled:get the event Data<<<<<<<<<");	
//				
//				if(RFID_API_SUCCESS == RFID_GetEventData( api3Data.hReader, ANTENNA_EVENT, 
//					(STRUCT_HANDLE)&antEvntData))
//				{
//					SetLogResult( TRUE );
//					fprintf(api3Data.fpLog," Antenna ID:%d,Antenna State:%d",antEvntData.id,antEvntData.eventInfo );
//					//logToResultFile();
//				}					
//			}
//			break;
//		
//		case WAIT_OBJECT_0 + 2:	// Reader DisConnect Event
//			{
//				DISCONNECTION_EVENT_DATA disconEventData;
//				fprintf(api3Data.fpLog,"\n>>>>>Reader Disconnect Event is Signalled<<<<<<<<<");
//				if(RFID_API_SUCCESS == RFID_GetEventData( api3Data.hReader, DISCONNECTION_EVENT,(STRUCT_HANDLE)&disconEventData))
//				{
//					SetLogResult( TRUE );
//					if( disconEventData.eventInfo == 0 )
//					{
//						fprintf( api3Data.fpLog," \n Disconnet Event Data Reason : %S",L"READER_INITIATED_DISCONNECTION:Disconnection notified from Reader");
//					}
//					else if( disconEventData.eventInfo == 1 )
//					{
//						fprintf( api3Data.fpLog," \n Disconnet Event Data Reason : %S",L"READER_EXCEPTION:Reader has reported an exception");
//					}
//					else if( disconEventData.eventInfo == 2 )
//					{
//						fprintf( api3Data.fpLog," \n Disconnet Event Data Reason : %S",L"CONNECTION_LOST:Reader has reported loss of connectivity");
//					}
//
//				}
//
//				//logToResultFile();
//			}
//			break;
//		
//		case WAIT_OBJECT_0 + 3:	// EAS event
//		{
//			NXP_EAS_ALARM_DATA myEASevent;
//			RFID_GetEventData( api3Data.hReader,NXP_EAS_ALARM_EVENT,&myEASevent);
//			fprintf(api3Data.fpLog,"\n>>>EAS Event is signalled with...");
//			fprintf(api3Data.fpLog,"\n Alarm code %lld Antenna ID: %d ",myEASevent.alarmCode,myEASevent.antennaID);
//			MessageBeep( 1 );
//			SetLogResult( TRUE );
//			//logToResultFile();
//		}
//			break;
//		case WAIT_OBJECT_0 + 4:	// Reader Exception event
//		{
//			READER_EXCEPTION_EVENT_DATA exceptnEvent;
//			RFID_GetEventData( api3Data.hReader,READER_EXCEPTION_EVENT,&exceptnEvent);
//			fprintf(api3Data.fpLog,"\n>>>EXCEPTION Event is signalled with...");
//			fprintf(api3Data.fpLog,"\n Exception type: %S ",exceptnEvent.exceptionType);
//			fprintf(api3Data.fpLog,"\n Exception info: %S ",exceptnEvent.exceptionInfo);
//			SetLogResult( TRUE );
//			//logToResultFile();
//		}
//			break;
//		default:
//			break;
//
//		}
//	}
//	
//return TRUE;
//}
//
//BOOLEAN TestRFID3EventsManual( RFID_HANDLE32 hReader )
//{
//	RFID_STATUS rfidStatus;
//	DWORD dwThreadID;
//	api3Data.bContinue = TRUE;
//
//	// Create events that can be processed in a different thread
//	//
//	hList.handles[0] = CreateEvent(NULL, FALSE, FALSE, NULL);	// gpi Event Handle
//	hList.handles[1] = CreateEvent(NULL, FALSE, FALSE, NULL);	// antenna Connect or DisConnect Event
//	hList.handles[2] = CreateEvent(NULL, FALSE, FALSE, NULL);	// Reader DisConnect Event
//	hList.handles[3] = CreateEvent(NULL, FALSE, FALSE, NULL);	// EAS alarm event
//	hList.handles[4] = CreateEvent(NULL, FALSE, FALSE, NULL);  // Reader exception event
//
//	
//	hList.nTotalHandles = 5;
//
//	// Register for all events 
//	//
//	rfidStatus = RFID_RegisterEventNotification( hReader,GPI_EVENT,hList.handles[0]);
//	rfidStatus = RFID_RegisterEventNotification( hReader,ANTENNA_EVENT,hList.handles[1]);
//	rfidStatus = RFID_RegisterEventNotification( hReader,DISCONNECTION_EVENT,hList.handles[2]);
//	rfidStatus = RFID_RegisterEventNotification( hReader,NXP_EAS_ALARM_EVENT,hList.handles[3]);
//	rfidStatus = RFID_RegisterEventNotification( hReader,READER_EXCEPTION_EVENT,hList.handles[4]);
//	
//	HANDLE hThread = CreateThread(NULL, 0, ProcessMyEvents, (LPVOID)(&hList), 0, &dwThreadID );
//	
//	TRIGGER_INFO trInfo;
//	/*SYSTEMTIME sysTime;
//	GetSystemTime( &sysTime );
//	sysTime.wSecond += 10;
//	trInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;*/
//	//trInfo.stopTrigger.type = STOP_TRIGGER_TYPE_IMMEDIATE;
//	/*trInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
//	trInfo.stopTrigger.value.duration = 5000;*/
//	
//
//	//trInfo.tagReportTrigger = 1;
//	/*trInfo.startTrigger.type = START_TRIGGER_TYPE_GPI;
//	trInfo.startTrigger.value.gpi.gpiEvent = true;
//	trInfo.startTrigger.value.gpi.portNumber = 1;
//	trInfo.startTrigger.value.gpi.timeoutMilliseconds = 10000;*/
//
//	/*trInfo.stopTrigger.type = STOP_TRIGGER_TYPE_GPI_WITH_TIMEOUT;
//	trInfo.stopTrigger.value.gpi.gpiEvent = true;
//	trInfo.stopTrigger.value.gpi.portNumber = 1;
//	trInfo.stopTrigger.value.gpi.timeoutMilliseconds = 5000;*/
//
//	////trInfo.tagReportTrigger = 1;
//	////trInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
//	////trInfo.stopTrigger.value.duration = 15000;
//
//	 ////Perform inventory to Start access and inventory
//	//RFID_PerformInventory( hReader,NULL,NULL,&trInfo,NULL);
//	//rfid3Sleep( 20000 );
//	//RFID_StopInventory( hReader );
//	//rfid3Sleep( 2000 ); //wait for the inventory to stop.
//	
//
//	// Fill up AntennaInfo structure to specify EAS operation
//	/*READER_CAPS readerCaps;
//    RFID_GetReaderCaps(readerHandle,&readerCaps);
//    ANTENNA_INFO AntennaInfo;
//    AntennaInfo.length = readerCaps.numAntennas;
//    AntennaInfo.pAntennaList = new UINT16[AntennaInfo.length];
//    AntennaInfo.pAntennaOpList = new OPERATION_QUALIFIER[AntennaInfo.length];*/
//
//	/*READER_CAPS readerCaps;
//	RFID_GetReaderCaps(api3Data.hReader,&readerCaps);*/
//	
//	SINGULATION_CONTROL singulationControl;
//	
//	singulationControl.stateAwareSingulationAction.perform			= FALSE;
//	singulationControl.session										= SESSION_S0;
//	//singulationControl.stateAwareSingulationAction.slFlag			= SL_FLAG_ASSERTED;
//	singulationControl.tagPopulation								= 1;
//	singulationControl.tagTransitTimeMilliseconds                   = 3000;
//	//rfidStatus = RFID_GetSingulationControl(ReaderHandle, 1, &singulationControl);		
//	//singulationControl.tagPopulation								= 5;
//	api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &singulationControl);	
//	
//	//Buffer Full Event - Set the below tagStorage settings
//	TAG_STORAGE_SETTINGS tgSettings;
//
//	tgSettings.maxMemoryBankByteCount = 512;
//	tgSettings.maxTagCount = 1;
//	tgSettings.maxTagIDByteCount=512;
//	tgSettings.enableAccessReports = TRUE;
//	tgSettings.tagFields = ANTENNA_ID | FIRST_SEEN_TIME_STAMP| LAST_SEEN_TIME_STAMP | PEAK_RSSI | PC | CRC;
//	
//	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings );
//	
//	ANTENNA_INFO antennaInfo;
//	UINT16 antennaIDList[2] = {1,3};
//	OPERATION_QUALIFIER opq[2] = {C1G2_OPERATION,C1G2_OPERATION};
//	antennaInfo.pAntennaList = antennaIDList;
//	antennaInfo.length = 2;
//	antennaInfo.pAntennaOpList = opq;
//
//
//	/*for(UINT32 nIndex = 0; nIndex < AntennaInfo.length; nIndex++)
//	{
//		AntennaInfo.pAntennaList[nIndex] = nIndex+1;
//		AntennaInfo.pAntennaOpList[nIndex] = NXP_EAS_SCAN;
//	}*/
//	
//	//api3Data.rfidStatus = RFID_PurgeTags( hReader,0 );
//	//trInfo.lpTagEventReportInfo = NULL;
//	
//	fprintf( api3Data.fpLog,"\n  Test for EAS event \n");
//	api3Data.rfidStatus = RFID_PerformInventory(hReader,NULL,&antennaInfo,&trInfo,NULL);
//	rfid3Sleep( 3000 );
//	RFID_StopInventory( hReader );
//	rfid3Sleep( 2000 ); //wait for the inventory to stop
//	
//	
//	
//	//rfid3Sleep( 30000 );
//
//	api3Data.bContinue = FALSE;
//	
//	
//	////Code for disconnect event
//	/*DISCONNECTION_EVENT_TYPE disEvent;
//	api3Data.rfidStatus = RFID_GetEventData ( api3Data.hReader, DISCONNECTION_EVENT , &disEvent );
//	api3Data.rfidStatus = RFID_Reconnect( api3Data.hReader );
//	LastErrorInfo();*/
//	//api3Data.rfidStatus = RFID_Reconnect( 1234567 );
//	//RFID_PerformInventory( hReader,NULL,&antennaInfo,&trInfo,NULL);
//	//rfid3Sleep( 10000 );
//	//RFID_StopInventory( hReader );
//	//rfid3Sleep( 2000 ); //wait for the inventory to stop
//	
//	//Do the Cleanup and Close the event handles.
//	//
//	rfidStatus = RFID_DeregisterEventNotification( hReader,GPI_EVENT );
//	rfidStatus = RFID_DeregisterEventNotification( hReader,ANTENNA_EVENT );
//	rfidStatus = RFID_DeregisterEventNotification( hReader,DISCONNECTION_EVENT );
//	rfidStatus = RFID_DeregisterEventNotification( hReader,NXP_EAS_ALARM_EVENT );
//	rfidStatus = RFID_DeregisterEventNotification( hReader,READER_EXCEPTION_EVENT );
//
//	//
//	for( int handleIndex=0; handleIndex<5; handleIndex++ )
//	{
//		CloseHandle( hList.handles[handleIndex]);hList.handles[handleIndex] = INVALID_HANDLE_VALUE;
//	}
//
//	return TRUE;
//}
//
//
//static DWORD WINAPI ProcessTagEvents( LPVOID handleList )
//{
//	HANDLE_LIST* pList = ( HANDLE_LIST*)handleList;
//	DWORD dwStatus;
//	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
//
//	while( api3Data.bContinue )
//	{
//		dwStatus = WaitForMultipleObjects(pList->nTotalHandles, pList->handles, FALSE, INFINITE);
//
//		switch(dwStatus)
//		{
//		case WAIT_OBJECT_0: // access Start event
//			{
//				api3Data.bStartEvent = TRUE;
//			}
//			break;
//		case WAIT_OBJECT_0+1: // Tag read eventp
//		{
//			while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
//			{
//				printTagMemoryBankData( pTagData->antennaID,pTagData->PC,pTagData->CRC,pTagData->pTagID,pTagData->tagIDLength,pTagData->pMemoryBankData,pTagData->memoryBankDataLength,pTagData->memoryBank );
//				Print2Console( pTagData->antennaID,pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);
//			}	
//		
//		}
//		break;
//		case WAIT_OBJECT_0+2: // access Stop event
//			{	
//				SetEvent( hMyEvent );
//			}
//			break;
//		case WAIT_OBJECT_0+3: // Reader DisConnect event
//		{
//			fprintf(api3Data.fpLog,"\nReader Disconnect Event");
//			
//		}
//		break;
//		default:
//			;
//		}
//
//	}	
//
//	RFID_DeallocateTag(api3Data.hReader,pTagData );
//
//	return TRUE;
//}
//void TestOPSequenceLock( RFID_HANDLE32 hReader ,UINT8* data2Write,ANTENNA_INFO* pInfo )
//{
//	UINT8 Pattern1[2];
//	UINT8 Pattern2[2];
//	UINT8 lockPwd[]={0x12,0x34,0x56,0x78};
//	// Initialize the patterns with CRC+PC as below
//	//
//	Pattern1[0] = HIBYTE(MyTags[0].PC);
//	Pattern1[1] = LOBYTE(MyTags[0].PC);
//
//	Pattern2[0] = HIBYTE(MyTags[1].PC);
//	Pattern2[1] = LOBYTE(MyTags[1].PC);
//	 
//	UINT8 TagMask[24];
//	memset(TagMask,0xFF,24);
//	DWORD dwThreadID;
//	api3Data.bContinue = TRUE;
//	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
//	
//	// Opcode Related Data 
//	UINT32 opIndex;
//	TRIGGER_INFO triggerInfo;
//	
//	// first read the tags which are present in the field
//	//
//	
//	LPTAG_DATA ptagData = RFID_AllocateTag( hReader);
//	
//	// create an access stop event
//	//
//	hList.handles[0] = CreateEvent( NULL,0,0,NULL);	// start
//	hList.handles[1] = CreateEvent( NULL,0,0,NULL); // Read event
//	hList.handles[2] = CreateEvent( NULL,0,0,NULL); // stop access
//	hList.handles[3] = CreateEvent( NULL,0,0,NULL); // disconnect access
//	hMyEvent = CreateEvent( NULL,0,0,NULL); // to signal stop access locally
//
//	hList.nTotalHandles = 4;
//
//	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_START_EVENT,hList.handles[0] );
//	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,TAG_READ_EVENT,hList.handles[1] );
//	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hList.handles[2]);
//	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,DISCONNECTION_EVENT,hList.handles[3]);
//
//	// Do the processing in sepearate thread
//	//
//	HANDLE hEventThread = CreateThread(NULL, 0, ProcessTagEvents, (LPVOID)(&hList), 0, &dwThreadID);
//
//	// Initialize op sequence
//	//
//	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );
//
//	//Access read for all memory banks
//	//
//	triggerInfo.tagReportTrigger	= 0; 
//	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
//	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
//	if( api3Data.apiVersion ==  RFID_API3_5_1 )
//	{
//		triggerInfo.lpTagEventReportInfo = NULL;
//	}
//
//	{
//		fprintf(api3Data.fpLog,"\n------TestCaseNo:%d TEST OP SEQUENCE LOCK TO EPC --------------",api3Data.dwTestCaseNo);
//		fprintf(api3Data.fpLog,"\n------ Only Tags which match with PC %X Should be Written------",MyTags[0].PC );	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,4,lockPwd,4,0}; 
//		TAG_PATTERN tpA = {MEMORY_BANK_EPC,16,Pattern1,16,TagMask,16,0};
//		TAG_PATTERN tpB = {MEMORY_BANK_EPC,16,Pattern2,16,TagMask,16,0};
//		ACCESS_FILTER aFilter ;aFilter.lpTagPatternA = &tpA;aFilter.lpTagPatternB = &tpB;
//		aFilter.matchPattern = A_AND_NOTB;
//
//		if( api3Data.apiVersion ==  RFID_API3_5_1 )
//		{
//			triggerInfo.lpTagEventReportInfo = NULL;
//			aFilter.lpRSSIRangeFilter = NULL;
//		}
//
//		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_WRITE,&wParams,0};
//		
//		// Write to Res,then Lock EPC, then Write to EPC with PWD, Write to EPC without PWD
//		// Plcae OPSECS for : Write to Res, LOCK EPC, Write to EPC with PWD and Write to EPC without PWD
//		//
//		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
//		LOCK_ACCESS_PARAMS lockParams;
//		lockParams.accessPassword = 0x12345678;
//		lockParams.privilege[LOCK_EPC_MEMORY] = LOCK_PRIVILEGE_READ_WRITE;
//		opParams.opCode = ACCESS_OPERATION_LOCK;
//		opParams.opParams = &lockParams;
//		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
//		WRITE_ACCESS_PARAMS wParams1 = { MEMORY_BANK_EPC,4,data2Write,4,0x12345678 }; 
//		opParams.opCode = ACCESS_OPERATION_WRITE;
//		opParams.opParams = &wParams1;
//		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
//		
//		fprintf(api3Data.fpLog,"\n------TestCaseNo:%d TEST OP SEQUENCE UNLOCK TO EPC --------------",api3Data.dwTestCaseNo);
//		lockParams.privilege[LOCK_EPC_MEMORY] = LOCK_PRIVILEGE_UNLOCK;
//		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
//		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,&triggerInfo,0);
//		rfid3Sleep( 10000 );
//		RFID_StopAccessSequence( hReader );
//		DWORD wait = WaitForSingleObject( hMyEvent,10000 );		
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( 2 );
//		}
//		logToResultFile();
//
//		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
//		RFID_ResetConfigToFactoryDefaults( hReader );
//
//		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,0,0}; 
//		opParams.opCode = ACCESS_OPERATION_READ;
//		opParams.opParams = &rParams;
//		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
//		rParams.memoryBank = MEMORY_BANK_RESERVED;
//		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
//
//		fprintf(api3Data.fpLog,"\n------TestCaseNo:%d TEST OP SEQUENCE READ TO EPC --------------",api3Data.dwTestCaseNo);
//		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,&triggerInfo,0);
//		rfid3Sleep( 10000 );
//		RFID_StopAccessSequence( hReader );
//		wait = WaitForSingleObject( hMyEvent,10000 );		
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( 2 );
//		}
//		logToResultFile();
//
//		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
//		RFID_ResetConfigToFactoryDefaults( hReader );
//		
//	}
//
//	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
//	
//	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,ACCESS_STOP_EVENT );
//	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,TAG_READ_EVENT );
//	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,ACCESS_START_EVENT );
//	api3Data.rfidStatus = RFID_DeregisterEventNotification( hReader,DISCONNECTION_EVENT ); 
//	
//	api3Data.bContinue = FALSE;
//	CloseHandle( hMyEvent );
//	RFID_DeallocateTag( hReader,ptagData);
//}
//
//void TestLockOperations( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo )					
//{
//	UINT8 Data2Write[4] = { 0xDE,0xC0,0xDE,0xDD };
//	UINT8 accessPwd[4]  = { 0x12,0x34,0x56,0x78 };
//	
//	SetLogText("TestLockOperations()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperations - Start ------------------\n\n");	
//	logCurrentExecution( "TestLockOperations");
//
//	HANDLE hStopAccess = CreateEvent(NULL, FALSE, FALSE, NULL);	// access stop event
//	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hStopAccess );
//
//	TRIGGER_INFO triggerInfo;
//	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
//	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
//	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
//	//triggerInfo.stopTrigger.value.duration = 4000; // perform 3 rounds of inventory	
//	
//	if( api3Data.apiVersion ==  RFID_API3_5_1 )
//	{
//		triggerInfo.lpTagEventReportInfo = NULL;
//	}
//
//	// TEST with Simple Lock Operations without AnyFilters 
//	// Read the first tag in the inventory list and lock and perform write with and without password
//	//
//	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
//	
//	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
//	rfid3Sleep( 4000 );
//	RFID_StopInventory( hReader );
//
//	while( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
//	{	
//		LOCK_ACCESS_PARAMS lockParams;
//
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,accessPwd, 4, 0,0 };
//		
//		// Write Access Password and KILL Password to the MemoryBank
//		//
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		
//		wParams.byteOffset = 4;
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		
//		lockParams.privilege[ LOCK_USER_MEMORY ] = LOCK_PRIVILEGE_READ_WRITE;
//		lockParams.accessPassword = 0x00000000;			
//		logToFile( "RFID_Lock() to all memory Banks without PASSWORD ");
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile( );
//		
//		logToFile( "RFID_Lock() to all memory Banks with ACCESS PASSWORD ");
//		lockParams.accessPassword = 0x12345678;
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile( );
//	
//		// Verify Write on a LOCKED MEMORY Bank USER without PASSWORD
//		//
//		{	
//			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write, 4, 0,0 };
//
//			logToFile("RFID_Write() on a locked memory bank : USER");
//			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//			WaitForSingleObject( hStopAccess, 10000 );
//			if( api3Data.rfidStatus != RFID_API_SUCCESS )
//			{
//				SetLogResult( TRUE );
//			}
//
//			logToResultFile();
//		}
//
//		// Verify Write on a LOCKED MEMORY Bank USER with WRONG PASSWORD
//		//
//		{
//			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write, 4, 0x12341234,0 };
//
//			logToFile("RFID_Write() on a locked memory bank : USER");
//			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//			WaitForSingleObject( hStopAccess, 10000 );
//			if( api3Data.rfidStatus != RFID_API_SUCCESS )
//			{
//				SetLogResult( TRUE );
//			}
//
//			logToResultFile();
//		}
//
//		// Verify Write on a LOCKED MEMORY Bank USER with PASSWORD
//		//
//		{
//			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER, 0, Data2Write,4,0x12345678,0 };
//
//			logToFile("RFID_Write() on a locked memory bank : USER");
//			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//			WaitForSingleObject( hStopAccess, 10000 );
//			if( api3Data.rfidStatus == RFID_API_SUCCESS )
//			{
//				SetLogResult( TRUE );
//			}
//
//			logToResultFile();
//		}
//
//		//unlock all memory banks
//		//
//		lockParams.privilege[LOCK_USER_MEMORY] = LOCK_PRIVILEGE_UNLOCK;
//		lockParams.accessPassword = 0x12345678;
//
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		logToFile( "RFID_Lock() unlock to all memory Banks");
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//	}
//
//	{
//		LOCK_ACCESS_PARAMS lockParams;
//		
//		lockParams.privilege[ LOCK_EPC_MEMORY ] = LOCK_PRIVILEGE_READ_WRITE;
//				
//		logToFile( "RFID_Lock() to EPC Memory Banks without PASSWORD ");
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile( );
//		
//		logToFile( "RFID_Lock() to all memory Banks with ACCESS PASSWORD ");
//		lockParams.accessPassword = 0x12345678;
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile( );
//	
//		// Verify Write on a LOCKED memory Bank EPC without PASSWORD
//		//
//		{	
//			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_EPC,4,Data2Write, 4, 0,0 };
//
//			logToFile("RFID_Write() on a locked memory bank : EPC");
//			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//			WaitForSingleObject( hStopAccess, 10000 );
//			if( api3Data.rfidStatus != RFID_API_SUCCESS )
//			{
//				SetLogResult( TRUE );
//			}
//
//			logToResultFile();
//		}
//
//		// Verify Write on a LOCKED memory Bank EPC Wrong PASSWORD
//		//
//		{		
//			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_EPC,4,Data2Write, 4, 0x12341234,0 };
//
//			logToFile("RFID_Write() on a locked memory bank : EPC");
//			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//			WaitForSingleObject( hStopAccess, 10000 );
//			if( api3Data.rfidStatus != RFID_API_SUCCESS )
//			{
//				SetLogResult( TRUE );
//			}
//
//			logToResultFile();
//		}
//
//					
//		//unlock all memory banks
//		//
//		lockParams.privilege[LOCK_EPC_MEMORY]= LOCK_PRIVILEGE_UNLOCK;
//		lockParams.accessPassword = 0x12345678;
//
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		logToFile( "RFID_Lock() unlock to all memory Banks");
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//		logToResultFile();
//	}
//	
//	RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT );
//	RFID_DeletePreFilter( hReader,0,0 );
//	RFID_DeallocateTag( hReader,pTagData);	
//
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperations - End ------------------\n\n");	
//}
//
//void TestLockOperationsOnResMemory( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo )
//{
//	LOCK_ACCESS_PARAMS lockParams;
//	UINT8 Data2Write[4] = { 0xDE,0xC0,0xDE,0xDD };
//	UINT8 accessPwd[4]  = { 0x12,0x34,0x56,0x78 };
//	HANDLE hStopAccess = CreateEvent(NULL, FALSE, FALSE, NULL);	// access stop event
//	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hStopAccess );
//	
//	SetLogText("TestLockOperationsOnResMemory()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperationsOnResMemory - Start ------------------\n\n");	
//	logCurrentExecution( "TestLockOperationsOnResMemory");
//
//			
//	TRIGGER_INFO triggerInfo;
//	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
//	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
//	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
//	if( api3Data.apiVersion ==  RFID_API3_5_1 )
//	{
//		triggerInfo.lpTagEventReportInfo = NULL;
//	}	
//	// TEST with Simple Lock Operations without AnyFilters 
//	// Read the first tag in the inventory list and lock and perform write with and without password
//	//
//	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
//
//	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
//	rfid3Sleep( 4000 );
//	RFID_StopInventory( hReader );
//
//	while( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
//	{
//		logToFile( "RFID_Lock() to all memory Banks without PASSWORD ");
//		
//		lockParams.privilege[ LOCK_KILL_PASSWORD ] = LOCK_PRIVILEGE_READ_WRITE;
//		lockParams.privilege[ LOCK_ACCESS_PASSWORD ] = LOCK_PRIVILEGE_READ_WRITE;
//
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,accessPwd, 4, 0,0 };
//		
//		// Write Access Password and KILL Password to the MemoryBank
//		//
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		wParams.byteOffset = 4;
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile( );
//		
//		logToFile( "RFID_Lock() to all memory Banks with ACCESS PASSWORD ");
//		lockParams.accessPassword = 0x12345678;
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile( );
//	}
//
//
//	// Verify Write on a LOCKED MEMORY BAnk RES( killpw) without PASSWORD
//	//
//	{		
//		SetLogText( "RFID_Write() on a locked memory bank RES( KILLPW )");	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,Data2Write,4,0,0 };
//
//		logToFile("RFID_Write() on a locked memory bank : RES( KILLPW )");
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile();
//	}
//
//	// Verify Write on a LOCKED MEMORY BAnk RES( killpw) with WRONG PASSWORD
//	//
//	{
//		SetLogText( "RFID_Write() on a locked memory bank RES( KILLPW )");	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,Data2Write,4, 0x12341234,0 };
//
//		logToFile("RFID_Write() on a locked memory bank : RES( KILLPW )");
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile();
//	}
//
//
//	// Verify Write on a LOCKED MEMORY BAnk RES( access pw) without PASSWORD
//	//
//	{
//		SetLogText( "RFID_Write() on a locked memory bank RES( ACCESSPW )");	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,4,Data2Write, 4, 0,0 };
//
//		logToFile("RFID_Write() on a locked memory bank : RES( ACCESSPW )");
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile();
//	}
//
//	// Verify Write on a LOCKED MEMORY BAnk RES( access pw) with WRONG PASSWORD
//	//
//	{
//		SetLogText( "RFID_Write() on a locked memory bank RES( ACCESSPW )");	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,4,Data2Write, 4, 0x12341234,0 };
//
//		logToFile("RFID_Write() on a locked memory bank : RES( ACCESSPW )");
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile();
//	}
//
//	
//	// Verify Write on a LOCKED MEMORY BAnk RES( killpw) with PASSWORD
//	//
//	{
//		SetLogText( "RFID_Write() on a locked memory bank RES( KILLPW )");	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 0, accessPwd,4, 0x1234578,0 };
//
//		logToFile("RFID_Write() on a locked memory bank : RES( KILLPW )");
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile();
//	}
//
//	// Verify Write on a LOCKED MEMORY BAnk RES( access pw) with PASSWORD
//	//
//	{
//		SetLogText( "RFID_Write() on a locked memory bank RES( ACCESSPW )");	
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 4, accessPwd,4, 0x12345678,0 };
//
//		logToFile("RFID_Write() on a locked memory bank : RES( ACCESSPW )");
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
//		WaitForSingleObject( hStopAccess, 10000 );
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//
//		logToResultFile();
//	}
//
//	//unlock all memory banks
//	//
//	lockParams.privilege[LOCK_KILL_PASSWORD] = LOCK_PRIVILEGE_UNLOCK;
//	lockParams.privilege[LOCK_ACCESS_PASSWORD]= LOCK_PRIVILEGE_UNLOCK;
//	lockParams.accessPassword = 0x12345678;
//
//	api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL, pInfo,NULL );
//	WaitForSingleObject( hStopAccess, 10000 );
//	logToFile( "RFID_Lock() unlock to all memory Banks");
//	if( api3Data.rfidStatus == RFID_API_SUCCESS )
//	{
//		SetLogResult( TRUE );
//	}
//	logToResultFile();
//
//	RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT );
//	RFID_DeletePreFilter( hReader,0,0 );
//	RFID_DeallocateTag( hReader,pTagData);	
//	
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperationsOnResMemory - End ------------------\n\n");	
//	
//}
//void TestRFID3KillTag( RFID_HANDLE32 hReader,UINT8* tPattern,ANTENNA_INFO* pInfo )
//{
//	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( hReader );
//	UINT8 accessPwd[4] = { 0x12,0x34,0x56,0x78 };
//	fprintf(api3Data.fpLog," \n >>> KILL OPERATIONS TESTS.<< \n");
//	SetLogText("RFID_Kill()- FUN");
//			
//	TRIGGER_INFO triggerInfo;
//
//	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
//	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
//	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_DURATION;
//	triggerInfo.stopTrigger.value.duration = 5000; // perform 3 rounds of inventory	
//	if( api3Data.apiVersion ==  RFID_API3_5_1 )
//	{
//		triggerInfo.lpTagEventReportInfo = NULL;
//	}
//	// TEST with Simple Lock Operations without AnyFilters 
//	// Read the first tag in the inventory list and lock and perform write with and without password
//	//
//	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
//	addanWareSelectRecord( hReader,tPattern,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);
//
//	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
//	rfid3Sleep( 6000 );
//	
//	if( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
//	{
//		SetLogText( "RFID_Lock() to PERMALOCK USER and PERMAUNLOCK EPC without ACCESS PASSWORD");
//
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 4, accessPwd,4,0 };
//		
//		// Write Access Password and KILL Password to the MemoryBank
//		//
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		wParams.byteOffset = 0;
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		KILL_ACCESS_PARAMS killParams = { 0x12345678,0 }; 
//		api3Data.rfidStatus = RFID_Kill( hReader,pTagData->pTagID,pTagData->tagIDLength,&killParams, 0, pInfo, 0 );
//
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			fprintf( api3Data.fpLog, "\n\n Kill Success ");
//		}
//	}
//}
//
//// Verify this on a Single Tag in the field and kill it in the final part of the tes to verify the KILL OPeration.
//// PLACE a SINGLE tag in the field with PC+EPC  as 3000FFFF and verify the test  for EPC and USER MEMORY 
//void TestPermaLockANDPermaUNLock( RFID_HANDLE32 hReader,UINT8* tPattern, ANTENNA_INFO* pInfo)					
//{
//	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( hReader );
//	UINT8 accessPwd[4] = { 0x12,0x34,0x56,0x78 };
//	SetLogText("TestPermaLockANDPermaUNLock()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------TestPermaLockANDPermaUNLock - Start ------------------\n\n");	
//	logCurrentExecution( "TestPermaLockANDPermaUNLock");
//			
//	TRIGGER_INFO triggerInfo;
//
//	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
//	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
//	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
//	if( api3Data.apiVersion ==  RFID_API3_5_1 )
//	{
//		triggerInfo.lpTagEventReportInfo = NULL;
//	}
//	// TEST with Simple Lock Operations without AnyFilters 
//	// Read the first tag in the inventory list and lock and perform write with and without password
//	//
//	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
//	addanWareSelectRecord( hReader,tPattern,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);
//
//	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
//	rfid3Sleep( 6000 );
//	RFID_StopInventory( hReader );
//
//	if( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
//	{
//		logToFile( "RFID_Lock() to PERMALOCK USER and PERMAUNLOCK EPC without ACCESS PASSWORD");
//
//		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 4, accessPwd,4,0 };
//		
//		// Write Access Password and KILL Password to the MemoryBank
//		//
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//		wParams.byteOffset = 0;
//		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
//	
//		LOCK_ACCESS_PARAMS lockParams;
//
//		lockParams.privilege[ LOCK_USER_MEMORY ] = LOCK_PRIVILEGE_PERMA_LOCK;
//		lockParams.privilege[ LOCK_EPC_MEMORY ] = LOCK_PRIVILEGE_PERMA_UNLOCK;
//		
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}		
//		logToResultFile( );
//
//		logToFile( "RFID_Lock() to PERMALOCK USER and PERMAUNLOCK EPC with ACCESS PASSWORD");
//
//		lockParams.accessPassword = 0x12345678;
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//
//		if( api3Data.rfidStatus == RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}
//		logToResultFile( );
//				
//		logToFile( "RFID_Lock() try unLOCK on a PERMA LOCKEd MEM");
//		lockParams.privilege[ LOCK_USER_MEMORY ] = LOCK_PRIVILEGE_UNLOCK;
//		lockParams.privilege[ LOCK_EPC_MEMORY ] = LOCK_PRIVILEGE_PERMA_LOCK;
//
//		lockParams.accessPassword = 0x12345678;
//		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
//
//		if( api3Data.rfidStatus != RFID_API_SUCCESS )
//		{
//			SetLogResult( TRUE );
//		}		
//
//		logToResultFile( );
//	}
//	api3Data.rfidStatus = RFID_DeletePreFilter( hReader,0,0 );
//	api3Data.rfidStatus = RFID_DeallocateTag( hReader, pTagData );
//	
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------TestPermaLockANDPermaUNLock - End ------------------\n\n");	
//}
//
//void test_SoftwareUpdate(void)
//{
//    wprintf(L"Executing test_SoftwareUpdate()\n");
//	
//	FTPSERVER_INFO loginInfo;
//	wcscpy(loginInfo.userName,L"");
//	wcscpy(loginInfo.password,L"");
//	wcscpy(loginInfo.hostName,L"ftp://157.235.207.23/Falcon/Merlin-5.00.29-Plat37/");
//	//wcscpy(loginInfo.hostName,L"ftp://157.235.206.138/1.0/firmware/Merlin-5.00.29-Plat036/"));
//	//wcscpy(loginInfo.hostName,L"ftp://157.235.207.23/Merlin-5.00.105-Plat108/"));
//    api3Data.rfidStatus = RFID_UpdateSoftware(api3Data.hReaderMgmt, &loginInfo);
//	ERROR_INFO einfo;
//	RFID_GetLastErrorInfo( api3Data.hReaderMgmt,&einfo );
//
//	if(RFID_API_SUCCESS == api3Data.rfidStatus)
//	{
//		
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//	
//	api3Data.rfidStatus =  RFID_UpdateSoftware((RFID_HANDLE32)INVALID_HANDLE_VALUE, &loginInfo);
//
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//
//	api3Data.rfidStatus =  RFID_UpdateSoftware(api3Data.hReaderMgmt, NULL);
//	
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//}
//   
//void test_GetUpdateStatus(void)
//{
//	UPDATE_STATUS upgradeStatus;
//    
//	for( int i= 0;i<500;i++ )
//	{
//		api3Data.rfidStatus = RFID_GetUpdateStatus(api3Data.hReaderMgmt, &upgradeStatus);
//		fprintf(api3Data.fpLog," ret val code: %S\tparition Name: %S \t Percentage Complete:%d\n",RFID_GetErrorDescription(api3Data.rfidStatus), upgradeStatus.updateInfo,upgradeStatus.percentage);
//		
//		if(RFID_API_SUCCESS  == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//
//		logToResultFile( );
//		rfid3Sleep(1000 );
//		if( upgradeStatus.percentage == 100 ) break;
//	}
//
//	api3Data.rfidStatus =  RFID_GetUpdateStatus((RFID_HANDLE32)INVALID_HANDLE_VALUE,&upgradeStatus);
//	
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//
//	api3Data.rfidStatus =  RFID_GetUpdateStatus( api3Data.hReaderMgmt,NULL );
//	
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//}

//void test_ExportProfileToReader(void)
//{
//	SetLogText("test_ExportProfileToReader()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------test_ExportProfileToReader - Start ------------------\n\n");	
//	logCurrentExecution( " test_ExportProfileToReader");
//	
//	wchar_t tcPath[512];
//	wcscpy(tcPath,L"C:\\Temp");
//	PROFILE_LIST ProfileList ;
//#ifdef FIXEDREADER
//	if( api3Data.readerType != MC )
//	{		
//		logToFile( "Test RFID_GetProfileList with Valid Data");
//		api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &ProfileList);
//		if(RFID_API_SUCCESS == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//		
//		wcscpy( api3Data.tcProfileName,*(ProfileList.pProfileName) );
//		api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt,api3Data.tcProfileName,tcPath);
//		wchar_t filename1[512];
//		wchar_t filename2[512];
//		wsprintf( filename1,L"%s\\%s",tcPath,api3Data.tcProfileName );
//		wsprintf( filename2,L"%s\\%s",tcPath,L"myprofile.xml" );
//		int result = _wrename( filename1,filename2 );
//		logToFile( "Test test_ExportProfileToReader Export a profile to reader");
//		api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"myprofile.xml",tcPath,TRUE);
//		if(RFID_API_SUCCESS == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//
//		logToFile( "Test test_ExportProfileToReader Export a profile to reader");
//		api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"myprofile.xml",tcPath,FALSE);
//		if(RFID_API_SUCCESS == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//	}
//	else
//#endif
//	{
//		api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReader, api3Data.tcProfileName,tcPath,TRUE);
//		if( RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//	}
//
//	logToFile( "Test test_ExportProfileToReader with invalid handle value");
//    api3Data.rfidStatus = RFID_ExportProfileToReader((RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.tcProfileName,tcPath,TRUE);
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//	
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------test_ExportProfileToReader - End ------------------\n\n");
//}
//
//void test_ImportProfileFromReader( void )
//{
//	SetLogText("test_ImportProfileFromReader()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------test_ImportProfileFromReader - Start ------------------\n\n");	
//	logCurrentExecution( " test_ImportProfileFromReader");
//
//	wchar_t tcPath[512];
//	wcscpy(tcPath,L"C:\\Temp");
//	PROFILE_LIST ProfileList ;
//	if( api3Data.readerType != MC )
//	{	
//		api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &ProfileList);
//
//		wcscpy( api3Data.tcProfileName,*(ProfileList.pProfileName) );
//		api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt,api3Data.tcProfileName,tcPath);
//		logToFile("RFID_ImportProfileFromReader() with vaild parameters");
//		if(RFID_API_SUCCESS == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//		
//		api3Data.rfidStatus = RFID_ImportProfileFromReader(api3Data.hReaderMgmt, api3Data.tcProfileName, L"C:\\Program Files\\Microsoft Visual Studio 8\\Common7\\Tools\\Templates\\Database Project Items\\" );
//		logToFile("RFID_ImportProfileFromReader()with Invalid Handle");
//		if(RFID_API_SUCCESS == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//
//		api3Data.rfidStatus = RFID_ImportProfileFromReader((RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.tcProfileName,tcPath);
//		logToFile("RFID_ImportProfileFromReader()Test with Invalid Handle");
//		if(RFID_API_SUCCESS != api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//
//		api3Data.rfidStatus = RFID_ImportProfileFromReader(api3Data.hReaderMgmt, NULL, NULL );
//		logToFile("RFID_ImportProfileFromReader()Test with Invalid Handle");
//		if(RFID_API_SUCCESS != api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//	}
//	else
//	{
//		api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt, L"myprofile.xml",tcPath );
//		if( RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus)
//		{
//			SetLogResult(TRUE );
//		}
//		logToResultFile( );
//	}
//
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------RFID_ImportProfileFromReader - End ------------------\n\n");
//}   
//
//void test_GetProfileList(void)
//{
//	SetLogText("test_GetProfileList()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------test_GetProfileList - Start ------------------\n\n");	
//	logCurrentExecution( " test_GetProfileList");
//
//	PROFILE_LIST lpProfileList ;
//	if( api3Data.readerType == MC )
//	{
//		lpProfileList.pProfileName = NULL;
//	}
//	api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &lpProfileList);
//	logToFile("RFID_GetProfileList() with vaild parameters");
//	if(RFID_API_SUCCESS == api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//		fprintf( api3Data.fpLog,"\n        No. of Profiles = %d",lpProfileList.numProfiles );
//	}
//	else if( api3Data.readerType != XR &&  RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus )
//	{
//		SetLogResult(TRUE );
//	}
//    logToResultFile( );
//
//	api3Data.rfidStatus = RFID_GetProfileList((RFID_HANDLE32)INVALID_HANDLE_VALUE,&lpProfileList);
//	logToFile("RFID_GetProfileList() with Invalid Handle" );
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//
//	api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt,NULL );
//	logToFile("RFID_GetProfileList() with NULL Buffer");
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult(TRUE );
//	}
//	logToResultFile( );
//	
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------test_GetProfileList - End ------------------\n\n");
//}
//
//void test_UpdateRadioFirmware(void)
//{
//	SetLogText("test_UpdateRadioFirmware()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------test_UpdateRadioFirmware - Start ------------------\n\n");	
//	logCurrentExecution( " test_UpdateRadioFirmware");
//
//    wchar_t pFileName;
//    api3Data.rfidStatus = RFID_UpdateRadioFirmware( api3Data.hReaderMgmt,&pFileName);
//	logToFile("RFID_UpdateRadioFirmware()Test with vaild parameters");
//	if( ( api3Data.readerType == XR ||  api3Data.readerType == FX ) && RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus )
//	{
//		SetLogResult( TRUE);
//	}
//	else if( api3Data.rfidStatus == RFID_API_SUCCESS )
//	{
//		SetLogResult(TRUE );
//	}
//    logToResultFile( );
//		
//    api3Data.rfidStatus = RFID_UpdateRadioFirmware( (RFID_HANDLE32)INVALID_HANDLE_VALUE,&pFileName);
//	logToFile("RFID_UpdateRadioFirmware() with Invalid Handle");
//	if( RFID_API_INVALID_HANDLE == api3Data.rfidStatus ||  RFID_API_SUCCESS != api3Data.rfidStatus )
//    {
//		SetLogResult(TRUE);
//	}	
//    logToResultFile( );
//
//	api3Data.rfidStatus = RFID_UpdateRadioFirmware(api3Data.hReaderMgmt,NULL);
//	logToFile("RFID_UpdateRadioFirmware()with Invalid Firmware File");
//	if( api3Data.rfidStatus == RFID_API_PARAM_ERROR || RFID_API_SUCCESS != api3Data.rfidStatus )
//    {
//		SetLogResult(TRUE);
//	}	
//    logToResultFile( );
//
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------test_UpdateRadioFirmware - End ------------------\n\n");	
//}
//
//void test_Restart(void)
//{
//	SetLogText("test_Restart()- FUN ");
//	fprintf( api3Data.fpLog,"\n\n----------------test_Restart - Start ------------------\n\n");	
//	logCurrentExecution( " test_Restart");
//
//    api3Data.rfidStatus = RFID_Restart( (RFID_HANDLE32)INVALID_HANDLE_VALUE );
//	logToFile("RFID_Restart() Test with Invalid Handle");
//	if(RFID_API_SUCCESS != api3Data.rfidStatus)
//	{
//		SetLogResult( TRUE );
//	}
//	logToResultFile( );
//
//	api3Data.rfidStatus = RFID_Restart( api3Data.hReaderMgmt );
//	logToFile("RFID_Restart() Test with Valid Parameters");
//	if(RFID_API_SUCCESS == api3Data.rfidStatus)
//	{
//		SetLogResult( TRUE );
//	}
//	logToResultFile( );
//
//	LogPassFailCount();
//	fprintf( api3Data.fpLog,"\n\n----------------test_Restart - End ------------------\n\n");	
//}
