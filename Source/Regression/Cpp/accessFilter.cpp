#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;

#ifdef linux
sem_t AccessStopSemaphore;	// Access Stop Semaphore
// Access Stop event Handler
static void MonitorAccessSopEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	wprintf(L"\n monitor stop event function- post event");
	sem_post(&AccessStopSemaphore);
}
#else
HANDLE hAccessStop;
#endif

void TestBlockWriteAccessAPI( RFID_HANDLE32 hReader,UINT8* pData2Write ,MEMORY_BANK mBank,UINT8 offSet,UINT16 dataLength,ACCESS_FILTER* pAccessFilter,ANTENNA_INFO* pInfo  )					
{
	BOOLEAN bSuccess = FALSE;
	
	//LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	LPTAG_DATA pTagData = RFID_AllocateTag(hReader);
	
	WRITE_ACCESS_PARAMS wParams = { mBank,offSet,pData2Write,dataLength,0 };
	READ_ACCESS_PARAMS rParams = { mBank,offSet,dataLength,0 };
	
	//triggerInfo.stopTrigger.value.duration = 2000; 
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RFID_BlockWrite()-%d></a> CAPI-RFID_BlockWrite()-%d:\n\n<br>Description: Writing to Mem Bank:%s  offSet:%d len:%d Data:%s \n <br> Expected OutPut :Should Write to Memory Bank \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],wParams.byteOffset,wParams.writeDataLength,Array2Char(wParams.pWriteData,0,wParams.writeDataLength) );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");

	for(int iteration=0; iteration < 1; iteration++)
	{
		api3Data.rfidStatus = RFID_BlockWrite(hReader,NULL,NULL,&wParams,pAccessFilter,pInfo,0);
#ifdef linux
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&AccessStopSemaphore,&ts) == 0 )
		//if( sem_wait(&AccessStopSemaphore) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
		{
#endif
			wprintf(L"\naccess Stop Event is received\n");

			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				ERROR_INFO einfo;
				RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
				wprintf(L"\n errorTimeStamp:%s \t vendorMessage: %ls \t statusDesc:%ls ",PrintTime(), einfo.vendorMessage,einfo.statusDesc);
				fprintf( api3Data.fpLog, "\n\n Write failure Vendor Msg %S <br>\n", einfo.vendorMessage );	
			}
			else
			{
				fprintf( api3Data.fpLog,"\n Write is Success verify the Data Reading back <br>\n");
				api3Data.rfidStatus = RFID_PurgeTags(hReader,0); 
				api3Data.rfidStatus = RFID_Read( hReader,NULL,NULL,&rParams,pAccessFilter,pInfo,pTagData,0);
#ifdef linux
				clock_gettime(CLOCK_REALTIME, &ts);
				ts.tv_sec += 10;
				if( sem_timedwait(&AccessStopSemaphore,&ts) == 0 )
				//if( sem_wait(&AccessStopSemaphore) == 0 )
				{
#else	
				if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
				{
#endif
					wprintf(L"\naccess Stop Event is received\n");
					while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
					{
						if(pTagData->opStatus == ACCESS_SUCCESS )	printTagDataWithResults( pTagData);
					}
			
					if( memcmp( pTagData->pMemoryBankData,pData2Write,dataLength ) == 0 )
					{
						bSuccess = TRUE;
					}
				}
			}
		}
		else
		{
			wprintf(L"\n RFID_BlockWrite() failure error Msg %ls",RFID_GetErrorDescription( api3Data.rfidStatus));
		}
	}

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	
	logToResultFile(L"CAPI-RFID_BlockWrite()");
	
	RFID_PurgeTags( api3Data.hReader, 0 );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestBlockEraseAccessAPI( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT8 offSet,UINT8 len,ACCESS_FILTER* pAccessFilter,ANTENNA_INFO* pInfo  )					
{	
	BOOLEAN bSuccess = FALSE;
	UINT8 pEraseData[128];
	memset( pEraseData,0x00,128 );
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	
	// TEST with Simple Write Operations without AnyFilters 	
	BLOCK_ERASE_ACCESS_PARAMS blkParams = { mBank,offSet,len,0,{0} };
	READ_ACCESS_PARAMS rParams = { mBank, offSet,len,0 };	

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RFID_BlockErase-%d></a> CAPI-RFID_BlockErase--%d:\n\n<br>Description: Erase Mem Bank :%s with Block Erase  offSet:%d len:%d \n <br> Expected OutPut :Should Erase Mem with lenth and offset \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],blkParams.byteOffset,len );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	for(int iteration=0; iteration < 1; iteration++)
	{
 		api3Data.rfidStatus = RFID_BlockErase(hReader,NULL,NULL,&blkParams,pAccessFilter,pInfo,0);
#ifdef linux
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&AccessStopSemaphore,&ts) == 0 )
		//if( sem_wait(&AccessStopSemaphore) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
		{
#endif
			wprintf(L"\naccess Stop Event is received\n");
			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				ERROR_INFO einfo;
				RFID_GetLastErrorInfo( hReader, &einfo );
				fprintf( api3Data.fpLog, "\n\n Erase failure Vendor Msg %S <br> \n", einfo.vendorMessage );
				wprintf(L"\n Erase failure Vendor Msg %ls",einfo.vendorMessage);
			}
			else
			{
				fprintf( api3Data.fpLog,"\n Erase is Success verify the Data Reading back<br>\n");
				wprintf(L"Erase is Success verify the Data Reading back");
				bSuccess = TRUE;
			}
		}
	}
	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-RFID_BlockErase");

	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		wprintf(L"\n Erase failure error Msg %ls",RFID_GetErrorDescription( api3Data.rfidStatus));
	}
	else
	{
		fprintf( api3Data.fpCurLog,"\n Erase is Success verify the Data Reading back<br>\n");
		bSuccess = TRUE;
		api3Data.rfidStatus = RFID_PurgeTags(hReader,0); 
		api3Data.rfidStatus = RFID_Read( hReader,NULL,NULL,&rParams,pAccessFilter,pInfo,NULL,0);
#ifdef linux
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&AccessStopSemaphore,&ts) == 0 )
		//if( sem_wait(&AccessStopSemaphore) == 0 )
		{
			//sem_init(&AccessStopSemaphore, 0, 0);
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
		{
#endif
			wprintf(L"\naccess Stop Event is received\n");
			while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
			{
				if(pTagData->opStatus == ACCESS_SUCCESS )	printTagDataWithResults( pTagData);
			}
		}
	}
	
	api3Data.rfidStatus = RFID_PurgeTags( hReader,0 );

	//clean up tag allocation	
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestSimpleAccessAPI( RFID_HANDLE32 hReader,UINT8* pData2Write ,MEMORY_BANK mBank,UINT8 offSet,UINT8 len,ACCESS_FILTER* pAccessFilter,ANTENNA_INFO* pInfo )					
{
	BOOLEAN bSuccess = FALSE;
	
	// TEST with Simple Write Operations without AnyFilters 
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	
	READ_ACCESS_PARAMS rParams = { mBank,offSet,len,0};
	WRITE_ACCESS_PARAMS wParams = { mBank,offSet,pData2Write,len,0 };
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RFID_Write-%d></a> CAPI-RFID_Write-%d:\n\n<br>Description: Writing to Mem Bank:%s  offSet:%d len:%d Data:%s \n <br> Expected OutPut :Should Write to Memory Bank \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],wParams.byteOffset,wParams.writeDataLength,Array2Char(wParams.pWriteData,0,wParams.writeDataLength) );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	for(int iteration=0; iteration < 1; iteration++)
	{
		api3Data.rfidStatus = RFID_Write(hReader,NULL,NULL,&wParams,pAccessFilter,pInfo,0);
#ifdef linux
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 10;
		if( sem_timedwait(&AccessStopSemaphore,&ts) == 0 )
		//if( sem_wait(&AccessStopSemaphore) == 0 )
		{
#else	
		if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
		{
#endif		
			wprintf(L"\naccess Stop Event is received\n");
			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				ERROR_INFO einfo;
				RFID_GetLastErrorInfo( hReader, &einfo );
				fprintf( api3Data.fpLog, "\n\n Write failure Vendor Msg %S <br> \n", einfo.vendorMessage );
				wprintf(L"\n Write failure Vendor Msg %ls",einfo.vendorMessage);
			}
			else
			{
				fprintf( api3Data.fpLog,"\n Write is Success verify the Data Reading back<br>\n");
				bSuccess = TRUE;
			}
		}
		else
		{
			wprintf(L"\n RFID_Write() failure error Msg %ls",RFID_GetErrorDescription( api3Data.rfidStatus));
		}
	}

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-RFID_Write");

	api3Data.rfidStatus = RFID_Read( hReader,NULL,NULL,&rParams,pAccessFilter,pInfo,pTagData,0);
