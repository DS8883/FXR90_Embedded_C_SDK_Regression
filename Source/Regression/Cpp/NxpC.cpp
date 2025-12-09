// NXPC.cpp : Tests NXP functionality

#include "Common.h"
#include <stdlib.h>
#include "NxpC.h"
#include "APITest.h"

// global data for API
extern struct RFID3DATA api3Data;

// Semaphore for linux support
#ifdef linux
static sem_t NXPAlarmSemaphore;
static void MonitorRFIDNXPEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&NXPAlarmSemaphore);
}
#endif

// NXP Alarm Code Test with trigger info --- RFID3 API
void nxpAlarmCodeTest( bool bAlarmCode, TRIGGER_INFO* ptInfo )
{
	bool bAlarmSeen = FALSE;
	RFID_EVENT_TYPE rfideventType = NXP_EAS_ALARM_EVENT;

	// linux mechanism of call back and eventing
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorRFIDNXPEvent,NULL,NULL);
	sem_init(&NXPAlarmSemaphore, 0, 0);		
#else
	// for Windows based
	HANDLE hNxpAlarmEvent =  CreateEvent(NULL, FALSE, FALSE, NULL);	// EAS alarm event
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,rfideventType,hNxpAlarmEvent );
#endif
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-ALARM-%d></a> CAPI-NXP-ALARM-%d:\n\n<br>Description: Check EAS Alarm <br> Expected OutPut: Alarm Code to be seen\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_PerformNXPEASScan( api3Data.hReader,NULL,ptInfo,0 );
#ifdef linux
	// Wait for the Event to Trigger
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 10;
	if( sem_timedwait(&NXPAlarmSemaphore,&ts) == 0 )
	{
		wprintf(L"\n alarm event triggered");
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( hNxpAlarmEvent, 5000 ) )
	{
#endif
		
		fprintf( api3Data.fpCurLog,"\n-------NXP alarm Event is Signalled-------" );
		wprintf(L"\n-------NXP alarm Event is Signalled-------" );
		NXP_EAS_ALARM_DATA myEASevent;
		api3Data.rfidStatus = RFID_GetEventData( api3Data.hReader,NXP_EAS_ALARM_EVENT,&myEASevent );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			//wprintf(L"\n Alarm code %I64d Antenna ID: %d ",myEASevent.alarmCode,myEASevent.antennaID );
			wprintf(L"\n Alarm code %lld Antenna ID: %d ",myEASevent.alarmCode,myEASevent.antennaID );
			fprintf( api3Data.fpCurLog,"\n Alarm code %I64d Antenna ID: %d ",myEASevent.alarmCode,myEASevent.antennaID );
			bAlarmSeen = TRUE;
		}			
	}

	// Looking for Alarm Code : Pass if Alarm is seen
	if( bAlarmCode && bAlarmSeen )
	{
		SetLogResult( TRUE );
	}
	// Looking for Alarm Code : FAIL is Alarm is not Seen
	if( bAlarmCode && !bAlarmSeen )
	{
		SetLogResult( FALSE );
	}
	// Not Looking for Alarm Code : FAIL if Alaram is Seen
	if( !bAlarmCode && bAlarmSeen )
	{
		SetLogResult( FALSE );
	}
	// Not Looking for Alarm Code : PASS if Alarm is not Seen 
	if( !bAlarmCode && !bAlarmSeen )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-NXP-ALARM");
	api3Data.rfidStatus = RFID_StopNXPEASScan( api3Data.hReader );
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader, 0 );
#ifdef linux
	sem_destroy(&NXPAlarmSemaphore);	// Destroy Semaphore
#else
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,NXP_EAS_ALARM_EVENT); 
	CloseHandle(hNxpAlarmEvent);
#endif
}

