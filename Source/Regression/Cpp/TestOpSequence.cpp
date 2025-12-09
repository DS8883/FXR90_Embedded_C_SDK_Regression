#include "Common.h"


extern struct RFID3DATA api3Data;
extern struct HANDLE_LIST hList;
extern MYTAGDATA MyTags[ 2 ];

void TestOPSequenceWriteANDRead( RFID_HANDLE32 hReader, ACCESS_OPERATION_CODE opCode ,UINT8* Data2Write,ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo )
{
	BOOLEAN bTid,bUser;

	// Initialize the patterns with CRC+PC as below
	//
	UINT8 TagMask[24];
	memset(TagMask,0xFF,24);
	
	UINT8 beddTag[] = { 0xBE,0xDD };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		accessFilter.lpRSSIRangeFilter = NULL;
	}

	// Opcode Related Data 
	UINT32 opIndex;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );

	//Access read for all memory banks
	//
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-WRITE-%d></a> CAPI-OPSEQ-WRITE-%d:\n\n<br>Description: TEST OP SEQUENCE WRITE TO EP <br> Expected OutPut:Only Tags which match with EPC:BEDD Should be Written\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		 bTid=bUser	=FALSE;

		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write,8,0}; 	
		OP_CODE_PARAMS opParams2 ;
		opParams2.opCode = opCode;
		opParams2.opParams = &wParams;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams2,&opIndex );
		OP_CODE_PARAMS opParams1 ;
		
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep(10000);
		api3Data.rfidStatus = RFID_StopAccessSequence(hReader);

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			
			if(pTagData->memoryBank == MEMORY_BANK_USER && pTagData->opStatus == ACCESS_SUCCESS)
			{
				bUser = TRUE;
			}
		}

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,4,0}; 
		opParams1.opCode = ACCESS_OPERATION_READ;
		opParams1.opParams = &rParams;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams1,&opIndex );

		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep(10000);
		api3Data.rfidStatus = RFID_StopAccessSequence(hReader);

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);

			if(pTagData->memoryBank == MEMORY_BANK_TID && pTagData->opStatus == ACCESS_SUCCESS )
			{
				bTid = TRUE;
			}
		}

		if( bUser && bTid )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-OPSEQ-WRITE");
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}


	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}
void TestOPSequenceBlkEraseANDRead( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT8 offSet,UINT8 Length,ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo  )
{
	BOOLEAN bSuccess = FALSE;
	UINT8 beddTag[] = { 0xBE,0xDD };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };
	
	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		accessFilter.lpRSSIRangeFilter = NULL;
	}
	
	// Opcode Related Data 
	UINT32 opIndex;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );

	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-BLKERASE-%d></a> CAPI-OPSEQ-BLKERASE-%d:\n\n<br>Description:TEST OP SEQUENCE BLOCK ERASE <br> Expected OutPut: Erase mem Bank %s offset : %d length %d\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],offSet,Length );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		BLOCK_ERASE_ACCESS_PARAMS blkParams = { mBank,offSet,Length,0,{0}};
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_BLOCK_ERASE,&blkParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		READ_ACCESS_PARAMS rParams = { mBank,offSet,Length,0}; 
		opParams.opCode = ACCESS_OPERATION_READ;
		opParams.opParams = &rParams;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );

		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		RFID_StopAccessSequence( hReader );
		
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			
			if(pTagData->memoryBank == MEMORY_BANK_USER && pTagData->opStatus == ACCESS_SUCCESS)
			{
				bSuccess = TRUE;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-BLKERASE");	

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}

	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestOPSequenceBlkErase( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT8 offSet,UINT8 Length,ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo  )
{
	BOOLEAN bSuccess = FALSE;
	UINT8 beddTag[] = { 0xBE,0xDD };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		accessFilter.lpRSSIRangeFilter = NULL;
	}
	
	// Opcode Related Data 
	UINT32 opIndex;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );

	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-BLKERASE-%d></a> CAPI-OPSEQ-BLKERASE-%d:\n\n<br>Description:TEST OP SEQUENCE BLOCK ERASE <br> Expected OutPut: Erase mem Bank %s offset : %d length %d\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],offSet,Length );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		BLOCK_ERASE_ACCESS_PARAMS blkParams = { mBank,offSet,Length,0,{0}};
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_BLOCK_ERASE,&blkParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		RFID_StopAccessSequence( hReader );
		
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);

			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-BLKERASE");	

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);

		READ_ACCESS_PARAMS rParams = { mBank,offSet,Length,0}; 
		opParams.opCode = ACCESS_OPERATION_READ;
		opParams.opParams = &rParams;
		
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		RFID_StopAccessSequence( hReader );
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		while( RFID_API_SUCCESS == RFID_GetReadTag( hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
		}
	}

	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}