#ifdef linux
	//if( sem_wait(&AccessStopSemaphore) == 0 )
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 10;
	if( sem_timedwait(&AccessStopSemaphore,&ts) == 0 )
	{
#else	
	if( WAIT_OBJECT_0 == WaitForSingleObject( hAccessStop, 10000 ) )
	{
#endif
		wprintf(L"\naccess Stop Event is received\n");
		while( RFID_API_SUCCESS == RFID_GetReadTag( hReader,pTagData ) )
		{
			if(pTagData->opStatus == ACCESS_SUCCESS )	printTagDataWithResults( pTagData);
		}
	}
	else
	{
		wprintf(L"\n RFID_Read() failure error Msg %ls",RFID_GetErrorDescription( api3Data.rfidStatus));
	}
	RFID_PurgeTags( hReader,0 );

	RFID_DeletePreFilter( api3Data.hReader,0,0 );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);	
}

// Test Access API with access filter and no Tag ID given.Look for access Start and Access Stop event
//
void TestAccessFilterAPI( )
{
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 15;
	RFID_EVENT_TYPE rfideventType = ACCESS_STOP_EVENT;
	UINT8 Pattern1[2] = { 0xBE,0xDD };
	UINT8 Pattern2[2] = { 0xFE,0xFE };

	//rfid3Sleep(2000);
	// Reader may have disconnected due to profile change, reconnect back.
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort, 0, &api3Data.conInfo );
	wprintf(L"\n Connect Status : %s", RFID_GetErrorDescription(api3Data.rfidStatus));
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.blockEraseSupported && !api3Data.rCaps.blockWriteSupported )
	{
		api3Data.dwSkipCount = 15;
		LogPassFailCount();
		return;
	}
	
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorAccessSopEvent,NULL,NULL);
	sem_init(&AccessStopSemaphore, 0, 0);		