// Verify NXP functionality using Synchronous API
//
void nxpEAS( )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	NXP_SET_EAS_PARAMS setEASParams = { 0x00000001,TRUE,NULL };	// Set NXP EAS Parameters
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-EAS-%d></a> CAPI-NXP-EAS-%d:\n\n<br>Description: RFID_NXPSetEAS --->TRUE <br> Expected OutPut: Set EAS Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	// Set EAS Parameters to the Tag so that EAS functionality gets enabled in NXP Tag
	//
	for( int k=0; k < 2; k++ )
	{
		api3Data.rfidStatus = RFID_NXPSetEAS( api3Data.hReader,api3Data.pnxpTag->pTagID,api3Data.pnxpTag->tagIDLength,&setEASParams,NULL,NULL,NULL);
		printTagDataWithResults(api3Data.pnxpTag); 
		fprintf(api3Data.fpCurLog,"<br> rfidStatus:%S ", RFID_GetErrorDescription( api3Data.rfidStatus ) );
		wprintf(L"\n RFID_NXPSetEAS : %ls",RFID_GetErrorDescription( api3Data.rfidStatus ));
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
		}
		else
		{
			ERROR_INFO einfo;
			RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
			fprintf( api3Data.fpLog, "\n\n Erase failure Vendor Msg %S <br> \n", einfo.vendorMessage );
		}
		
		if( bSuccess )
		{
			SetLogResult( TRUE );
			break;
		}
	}

	logToResultFile(L"CAPI-NXP-EAS");
	bSuccess = FALSE;
	setEASParams.EASState = FALSE;
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-EAS-%d></a> CAPI-NXP-EAS-%d:\n\n<br>Description: RFID_NXPSetEAS ---> FALSE <br> Expected OutPut: Set EAS Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	// Set EAS to FALSE
	//
	for( int k=0; k < 2; k++ )
	{
		api3Data.rfidStatus = RFID_NXPSetEAS( api3Data.hReader,api3Data.pnxpTag->pTagID,api3Data.pnxpTag->tagIDLength,&setEASParams,NULL,NULL,NULL);
		wprintf(L"\n RFID_NXPSetEAS : %ls",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf(api3Data.fpCurLog,"<br> rfidStatus:%S ", RFID_GetErrorDescription( api3Data.rfidStatus ) );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
			break;
		}
	}

	logToResultFile(L"CAPI-NXP-EAS");
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
}

void nxpReadProtect( )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	NXP_READ_PROTECT_PARAMS readProtectParams = { 0x00000001,{0} };	// Set Read Protect params
	NXP_RESET_READ_PROTECT_PARAMS resetParams = { 0x00000001,{0} };	// Reset Read Protect Params
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-EAS-%d></a> CAPI-NXP-EAS-%d:\n\n<br>Description: RFID_NXPReadProtect <br> Expected OutPut: API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	// Attempt three times for the success
	for( int k=0; k < 10; k++ )
	{
		api3Data.rfidStatus = RFID_NXPReadProtect( api3Data.hReader,api3Data.pnxpTag->pTagID,api3Data.pnxpTag->tagIDLength,&readProtectParams,0,0,0);
		rfid3Sleep(1000);
		fprintf(api3Data.fpCurLog,"<br> rfidStatus:%S ", RFID_GetErrorDescription( api3Data.rfidStatus ) );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
			break;
		}
			

		if( bSuccess )
		{
			SetLogResult( TRUE );
			break;
		}
	}

	logToResultFile(L"CAPI-NXP-EAST");
	bSuccess = FALSE;
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-EAS-%d></a> CAPI-NXP-EAS-%d:\n\n<br>Description: RFID_NXPResetReadProtect <br> Expected OutPut: API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	
	// Attempt three times for the success
	for( int k=0; k < 10; k++ )
	{
		api3Data.rfidStatus = RFID_NXPResetReadProtect( api3Data.hReader,&resetParams,0,0 );
		rfid3Sleep( 3000 );
		fprintf(api3Data.fpCurLog,"<br> rfidStatus:%S ", RFID_GetErrorDescription( api3Data.rfidStatus ) );
		wprintf(L"\n nxp Reset Read ProtectStatus :%ls ", RFID_GetErrorDescription( api3Data.rfidStatus ) );

		UINT32 success,failure;
		api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader,&success,&failure );
		fprintf( api3Data.fpCurLog,"<br> Access Sucess: %d failure :%d",success,failure );
		if( success > 0 )
		{
			SetLogResult( TRUE );
			break;
		}
	}
	
	logToResultFile(L"CAPI-NXP-EAS");

	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
}

