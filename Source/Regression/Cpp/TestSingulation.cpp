#include "stdafx.h"
#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[2];

void addSelectRecord( RFID_HANDLE32 hReader, UINT8* pattern,MEMORY_BANK mBank,UINT8 bitoffset,UINT8 patternbitCount,STATE_AWARE_ACTION_PARAMS sParams )
{
	PRE_FILTER pf;
	memset(&pf,0,sizeof(PRE_FILTER));
	UINT32 index;
	pf.memoryBank = mBank;
	pf.pTagPattern = pattern;
	pf.tagPatternBitCount = patternbitCount;
	pf.bitOffset = bitoffset;
	pf.filterAction = FILTER_ACTION_STATE_AWARE;
	pf.filterActionParams.stateAwareParams= sParams;
	pf.truncateAction = TRUNCATE_ACTION_DO_NOT_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( hReader, 0,&pf,&index );
}

void addanWareSelectRecord( RFID_HANDLE32 hReader, UINT8* pattern,MEMORY_BANK mBank,UINT16 bitoffset,UINT8 patternbitCount,FILTER_ACTION fAction,STATE_UNAWARE_ACTION selectParam )
{	
	PRE_FILTER pf;
	memset(&pf,0,sizeof(PRE_FILTER));
	UINT32 index;
	pf.memoryBank = mBank;
	pf.pTagPattern = pattern;
	pf.tagPatternBitCount = patternbitCount;
	pf.bitOffset = bitoffset;
	pf.filterAction = fAction;
	pf.filterActionParams.stateUnawareAction = selectParam;
	pf.truncateAction = TRUNCATE_ACTION_DO_NOT_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter( hReader, 0,&pf,&index );
}

BOOLEAN ReadTagsStateUnWareSingulation( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT16 offSet,ANTENNA_INFO* pInfo,STATE_UNAWARE_ACTION action )
{
	TRIGGER_INFO triggerInfo;
	UINT16 tagCount = 0;
	UINT16 byteOffSet  = offSet / 8; // change bit offset to byte off set.
	
	memset( &triggerInfo,0,sizeof(TRIGGER_INFO));
	
	triggerInfo.tagReportTrigger	= 1; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_DURATION;
	triggerInfo.stopTrigger.value.duration = 3000;
			
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
	api3Data.rfidStatus = RFID_PurgeTags( hReader, 0 );
	api3Data.rfidStatus = RFID_PerformInventory(hReader,NULL,pInfo,NULL,NULL );
	rfid3Sleep( 5000 );	
	api3Data.rfidStatus = RFID_StopInventory( hReader );	
	rfid3Sleep(3000);
	bool bSuccess = TRUE;

	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{		
		tagCount++;
		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d SeenCount :%d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),pTagData->peakRSSI,pTagData->tagSeenCount );
		wprintf(L"\nTAG ID : %ls RSSI: %d SeenCount :%d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength ),pTagData->peakRSSI,pTagData->tagSeenCount );
		switch(action)
		{
			case STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT:
				//read only matching tags , failure is any non matching tag is read.
				//
				if (mBank == MEMORY_BANK_EPC || mBank == MEMORY_BANK_USER )
				{
					if ((pTagData->pTagID[0] != 0xA2) || (pTagData->pTagID[1] != 0x2F))
					{
						bSuccess = FALSE;
					}
				}
				if (mBank == MEMORY_BANK_TID)
				{
					if ((pTagData->pTagID[0] != 0xDA) || (pTagData->pTagID[1] != 0xDA))
					{
						bSuccess = FALSE;
					}
				}
				break;
			case STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT:

				// read only non matching tags, failure if a matching tag is read.
				if (mBank == MEMORY_BANK_EPC || mBank == MEMORY_BANK_USER)
				{
					if (pTagData->pTagID[0] == 0xA2)
					{
						bSuccess = FALSE;
					}
				}
				if (mBank == MEMORY_BANK_TID)
				{
					if (pTagData->pTagID[0] == 0xDA)
					{
						bSuccess = FALSE;
					}
				}
				break;
			case STATE_UNAWARE_ACTION_UNSELECT:
				// read only non matching tags, failure if a matching tag is read.
				if (mBank == MEMORY_BANK_EPC || mBank == MEMORY_BANK_USER)
				{
					if (pTagData->pTagID[0] == 0xA2)
					{
						bSuccess = FALSE;
					}
				}
				if (mBank == MEMORY_BANK_TID)
				{
					if (pTagData->pTagID[0] == 0xDA)
					{
						bSuccess = FALSE;
					}
				}
				break;
			default:
				bSuccess = TRUE;
				break;
		}

		if(bSuccess == FALSE)
		{
			break;
		}
	}
	
	api3Data.rfidStatus = RFID_DeallocateTag( hReader,pTagData );
		
	if(tagCount == 0)
	{
		return FALSE;
	}

	return bSuccess;
}

