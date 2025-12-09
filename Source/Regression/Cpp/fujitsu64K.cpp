// Fujitsu64K.cpp : Support for verifying Fujitsu Tag Support.
//
#include "Common.h"
#include <stdlib.h>
#include "fujitsu64K.h"

// global data for API
extern struct RFID3DATA api3Data;

// Verify Fujitsu Burst Write API
//
void fujitsu64KBurstWrite(  )
{
	int min =0; int max=0xFF;
	UINT8 burstWriteData[512];
	TAG_STORAGE_SETTINGS tgs;
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader, &tgs );
	tgs.maxMemoryBankByteCount = 1024;
	tgs.maxTagCount = 100;
	tgs.enableAccessReports = TRUE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgs );
	
	LPTAG_DATA pResultTag = RFID_AllocateTag( api3Data.hReader );
	// burst write to the blocks of memory
	//
	for( UINT16 byteoffset = 0;byteoffset <= 64; byteoffset+=2 )
	{
		memset(burstWriteData,(min + rand( ) % max),512);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-FUJITSU-BURSTWRT-%d></a>CAPI-FUJITSU-BURSTWRT-%d:\n\n<br>Description: FUJITSU BURST WRITE OFFSET:%dK LENGTH:%d Data:%s \n <br> Expected OutPut :Successful Write \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,byteoffset,128,Byte2HexString(burstWriteData,128));
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		FUJITSU_BURST_WRITE_ACCESS_PARAMS burstWrite = { MEMORY_BANK_USER,(UINT16) (byteoffset*1024),burstWriteData,128,0,{0} };
		api3Data.rfidStatus = RFID_FujitsuBurstWrite( api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&burstWrite,NULL,NULL,pResultTag,0 );
		fprintf(api3Data.fpCurLog,"\n API Result Description: %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		wprintf(L"\n API Result Description: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			fprintf(api3Data.fpCurLog,"\n Number off bytes NOT written : %d <br>",pResultTag->lpAccessOperationResult->FujitsuBurstWriteResult.numberOfBytesNotWritten);
			fprintf(api3Data.fpCurLog,"\n Operation Status : %s<br>",ACCESSSTATUS[pResultTag->opStatus]);
			wprintf(L"\n Number off bytes NOT written : %d",pResultTag->lpAccessOperationResult->FujitsuBurstWriteResult.numberOfBytesNotWritten);
			wprintf(L"\n Operation Status : %ls",wACCESSSTATUS[pResultTag->opStatus]);
		}
		else
		{
			ERROR_INFO err;
			RFID_GetLastErrorInfo(api3Data.hReader,&err);
			wprintf(L"\n Error Info: %ls",err.statusDesc);
		}
		
		// log the result
		//
		if(api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult(TRUE);
		}
		
		logToResultFile(L"CAPI-FUJITSU-BURSTWRT");
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,(UINT16)(byteoffset*1024),128,0,{0} };
		api3Data.rfidStatus = RFID_Read(api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&rParams,NULL,0,pResultTag,0);
		fprintf(api3Data.fpCurLog,"\n API Result Description: %S<br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		printTagDataWithResults( api3Data.pfujitsu64Ktag );
		fprintf(api3Data.fpCurLog,"\n Operation Status : %s<br>",ACCESSSTATUS[pResultTag->opStatus]);
	}

	if( pResultTag != NULL ) RFID_DeallocateTag(api3Data.hReader,pResultTag);
}
// Verify Fujitsu Burst Write Using OP Sequence ( Asynchronous )
//
void TestOPSequenceBurstWrite( ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo,UINT16 byteoffset )
{
	UINT8 Pattern1[2] = { 0xF6,0x40 };
	UINT8 Pattern2[2] = { 0x11,0x11 };
	BOOLEAN bSuccess = FALSE;
	UINT8 burstWriteData[128];
	memset(burstWriteData,0xFF,128);

	// Initialize the patterns with CRC+PC as below
	//
	UINT8 TagMask[24];
	memset(TagMask,0xFF,24);
		
	UINT8 tagMask[] = { 0xFF,0xFF};
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL }; 

	// Opcode Related Data 
	UINT32 opIndex;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );

	//Access read for all memory banks
	//
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-FUJITSU-OPSEQ-%d></a>CAPI-FUJITSU-OPSEQ-%d:\n\n<br>Description: TEST OP SEQUENCE BURST WRITE <br> Expected OutPut:Only Tags which match with EPC:F640 Should be Written\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		FUJITSU_BURST_WRITE_ACCESS_PARAMS burstWrite = { MEMORY_BANK_USER, byteoffset,burstWriteData,128,0,{0} }; 
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_FUJITSU_BURST_WRITE,&burstWrite,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			wprintf(L"\n TagID : %ls AccessResult: %s EVENT:%ls",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus],wTAGEVENTS[pTagData->tagEvent]);
			fprintf(api3Data.fpCurLog,"\n TagID : %s AccessResult: %s EVENT:%s<br>",Byte2HexString(pTagData->pTagID,pTagData->tagIDLength),ACCESSSTATUS[pTagData->opStatus],TAGEVENTS[pTagData->tagEvent]);

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

		logToResultFile(L"CAPI-FUJITSU-OPSEQ");
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	}
	
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
}