#else
	// Create and Register Inv Stop
	//
	hAccessStop = CreateEvent(NULL, FALSE, FALSE, NULL);	
	//api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT,hAccessStop);
	api3Data.rfidStatus = RFID_RegisterEventNotification(api3Data.hReader, rfideventType, hAccessStop);
#endif

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Test RFID_BlockErase( )");
	logCurrentExecution( "TestBlockErase");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	
	UINT8 tagMask[] = { 0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{ 0 } };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{ 0 } };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL }; 
	
	TestBlockEraseAccessAPI( api3Data.hReader,MEMORY_BANK_USER,0,8,&accessFilter,&api3Data.pInfo );
	TestBlockEraseAccessAPI( api3Data.hReader,MEMORY_BANK_USER,0,16,&accessFilter,&api3Data.pInfo );
	TestBlockEraseAccessAPI( api3Data.hReader,MEMORY_BANK_USER,0,32,&accessFilter,&api3Data.pInfo );
	TestBlockEraseAccessAPI( api3Data.hReader,MEMORY_BANK_USER,0,64,&accessFilter,&api3Data.pInfo );

	// Test Writes to all memory banks
	SetLogText("Test RFID_Write()");
	{
		UINT8 Data2Write[4];
		memset( Data2Write,0x11,4);// = { 0xAB,0x99,0x66,0xFF };
		TestBlockWriteAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,4 ,&accessFilter,&api3Data.pInfo );
		TestBlockWriteAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,4,4,&accessFilter,&api3Data.pInfo  );
	}

	//
	{
		UINT8 Data2Write[] = {
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							 };

		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,64,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0xAA,60 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,4,60,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0xBB,56 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,8,56,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0xCC,52 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,12,52,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0xDD,48 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,16,48,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0xEE,44 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,20,44,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0xFF,40 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,24,40,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0x11,36 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,28,36,&accessFilter,&api3Data.pInfo );
		memset( Data2Write,0x22,32 );
		TestSimpleAccessAPI( api3Data.hReader,Data2Write,MEMORY_BANK_USER,32,32,&accessFilter,&api3Data.pInfo );
	}
	
	RFID_SetTraceLevel( api3Data.hReader,TRACE_LEVEL_OFF );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}