BOOLEAN ReadTagsAutoMode( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT16 offSet,ANTENNA_INFO* pInfo,STATE_UNAWARE_ACTION action  )
{
	TRIGGER_INFO triggerInfo;
	memset(&triggerInfo,0,sizeof(triggerInfo));
	UINT16 tagCount = 0;
	UINT16 byteOffSet  = offSet / 8; // change bit offset to byte off set.
	
	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	
	triggerInfo.lpTagEventReportInfo = &tagEventInfo;
	
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
	api3Data.rfidStatus = RFID_PurgeTags( hReader, 0 );
	api3Data.rfidStatus = RFID_PerformInventory(hReader,NULL,pInfo,&triggerInfo,NULL );
	rfid3Sleep( 5000 );	
	api3Data.rfidStatus = RFID_StopInventory( hReader );	
	
	bool bSuccess = TRUE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{		
		tagCount++;
		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d SeenCount :%d TAGEVENT: %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ),pTagData->peakRSSI,pTagData->tagSeenCount,TAGEVENTS[pTagData->tagEvent] );
		wprintf(L"\nTAG ID : %ls RSSI: %d SeenCount :%d TAGEVENT: %ls",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength ),pTagData->peakRSSI,pTagData->tagSeenCount,wTAGEVENTS[pTagData->tagEvent] );
		switch (action)
		{
			case STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT:
				//read only matching tags , failure is any non matching tag is read.
				//
				if (mBank == MEMORY_BANK_EPC || mBank == MEMORY_BANK_USER)
				{
					if ((pTagData->pTagID[0] != 0xA2) || (pTagData->pTagID[1] != 0x2F))
					{
						bSuccess = FALSE;
					}
				}
				if (mBank == MEMORY_BANK_TID)
				{
					if ((pTagData->pTagID[0] != 0xDA) || (pTagData->pTagID[1] != 0xDA))
					{
						bSuccess = FALSE;
					}
				}
				break;
			case STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT:

				// read only non matching tags, failure if a matching tag is read.
				if (mBank == MEMORY_BANK_EPC || mBank == MEMORY_BANK_USER)
				{
					if (pTagData->pTagID[0] == 0xA2)
					{
						bSuccess = FALSE;
					}
				}
				if (mBank == MEMORY_BANK_TID)
				{
					if (pTagData->pTagID[0] == 0xDA)
					{
						bSuccess = FALSE;
					}
				}
				break;
			case STATE_UNAWARE_ACTION_UNSELECT:
				// read only non matching tags, failure if a matching tag is read.
				if (mBank == MEMORY_BANK_EPC || mBank == MEMORY_BANK_USER)
				{
					if (pTagData->pTagID[0] == 0xA2)
					{
						bSuccess = FALSE;
					}
				}
				if (mBank == MEMORY_BANK_TID)
				{
					if (pTagData->pTagID[0] == 0xDA)
					{
						bSuccess = FALSE;
					}
				}
				break;
			default:
				bSuccess = TRUE;
				break;
		}

		if(bSuccess == FALSE)
		{
			break;
		}
	}

	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData );
	RFID_DeletePreFilter( hReader,0,0);
	if(tagCount == 0)
	{
		return FALSE;
	}
	return bSuccess;
}
void TestSingleSelectRecords( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo,TRIGGER_INFO* tInfo )
{
	UINT8 pattern1[4];
	pattern1[0] = MyTags[0].epc[0];
	pattern1[1] = MyTags[0].epc[1];
	pattern1[2] = MyTags[0].epc[2];
	pattern1[3] = MyTags[0].epc[3];
	{
		RFID_DeletePreFilter( hReader,0,0);
		RFID_ResetConfigToFactoryDefaults( hReader );
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL };
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s; 
		InitSingulationControl( &s,SESSION_S0,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Which Match EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
		
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	
	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S0,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);

		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
		
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");

	{
		RFID_DeletePreFilter( hReader,0,0);
		RFID_ResetConfigToFactoryDefaults( hReader );
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL };
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s; 
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
	
		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Which Match EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	
	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);

		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");

	{
		RFID_DeletePreFilter( hReader,0,0);
		RFID_ResetConfigToFactoryDefaults( hReader );
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_B_NOT_INV_A};	
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL };
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S2,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);

		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Which Match EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");

	{
		SINGULATION_CONTROL s ;
		InitSingulationControl( &s,SESSION_S2,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");

	{
		RFID_DeletePreFilter( hReader,0,0);
		RFID_ResetConfigToFactoryDefaults( hReader );
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S3,STATE_AWARE_ACTION_INV_B_NOT_INV_A};	
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL };
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S3,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Which Match EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S3,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		wprintf(L"\n %d - Testing StateAware Singulation: Single Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
}

void TestStateOWareSingulation( RFID_HANDLE32 hReader, ANTENNA_INFO* pInfo,TRIGGER_INFO* tInfo )
{
	UINT8 pattern1[4] = { MyTags[0].epc[0],MyTags[0].epc[1],MyTags[0].epc[2],MyTags[0].epc[3]};
	UINT8 pattern2[4] = { MyTags[1].epc[0],MyTags[2].epc[1],MyTags[3].epc[2],MyTags[4].epc[3]};
	UINT8 pattern3[2] = { 0xBE,0xDD };
	

	TestSingleSelectRecords( hReader, pInfo,tInfo );		
	
	{
		{
			RFID_DeletePreFilter( hReader,0,0);
			RFID_ResetConfigToFactoryDefaults( hReader );
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
			params.target = TARGET_SL; params.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_B_NOT_INV_A};			
			addSelectRecord( hReader,pattern2,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S0,100,0,INVENTORY_STATE_A,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Multiple Select Records \n <br> Expected OutPut :Tags other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Multiple Select Records \n <br> Expected OutPut :Tags withn EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_TWO ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	
	{
		// Test Multiple Select Records.
		{
			RFID_DeletePreFilter( hReader,0,0);
			RFID_ResetConfigToFactoryDefaults( hReader );
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_SL,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL};
			
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			
			addSelectRecord( hReader,pattern2,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
	
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing 3 Select Records\n  Expected OutPut :Tags with EPC: %s only Should be Read  \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[1].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,PATTERN_TWO_TAGS ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	
	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing 3 Select Records\n  Expected OutPut :Tags with EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,PATTERN_ONE_TAGS ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");

	// Add more than 3 filters.
	//
	{
		// Test Multiple Select Records.
		{
			RFID_DeletePreFilter( hReader,0,0);
			RFID_ResetConfigToFactoryDefaults( hReader );
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_SL,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL};
			
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			
			addSelectRecord( hReader,pattern2,MEMORY_BANK_EPC,32,32,params);
		}
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S3,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			
			addSelectRecord( hReader,pattern3,MEMORY_BANK_EPC,32,16,params);
		}
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing 4 Select Records\n  Expected OutPut :Tags with EPC: %s only Should be Read  \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,PATTERN_ONE_TAGS ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	
	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S2,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a nameCAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing 4 Select Records\n  Expected OutPut :Tags with EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[1].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,PATTERN_TWO_TAGS ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");

	{
		SINGULATION_CONTROL s;
		InitSingulationControl( &s,SESSION_S3,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation:  Multiple Select Records",api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a>CAPI_STATEAWARE-%d:\n\n<br>Description: Testing 4 Select Records\n  Expected OutPut :Tags with EPC: BEDD only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}

	
	if( ReadTags( hReader,MEMORY_BANK_EPC,4, pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_TWO ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
}

int TestStateanWareSingulation( RFID_HANDLE32 hReader,UINT8* Pattern1,MEMORY_BANK mBank ,ANTENNA_INFO* pInfo,UINT16 offSet )
{
	// 0: Matching tags: SELECT , Non-matching tags: UNSELECT	
	// 1: Matching tags: SELECT, Non-matching tags: do nothing	
	// 2 :Matching tags: do nothing, Non-matching tags: UNSELECT	
	// 3: Matching tags: UNSELECT , Non-matching tags: do nothing	
	// 4: Matching tags: SELECT , Non-matching tags: UNSELECT	
	// 5: Matching tags: do nothing, Non-matching tags: SELECT
	wprintf(L"\nTestStateanWareSingulation");
	rfid3Sleep( 5000 ); // to get back the tags to original state
	// Test PreFilters on different Tags
	
	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br> Description: Testing Stateunaware Singulation %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s  Should Only be Read </b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT );
		if( ReadTagsStateUnWareSingulation( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");

		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s  Should be Read Non Matching tags may Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_SELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT );
		if( ReadTagsStateUnWareSingulation( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_SELECT ) )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-STATEUNAWARE");

		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation %s on Memory Bank : %s \n <br>  Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s May Read NON Matching Should not Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_NOT_UNSELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_NOT_UNSELECT );
		if( ReadTagsStateUnWareSingulation( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_NOT_UNSELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");
		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s Should not Read NON Matching May Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_UNSELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_UNSELECT );
		if( ReadTagsStateUnWareSingulation( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_UNSELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");
		
		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s Should not be Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT );
		if( ReadTagsStateUnWareSingulation( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");
		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s may Read, Other tags should be Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_NOT_SELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");		
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_NOT_SELECT );
		if( ReadTagsStateUnWareSingulation( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_NOT_SELECT ) )
		{
			SetLogResult( TRUE );
		}
		
		logToResultFile(L"CAPI-STATEUNAWARE");
		CleanupTagQueue();
	}
	
	return TRUE;
}

int RFID3PreFilterAutoMode( RFID_HANDLE32 hReader,UINT8* Pattern1,MEMORY_BANK mBank ,ANTENNA_INFO* pInfo,UINT16 offSet )
{
	// 0: Matching tags: SELECT , Non-matching tags: UNSELECT	
	// 1: Matching tags: SELECT, Non-matching tags: do nothing	
	// 2 :Matching tags: do nothing, Non-matching tags: UNSELECT	
	// 3: Matching tags: UNSELECT , Non-matching tags: do nothing	
	// 4: Matching tags: SELECT , Non-matching tags: UNSELECT	
	// 5: Matching tags: do nothing, Non-matching tags: SELECT
	

	rfid3Sleep( 5000 ); // to get back the tags to original state
	// Test PreFilters on different Tags
	//
	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br> Description: Testing Stateunaware Singulation  %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s  Should Only be Read</b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT );
		if( ReadTagsAutoMode( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");

		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation  %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s  Should be Read Non Matching tags may Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_SELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT );
		if( ReadTagsAutoMode( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_SELECT ) )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-STATEUNAWARE");

		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation  %s on Memory Bank : %s \n <br>  Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s May Read NON Matching Should not Read<br></b></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_NOT_UNSELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_NOT_UNSELECT );
		if( ReadTagsAutoMode( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_NOT_UNSELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");
		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation  %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s Should not Read NON Matching May Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_UNSELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_UNSELECT );
		if( ReadTagsAutoMode( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_UNSELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");
		
		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation  %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s Should NOT be Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT );
		if( ReadTagsAutoMode( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-STATEUNAWARE");
		CleanupTagQueue();
	}

	{
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI-STATEUNAWARE-%d></a> CAPI-STATEUNAWARE-%d:\n\n<br>Description: Testing Stateunaware Singulation  %s on Memory Bank : %s \n <br> Expected OutPut :Tags Which Match MEMBANK: %s OffSet:%d starts as %s may Read, Other tags should be Read<br></b>\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,UNAWAREACTION[STATE_UNAWARE_ACTION_NOT_SELECT],G2MEMBANK[mBank],G2MEMBANK[mBank],offSet,Array2Char( Pattern1,0,4 ) );
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");		
		addanWareSelectRecord( hReader,Pattern1,mBank,offSet,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_NOT_SELECT );
		if( ReadTagsAutoMode( hReader,mBank,offSet,pInfo,STATE_UNAWARE_ACTION_NOT_SELECT ) )
		{
			SetLogResult( TRUE );
		}
		
		logToResultFile(L"CAPI-STATEUNAWARE");
		CleanupTagQueue();
	}
	
	return TRUE;
}
void TestMultiplePreFilters( )
{
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	
	BYTE Pattern1[2]={0xDE,0xCA};
	BYTE Pattern2[2]={0xA2,0x2F};
	BYTE Pattern3[2]={0xB2,0x2F};
	
	addanWareSelectRecord( api3Data.hReader,Pattern1,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT );
	addanWareSelectRecord( api3Data.hReader,Pattern2,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT );
	addanWareSelectRecord( api3Data.hReader,Pattern3,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT );
	
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 5000 );	
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
	rfid3Sleep( 2000 );

	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		printTagDataWithResults( pTagData);
	}
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);

}



void RFID3UnAwareAutoMode(  ANTENNA_INFO* pInfo )
{
	if (!api3Data.rCaps.tagEventReportingSupported)
	{
		wprintf(L"\n autonomous mode events are not supported\n");
		return;
	}

	UINT8 Pattern[ 4 ];
	// test singulation with TID memory bank
	//
	memcpy(Pattern, api3Data.pG2V2Tag->pMemoryBankData, 4);
	RFID3PreFilterAutoMode(api3Data.hReader, Pattern, MEMORY_BANK_TID, pInfo, 0);

	memcpy(Pattern, &(api3Data.pG2V2Tag->pMemoryBankData[4]), 4);
	RFID3PreFilterAutoMode(api3Data.hReader, Pattern, MEMORY_BANK_TID, pInfo, 32);

	memcpy(Pattern, &(api3Data.pG2V2Tag->pMemoryBankData[8]), 4);
	RFID3PreFilterAutoMode(api3Data.hReader, Pattern, MEMORY_BANK_TID, pInfo, 64);
	
	// Initialize the patterns with CRC+PC as below
	//
	Pattern[ 0 ] = HIBYTE( MyTags[0].PC);
	Pattern[ 1 ] = LOBYTE( MyTags[0].PC);
	Pattern[ 2 ] = MyTags[0].epc[0];
	Pattern[ 3 ] = MyTags[0].epc[1];
	
	// test singulation with EPC memory bank
	//
	RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,16 );
	
	// test singulation with USER memory bank
	//
	memcpy(Pattern,MyTags[0].user,4);
	RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,0 );	
	
	// test singulation from an off set 32 in the memory banks
	//
	if( MyTags[0].epclen >= 4 )
	{
		memcpy( Pattern,MyTags[0].epc,4 );
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,32 );
	}
	
	if( MyTags[0].userlen >= 8 )
	{
		memcpy( Pattern,&(MyTags[0].user[4]),4 );
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,32 );
	}
	
	// test singulation from an off set 32 in the memory banks
	//
	if( MyTags[0].epclen >= 8 )
	{
		memcpy( Pattern,&(MyTags[0].epc[4]),4 );
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,64 );
	}
	
	if( MyTags[0].userlen >= 12 )
	{
		memcpy(Pattern,&(MyTags[0].user[8]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,64 );
	}
	
	// test singulation from an off set 64 in the memory banks
	//
	if( MyTags[0].epclen >= 12 )
	{
		memcpy( Pattern,&(MyTags[0].epc[8]),4 );
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,96 );
	}

	if( MyTags[0].userlen >= 16 )
	{
		memcpy(Pattern,&(MyTags[0].user[12]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,96 );
	}
	if( MyTags[0].userlen >= 20 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[16]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,128 );
	}
	if( MyTags[0].userlen >= 24 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[20]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,160 );
	}

	if( MyTags[0].userlen >= 28 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[24]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,192 );
	}

	if( MyTags[0].userlen >= 32 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[28]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,224 );
	}
	if( MyTags[0].userlen >= 36 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[32]),4);
		RFID3PreFilterAutoMode( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,256 );
	}

}