void TestOPSequenceWrite( RFID_HANDLE32 hReader, ACCESS_OPERATION_CODE opCode ,UINT8* Data2Write,ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo )
{
	UINT8 Pattern1[2] = {0xBE,0xDD};
	UINT8 Pattern2[2] = {0x11,0x11};
	BOOLEAN bSuccess = FALSE;
	// Initialize the patterns with CRC+PC as below
	//
	UINT8 TagMask[24];
	memset(TagMask,0xFF,24);
	
	UINT8 beddTag[] = { 0xBE,0xDD };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,{ 0 } };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{ 0 } };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		accessFilter.lpRSSIRangeFilter = NULL;
	}

	// Opcode Related Data 
	UINT32 opIndex;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );

	//Access read for all memory banks
	//
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-WRITE-%d></a> CAPI-OPSEQ-WRITE-%d:\n\n<br>Description: TEST OP SEQUENCE WRITE TO EP <br> Expected OutPut:Only Tags which match with EPC:BEDD Should be Written\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_EPC,8,Data2Write,8,0}; 
		OP_CODE_PARAMS opParams = { opCode,&wParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-OPSEQ-WRITE");
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}

	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-WRITE-%d></a> CAPI-OPSEQ-WRITE-%d:\n\n<br>Description: TEST OP SEQUENCE WRITE TO RESERVED <br> Expected OutPut:Only Tags which match with EPC :BEDD Should be Written\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,Data2Write,8,0}; 
		OP_CODE_PARAMS opParams = { opCode,&wParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-WRITE");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0 );
	}

	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-WRITE-%d></a> CAPI-OPSEQ-WRITE-%d:\n\n<br>Description:  TEST OP SEQUENCE WRITE TO USER MEMORY <br> Expected OutPut:Only Tags which match with EPC:BEDD Should be WRITTEN\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");

		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write,8,0}; 
		OP_CODE_PARAMS opParams = { opCode,&wParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		// Write to the off set in user memory bank
		wParams.byteOffset = 8;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		wParams.byteOffset = 16;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		wParams.byteOffset = 24;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		wParams.byteOffset = 32;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		wParams.byteOffset = 40;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		wParams.byteOffset = 48;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		wParams.byteOffset = 54;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );

		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 20000 );
		RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-WRITE");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}

	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestOPSequenceRead( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo )
{
	BOOLEAN bSuccess = FALSE;
	UINT8 Pattern1[2]; Pattern1[0] = MyTags[0].epc[0];Pattern1[1] = MyTags[0].epc[1];
	UINT8 Pattern2[2]; Pattern2[0] = MyTags[1].epc[0];Pattern2[1] = MyTags[1].epc[1];
	UINT8 TagMask[24];
	memset(TagMask,0xFF,24);

	// Opcode Related Data 
	UINT32 opIndex;
	
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( hReader );

	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ <br> Expected OutPut:Only Tags which match with EPC %s Should be READ\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0}}; 
		TAG_PATTERN tpA = {MEMORY_BANK_EPC,32,Pattern1,16,TagMask,16,{0}};
		TAG_PATTERN tpB = {MEMORY_BANK_EPC,32,Pattern2,16,TagMask,16,{0}};
		ACCESS_FILTER aFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,{ 0 } };

		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}

		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,0};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ <br> Expected OutPut:Only Tags which match with EPC %s Should be READ\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[1].epc,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		aFilter.matchPattern = NOTA_AND_B;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");
		
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ <br> Expected OutPut:Tags other than %s and %s be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char( MyTags[0].epc,0,4 ),Array2Char( MyTags[1].epc,0,4 ) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		aFilter.matchPattern = NOTA_AND_NOTB;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}

		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ ( No Tags )<br> Expected OutPut:( No Tags )\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		aFilter.matchPattern = A_AND_B;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}

		bSuccess = TRUE;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = FALSE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ <br> Expected OutPut:Tags Which Match EPC %s and TID %s  Should be Read( No Tags since the Pattern matching both is not possible)\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char(MyTags[0].epc,0,4),Array2Char(MyTags[0].tid,0,4) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		
		UINT8 tidMask[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		rParams.memoryBank = MEMORY_BANK_TID;
		rParams.byteOffset = 0;
		aFilter.lpTagPatternB->bitOffset=0;
		aFilter.lpTagPatternB->memoryBank = MEMORY_BANK_TID;
		aFilter.lpTagPatternB->pTagMask = tidMask;
		aFilter.lpTagPatternB->pTagPattern = MyTags[0].tid;
		aFilter.lpTagPatternB->tagMaskBitCount = aFilter.lpTagPatternB->tagPatternBitCount=96;
		aFilter.matchPattern = A_AND_B;
		bSuccess = FALSE;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}
	
	//// Extend this to other memory banks TID 
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ<br> Expected OutPut:Only Tags which match with TID %s  Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char(MyTags[0].tid,0,MyTags[0].tidlen) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,12,0,0}; 
		TAG_PATTERN tpA = {MEMORY_BANK_TID,0,MyTags[0].tid,64,TagMask,64,{0}};
		TAG_PATTERN tpB = {MEMORY_BANK_TID,0,MyTags[1].tid,64,TagMask,64,{0}};
		ACCESS_FILTER aFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,{ 0 } };

		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}

		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,0};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ<br> Expected OutPut:Only Tags which match with TID %s  Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char(MyTags[1].tid,0,MyTags[1].tidlen) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		aFilter.matchPattern = NOTA_AND_B;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");
		
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ<br> Expected OutPut:Only Tags which DONT match with TID %s and %s Should be Read ( Tags other than A22F and B22F )\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char(MyTags[0].tid,0,MyTags[0].tidlen),Array2Char(MyTags[1].tid,0,MyTags[1].tidlen) );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		aFilter.matchPattern = NOTA_AND_NOTB;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				break;
			}
		}
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ<br> Expected OutPut:Only Tags which match with TID %s and %s Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Array2Char(MyTags[0].tid,0,MyTags[0].tidlen),Array2Char(MyTags[1].tid,0,MyTags[1].tidlen));
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		bSuccess = FALSE;
		aFilter.matchPattern = A_AND_B;
		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			aFilter.lpRSSIRangeFilter = NULL;
		}
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&aFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				break;
			}
		}
		
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}

	// Test for Max Number of OP Access Sequences 8 for Merlin
	{
		bSuccess = FALSE;
		TAG_PATTERN tpA = {MEMORY_BANK_EPC,32,Pattern1,16,TagMask,16,{0}};
		ACCESS_FILTER accessFilter = {{0,0},&tpA,0,A_AND_NOTB,NULL,NULL };

		if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
		{
			accessFilter.lpRSSIRangeFilter = NULL;
		}
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-OPSEQ-READ-%d></a> CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE READ WITH FOUR OP Sequences<br> Expected OutPut:API Success and Read the Data EPC TID USER\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,0,12,0,{0}}; 
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		rParams.memoryBank = MEMORY_BANK_TID;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		rParams.memoryBank = MEMORY_BANK_USER;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		rParams.memoryBank = MEMORY_BANK_EPC;
		rParams.byteOffset = 8;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		rParams.memoryBank = MEMORY_BANK_TID;
		/*api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		rParams.memoryBank = MEMORY_BANK_USER;
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );*/
		
		api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 5000 );
		RFID_StopAccessSequence( hReader );
		BOOLEAN bEPC,bTID,bUSER;
		bEPC=bTID=bUSER = FALSE;
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = TRUE;
				if(pTagData->memoryBank == MEMORY_BANK_TID )
				{
					bTID = TRUE;
				}

				if(pTagData->memoryBank == MEMORY_BANK_USER )
				{
					bUSER = TRUE;
				}

				if(pTagData->memoryBank == MEMORY_BANK_EPC )
				{
					bEPC = TRUE;
				}
			}

			if( bTID && bUSER && bEPC )
			{
				break;
			}
		}

		if( bTID && bUSER && bEPC )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-OPSEQ-READ");

		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);
	}

	// Verify if error reported when MAX OpSeq limit reached
	// Test Multiple Access Sequences 
	{
		bSuccess = FALSE;
		fprintf(api3Data.fpCurLog,"\n\n<a nameCAPI-OPSEQ-READ-%d></a>CAPI-OPSEQ-READ-%d:\n\n<br>Description: TEST OP SEQUENCE FOR MAX LIMIT  Sequences<br> Expected OutPut:API Success and Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,0,12,0,{0}}; 
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,{0}};
		
		for( int MyIndex =0 ;MyIndex <= api3Data.rCaps.maxNumOperationsInAccessSequence+5;MyIndex++)
		{
			api3Data.rfidStatus = RFID_AddOperationToAccessSequence( hReader, &opParams,&opIndex );
		}

		if( api3Data.rfidStatus == RFID_ACCESS_SEQUENCE_MAX_OP_EXCEEDED )
		{
			api3Data.rfidStatus = RFID_PerformAccessSequence( hReader,0,pInfo,ptInfo,0);
			rfid3Sleep( 5000 );
			RFID_StopAccessSequence( hReader );
			while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
			{
				printTagDataWithResults( pTagData);
				if(pTagData->opStatus == ACCESS_SUCCESS )
				{
					bSuccess = TRUE;
					break;
				}
			}

			if( bSuccess )
			{
				SetLogResult( TRUE );
			}
			logToResultFile(L"CAPI-OPSEQ-READ");

			fprintf(api3Data.fpLog,"\n MAX limit Passed");
		}
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( hReader,0);		
	}

	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( hReader );
	
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData);
}

