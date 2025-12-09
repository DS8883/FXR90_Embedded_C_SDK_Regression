#include "ManualTests.h"

extern struct RFID3DATA api3Data;

#ifndef linux
// Can be verified 3 things
// 1) Multiple operations in antenna info
// 2) EAS and C1G2 in NXP Scan
// 3) Discard tags on inv stop.( Capture inv stop time and observe the time reported by the tags.
//
HANDLE haldleList[3];
DWORD globalTagCount;
static DWORD WINAPI ProcessMyEvents( LPVOID lpParam )
{
	HANDLE *pHandleList = (HANDLE*)lpParam;
	DWORD dwStatus;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	globalTagCount =0;
	while( api3Data.bContinue )
	{
		dwStatus = WaitForMultipleObjects( 3,pHandleList,0,INFINITE );

		switch(dwStatus)
		{

			case WAIT_OBJECT_0:	// EAS event
				{
					NXP_EAS_ALARM_DATA myEASevent;
					while( RFID_API_SUCCESS == RFID_GetEventData( api3Data.hReader,NXP_EAS_ALARM_EVENT,&myEASevent))
					{
						wprintf(L"\n EAS Event is signalled with --> Alarm code %lld Antenna ID: %d ",myEASevent.alarmCode,myEASevent.antennaID);
					}
				}
				break;
			
			case WAIT_OBJECT_0 + 1:	// tag read event
				{
					//globalTagCount++;
					if( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
					{
						printTagDataWithResults( pTagData );
					}
				}
				break;
			case WAIT_OBJECT_0 + 2:	// inv stop event
				{
					wprintf(L"\n Inv Stop Time %ls",PrintTime());
				}
				break;
			default:
				break;
		}
	}
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	return TRUE;
}
		
void nxpSetOpSeqEAS( BOOLEAN eas )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pmyTagData = RFID_AllocateTag( api3Data.hReader );
	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	NXP_SET_EAS_PARAMS nxpEAS = { 0x00000001,eas,0 };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_NXP_SET_EAS,&nxpEAS,0};
	UINT32 opIndex;
	RFID_PurgeTags(api3Data.hReader,0);
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader, 0 );
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pmyTagData ) )
	{
		printTagDataWithResults( pmyTagData);
		if( pmyTagData->opStatus == ACCESS_SUCCESS )
		{
			bSuccess = TRUE;
			wprintf(L"\n Set EAS Success");
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );
			break;
		}
	}
	
	if( pmyTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pmyTagData );
}

void nxpOpSeqResetConfigG2IL(UINT16 ConfigWord)
{
	BOOLEAN bSuccess = FALSE;
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	
	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence(api3Data.hReader);
	NXP_CHANGE_CONFIG_PARAMS nxpChangeConfig = { 0x00000001,ConfigWord,{0} };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_NXP_CHANGE_CONFIG,&nxpChangeConfig,{0} };
	UINT32 opIndex;

	api3Data.rfidStatus = RFID_AddOperationToAccessSequence(api3Data.hReader, &opParams, &opIndex);
	api3Data.rfidStatus = RFID_PerformAccessSequence(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(10000);
	api3Data.rfidStatus = RFID_StopAccessSequence(api3Data.hReader);
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence(api3Data.hReader);

	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		wprintf(L"\n TagID : %ls AccessResult: %ls <br>", Byte2HexWString(pTagData->pTagID, pTagData->tagIDLength), wACCESSSTATUS[pTagData->opStatus]);
		printTagDataWithResults(pTagData);
		if (pTagData->opStatus == ACCESS_SUCCESS)
		{
			bSuccess = TRUE;
			api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
			break;
		}
	}

	
	if (pTagData != NULL) RFID_DeallocateTag(api3Data.hReader, pTagData);
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
}

void DoNxpScan( RFID_VERSION rfidVersion )
{
	DWORD dwThreadID;
	TRIGGER_INFO tInfo;
	ANTENNA_INFO antennaInfo;
	TAG_STORAGE_SETTINGS tgSettings;
	
	ANTENNA_INFO* pantennaInfo = new ANTENNA_INFO();
	pantennaInfo->length = 4;
	pantennaInfo->pAntennaList = new UINT16[4];
	pantennaInfo->pAntennaOpList = new OPERATION_QUALIFIER[4];
	pantennaInfo->pAntennaList[0] = 1;
	pantennaInfo->pAntennaList[1] = 2;
	pantennaInfo->pAntennaList[2] = 3;
	pantennaInfo->pAntennaList[3] = 4;
	pantennaInfo->pAntennaOpList[0] = pantennaInfo->pAntennaOpList[1] = C1G2_OPERATION;//NXP_EAS_SCAN;
	pantennaInfo->pAntennaOpList[2] = pantennaInfo->pAntennaOpList[3] = C1G2_OPERATION;

	memset(&tInfo,0,sizeof(tInfo));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 3000;
	tInfo.tagReportTrigger = 1;

	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	
	//nxpSetOpSeqEAS(TRUE); // Set EAS -->TRUE

	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	tgSettings.discardTagsOnInventoryStop = TRUE;
	tgSettings.maxTagCount = 10000;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings);
	
	haldleList[0] = CreateEvent(NULL, FALSE, FALSE, NULL);	
	haldleList[1] = CreateEvent(NULL, FALSE, FALSE, NULL);	
	haldleList[2] = CreateEvent(NULL, FALSE, FALSE, NULL);	

	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,NXP_EAS_ALARM_EVENT,haldleList[0]);
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,TAG_READ_EVENT,haldleList[1]);
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT,haldleList[2]);

	HANDLE hThread = CreateThread(NULL, 0, ProcessMyEvents, (LPVOID)(haldleList), 0, &dwThreadID );

	api3Data.bContinue = TRUE;
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, pantennaInfo,&tInfo, NULL );
	rfid3Sleep(5000);
	
	wprintf(L"\n tag count %ld\n",globalTagCount);
	pantennaInfo->pAntennaOpList[0] = pantennaInfo->pAntennaOpList[1] = pantennaInfo->pAntennaOpList[2] = pantennaInfo->pAntennaOpList[3] = NXP_EAS_SCAN;
	api3Data.rfidStatus = RFID_PerformNXPEASScan( api3Data.hReader,pantennaInfo,&tInfo,0 );
	rfid3Sleep(15000);
	api3Data.rfidStatus = RFID_StopNXPEASScan( api3Data.hReader );

	api3Data.bContinue = FALSE;
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
#endif