// Verify Fujitsu Burst Erase API
//
void fujitsu64KBurstErase(  )
{
	LPTAG_DATA pResultTag = RFID_AllocateTag( api3Data.hReader );
	
	// burst write to the blocks of memory
	//
	for( int byteoffset = 0;byteoffset <=64; byteoffset+=2 )
	{
		fprintf( api3Data.fpCurLog,"\n\n<a name=CAPI-FUJITSU-BURSTERASE-%d></a> CAPI-FUJITSU-BURSTERASE-%d:\n\n<br>Description: FUJITSU BURST ERASE OFFSET:%d LENGTH:%d BYTES \n <br> Expected OutPut :Successful Erase \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,byteoffset,128);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		FUJITSU_BURST_ERASE_ACCESS_PARAMS burstErase = { MEMORY_BANK_USER, byteoffset*1024,128,0,{0} };
		api3Data.rfidStatus = RFID_FujitsuBurstErase( api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&burstErase,NULL,NULL,pResultTag,0 );
		fprintf( api3Data.fpCurLog,"\n API Result Description: %S <br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		if( pResultTag->lpAccessOperationResult != NULL )fprintf(api3Data.fpCurLog,"\n Number off bytes NOT Erased : %d <br>",pResultTag->lpAccessOperationResult->FujitsuBurstEraseResult.numberOfBytesNotErased);
		fprintf(api3Data.fpCurLog,"\n Operation Status : %s<br>",ACCESSSTATUS[pResultTag->opStatus]);

		// log the result
		//
		if(api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult(TRUE);
		}
		logToResultFile(L"CAPI-FUJITSU-BURSTERASE");

		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,byteoffset*1024,128,0,0 };
		api3Data.rfidStatus = RFID_Read(api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&rParams,NULL,0,pResultTag,0);
		fprintf(api3Data.fpCurLog,"\n API Result Description: %S<br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		printTagDataWithResults( api3Data.pfujitsu64Ktag );
		fprintf(api3Data.fpCurLog,"\n Operation Status : %s<br>",ACCESSSTATUS[pResultTag->opStatus]);
		wprintf(L"\n Operation Status : %s",wACCESSSTATUS[pResultTag->opStatus]);
	}

	if( pResultTag != NULL ) RFID_DeallocateTag(api3Data.hReader,pResultTag);

}
// Verify Fujitsu Burst Write using OP Sequence
//
void TestOPSequenceBurstErase( ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo,int byteoffset )
{
	UINT8 Pattern1[2] = { 0xF6,0x40 };
	UINT8 Pattern2[2] = { 0x11,0x11 };
	BOOLEAN bSuccess = FALSE;
	// Initialize the patterns with CRC+PC as below
	//
	UINT8 TagMask[24];
	memset(TagMask,0xFF,24);
		
	UINT8 tagMask[] = { 0xFF,0xFF };
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL }; 

	// Opcode Related Data 
	UINT32 opIndex;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );

	//Access read for all memory banks
	//
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-FUJITSU-BURSTERASE-%d></a> CAPI-FUJITSU-BURSTERASE-%d:\n\n<br>Description: TEST OP SEQUENCE BURST ERASE <br> Expected OutPut:Only Tags which match with EPC:F640 Should be Erased\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is: \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		FUJITSU_BURST_ERASE_ACCESS_PARAMS burstErase = { MEMORY_BANK_USER, byteoffset,128,0,{0} }; 
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_FUJITSU_BURST_ERASE,&burstErase,{0} };
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			wprintf(L"\n TagID : %ls AccessResult: %ls EVENT:%ls",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus],wTAGEVENTS[pTagData->tagEvent]);
			fprintf(api3Data.fpCurLog,"\n TagID : %s AccessResult: %s EVENT:%s<br>",Byte2HexString(pTagData->pTagID,pTagData->tagIDLength),ACCESSSTATUS[pTagData->opStatus],TAGEVENTS[pTagData->tagEvent]);

			if(pTagData->opStatus == ACCESS_SUCCESS )
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

		logToResultFile(L"CAPI-FUJITSU-BURSTERASE");
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	}
	
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
}
// Change Word Lock 
//
void fujitsu64KChangeWordLock( )
{
	LPTAG_DATA pResultTag = RFID_AllocateTag( api3Data.hReader );
	FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_ACCESS_PARAMS blkGrpPwd = { 1, 0x12345678, 0x12345678, {0} };
	api3Data.rfidStatus = RFID_FujitsuChangeBlockOrAreaGroupPassword(api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&blkGrpPwd,NULL,NULL,0);
	FUJITSU_CHANGE_WORDLOCK_ACCESS_PARAMS wordLkParams = { MEMORY_BANK_USER, 2048, 0x03,0x03,0x12345678,{0} };
	api3Data.rfidStatus = RFID_FujitsuChangeWordLock(api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&wordLkParams,NULL,NULL,0);
	
	// try writing to the locked tag and verify if the data is writable.
	//
	UINT8 burstWriteData[128];
	FUJITSU_BURST_WRITE_ACCESS_PARAMS burstWrite = { MEMORY_BANK_USER, 2048,burstWriteData,128,0,{0} };
	api3Data.rfidStatus = RFID_FujitsuBurstWrite( api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&burstWrite,NULL,NULL,pResultTag,0 );
	wprintf(L"\n API Result Description: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	if( pResultTag->lpAccessOperationResult != NULL )
	{
		wprintf(L"\n Number off bytes NOT written : %d",pResultTag->lpAccessOperationResult->FujitsuBurstWriteResult.numberOfBytesNotWritten);
	}
	wprintf(L"\n Operation Status : %s",wACCESSSTATUS[pResultTag->opStatus]);
	
	// unlock the WordLock
	wordLkParams.payloadAction = 0x00;
	wordLkParams.payloadMask = 0x03;
	api3Data.rfidStatus = RFID_FujitsuChangeWordLock(api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&wordLkParams,NULL,NULL,0);

	burstWrite.accessPassword = 0x00;
	api3Data.rfidStatus = RFID_FujitsuBurstWrite( api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&burstWrite,NULL,NULL,pResultTag,0 );
	wprintf(L"\n API Result Description: %lS",RFID_GetErrorDescription(api3Data.rfidStatus));
	if( pResultTag->lpAccessOperationResult != NULL )
	{
		wprintf(L"\n Number off bytes NOT written : %d",pResultTag->lpAccessOperationResult->FujitsuBurstWriteResult.numberOfBytesNotWritten);
	}
	wprintf(L"\n Operation Status : %s",ACCESSSTATUS[pResultTag->opStatus]);
	
	if( pResultTag != NULL ) RFID_DeallocateTag(api3Data.hReader,pResultTag);
}
// Fujitsu Change Block Lock API
//
void fujitsu64KChangeBlockLock( )
{
	// note: the memory will be locked after writing the first time.
	//
	LPTAG_DATA pResultTag = RFID_AllocateTag( api3Data.hReader );
	FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_ACCESS_PARAMS blkGrpPwd = { 1, 0x12345678, 0x12345678, {0} };
	api3Data.rfidStatus = RFID_FujitsuChangeBlockOrAreaGroupPassword(api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&blkGrpPwd,NULL,NULL,0);
	
	//first block --> 0x8000
	//
	UINT16 Action = 0x8000;
	FUJITSU_CHANGE_BLOCKLOCK_ACCESS_PARAMS blkLockParams = { 1,0x4000,0xFFFF,0x12345678,{0} };
	FUJITSU_READ_BLOCKLOCK_ACCESS_PARAMS readblockParams ={ 0x1,{0} };
	
	api3Data.rfidStatus = RFID_FujitsuChangeBlockLock( api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&blkLockParams,NULL,NULL,0);
	api3Data.rfidStatus = RFID_FujitsuReadBlockLock( api3Data.hReader, api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength, &readblockParams,NULL,NULL,pResultTag,0);
	if( pResultTag->lpAccessOperationResult != NULL )
	{
		wprintf(L"\n block lock Status %04X",pResultTag->lpAccessOperationResult->FujitsuReadBlockLockResult.blockLockStatus);
	}
		
	// Change the block lock--->0x0000
	//
	blkLockParams.payloadAction = 0x0000;
	api3Data.rfidStatus = RFID_FujitsuChangeBlockLock( api3Data.hReader, api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&blkLockParams,NULL,NULL,0);
	
	api3Data.rfidStatus = RFID_FujitsuReadBlockLock( api3Data.hReader, api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength, &readblockParams,NULL,NULL,pResultTag,0);
	wprintf(L"\n block lock Status %04X",pResultTag->lpAccessOperationResult->FujitsuReadBlockLockResult.blockLockStatus);
	if( pResultTag != NULL ) RFID_DeallocateTag(api3Data.hReader,pResultTag);	
}
// Reading fujitsu 64K memory
//
void fujitsu64KReading( ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo )
{
	UINT8 Pattern1[ 2 ] = { 0xF6,0x40 };
	UINT8 Pattern2[ 2 ] = { 0x11,0x11 };
	BOOLEAN bSuccess = FALSE;
	ERROR_INFO einfo;

	// Initialize the patterns with CRC+PC as below
	//
	UINT8 TagMask[24];
	memset(TagMask,0xFF,24);
		
	UINT8 tagMask[] = { 0xFF,0xFF };
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{0} };
	ACCESS_FILTER accessFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	// Opcode Related Data 
	UINT32 opIndex;
	TAG_STORAGE_SETTINGS tgs;
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader, &tgs );
	tgs.maxMemoryBankByteCount = 64 * 1024;
	tgs.maxTagCount = 100;
	tgs.enableAccessReports = TRUE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgs );
	
	LPTAG_DATA pResultTag = RFID_AllocateTag( api3Data.hReader );
	
	// Reading 64K memory
	//
	for( int bytelen = 1;bytelen <= 64; bytelen++ )
	{
		fprintf( api3Data.fpCurLog,"\n\n<a name=CAPI-FUJITSU-READ-%d></a> CAPI-FUJITSU-READ-%d:\n\n<br>Description: RFID_Read()--> offset:%d LENGTH:%d BYTES \n <br> Expected OutPut :Successful Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,0,bytelen*16);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,0,bytelen*16,0,{0} };
		api3Data.rfidStatus = RFID_Read( api3Data.hReader,api3Data.pfujitsu64Ktag->pTagID,api3Data.pfujitsu64Ktag->tagIDLength,&rParams,NULL,0,pResultTag,0 );
		fprintf( api3Data.fpCurLog,"\n API Result Description: %S<br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		RFID_GetLastErrorInfo(api3Data.hReader,&einfo );
		wprintf(L"\n apiresult: %ls vendormsg: %ls",RFID_GetErrorDescription(api3Data.rfidStatus),einfo.vendorMessage );
		printTagDataWithResults( api3Data.pfujitsu64Ktag );
		fprintf(api3Data.fpCurLog,"\n Operation Status : %s<br>",ACCESSSTATUS[pResultTag->opStatus]);
		wprintf(L"\n Operation Status : %ls",wACCESSSTATUS[pResultTag->opStatus] );

		if(api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult(TRUE);
		}
		logToResultFile(L"CAPI-FUJITSU-READ");
	}

	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	for( int bytelen = 1;bytelen <= 64; bytelen++ )
	{
		fprintf( api3Data.fpCurLog,"\n\n<a name=CAPI-FUJITSU-READ-%d></a> CAPI-FUJITSU-READ-%d:\n\n<br>Description: OPS Read--> offset:%d LENGTH:%d BYTES \n <br> Expected OutPut :Successful Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,0,bytelen);
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,0,bytelen*32,0,{0} };
		OP_CODE_PARAMS opParams = { ACCESS_OPERATION_READ,&rParams,{0}};
				
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,&accessFilter,pInfo,ptInfo,0);
		rfid3Sleep( 10000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pResultTag ) )
		{
			wprintf(L"\n TagID : %ls AccessResult: %ls EVENT:%ls",Byte2HexWString(pResultTag->pTagID,pResultTag->tagIDLength),wACCESSSTATUS[pResultTag->opStatus],wTAGEVENTS[pResultTag->tagEvent]);
			fprintf( api3Data.fpCurLog,"\n TagID : %s AccessResult: %s EVENT:%s<br>",Byte2HexString(pResultTag->pTagID,pResultTag->tagIDLength),ACCESSSTATUS[pResultTag->opStatus],TAGEVENTS[pResultTag->tagEvent]);

			if( pResultTag->opStatus == ACCESS_SUCCESS )
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
		
		logToResultFile(L"CAPI-FUJITSU-READ");
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	}

	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pResultTag != NULL ) RFID_DeallocateTag( api3Data.hReader,pResultTag );
}

