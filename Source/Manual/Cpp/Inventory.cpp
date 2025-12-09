#include "ManualTests.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace std;

static vector<string>TagList;	// Vector Tag List

extern struct RFID3DATA api3Data;
struct ARGS
{
	wchar_t ip[128];
	UINT32 Port;
};

//extern char* PrintTime( void );
BOOLEAN stopTesting = FALSE;

#ifdef linux
static sem_t RfidEventSemaphore;
pthread_t * threadHandle;
void* MonitorTagEventEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&RfidEventSemaphore);
}
#else
static HANDLE hTagEvent;
#endif 

void* PrintTagCount( void * pvarg )
{
	RFID_HANDLE32 readerHandle = (RFID_HANDLE32)pvarg;

	while(!stopTesting)
	{
#ifndef linux
		if( WAIT_OBJECT_0 == WaitForSingleObject( hTagEvent, 10000 ) )
		{
#else
		if( sem_wait(&RfidEventSemaphore) == 0)
		{
#endif
			LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
			 while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
			 {
				 printTagDataWithResults(pTagData);
				 api3Data.TagCount++;
			 }
			RFID_DeallocateTag( api3Data.hReader, pTagData );
			
		}
	}
	return NULL;
}

void TestPeriodicReport( RFID_VERSION version)
{
	TAG_STORAGE_SETTINGS tgSettings;
	CONNECTION_INFO ConInfo = { version, 0 };
	RFID_EVENT_TYPE rfideventType = TAG_READ_EVENT ;
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	//api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, TEXT("COM30"), 921600, 0, &ConInfo);
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings);


#ifdef linux
		threadHandle = new pthread_t;
		sem_init(&RfidEventSemaphore, 0, 0); 
		api3Data.rfidStatus = RFID_RegisterEventNotificationCallback(api3Data.hReader, &rfideventType,  1, (RfidEventCallbackFunction) MonitorTagEventEvent, NULL, NULL);
		pthread_create(threadHandle, NULL, PrintTagCount, (void *)(api3Data.hReader));
		wprintf(L"\n ThreadCreate %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
#else
		DWORD dwThreadID; 
		hTagEvent = CreateEvent( NULL,0,0,NULL );	// Tag Event
		api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,rfideventType,hTagEvent);
		
		HANDLE hEventThread = CreateThread(NULL, 0, (unsigned long (WINAPI *)(void *))PrintTagCount,  api3Data.hReader, 0, &dwThreadID);
#endif	
	REPORT_TRIGGERS report;
	memset(&report,0,sizeof(report));
	TRIGGER_INFO trInfo;
	memset(&trInfo,0,sizeof(trInfo));
	report.periodicReportDuration = 10;

	trInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	trInfo.stopTrigger.type = STOP_TRIGGER_TYPE_IMMEDIATE;
	trInfo.tagReportTrigger = 10;
	trInfo.lpReportTriggers = &report;
	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,&trInfo,0);
	rfid3Sleep( 60000);
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
#ifdef linux
		//sem_destroy(&RfidEventSemaphore);	
		//pthread_join(*threadHandle, NULL );
		delete threadHandle;
#else
		api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,TAG_READ_EVENT );
		TerminateThread(hEventThread,0);