BOOLEAN getnxptag( )
{
	UINT8 nxpEPC[] = { 0xF0,0x0D };	// Tag EPC hardcoded with EPC as F00D, Always test with a filter.
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	BOOLEAN bnxpTag = FALSE;

	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,nxpEPC,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{0} };
	POST_FILTER postFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL }; 
	
	if( wcscmp(api3Data.rCaps.modelName,L"75004") == 0 )
	{
		postFilter.lpRSSIRangeFilter = NULL;
	}

	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	RFID_DeletePreFilter( api3Data.hReader,0,0 );

	// add a select command to read only NXP tag
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL ,0,0,0);
	rfid3Sleep( 10000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	RFID_DeletePreFilter( api3Data.hReader,0,0 );

	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		// Check if the Tag is desired Tag for Regression
		if (pTagData->pTagID[0] == 0xF0 && pTagData->pTagID[1] == 0x0D)
		{
			memcpy(api3Data.pnxpTag->pTagID, pTagData->pTagID, pTagData->tagIDLength);
			api3Data.pnxpTag->tagIDLength = pTagData->tagIDLength;
			bnxpTag = TRUE;
			break;
		}
	}

	if(bnxpTag == FALSE)
	{
		wprintf(L"\n NXp Tag Was tag was not found");
		return FALSE;
	}
	
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );

	if(api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		return TRUE;
	}
	else
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		return FALSE;
	}
}
// Verify Read Protect with Op Sequence
//
void nxpSetOpSeqProtect( ACCESS_OPERATION_CODE OP )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-READPROTECT-%d></a> CAPI-NXP-READPROTECT-%d:\n\n<br>Description: %s <br> Expected OutPut: Set READPROTECT Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,ACCESSOPERATIONCODES[OP] );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	NXP_RESET_READ_PROTECT_PARAMS nxpreset = { 0x00000001,{0} };
	OP_CODE_PARAMS opParams = { OP,&nxpreset,{0} };
	UINT32 opIndex;

	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 10000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader, 0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		//wprintf(L"\n TagID : %ls AccessResult: %ls ",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus] );
		//fprintf( api3Data.fpCurLog,"\n TagID : %s AccessResult: %s <br>",Byte2HexString(pTagData->pTagID,pTagData->tagIDLength),ACCESSSTATUS[pTagData->opStatus] );
		printTagDataWithResults( pTagData);
		if( pTagData->opStatus == ACCESS_SUCCESS )
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
			break;
		}
	}

	if( bSuccess )
	{
		SetLogResult( TRUE );
	}
	
	if( OP == ACCESS_OPERATION_NXP_RESET_READ_PROTECT )
	{
		UINT32 success,failure;
		api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader,&success,&failure );
		fprintf( api3Data.fpCurLog,"<br> Access Sucess: %d failure :%d",success,failure );
		wprintf( L"\nAccess Sucess: %ld failure :%ld",success,failure );
		if( success > 0 )
		{
			SetLogResult( TRUE );
		}
		else
		{
			SetLogResult(FALSE);
		}
	}

	logToResultFile(L"CAPI-NXP-READPROTECT");
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
}

