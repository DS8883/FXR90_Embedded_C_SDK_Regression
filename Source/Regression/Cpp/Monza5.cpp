// AbFlipSlAll.cpp : Test SL_ALL, AB_FLIP feature for FX7500 Readers.
//
#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;	// RFID3 Data

void Monza5TagFocus()
{
	api3Data.dwTestCaseNo = 7;
	UINT8 TagFocusMask[][4] ={{0xE2,0x80,0x19,0x30},{0xC5,0x00,0x32,0x60},{0x8A,0x00,0x64,0xC0},{0x14,0x00,0xC9,0x81},{0x28,0x01,0x93,0x02},{0x50,0x03,0x26,0x04},{0xA0,0x06,0x4C,0x08}};
	BOOLEAN bSuccess = FALSE;
	SINGULATION_CONTROL s; 
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 5000;
	tInfo.tagReportTrigger = 0;
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	

	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.ImpinjCommandSupported)
	{
		api3Data.dwSkipCount = 7;
		LogPassFailCount();
		return;
	}

	api3Data.itemTestCount = 7;

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Monza5TagFocus");
	logCurrentExecution( "Monza5TagFocus");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");

	//  The Tag will reported only once during the Session , Seen count is One 
	//
	for( int mask = 0; mask < 7;mask++)
	{
		bSuccess = FALSE;	// reinitialize
		fprintf(api3Data.fpCurLog,"\n\n<a name=M5TAGFOCUS-FUN-%d></a> M5TAGFOCUS-FUN-%d:\n\n<br>Description:Tag Focus TID  Mask: %ls \t Bit OffSet: %ld<br> Expected OutPut: Expect CAFE Tags to be Read only Once\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Byte2HexWString(TagFocusMask[mask],4),mask);
		fprintf( api3Data.fpCurLog,"<br>");
		
		STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_B_NOT_INV_A};
		addSelectRecord( api3Data.hReader,TagFocusMask[mask],MEMORY_BANK_TID,mask,32,params);
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_ALL );
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,&tInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			if( pTagData->pTagID[0] == 0xCA && pTagData->pTagID[1] == 0xFE && pTagData->tagSeenCount == 1 )
			{
				bSuccess = TRUE;
			}
			printTagDataWithResults( pTagData );
			
		}

		if( bSuccess )
		{
			SetLogResult(TRUE);
		}
		logToResultFile(L"M5-TAGFOCUS");
		RFID_DeletePreFilter(api3Data.hReader,0,0);
		RFID_SetRadioPowerState( api3Data.hReader,FALSE);	// To reinitialize the tag state
		rfid3Sleep( 4000 );
		RFID_SetRadioPowerState( api3Data.hReader,TRUE);
	}

	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));
}

void Monza5FastID()
{
	// Note : for the Last Mask the Tags does not result in EPC+TID and only EPC. The behaviour is expected.
	// For each iteration expect the tag to return EPC+TID for Inventory.PC >= 6000 
	api3Data.dwTestCaseNo = 7;
	UINT8 FastIDMask[][4] ={{0xE2,0x80,0x1B,0x30},{0xC5,0x00,0x36,0x60},{0x8A,0x00,0x6C,0xC0},{0x14,0x00,0xD9,0x81},{0x28,0x01,0xB3,0x02},{0x50,0x03,0x66,0x04},{0xA0,0x06,0xCC,0x08}};
	BOOLEAN bSuccess = FALSE;
	SINGULATION_CONTROL s; 
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 5000;
	tInfo.tagReportTrigger = 0;
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	

	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.ImpinjCommandSupported)
	{
		api3Data.dwSkipCount = 7;
		LogPassFailCount();
		return;
	}

	api3Data.itemTestCount = 7;

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Monza5FastID");
	logCurrentExecution( "Monza5FastID");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");

	//  The Tag will reported only once during the Session , Seen count is One 
	//
	for( int mask = 0; mask < 7;mask++)
	{
		bSuccess = FALSE;	// reinitialize
		fprintf(api3Data.fpCurLog,"\n\n<a name=M5FASTID-FUN-%d></a> M5FASTID-FUN-%d:\n\n<br>Description:FASTID TID  Mask: %ls \t Bit OffSet: %ld<br> Expected OutPut: Expect CAFE Tags to be returning EPC+TID \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Byte2HexWString(FastIDMask[mask],4),mask);
		fprintf( api3Data.fpCurLog,"<br>");
		
		STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_A};
		addSelectRecord( api3Data.hReader,FastIDMask[mask],MEMORY_BANK_TID,mask,32,params);
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_ALL );
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,&tInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			if( pTagData->pTagID[0] == 0xCA && pTagData->pTagID[1] == 0xFE && pTagData->PC > 0x6000)
			{
				bSuccess = TRUE;
			}

			printTagDataWithResults( pTagData );
			
		}
		if( bSuccess )
		{
			SetLogResult(TRUE);
		}
		logToResultFile(L"M5-FASTID");
		RFID_DeletePreFilter(api3Data.hReader,0,0);
		
	}

	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));
}