#endif
	stopTesting = TRUE;	
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
void TestPostFilters()
{
	wchar_t pmatchPattern[4][32] = { L"A_AND_B",L"NOTA_AND_B",L"NOTA_AND_NOTB",L"A_AND_NOTB" };
	UINT8 patternA[] = { 0xE2,0xC0, 0x68, 0x92, 0x20, 0x00, 0x4C, 0x02, 0x1F, 0x0B, 0x3C, 0x23 };
	UINT8 patternB[] = { 0xE2,0x00,0x34,0x12,0x01,0x34,0xF4,0x00,0x01,0x0D,0xB3,0x22,0x0E,0x07,0x01,0x40,0x30,0x04,0x5F,0xFB,0xFF,0xFF,0xDC,0x50 };
	UINT8 TagMask[64];
	memset(TagMask, 0xFF, 64);

	POST_FILTER postFilter;
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	TRIGGER_INFO triggerInfo;									// trigger info
	memset(&triggerInfo, 0, sizeof(triggerInfo));
	triggerInfo.tagReportTrigger = 0;						// Report after the Stop 
	triggerInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;	// Imm Start
	triggerInfo.stopTrigger.type = STOP_TRIGGER_TYPE_IMMEDIATE;	// Imm Stop
#ifndef PIXIE
	triggerInfo.lpExtraTriggerInfo = NULL;
#endif
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000 }; // Tag event info initialization

	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 10000, &ConInfo);
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	for (int dataLen = 16; dataLen <= 96; dataLen = dataLen + 16)
	{
		
		TAG_PATTERN tpA = { MEMORY_BANK_TID,0,patternA,dataLen,TagMask,dataLen,0 };
		TAG_PATTERN tpB = { MEMORY_BANK_TID,0,patternB,dataLen,TagMask,dataLen,0 };
		POST_FILTER postFilter; postFilter.lpTagPatternA = &tpA; postFilter.lpTagPatternB = &tpB;
		postFilter.lpRSSIRangeFilter = NULL;
#ifndef PIXIE
		postFilter.lpTagListFilter = NULL;
#endif
		for (int matchPattern = A_AND_B; matchPattern <= A_AND_NOTB; matchPattern++)
		{
			wprintf(L"\n Test1 with match Pattern: %ls \n", pmatchPattern[matchPattern]);
			postFilter.matchPattern = (MATCH_PATTERN)matchPattern;
			api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, &api3Data.pInfo, &triggerInfo, 0);
			rfid3Sleep(5000);
			api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
			while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
			{
				printTagDataWithResults(pTagData);
			}
		}
	}
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	

}

void StressInventory( int Count,RFID_VERSION version )
{
	TAG_STORAGE_SETTINGS tgSettings;
	wprintf(L"\n Stress Invnentory  START TIME: %ls",PrintTime() );
	
	if(api3Data.hReader)RFID_Disconnect(api3Data.hReader);
	for(int i=0; i < Count ;i++)
	{
		CONNECTION_INFO ConInfo = { version, 0 };
		//api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
		api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 10000, &ConInfo);
		//api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 10000, &ConInfo);
		//api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, TEXT("10.17.129.227"), 9180, 0, &ConInfo);
		//api3Data.rfidStatus = RFID_ConnectA(&api3Data.hReader, "10.17.130.229", 5084, 0, &ConInfo);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			READER_CAPS rCaps;
			VERSION_INFOA versionInfoA;
			api3Data.rfidStatus = RFID_GetDllVersionInfoA(&versionInfoA);
			printf("\n Dll Verison: %s\n", versionInfoA.dllVersion);
			wprintf(L"\nRFID Connection Succeeded...attempt %ld",i);
			api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
			tgSettings.tagFields = ALL_TAG_FIELDS;
			api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);
			api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &rCaps);
			wprintf(L"\n CountryCode : %ld\t CountryName: %ls\n",rCaps.countryCode,GetCountryNamebyCode(rCaps.countryCode) );
		}
		else
		{
			wprintf(L"\n Failed to Connect the Reader: ErrorText: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		}
		
		api3Data.pInfo.length = 0;
		api3Data.pInfo.pAntennaList = NULL;
		api3Data.pInfo.pAntennaOpList = NULL;
		TRIGGER_INFO triggerInfo = { {START_TRIGGER_TYPE_IMMEDIATE},{STOP_TRIGGER_TYPE_IMMEDIATE},0,NULL,NULL,NULL,NULL };
		//api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, &api3Data.pInfo,&triggerInfo, NULL );
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL,NULL, NULL);
		wprintf(L"\n Called inventory\n");
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
		while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
		{
			api3Data.TagCount++;
			printTagDataWithResults(pTagData);
		}
		wprintf(L"\nTag Count for iteration:%d TagCount:%d\n",i,api3Data.TagCount);
		api3Data.TagCount=0;
		RFID_PurgeTags(api3Data.hReader,0);
		wprintf(L"\n Purge Complete\n");
		api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader, pTagData);
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
		wprintf(L"\n rfid Disconnect Status %ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));
	}

	wprintf(L" END TIME: %ls",PrintTime() );
	api3Data.bContinue = FALSE;



	//Memory Leak Test for RFID_Connect()
	//VERSION_INFO versionInfo;
	//VERSION_INFOA versionInfoA;
	//while (1)
	//{

	//	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 5000, NULL);
	//	wprintf(L"\n rfid Connect Stauts:%ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));
	//	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	//	{
	//		// Set trace level
	//		api3Data.rfidStatus = RFID_SetTraceLevel(&api3Data.hReader, TRACE_LEVEL_ALL);
	//		// Get DLL Version
	//		RFID_GetDllVersionInfoA(&versionInfoA);
	//		printf("\n Dll Verison: %ls\n", versionInfoA.dllVersion);

	//		RFID_GetDllVersionInfo(&versionInfo);
	//		wprintf(L"\n Dll Verison: %ls", versionInfo.dllVersion);
	//		// Do something - Configurations, Capabilities, Inventory, Access
	//		api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	//		wprintf(L"\n rfid disConnect Stauts:%ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));
	//	}
	//	else
	//	{
	//		wprintf(L"RFID_Connect() fail with error : %ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));
	//	}

	//	rfid3Sleep(3000);

	//}
}