// Set EAS with Op Sequence
//
void nxpSetOpSeqEAS( BOOLEAN eas )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-EAS-%d></a> CAPI-NXP-EAS-%d:\n\n<br>Description: %s ---> %d <br> Expected OutPut: Set EAS Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,ACCESSOPERATIONCODES[ACCESS_OPERATION_NXP_SET_EAS],eas );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	NXP_SET_EAS_PARAMS nxpEAS = { 0x00000001,eas,{0} };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_NXP_SET_EAS,&nxpEAS,{0} };
	UINT32 opIndex;
	RFID_PurgeTags(api3Data.hReader,0);
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader, 0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		//wprintf(L"\n TagID : %ls AccessResult: %ls <br>",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus] );
		//fprintf( api3Data.fpCurLog,"\n TagID : %s AccessResult: %s <br>",Byte2HexString(pTagData->pTagID,pTagData->tagIDLength),ACCESSSTATUS[pTagData->opStatus] );
		printTagDataWithResults( pTagData);
		if( pTagData->opStatus == ACCESS_SUCCESS )
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
			break;
		}
	}

	if( bSuccess )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-NXP-EAS");
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
}
// G2IL Testing
void nxpOpSeqResetConfigG2IL( UINT16 ConfigWord )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-NXP-G2IL-%d></a> CAPI-NXP-G2IL-%d:\n\n<br>Description: Change ConfigWord %4x <br> Expected OutPut:Change ConfigWord Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,ConfigWord );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	NXP_CHANGE_CONFIG_PARAMS nxpChangeConfig = { 0x00000001,ConfigWord,{0} };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_NXP_CHANGE_CONFIG,&nxpChangeConfig,{0} };
	UINT32 opIndex;

	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 10000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader, 0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		wprintf(L"\n TagID : %ls AccessResult: %ls <br>",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus] );
		fprintf( api3Data.fpCurLog,"\n TagID : %s AccessResult: %s <br>",Byte2HexString(pTagData->pTagID,pTagData->tagIDLength),ACCESSSTATUS[pTagData->opStatus] );
		printTagDataWithResults( pTagData);
		if( pTagData->opStatus == ACCESS_SUCCESS )
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
			break;
		}
	}

	if( bSuccess )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-NXP-G2IL");
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );

}
// G2IL Change Config - Manual
void nxpChangeConfigG2IL( )
{
	UINT16 ConfigWords[] = { 0x01,0x01, 0x10, 0x10,0x40,0x40, 0x200,0x200, 0x400,0x400, 0x600,0x600 };
	NXP_CHANGE_CONFIG_PARAMS nxpChangeConfig;
    nxpChangeConfig.accessPassword = 0x00000001;

	for( int ConfigWordCount =0;ConfigWordCount<12;ConfigWordCount++)
	{
		nxpChangeConfig.NXPChangeConfigWord = ConfigWords[ConfigWordCount];
		// Test RFID Change Config for NXPG2IL
		api3Data.rfidStatus = RFID_NXPChangeConfig( api3Data.hReader,api3Data.pnxpTag->pTagID,api3Data.pnxpTag->tagIDLength,&nxpChangeConfig,NULL,NULL,api3Data.pnxpTag,0);
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			wprintf(L"\n Change Config Result %x",api3Data.pnxpTag->lpAccessOperationResult->NXPChangeConfigWordResult.ChangeConfigWord);
		}
		else
		{
			wprintf(L"\n Change Config FAILED %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		}
		nxpChangeConfig.NXPChangeConfigWord = 0x00;
		api3Data.rfidStatus = RFID_NXPChangeConfig( api3Data.hReader,api3Data.pnxpTag->pTagID,api3Data.pnxpTag->tagIDLength,&nxpChangeConfig,NULL,NULL,api3Data.pnxpTag,0);
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			wprintf(L"\n Change Config Result %x",api3Data.pnxpTag->lpAccessOperationResult->NXPChangeConfigWordResult.ChangeConfigWord);
		}
		else
		{
			wprintf(L"\n Change Config FAILED %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		}
	}

}
// Nxp Testing.
void nxpTagTest( )
{
	api3Data.dwTestCaseNo = 01;
	SetLogText("nxpTagTest");
	logCurrentExecution( "nxpTagTest");
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.NXPCommandsSupported)
	{
		api3Data.dwSkipCount = 13;
		LogPassFailCount();
		return;
	}
	wprintf(L"\n Executing %ls Started\n ",MultiByte2WideChar( __FUNCTION__));
	
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 13;
	TRIGGER_INFO trInfo;
	memset(&trInfo,0,sizeof(trInfo));
	trInfo.tagReportTrigger	= 1; 
	trInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	trInfo.stopTrigger.value.tagObservation.n = 5; 
	trInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 3000;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	trInfo.lpTagEventReportInfo = &tagEventInfo;

	TAG_STORAGE_SETTINGS tgSettings;
	
	api3Data.rfidStatus  = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings );
	tgSettings.enableAccessReports = TRUE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );
	
	nxpSetOpSeqProtect( ACCESS_OPERATION_NXP_RESET_READ_PROTECT );
	
	if( FALSE == getnxptag( ) )
	{
		wprintf(L"\n Could not get NXP tag, test cannot proceed");
		fprintf(api3Data.fpCurLog,"\n Could not get NXP tag, test cannot proceed");
	}
	else
	{
		nxpSetOpSeqEAS( TRUE );
		nxpAlarmCodeTest(TRUE, NULL );
		nxpAlarmCodeTest( TRUE, &trInfo );
		nxpSetOpSeqEAS( FALSE );
		nxpAlarmCodeTest(FALSE, NULL );
		nxpAlarmCodeTest(FALSE, &trInfo );
		nxpEAS( );
		nxpReadProtect( );
		nxpSetOpSeqProtect( ACCESS_OPERATION_NXP_READ_PROTECT );
		nxpSetOpSeqProtect( ACCESS_OPERATION_NXP_RESET_READ_PROTECT );
		//nxpOpSeqResetConfigG2IL(0x00);
		//nxpChangeConfigG2IL( );
		
	}

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Completed",MultiByte2WideChar( __FUNCTION__));
}

