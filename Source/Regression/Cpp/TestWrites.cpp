#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[2];
//static char G2MEMBANK[][32] = {"MEMORY_BANK_RESERVED", "MEMORY_BANK_EPC", "MEMORY_BANK_TID", "MEMORY_BANK_USER"};

void TestBlockWriteOperations( RFID_HANDLE32 hReader,UINT8* pData2Write ,MEMORY_BANK mBank,UINT8 offSet,UINT16 dataLength,ANTENNA_INFO* pInfo  )					
{
	BOOLEAN bSuccess = FALSE;
	RFID_STATUS rfidStatus;
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	
	WRITE_ACCESS_PARAMS wParams = { mBank,offSet,pData2Write,dataLength,0 };
	READ_ACCESS_PARAMS rParams = { mBank,offSet,dataLength,0 };
	
	//triggerInfo.stopTrigger.value.duration = 2000; 
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-BLKWRITE-%d></a> CAPI-BLKWRITE-%d:\n\n<br>Description: Writing to Mem Bank:%s  offSet:%d len:%d Data:%s \n <br> Expected OutPut :Should Write to Memory Bank \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],wParams.byteOffset,wParams.writeDataLength,Array2Char(wParams.pWriteData,0,wParams.writeDataLength) );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	if(mBank == MEMORY_BANK_EPC )
	{
		getBEDDTag();
	}
	for(int iteration=0; iteration < 5; iteration++)
	{
		rfidStatus = RFID_BlockWrite( hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&wParams,NULL,pInfo,0);
		if(rfidStatus == RFID_API_SUCCESS )
		{
			break;
		}
	}
	ERROR_INFO einfo;
	if(mBank == MEMORY_BANK_EPC )
	{
		getBEDDTag();
	}

	if( rfidStatus != RFID_API_SUCCESS )
	{
		RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
		fprintf( api3Data.fpLog, "\n\n Write failure Vendor Msg %S <br>\n", einfo.vendorMessage );	
	}
	else
	{
		fprintf( api3Data.fpLog,"\n Write is Success verify the Data Reading back <br>\n");
		
		if(( api3Data.rfidStatus = RFID_Read( hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&rParams,NULL,pInfo,pTagData,0) ) == RFID_API_SUCCESS )
		{
			printTagDataWithResults( pTagData);
		}
	
		if( memcmp( pTagData->pMemoryBankData,pData2Write,dataLength ) == 0 )
		{
			bSuccess = TRUE;
		}
	}

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	
	logToResultFile(L"CAPI-BLKWRITE");
	
	RFID_PurgeTags( api3Data.hReader, 0 );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestBlockEraseOperations( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT8 offSet,UINT8 len,ANTENNA_INFO* pInfo  )					
{	
	BOOLEAN bSuccess = FALSE;
	RFID_STATUS rfidStatus;
	UINT8 pEraseData[128];
	memset( pEraseData,0x00,128 );
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	
	// TEST with Simple Write Operations without AnyFilters 	
	BLOCK_ERASE_ACCESS_PARAMS blkParams = { mBank,offSet,len,0,{0} };
	READ_ACCESS_PARAMS rParams = { mBank, offSet,len,0 };	

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-BLKERASE-%d></a> CAPI-BLKERASE--%d:\n\n<br>Description: Erase Mem Bank :%s with Block Erase  offSet:%d len:%d \n <br> Expected OutPut :Should Erase Mem with lenth and offset \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],blkParams.byteOffset,len );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	for(int iteration=0; iteration < 5; iteration++)
	{
		rfidStatus = RFID_BlockErase( hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&blkParams,NULL,pInfo,0);
		if(rfidStatus == RFID_API_SUCCESS )
		{
			break;
		}
		ERROR_INFO einfo;
		if( rfidStatus != RFID_API_SUCCESS )
		{
			RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
			fprintf( api3Data.fpCurLog, "\n\n Erase failure Vendor Msg %S <br>\n", einfo.vendorMessage );
			wprintf(L"\n Erase failure Vendor Msg %ls",einfo.vendorMessage);
		}
	}
	ERROR_INFO einfo;
	if( rfidStatus != RFID_API_SUCCESS )
	{
		RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
		fprintf( api3Data.fpCurLog, "\n\n Erase failure Vendor Msg %S <br>\n", einfo.vendorMessage );
		wprintf(L"\n Erase failure Vendor Msg %ls",einfo.vendorMessage);
	}
	else
	{
		fprintf( api3Data.fpCurLog,"\n Erase is Success verify the Data Reading back<br>\n");
		bSuccess = TRUE;
		if(( api3Data.rfidStatus = RFID_Read( hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&rParams,NULL,pInfo,pTagData,0) ) == RFID_API_SUCCESS )
		{
			printTagDataWithResults( pTagData);
		}
	}
	

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-BLKERASE");
	
	RFID_PurgeTags( api3Data.hReader,0 );
	//clean up tag allocation	
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestSimpleWriteOperations( RFID_HANDLE32 hReader,UINT8* pData2Write ,MEMORY_BANK mBank,UINT8 offSet,UINT8 len,ANTENNA_INFO* pInfo )					
{
	BOOLEAN bSuccess = FALSE;
	RFID_STATUS rfidStatus;
	RFID_DeletePreFilter( hReader,0,0 );
	RFID_ResetConfigToFactoryDefaults( hReader );
	
	// TEST with Simple Write Operations without AnyFilters 
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	
	READ_ACCESS_PARAMS rParams = { mBank,offSet,len,0};
	WRITE_ACCESS_PARAMS wParams = { mBank,offSet,pData2Write,len,0};
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-WRITE-%d></a> CAPI-WRITE-%d:\n\n<br>Description: Writing to Mem Bank:%s  offSet:%d len:%d Data:%s \n <br> Expected OutPut :Should Write to Memory Bank \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],wParams.byteOffset,wParams.writeDataLength,Array2Char(wParams.pWriteData,0,wParams.writeDataLength) );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	if(mBank == MEMORY_BANK_EPC )
	{
		getBEDDTag();
	}
	
	for(int iteration=0; iteration < 5; iteration++)
	{
		rfidStatus = RFID_Write(hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&wParams,NULL,pInfo,0);
		if(rfidStatus == RFID_API_SUCCESS )
		{
			break;
		}
	}

	ERROR_INFO einfo;
	if( rfidStatus != RFID_API_SUCCESS )
	{
		RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
		fprintf( api3Data.fpLog, "\n\n Write failure Vendor Msg %S <br> \n", einfo.vendorMessage );
		wprintf(L"\n Write failure Vendor Msg %ls",einfo.vendorMessage);
	}
	else
	{
		fprintf( api3Data.fpLog,"\n Write is Success verify the Data Reading back<br>\n");
		bSuccess = TRUE;
	}


	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-WRITE");

	// de register stop access event notification
	//
	if(mBank == MEMORY_BANK_EPC )
	{
		getBEDDTag();
	}

	if(( api3Data.rfidStatus = RFID_Read( hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&rParams,NULL,pInfo,pTagData,0) ) == RFID_API_SUCCESS )
	{
		printTagDataWithResults( pTagData);
	}
	
	RFID_PurgeTags( api3Data.hReader,0 );

	RFID_DeletePreFilter( api3Data.hReader,0,0 );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);	
}

