//ImpinjQT.cpp : Verify Impinj Tag functionality
#include "Common.h"
#include <stdlib.h>
#include "ImpinjQT.h"

extern struct RFID3DATA api3Data; // API3 Data
// Test Impinj functionality with Synchronous API
//
void ImpinjQTTest( )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);
	IMPINJ_QT_DATA qtData = { 0x0000,{0} };	
	IMPINJ_QT_WRITE_PARAMS qtParams = { 0x00000001, TRUE, qtData };
	fprintf( api3Data.fpCurLog,"\n\n<a name=CAPI-IMPINJ-QTWRITE-%d></a> CAPI-IMPINJ-QTWRITE-%d:\n\n<br>Description: RFID_ImpinjQTWrite() \n <br> Expected OutPut :Successful Write \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	api3Data.rfidStatus = RFID_ImpinjQTWrite( api3Data.hReader,api3Data.pImpinjTag->pTagID,api3Data.pImpinjTag->tagIDLength,&qtParams,NULL,NULL,0 );
	fprintf( api3Data.fpCurLog,"<br> Error Description %ls",RFID_GetErrorDescription(api3Data.rfidStatus));

	if( RFID_API_SUCCESS != api3Data.rfidStatus )
	{
		wprintf(L"\n error reading control data %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		SetLogResult( TRUE );
	}
	else
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-IMPINJ-QTWRITE");

	// Read back the written control data
	//
	fprintf( api3Data.fpCurLog,"\n\n<a name=CAPI-IMPINJ-QTREAD-%d></a> CAPI-IMPINJ-QTREAD-%d:\n\n<br>Description: RFID_ImpinjQTRead() \n <br> Expected OutPut :Successful Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	IMPINJ_QT_READ_PARMS qtReadParams = { 0x00000001,{0} }; 
	api3Data.rfidStatus = RFID_ImpinjQTRead( api3Data.hReader,api3Data.pImpinjTag->pTagID,api3Data.pImpinjTag->tagIDLength,&qtReadParams,NULL,NULL,pTagData,0 );
	if( api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		wprintf( L"\n Control Data %02X",pTagData->lpAccessOperationResult->QtData.QTControlData);
		fprintf( api3Data.fpCurLog, "\n <br> Control Data %X",pTagData->lpAccessOperationResult->QtData.QTControlData);
	}
	else
	{
		wprintf(L"\n TagID : %ls AccessResult: %ls EVENT:%ls<br>",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus],wTAGEVENTS[pTagData->tagEvent]);
		wprintf(L"\n error reading control data %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	}

	if(api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-IMPINJ-QTREAD");
	if( pTagData != NULL ) RFID_DeallocateTag(api3Data.hReader,pTagData );
}
// Test Impinj functionality with OP Sequence
//
void ImpinjOPSQTWrite( UINT16 ControlData )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);

	// Opcode Related Data 
	UINT32 opIndex;
	IMPINJ_QT_DATA qtData = { ControlData,{0} };	
	IMPINJ_QT_WRITE_PARAMS qtParams = { 0x00000001, TRUE, qtData,{0} };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_IMPINJ_QT_WRITE,&qtParams,{0}};
	
	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-IMPINJ-QTWRITE-%d></a> CAPI-IMPINJ-QTWRITE-%d:\n\n<br>Description: OPSEQ IMPINJ WRITE CONTROL DATA %X \n <br> Expected OutPut :Successful Write \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,ControlData );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		wprintf(L"\n TagID : %ls AccessResult: %ls EVENT:%ls<br>",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus],wTAGEVENTS[pTagData->tagEvent]);
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
		
		// Perform inventory and Verify the EPC lenght is 128 bit
		//
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, NULL,NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}		
		
		// Read TID Memory
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,0,0,{0} };
		opParams.opCode = ACCESS_OPERATION_READ;
		opParams.opParams = &rParams;
		api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
		rfid3Sleep( 3000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
		rfid3Sleep( 1000 );
		api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);

		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}	

		// READ USER MEMory 
		rParams.memoryBank = MEMORY_BANK_USER;
		opParams.opParams = &rParams;
		api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
		api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
		api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
		rfid3Sleep( 3000 );
		api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
		rfid3Sleep( 1000 );
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}	
	
		if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
	}

	logToResultFile(L"CAPI-IMPINJ-QTWRITE");
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
}
// Imnpinj QT Read with OP Sequence
//
void ImpinjOPSQTRead(  )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);
	// Opcode Related Data 
	UINT32 opIndex;
	IMPINJ_QT_READ_PARMS qtReadParams = { 0x00000001,{0} };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_IMPINJ_QT_READ,&qtReadParams,{0}};
	
	// Initialize op sequence
	//
	fprintf( api3Data.fpCurLog,"\n\n<a name=CAPI-IMPINJ-QTREAD-%d></a> CAPI-IMPINJ-QTREAD-%d:\n\n<br>Description: OPSEQ QT READ \n <br> Expected OutPut :Successful Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		wprintf(L"\n TagID : %ls AccessResult: %ls <br>",Byte2HexWString(pTagData->pTagID,pTagData->tagIDLength),wACCESSSTATUS[pTagData->opStatus] );
		fprintf(api3Data.fpCurLog,"\n <br> TagID : %s AccessResult: %s <br>",Byte2HexString(pTagData->pTagID,pTagData->tagIDLength),ACCESSSTATUS[pTagData->opStatus] );
		if(pTagData->opStatus == ACCESS_SUCCESS)
		{
			wprintf(L"\n Control Data :%02X", pTagData->lpAccessOperationResult->QtData.QTControlData );
			fprintf( api3Data.fpCurLog,"\n<br> Control Data :%X", pTagData->lpAccessOperationResult->QtData.QTControlData );
		}

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
	logToResultFile(L"CAPI-IMPINJ-QTREAD");
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
}
// get Impinj tag from the field
//
BOOLEAN getImpinjTag()
{
	UINT8 impinj[] = { 0xD0,0x0D };	// Tag EPC hardcoded with EPC as D00D, Always test with a filter.
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,impinj,16,tagMask,16,{ 0 } };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{ 0 } };
	POST_FILTER postFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL }; 

	if( wcscmp(api3Data.rCaps.modelName,L"75004") == 0 )
	{
		postFilter.lpRSSIRangeFilter = NULL;
	}

	if (wcscmp(api3Data.rCaps.modelName, L"96004") == 0)
	{
		postFilter.lpRSSIRangeFilter = NULL;
	}
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);

	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,&postFilter,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	
	if( (api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader,pTagData ) ) == RFID_API_SUCCESS )
	{
		printTagDataWithResults( pTagData );
		memcpy( api3Data.pImpinjTag->pTagID,pTagData->pTagID,pTagData->tagIDLength);	
		api3Data.pImpinjTag->tagIDLength = pTagData->tagIDLength;
	}
	else
	{
		wprintf(L"\n No Tags are Read, Verify Read Criteria\n");
		return FALSE;
	}
	
	RFID_DeallocateTag( api3Data.hReader,pTagData );

	if(api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		wprintf(L"\n Successfully Read Impinj QT Tag\n");
		return TRUE;
	}
	else
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		wprintf(L"\n Could not Read Impinj QT Tag\n");
		return FALSE;
	}
	return FALSE;
}

// main function that calls impinj functions.
void ImpinjTest( )
{
	api3Data.dwTestCaseNo = 01;
	SetLogText("ImpinjTest");
	logCurrentExecution( "ImpinjTest");
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.ImpinjCommandSupported)
	{
		api3Data.dwSkipCount = 6;
		LogPassFailCount();
		return;
	}
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	TAG_STORAGE_SETTINGS tgSettings;
	api3Data.itemTestCount = 6;
	api3Data.rfidStatus  = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings );
	tgSettings.enableAccessReports = TRUE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );
	//
	/*ImpinjOPSQTWrite(0xC000);
	ImpinjOPSQTWrite(0x8000);
	ImpinjOPSQTWrite(0x4000);
	ImpinjOPSQTWrite(0x0000);*/
	//
	ImpinjOPSQTWrite(0x0000);
	ImpinjOPSQTWrite(0x4000);
	ImpinjOPSQTRead( );
	ImpinjOPSQTWrite(0x0000);
	if( FALSE == getImpinjTag( ) )
	{
		wprintf(L"\n Could not get Impinj QT Tag, test cannot proceed");
	}
	else
	{
		ImpinjQTTest();
	}

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Completed",MultiByte2WideChar( __FUNCTION__));
}