// Read performance for perticular duration 
// [in] iterations -- Number of iterations to be done for average count
// [in] DurationInMs -- Duration in Milli Seconds
// [in] session -- Session in which perfromance to be measures.
// Not Considered -- DutyCycle, PowerLevel etc.
//
void ReadPerformance(int antenna,int iterations,int DurationInMs,SESSION session,RFID_VERSION version )
{
	UINT32 TagCount = 0;
	ANTENNA_INFO* pantennaInfo = new ANTENNA_INFO();
	CONNECTION_INFO ConInfo = { version, 0 };
	UINT16 antennaIDList[1] = {antenna};
	OPERATION_QUALIFIER opq[1] = {C1G2_OPERATION};
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 10000, &ConInfo);
	if( antenna > 0 )
	{
		pantennaInfo->length = 1;
		pantennaInfo->pAntennaList = new UINT16(antenna);
		pantennaInfo->pAntennaOpList = new OPERATION_QUALIFIER(C1G2_OPERATION); 
	}
	
	// Set the tag storage settigns
	//
	TAG_STORAGE_SETTINGS tgSettings;
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
	tgSettings.discardTagsOnInventoryStop = FALSE;
	tgSettings.maxMemoryBankByteCount = 64;
	tgSettings.maxTagCount = 10000;
	tgSettings.maxTagIDByteCount = 12;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader,&tgSettings);

	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	for( int iterationCount=0;iterationCount<iterations;iterationCount++)
	{
		SINGULATION_CONTROL singControl;
		RFID_GetSingulationControl(api3Data.hReader,1,&singControl);
		singControl.session = session;
		RFID_SetSingulationControl(api3Data.hReader,0,&singControl);
		TagCount = 0;
		api3Data.rfidStatus		= RFID_PerformInventory(api3Data.hReader, NULL,pantennaInfo,NULL, 0);
		rfid3Sleep( DurationInMs );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		rfid3Sleep(2000);
		wprintf(L"\n------------------------------------");
		wprintf(L"\n Iteration Count : %d",iterationCount);
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
		{
			printTagDataWithResults(pTagData);
			//Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,pTagData->pTagID,pTagData->tagIDLength);
			TagCount++;	
		}

		int timeInSec = DurationInMs / 1000;
		wprintf( L"\nTotal tags read- %d " , TagCount);
		//double rate = (double)TagCount /5;//divisor;
		double rate = (double)TagCount / timeInSec;//divisor;
		wprintf(L"\nRead rate- %2.2f " , rate);
		
		vector<string>::const_iterator ii;
		std::sort(TagList.begin(), TagList.end());
		cout<<"\n sorted tagcount : "<<TagList.size()<<endl;
		TagList.erase(std::unique(TagList.begin(), TagList.end()), TagList.end());
		
		cout<< "\n Unique tag Count: "<<TagList.size()<<endl;

		for(ii=TagList.begin();ii!=TagList.end();ii++)
		{
			cout << *ii << endl;
		}

		TagList.clear();
	}

	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}