void TestOpSeqRecomm( ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	RECOMMISSION_OPERATION_CODE op[7] = { RECOMMISSION_DISABLE_PERMALOCK,RECOMMISSION_DISABLE_USER_MEMORY,RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION,RECOMMISSION_DISABLE_PASSWORD,RECOMMISSION_DISABLE_PERMALOCK_PASSWORD,RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD,RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION };
	
	// Opcode Related Data 
	UINT32 opnum;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	for(int opIndex = 0; opIndex < 7; opIndex++)
	{
		BOOLEAN bSuccess = TRUE;
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-RECOM-%d></a> CAPI-NEG-RECOM-%d:\n\n<br>Description:Recom Op Sequence with%s <br> Expected OutPut: NOT SUPPORTED\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,RECOMOPCODES[opIndex] );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		RECOMMISSION_ACCESS_PARAMS reComParams = { 0xAAAABBBB,op[opIndex]};
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_RECOMMISSION,&reComParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opnum );
		fprintf(api3Data.fpCurLog," %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,pInfo,ptInfo,0);
		fprintf(api3Data.fpCurLog," %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		wprintf(L"\n Status : %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		rfid3Sleep( 5000 );
		RFID_StopAccessSequence( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->opStatus == ACCESS_SUCCESS )
			{
				bSuccess = FALSE;
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
				break;
			}
		}

		if( bSuccess )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-NEG-RECOM");
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	}
	
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