void TestRFID3StateUnAwareSingulation(  ANTENNA_INFO* pInfo )
{
	UINT8 Pattern[ 4 ];

	// test singulation with TID memory bank
	//
	memcpy(Pattern,api3Data.pG2V2Tag->pMemoryBankData,4);
	TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_TID,pInfo,0 );
	
	memcpy( Pattern,&(api3Data.pG2V2Tag->pMemoryBankData[4]),4 );
	TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_TID,pInfo,32 );
	
	memcpy(Pattern, &(api3Data.pG2V2Tag->pMemoryBankData[8]), 4);
	TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_TID,pInfo,64 );

	// Initialize the patterns with CRC+PC as below
	//
	Pattern[ 0 ] = HIBYTE( MyTags[0].PC);
	Pattern[ 1 ] = LOBYTE( MyTags[0].PC);
	Pattern[ 2 ] = MyTags[0].epc[0];
	Pattern[ 3 ] = MyTags[0].epc[1];
	
	// test singulation with EPC memory bank
	//
	TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,16 );
	
	// test singulation with USER memory bank
	//
	memcpy(Pattern,MyTags[0].user,4);
	TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,0 );	
	
	// test singulation from an off set 32 in the memory banks
	//
	if( MyTags[0].epclen >= 4 )
	{
		memcpy( Pattern,MyTags[0].epc,4 );
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,32 );
	}
	
	if( MyTags[0].userlen >= 8 )
	{
		memcpy( Pattern,&(MyTags[0].user[4]),4 );
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,32 );
	}
	
	// test singulation from an off set 32 in the memory banks
	//
	if( MyTags[0].epclen >= 8 )
	{
		memcpy( Pattern,&(MyTags[0].epc[4]),4 );
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,64 );
	}
	
	if( MyTags[0].userlen >= 12 )
	{
		memcpy(Pattern,&(MyTags[0].user[8]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,64 );
	}
	
	// test singulation from an off set 64 in the memory banks
	//
	if( MyTags[0].epclen >= 12 )
	{
		memcpy( Pattern,&(MyTags[0].epc[8]),4 );
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_EPC,pInfo,96 );
	}

	if( MyTags[0].userlen >= 16 )
	{
		memcpy(Pattern,&(MyTags[0].user[12]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,96 );
	}
	if( MyTags[0].userlen >= 20 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[16]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,128 );
	}
	if( MyTags[0].userlen >= 24 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[20]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,160 );
	}

	if( MyTags[0].userlen >= 28 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[24]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,192 );
	}

	if( MyTags[0].userlen >= 32 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[28]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,224 );
	}
	if( MyTags[0].userlen >= 36 )
	{
		// varying offset for user mem bank
		memcpy(Pattern,&(MyTags[0].user[32]),4);
		TestStateanWareSingulation( api3Data.hReader,Pattern,MEMORY_BANK_USER,pInfo,256 );
	}

}