bool getfujitsu64KTag( )
{
	UINT8 beddTag[] = { 0xF6,0x40 };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF };
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{0} };
	POST_FILTER postFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL }; 

	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,&postFilter,NULL,NULL,0);
	rfid3Sleep( 10000 );
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	
	if( (api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader,pTagData ) ) == RFID_API_SUCCESS )
	{
		memcpy( api3Data.pfujitsu64Ktag->pTagID,pTagData->pTagID,pTagData->tagIDLength);	
		api3Data.pfujitsu64Ktag->tagIDLength = pTagData->tagIDLength;
		wprintf(L"\n fujitsu tag: %ls",Byte2HexWString( api3Data.pfujitsu64Ktag->pTagID,pTagData->tagIDLength ) );
	}
	else
	{
		wprintf(L"\n tag was not found");
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

void Fujitsu64KTest( )
{
	api3Data.dwTestCaseNo = 01;
	SetLogText("Fujitsu64KTest");
	logCurrentExecution( "Fujitsu64KTest");
	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader, &api3Data.rCaps );
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.FujitsuCommandsSupported )
	{
		api3Data.dwSkipCount = 108;
		LogPassFailCount();
		return;
	}
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 108;
	TAG_STORAGE_SETTINGS tgSettings;
	api3Data.rfidStatus  = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings );
	tgSettings.enableAccessReports = TRUE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );

	if(!getfujitsu64KTag( ) )
	{
		wprintf(L"\n Could not get fujitsu 64K tag, test cannot proceed");
		fprintf(api3Data.fpCurLog,"\n Could not get fujitsu 64K tag, test cannot proceed");
	}
	else
	{
		fujitsu64KBurstWrite( );
		fujitsu64KReading( NULL,NULL );
		TestOPSequenceBurstWrite(NULL,NULL,0);
		fujitsu64KBurstErase();
		TestOPSequenceBurstErase( NULL,NULL ,0);
	/*	fujitsu64KChangeWordLock( );
		fujitsu64KChangeBlockLock();*/
	}
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Completed",MultiByte2WideChar( __FUNCTION__));
}