void TestOpSeqLockKillNeg( ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	LOCK_PRIVILEGE lockprevileges[5] = { LOCK_PRIVILEGE_NONE, LOCK_PRIVILEGE_READ_WRITE, LOCK_PRIVILEGE_PERMA_LOCK, LOCK_PRIVILEGE_PERMA_UNLOCK, LOCK_PRIVILEGE_UNLOCK };
	LOCK_DATA_FIELD lockdatafields[5] = { LOCK_KILL_PASSWORD, LOCK_ACCESS_PASSWORD, LOCK_EPC_MEMORY, LOCK_TID_MEMORY, LOCK_USER_MEMORY };
	UINT32 opCodeIndex;
	UINT8 beddTag[] = { 0xCC,0xDF,0x11,0xFF,0xAA };
	UINT8 ffTag[] = { 0xDD,0xDD,0x11,0xEE,0xBB };
	UINT8 tagMask[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,48,tagMask,48,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,48,tagMask,48,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );

	// for each lock previlege and memory bank iterate lock function on a Non existant tag
	//
	for(int lockIndex = 0; lockIndex < 5; lockIndex++)
	{
		for(int lockdataIndex = 0; lockdataIndex < 5; lockdataIndex++)
		{
			BOOLEAN bSuccess = TRUE;
			fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-LOCK-%d></a> CAPI-NEG-LOCK-%d:\n\n<br>Description:Lock Op Sequence with%s %s<br> Expected OutPut: FAIL\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,LOCKPRIVILEGE[lockIndex],LOCKDATAFIELD[lockdataIndex] );
			fprintf( api3Data.fpCurLog,"<br>");
			fprintf( api3Data.fpCurLog, "\n Result is: \n ");
			fprintf( api3Data.fpCurLog,"<br>");
			LOCK_ACCESS_PARAMS lockParams;
			lockParams.accessPassword = 0x12345678;
			lockParams.privilege[lockdatafields[lockdataIndex]] = lockprevileges[lockIndex];
			OP_CODE_PARAMS opParams = { ACCESS_OPERATION_LOCK,&lockParams,{0} }; ;
			api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opCodeIndex );
			fprintf(api3Data.fpCurLog," %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
			api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,&accessFilter,pInfo,ptInfo,0);
			fprintf(api3Data.fpCurLog," %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
			wprintf(L"\n RFID_PerformAccessSequence : %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
			rfid3Sleep( 5000 );
			RFID_StopAccessSequence( api3Data.hReader );
			while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
			{
				printTagDataWithResults( pTagData);
				if(pTagData->opStatus == ACCESS_SUCCESS )
				{
					bSuccess = FALSE;
					api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
					break;
				}
			}

			if( bSuccess )
			{
				SetLogResult( TRUE );
			}
			logToResultFile(L"CAPI-NEG-LOCK");
			api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
		}
	}

	//
	// Kill Op Sequence..
	//
	BOOLEAN bSuccess = TRUE;
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-KILL-%d></a> CAPI-NEG-KILL-%d:\n\n<br>Description:Lock Op Sequence KILL Op Sequence<br> Expected OutPut: FAIL\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	KILL_ACCESS_PARAMS KillParams;
	KillParams.killPassword = 0x12345678;

	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_KILL,&KillParams,0 }; ;
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opCodeIndex );
	fprintf(api3Data.fpCurLog," %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,&accessFilter,pInfo,ptInfo,0);
	fprintf(api3Data.fpCurLog," %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
	wprintf(L"\n RFID_PerformAccessSequence : %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	
	rfid3Sleep( 5000 );
	RFID_StopAccessSequence( api3Data.hReader );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData);
		if(pTagData->opStatus == ACCESS_SUCCESS )
		{
			bSuccess = FALSE;
			api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
			break;
		}
	}

	if( bSuccess )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-NEG-KILL");
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

void TestOpSeqNeg( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	// Test Op Sequence Negative Cases
	//
	SetLogText("TestOpSeqNeg");
	logCurrentExecution( "TestOpSeqNeg");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 33;
	TestOpSeqRecomm( NULL, NULL );
	TestOpSeqLockKillNeg( NULL, NULL );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
void TestOpSequenceReadWriteErase( )
{
	api3Data.dwTestCaseNo = 01;
	UINT8 Data2Write[8];
	memset( Data2Write, 0xAA,8 );

	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.blockEraseSupported && !api3Data.rCaps.blockWriteSupported )
	{
		api3Data.dwSkipCount = 44;
		LogPassFailCount();
		return;
	}

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	// Test Writes to all memory banks
	SetLogText("TestOpSeqWriteRead");
	logCurrentExecution( "TestOpSeqBlkErase");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 10;
	
	TRIGGER_INFO triggerInfo;
	memset(&triggerInfo,0,sizeof(triggerInfo));
	triggerInfo.tagReportTrigger	= 1; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT;
	triggerInfo.stopTrigger.value.numAttempts.n = 3;
	triggerInfo.stopTrigger.value.numAttempts.timeoutMilliseconds = 5000;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	triggerInfo.lpTagEventReportInfo = &tagEventInfo;
	api3Data.pTriggerInfo = &triggerInfo;

	TAG_STORAGE_SETTINGS tgSettings;
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
	tgSettings.enableAccessReports = TRUE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);
	
	TestOPSequenceWriteANDRead(api3Data.hReader, ACCESS_OPERATION_BLOCK_WRITE,Data2Write ,&api3Data.pInfo, NULL);

	TestOPSequenceBlkEraseANDRead( api3Data.hReader,MEMORY_BANK_USER,0,8,NULL,NULL);

	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_RESERVED,0,8,&api3Data.pInfo,NULL );	
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_RESERVED,0,8, &api3Data.pInfo, api3Data.pTriggerInfo);
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_USER,0,24,&api3Data.pInfo,NULL );
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_USER,0,24,&api3Data.pInfo, api3Data.pTriggerInfo);
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_USER,0,48,&api3Data.pInfo,NULL );
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_USER,0,48,&api3Data.pInfo, api3Data.pTriggerInfo);
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_USER,0,64,&api3Data.pInfo,NULL );
	TestOPSequenceBlkErase( api3Data.hReader,MEMORY_BANK_USER,0,64,&api3Data.pInfo, api3Data.pTriggerInfo);
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	
	logCurrentExecution( "TestOpSeqWrite_Read");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 34;
		
	
	{
		UINT8 Data2Write[] = { 0xDE,0xFA,0xCE,0xD0,0xBE,0xDD,0xEE,0xDD };
		TestOPSequenceWrite(api3Data.hReader, ACCESS_OPERATION_WRITE,Data2Write,&api3Data.pInfo,NULL );
		TestOPSequenceWrite(api3Data.hReader, ACCESS_OPERATION_WRITE,Data2Write,&api3Data.pInfo, api3Data.pTriggerInfo);
	}

	{
		UINT8 Data2Write[] = { 0x11,0x22,0x33,0x44,0x55,0x66,0xCE,0xD0 };
		TestOPSequenceWrite(api3Data.hReader, ACCESS_OPERATION_BLOCK_WRITE,Data2Write ,&api3Data.pInfo,NULL);
		TestOPSequenceWrite(api3Data.hReader, ACCESS_OPERATION_BLOCK_WRITE,Data2Write ,&api3Data.pInfo, api3Data.pTriggerInfo);
	}
	
	TestOPSequenceRead( api3Data.hReader,&api3Data.pInfo,NULL );	
	TestOPSequenceRead( api3Data.hReader,&api3Data.pInfo, api3Data.pTriggerInfo);
	//{
	//	UINT8 Data2Write[] = { 0xDE,0xCA,0xDE,0xDE,0xCA,0xDE,0xDE,0xCA };
	//	TestOPSequenceLock( api3Data.hReader, Data2Write,pInfo );*/
	//}
	tgSettings.enableAccessReports = FALSE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