void TestRFIDStateUnAware( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	
	// Test State unaware Singulaton with diferent memorybanks and offsets
	//
	SetLogText("RFID3Singulation-UnAWARE");
	logCurrentExecution( "RFID3Singulation-UnAWARE");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 96;
	api3Data.dwTestCaseNo = 01;
	TestRFID3StateUnAwareSingulation( &api3Data.pInfo );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

void StateunAwareWithTagevents( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	api3Data.itemTestCount = 96;
	api3Data.dwTestCaseNo = 01;
	SetLogText("RFID3Singulation-UnAWARE-AUTOMODE");
	logCurrentExecution( "RFID3Singulation-UnAWARE-AUTOMODE");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	RFID3UnAwareAutoMode( &api3Data.pInfo );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

/*void Test_SLALL_ABFLIP()
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Test_SLALL_ABFLIP ");
	logCurrentExecution( "Test_SLALL_ABFLIP");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	UINT8 pattern1[4];
	pattern1[0] = MyTags[0].epc[0];
	pattern1[1] = MyTags[0].epc[1];
	pattern1[2] = MyTags[0].epc[2];
	pattern1[3] = MyTags[0].epc[3];

	api3Data.itemTestCount = 30;
	api3Data.dwTestCaseNo = 01;
	RFID_PurgeTags( api3Data.hReader, 0 );
	
	{
		RFID_DeletePreFilter( api3Data.hReader,0,0);
		RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
			addSelectRecord( api3Data.hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_DSRT_SL_NOT_ASRT_SL };
			addSelectRecord( hReader,pattern1,MEMORY_BANK_EPC,32,32,params);
		}
	}

	{
		SINGULATION_CONTROL s; 
		InitSingulationControl( &s,SESSION_S0,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_DEASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( hReader, 0,&s);
		
		wprintf(L"\n %d - Testing StateAware Singulation: ABFLIP",api3Data.dwTestCaseNo);
		
		fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_STATEAWARE-%d></a> CAPI_STATEAWARE-%d:\n\n<br>Description: Testing StateAware Singulation: Single Select Records \n <br> Expected OutPut :Tags Which Match EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
	}
		
	if( ReadTags( hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_STATEAWARE");
	
	
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings,0,sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings);
	{
		STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_A_NOT_INV_B};
		addSelectRecord( api3Data.hReader,TagFocusMask,MEMORY_BANK_EPC,32,16,params);
	}

	{
		STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL };
		addSelectRecord( api3Data.hReader,TagFocusMask,MEMORY_BANK_EPC,32,16,params);
	}
	
	
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_ALL );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n ----------------Tags E200 should only be reported-----------------------\n");
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_ALL );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n ----------------Tags other than E200 should be reported---------------------------\n");
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n --------------------Tags E200 should only be reported---------------------------\n");
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n --------------------Tags other than E200 should be reported---------------\n");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_ASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	wprintf(L"\n ------------------Tags E200 should only be reported--------------------\n");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_DEASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	wprintf(L"\n ------------------All Tags be reported--------------------\n");
}*/
void TestRFIDStateAware( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestRFID3Singulation-State A Ware");
	logCurrentExecution( "TestRFID3Singulation-State A Ware");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 30;
	api3Data.dwTestCaseNo = 01;
	RFID_PurgeTags( api3Data.hReader, 0 );
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);
	api3Data.rfidStatus = RFID_DeletePreFilter(api3Data.hReader,0,0);

	// Test State Awre Singulaton with Single,Double,Three Select Commands by adding Prefilters.
	// Call this function after emptying the queue with Purging the Tags
	//
	TRIGGER_INFO triggerInfo = { {START_TRIGGER_TYPE_IMMEDIATE},{STOP_TRIGGER_TYPE_IMMEDIATE},1,NULL,NULL,NULL,NULL };
	
	TestStateOWareSingulation( api3Data.hReader,NULL,NULL );
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	triggerInfo.lpTagEventReportInfo = &tagEventInfo;
	TestStateOWareSingulation( api3Data.hReader,&api3Data.pInfo,&triggerInfo );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