static void rfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	switch (eventType)
	{

	case TAG_DATA_EVENT:
	{
		wprintf(L"\nTAG_DATA_EVENT Triggered handle %ld \n", readerHandle);
		LPTAG_DATA pTagData = RFID_AllocateTag(readerHandle);
		while (RFID_API_SUCCESS == RFID_GetReadTag(readerHandle, pTagData))
		{
			printTagDataWithResults(pTagData);
		}
		RFID_DeallocateTag(readerHandle, pTagData);
		break;
	}
	case INVENTORY_START_EVENT:
		wprintf(L"\nINVENTORY_START_EVENT Triggered \n");
		break;
	case INVENTORY_STOP_EVENT:
		wprintf(L"\nINVENTORY_STOP_EVENT Triggered\n");
		break;
	default:
		break;
	}
}

void* threadFunction( void* pthreadArgs)
{
	ARGS* pArgs = (ARGS*)pthreadArgs;
	RFID_HANDLE32 hReader;
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	RFID_EVENT_TYPE rfidEvents[3] = { INVENTORY_START_EVENT,TAG_DATA_EVENT,INVENTORY_STOP_EVENT};
	wprintf(L"\n IP : %ls Port : %ld\n", pArgs->ip, pArgs->Port);
	RFID_STATUS rfidStatus = RFID_Connect( &hReader,pArgs->ip,pArgs->Port,0,&ConInfo );
	wprintf(L"\n rfidStatus: %ls", RFID_GetErrorDescription(rfidStatus));
	rfidStatus = RFID_RegisterEventNotificationCallback(hReader, rfidEvents, 3, (RfidEventCallbackFunction)rfidEventCallback, NULL, NULL);
	rfidStatus		= RFID_PerformInventory(hReader, NULL,NULL,NULL, 0);
	rfid3Sleep( 100000 );
	/*rfidStatus = RFID_StopInventory( hReader );
	rfid3Sleep( 5000);
	wprintf(L"\n-------------------------------------------------------");
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		printTagDataWithResults(pTagData);
	}
	wprintf(L"\n-------------------------------------------------------");
	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( hReader );*/
	return 0;
}

void Connect2multipleHosts( wchar_t* ip, UINT32 Port)
{
	ARGS* pmyArgs =(ARGS*) malloc(sizeof(ARGS)); ;
	wcscpy(pmyArgs->ip, ip);
	pmyArgs->Port = Port;
	
#ifdef linux
		threadHandle = new pthread_t;
		pthread_create(threadHandle, NULL, threadFunction,(void*)ip);
		wprintf(L"\n ThreadCreate %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
#else
		DWORD dwThreadID; 
		CreateThread(NULL, 0, (unsigned long (WINAPI *)(void *))threadFunction, pmyArgs, 0, &dwThreadID);
#endif	

}

void PeriodicReportingTags( RFID_VERSION version, int periodic )
{
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );

}