void TestWriteSpecificAPI( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	ERROR_INFO einfo;
	UINT8 WriteData[4] = { 0xBE,0xDD,0x00,0x01 };
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 3;
	logCurrentExecution("Test WriteSpecific Parameters");
	SetLogText("Write Specific API");
	LPTAG_DATA pResultTag = RFID_AllocateTag( api3Data.hReader );

	if(api3Data.pbeddTag->tagIDLength == 0 )
	{
		getBEDDTag();
	}

	if(api3Data.pbeddTag->tagIDLength == 0 )
	{
		fprintf(api3Data.fpCurLog,"\n Cound not get the tag..");
	}
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-WRITESPECIFIC-%d></a> CAPI-WRITESPECIFIC-%d:\n\n<br>Description: Test WriteSpecific Parameters \n <br> Expected OutPut : API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	WRITE_SPECIFIC_FIELD_ACCESS_PARAMS wSpecific;
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_RESERVED,0,4,0,{0} };
	
	wSpecific.accessPassword = 0;
	wSpecific.pWriteData = WriteData;
	wSpecific.writeDataLength = 4;
	for( int iter=0;iter<5;iter++)
	{
		api3Data.rfidStatus = RFID_WriteKillPassword( api3Data.hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&wSpecific,NULL,0 );
		
		api3Data.rfidStatus = RFID_Read(api3Data.hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&rParams,NULL,0,pResultTag,0);
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			printTagDataWithResults(pResultTag);
			break;
		}
	}

	RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, " RFID_WriteKillPassword with WRITE_SPECIFIC_FIELD_ACCESS_PARAMS  %S <br>\n", einfo.vendorMessage );	
	}
	else
	{
		fprintf( api3Data.fpCurLog, " RFID_WriteKillPassword with WRITE_SPECIFIC_FIELD_ACCESS_PARAMS  %s <br>\n", ERRORSTATUS[api3Data.rfidStatus] );
		SetLogResult(TRUE);
	}

	logToResultFile(L"CAPI-WRITESPECIFIC");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-WRITESPECIFIC-%d></a> CAPI-WRITESPECIFIC-%d:\n\n<br>Description: Test WriteSpecific Parameters \n <br> Expected OutPut : API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	
	rParams.byteOffset = 4;
	for( int iter=0;iter<5;iter++)
	{
		api3Data.rfidStatus = RFID_WriteAccessPassword( api3Data.hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&wSpecific,NULL,0 );
		api3Data.rfidStatus = RFID_Read(api3Data.hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&rParams,NULL,0,pResultTag,0);
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			printTagDataWithResults(pResultTag);
			break;
		}
	}

	RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, " RFID_WriteAccessPassword with WRITE_SPECIFIC_FIELD_ACCESS_PARAMS  %S <br>\n", einfo.vendorMessage );	
	}
	else
	{
		fprintf( api3Data.fpCurLog, " RFID_WriteAccessPassword with WRITE_SPECIFIC_FIELD_ACCESS_PARAMS  %s <br>\n", ERRORSTATUS[api3Data.rfidStatus] );
		SetLogResult(TRUE);
	}

	logToResultFile(L"CAPI-WRITESPECIFIC");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-WRITESPECIFIC-%d></a> CAPI-WRITESPECIFIC-%d:\n\n<br>Description: Test WriteSpecific Parameters \n <br> Expected OutPut : API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");

	UINT8 epcWriteData[12] = { 0xBE, 0xDD, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01 };
	wSpecific.pWriteData = epcWriteData;
	wSpecific.writeDataLength = 12;
	for( int iter=0;iter<5;iter++)
	{
		api3Data.rfidStatus = RFID_WriteTagID( api3Data.hReader,api3Data.pbeddTag->pTagID,api3Data.pbeddTag->tagIDLength,&wSpecific,NULL,0 );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			break;
		}
	}

	RFID_GetLastErrorInfo( api3Data.hReader, &einfo );
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "RFID_WriteTagID with WRITE_SPECIFIC_FIELD_ACCESS_PARAMS  %S <br>\n", einfo.vendorMessage );	
	}
	else
	{
		fprintf( api3Data.fpCurLog, "RFID_WriteTagID with WRITE_SPECIFIC_FIELD_ACCESS_PARAMS  %s <br>\n", ERRORSTATUS[api3Data.rfidStatus] );
		SetLogResult(TRUE);
	}
	logToResultFile(L"CAPI-WRITESPECIFIC");
		
	if( pResultTag != NULL ) RFID_DeallocateTag(api3Data.hReader,pResultTag);
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

