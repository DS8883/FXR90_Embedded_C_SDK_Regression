// Monza5.cpp : Test Monza 5 Tags for tag fous and fast ID features
//
#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;	// RFID3 Data
extern MYTAGDATA MyTags[2];

void TestNewStateAwareFilters( ANTENNA_INFO* pInfo, TRIGGER_INFO* tInfo )
{
	
	SINGULATION_CONTROL s;  
	UINT8 TagA22FMask[ ]= { MyTags[0].epc[0], MyTags[0].epc[1], MyTags[0].epc[2], MyTags[0].epc[3] };
	
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings,0,sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings);

	// add a prefilter with the tag mask as A22F
	{
		STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_A_NOT_INV_B};
		addSelectRecord( api3Data.hReader,TagA22FMask,MEMORY_BANK_EPC,32,32,params);
	}

	{
		STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL };
		addSelectRecord( api3Data.hReader,TagA22FMask,MEMORY_BANK_EPC,32,32,params);
	}
	
	// Initialize the Singulation control to S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_ALL );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d - Testing StateAware Singulation: SLALL/ABFLIP",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_SLALL-%d></a> CAPI_SLALL-%d:\n\n<br>Description: Testing SL_ALL and AB_FLIP \n <br> Expected OutPut :Tags with EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
	fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	if( ReadTags( api3Data.hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_SLALL_ABFLIP");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_ALL );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d - Testing StateAware Singulation: SLALL/ABFLIP",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_SLALL-%d></a> CAPI_SLALL-%d:\n\n<br>Description: Testing SL_ALL and AB_FLIP \n <br> Expected OutPut :Tags other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
	fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	if( ReadTags( api3Data.hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_SLALL_ABFLIP");
	
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d - Testing StateAware Singulation: SLALL/ABFLIP",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_SLALL-%d></a> CAPI_SLALL-%d:\n\n<br>Description: Testing SL_ALL and AB_FLIP \n <br> Expected OutPut :Tags with EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
	fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	if( ReadTags( api3Data.hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_SLALL_ABFLIP");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d - Testing StateAware Singulation: SLALL/ABFLIP",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_SLALL-%d></a> CAPI_SLALL-%d:\n\n<br>Description: Testing SL_ALL and AB_FLIP \n <br> Expected OutPut :Tags other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
	fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	if( ReadTags( api3Data.hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,TAGS_OTHERTHAN_PATTERN_ONE) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_SLALL_ABFLIP");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_ASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d - Testing StateAware Singulation: SLALL/ABFLIP",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_SLALL-%d></a> CAPI_SLALL-%d:\n\n<br>Description: Testing SL_ALL and AB_FLIP \n <br> Expected OutPut :Tags with EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
	fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	if( ReadTags( api3Data.hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,PATTERN_ONE_TAGS) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI_SLALL_ABFLIP");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_DEASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	wprintf(L"\n %d - Testing StateAware Singulation: SLALL/ABFLIP",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog,"<br><b><a name=CAPI_SLALL-%d></a> CAPI_SLALL-%d:\n\n<br>Description: Testing SL_ALL and AB_FLIP \n <br> Expected OutPut :Tags other than EPC: %s only Should be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
	fprintf(api3Data.fpCurLog,"<br> Singulation Parameters: Session ->%s InvFlag->%s SlFlag->%s</b>",SESSIONS[s.session],INVENTORYSTATES[s.stateAwareSingulationAction.inventoryState],SLFLAGS[s.stateAwareSingulationAction.slFlag]);

	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	if( ReadTags( api3Data.hReader,MEMORY_BANK_EPC,4 ,pInfo,tInfo,ALL_TAGS) )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI_SLALL_ABFLIP");
}

void Fx7500AbFlipSLAll( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Test_SLALL_ABFLIP ");
	logCurrentExecution( "Test_SLALL_ABFLIP");
	
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 24;
	api3Data.dwTestCaseNo = 01;

	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT;
	tInfo.stopTrigger.value.numAttempts.n = 4; //Test with Two Tags in the field
	tInfo.stopTrigger.value.numAttempts.timeoutMilliseconds = 5000;
	tInfo.tagReportTrigger = 1;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	tInfo.lpTagEventReportInfo = &tagEventInfo;
	
	TestNewStateAwareFilters( &api3Data.pInfo,NULL);	// test with no tag events

	// test with Tag events if supported
	if( api3Data.rCaps.tagEventReportingSupported )
	{
		TestNewStateAwareFilters( &api3Data.pInfo,&tInfo );
	}

	tInfo.lpTagEventReportInfo = NULL;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
	tInfo.stopTrigger.value.tagObservation.n = 5;
	tInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 5000;
	
	TestNewStateAwareFilters( &api3Data.pInfo,&tInfo);

	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 4000;

	TestNewStateAwareFilters( &api3Data.pInfo,&tInfo );
	
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}