void TagObservationwithTagEvents( RFID_VERSION version )
{
	int Count;
	wprintf(L"\n Input the number of times you want to Run the Test:");
	wscanf(L"%ld", &Count);
	/*for (int i = 0; i < Count; i++) {*/
		CONNECTION_INFO ConInfo = { version, 0 };
		api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
		LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
		TRIGGER_INFO tInfo;
		int tagObservation = 0;
		memset(&tInfo, 0, sizeof(tInfo));
		/*wprintf(L"\n TagObservation Count[1 .. 1000]:");
		wscanf(L"%d", &tagObservation);*/

		tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
		tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
		tInfo.stopTrigger.value.duration = 10000;
		//tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_IMMEDIATE;
		//tInfo.stopTrigger.value.tagObservation.n = tagObservation; // Test with Two Tags in the field
		tInfo.tagReportTrigger = 1;
		TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000 };
		//TAG_EVENT_REPORT_INFO tagEventInfo = { MODERATED,8000,MODERATED,8000,MODERATED,8000 };
		//TAG_EVENT_REPORT_INFO tagEventInfo = { MODERATED,500,MODERATED,500,MODERATED,500 };
		tInfo.lpTagEventReportInfo = &tagEventInfo;
		//RFID_PurgeTags(api3Data.hReader, NULL);
		for (int i = 0; i < Count; i++) {
			api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, &tInfo, NULL);
			rfid3Sleep(12000);
			api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
			UINT16 tagSeenCount = 0;
			while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
			{
				tagSeenCount += pTagData->tagSeenCount;
				//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ));
				printTagDataWithResults(pTagData);
			}

			wprintf(L"\nIteration - %d, Number of tags observed - %d", i, tagSeenCount);
		}
		RFID_DeallocateTag(api3Data.hReader, pTagData);
		api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	//}
}

void Truncate( RFID_VERSION version )
{
	PRE_FILTER pf;
	SINGULATION_CONTROL s;
	UINT32 index;					// pre filter index
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );
	LPTAG_DATA pboobTag = RFID_AllocateTag(api3Data.hReader);
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		//  If Truncate action is TRUNCATE_ACTION_TRUNCATE the PC should  be ZERO.
		//
		printTagDataWithResults(pTagData);
		if (pTagData->pTagID[0] == 0xb0 && pTagData->pTagID[1] == 0x0b)
		{
			pboobTag->PC = pTagData->PC;
			pboobTag->pTagID[0] = HIBYTE(pTagData->PC);
			pboobTag->pTagID[1] = LOBYTE(pTagData->PC);
			//pboobTag->pTagID++; pboobTag->pTagID++;
			memcpy(&(pboobTag->pTagID[2]), pTagData->pTagID, pTagData->tagIDLength);
			RFID_PurgeTags(api3Data.hReader, NULL);
			break;
		}
	}
	/*UINT8 pmyTagPattern[256] = { 0 };
	int myindex = 2;
	pmyTagPattern[0] = 0x78; pmyTagPattern[1] = 0x00;
	for (int tagidlen = 0; tagidlen < 30; tagidlen++)
	{
		pmyTagPattern[myindex] = pboobTag->pTagID[tagidlen];
		myindex++;
	}*/

	for (int bitcount = 16; bitcount < 256; bitcount = bitcount + 16)
	{
		
		// Adding pre filter with truncate action unspecified
		//
		pf.bitOffset = 16; pf.filterAction = FILTER_ACTION_STATE_AWARE;
		pf.filterActionParams.stateAwareParams.target = TARGET_SL;
		pf.filterActionParams.stateAwareParams.stateAwareAction = STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL;
		pf.memoryBank = MEMORY_BANK_EPC; pf.pTagPattern = pboobTag->pTagID; pf.tagPatternBitCount = bitcount;
		pf.truncateAction = TRUNCATE_ACTION_TRUNCATE;
		//pf.truncateAction = TRUNCATE_ACTION_DO_NOT_TRUNCATE;
		

		api3Data.rfidStatus = RFID_AddPreFilter(api3Data.hReader, 0, &pf, &index);

		s.session = SESSION_S0; s.stateAwareSingulationAction.inventoryState = INVENTORY_STATE_A;
		s.stateAwareSingulationAction.slFlag = SL_FLAG_ASSERTED;
		s.stateAwareSingulationAction.perform = TRUE;

		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
		{
			//  If Truncate action is TRUNCATE_ACTION_TRUNCATE the PC should  be ZERO.
			//
			printTagDataWithResults(pTagData);
		}
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
	}

	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