void TestSimpleReadWriteErase( )
{
	api3Data.dwTestCaseNo = 01;
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.blockEraseSupported && !api3Data.rCaps.blockWriteSupported )
	{
		api3Data.dwSkipCount = 43;
		LogPassFailCount();
		return;
	}
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Test BlockErase-RFID_BlockErase()");
	logCurrentExecution( "TestBlockErase");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 43;

	if( FALSE ==  getBEDDTag() )
	{
		wprintf(L"\n Error getting Writable Tag ---BEDD");
		return ;
	}

	TestBlockEraseOperations(api3Data.hReader,MEMORY_BANK_USER,0,8,&api3Data.pInfo);
	TestBlockEraseOperations(api3Data.hReader,MEMORY_BANK_USER,0,16,&api3Data.pInfo);
	TestBlockEraseOperations(api3Data.hReader,MEMORY_BANK_USER,0,32,&api3Data.pInfo);
	TestBlockEraseOperations(api3Data.hReader,MEMORY_BANK_USER,0,64,&api3Data.pInfo);
	
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");

	// Test Writes to all memory banks
	SetLogText("Test RFID_Write()");
	logCurrentExecution( "TestRFIDWrite");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	{
		UINT8 Data2Write[4];
		memset( Data2Write,0x11,4);// = { 0xAB,0x99,0x66,0xFF };
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_RESERVED,0,4 ,&api3Data.pInfo );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_RESERVED,4,4,&api3Data.pInfo  );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_EPC,6,4 ,&api3Data.pInfo );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_EPC,8,4 ,&api3Data.pInfo );
	}
	
	// Test Writes to USER MEMORY at Different OFFSETS
	//
	{
		UINT8 Data2Write[] = { 0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF };
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,8 ,&api3Data.pInfo );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,8,8 ,&api3Data.pInfo);
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,16,8 ,&api3Data.pInfo);
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,24,8 ,&api3Data.pInfo);
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,32,8 ,&api3Data.pInfo);
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,56,8 ,&api3Data.pInfo);
	}
	// Test Writes to USER with large data
	//
	{
		UINT8 Data2Write[] = {
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							   0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,0xDE,0xAD,0xBE,0xEF,
							 };

		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,64,&api3Data.pInfo );
		memset( Data2Write,0xAA,60 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,4,60,&api3Data.pInfo );
		memset( Data2Write,0xBB,56 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,8,56,&api3Data.pInfo );
		memset( Data2Write,0xCC,52 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,12,52,&api3Data.pInfo );
		memset( Data2Write,0xDD,48 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,16,48,&api3Data.pInfo );
		memset( Data2Write,0xEE,44 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,20,44,&api3Data.pInfo );
		memset( Data2Write,0xFF,40 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,24,40,&api3Data.pInfo );
		memset( Data2Write,0x11,36 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,28,36,&api3Data.pInfo );
		memset( Data2Write,0x22,32 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,32,32,&api3Data.pInfo );
		memset( Data2Write,0x33,28 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,36,28,&api3Data.pInfo );
		memset( Data2Write,0x44,24 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,40,24,&api3Data.pInfo );
		memset( Data2Write,0x55,20 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,44,20,&api3Data.pInfo );
		memset( Data2Write,0x66,16 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,48,16,&api3Data.pInfo );
		memset( Data2Write,0x77,12 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,52,12,&api3Data.pInfo );
		memset( Data2Write,0xAA,8 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,56,8,&api3Data.pInfo );
		memset( Data2Write,0xBB,4 );
		TestSimpleWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,60,4,&api3Data.pInfo );		
	}

	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	
	SetLogText("Test Block Write");
	logCurrentExecution( "TestBlockWrite");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	{
		UINT8 Data2Write[] = { 0xDE,0xAD,0xBE,0xEF };
		TestBlockWriteOperations(api3Data.hReader,Data2Write,MEMORY_BANK_EPC,6,4 ,&api3Data.pInfo);
		TestBlockWriteOperations(api3Data.hReader,Data2Write,MEMORY_BANK_EPC,8,4 ,&api3Data.pInfo);
	}

	{
		{
			UINT8 Data2Write[8];
			memset( Data2Write, 0xAA,8 );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,8,&api3Data.pInfo );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,8,8,&api3Data.pInfo );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,16,8 ,&api3Data.pInfo);
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,32,8 ,&api3Data.pInfo);
		}

		{
			UINT8 Data2Write[16];
			memset( Data2Write, 0xBB,16 );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,16,&api3Data.pInfo );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,16,16,&api3Data.pInfo );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,32,16,&api3Data.pInfo );

			// Causing Script to Hang..need to check manually, Write a seperate Stress Script
			// for testing the Same
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,48,16 ,&api3Data.pInfo);
		}
		{
			UINT8 Data2Write[32];
			memset( Data2Write, 0xCC,32 );
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,0,32 ,&api3Data.pInfo);
			TestBlockWriteOperations( api3Data.hReader,Data2Write,MEMORY_BANK_USER,32,32 ,&api3Data.pInfo);
		}
	}

	RFID_SetTraceLevel( api3Data.hReader,TRACE_LEVEL_OFF );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

