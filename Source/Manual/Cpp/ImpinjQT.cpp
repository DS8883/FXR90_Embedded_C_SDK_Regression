//ImpinjQT.cpp : Verify Impinj Tag functionality
#include "ManualTests.h"

extern struct RFID3DATA api3Data; // API3 Data

// Test Impinj functionality with Synchronous API
//

// Test Impinj functionality with OP Sequence
//
void ImpinjOPSQTWrite( UINT16 ControlData )
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader);
	LPTAG_DATA pImpinjTag = RFID_AllocateTag( api3Data.hReader);
	
	// Opcode Related Data 
	UINT32 opIndex;
	IMPINJ_QT_DATA qtData = { ControlData,0 };	
	IMPINJ_QT_WRITE_PARAMS qtParams = { 0x00000001, TRUE, qtData };
	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_IMPINJ_QT_WRITE,&qtParams,0};
	
	// Initialize op sequence
	//
	api3Data.rfidStatus = RFID_InitializeAccessSequence( api3Data.hReader );
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader, &opParams,&opIndex );
	api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopAccessSequence( api3Data.hReader );
	wprintf(L"\n Writing Control Data to Impinj QT Tag:  %4x\n ",ControlData);
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		if(pTagData->opStatus == ACCESS_SUCCESS )
		{
			memcpy(pImpinjTag,pTagData,sizeof(TAG_DATA));
			wprintf(L"\n Written Successfully\n");
			api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
			bSuccess = TRUE;
			break;
		}
	}

	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence( api3Data.hReader );
	if( bSuccess )
	{
		// Perform inventory and Verify the EPC lenght is 128 bit
		//
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, NULL,NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			memcpy(pImpinjTag,pTagData,sizeof(TAG_DATA));
			printTagDataWithResults( pTagData);
			break;
		}	
		
		READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,0,0,0 };
		// Read TID Memory
		for(int i=0;i<5;i++)
		{
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, pImpinjTag->pTagID,pImpinjTag->tagIDLength, &rParams, NULL, NULL, pImpinjTag, 0);
			if( api3Data.rfidStatus == RFID_API_SUCCESS)
			{
				printTagDataWithResults(pImpinjTag);
				wprintf(L"\n TID memoryBank Length %ld\n",pImpinjTag->memoryBankDataLength);
				break;
			}
			else
			{
				wprintf(L"\n result %ls\n",RFID_GetErrorDescription(api3Data.rfidStatus));
			}
		}

		// READ USER MEMory 
		rParams.memoryBank = MEMORY_BANK_USER;
		for(int i=0;i<5;i++)
		{
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, pImpinjTag->pTagID,pImpinjTag->tagIDLength, &rParams, NULL, NULL, pImpinjTag, 0);
			if( api3Data.rfidStatus == RFID_API_SUCCESS)
			{
				printTagDataWithResults(pImpinjTag);
				wprintf(L"\n USer memoryBank Length %ld\n",pImpinjTag->memoryBankDataLength);
				break;
			}
			else
			{
				wprintf(L"\n result %ls\n",RFID_GetErrorDescription(api3Data.rfidStatus));
			}
		}
	}
	
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
}

// main function that calls impinj functions.
void ImpinjTest(RFID_VERSION version )
{
	// Make sure the PC of the Impinj Tag is greater than or equal to 4000 to verify 128 bit EPC.
	// 
	wprintf(L"\n Impinj QT Testing");

	// Connect to the Reader
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	
	TAG_STORAGE_SETTINGS tgSettings;
	api3Data.rfidStatus  = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings );
	tgSettings.enableAccessReports = TRUE;
	tgSettings.maxMemoryBankByteCount = tgSettings.maxTagIDByteCount = 64;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings );
	//| Step Name       | Description                         | Expected Result                                                                                                                     
	//| Step 4          | 4. Perform Inventory                | 4. Tag should return to the Query command with 128 bit Private EPC value and the PC word and CRC word should alsorelfelct the same. 
	//| Step 5          | 5. Trt to read the TID Memory Bank  | 5. Tag should return 32 bit standard TID, 16 bit TIDheader, 48 bit serial number and 96 bit public EPC                              
	//| Step 6          | 6. Try to Read the User Memory Bank | 6. Should return 512 bit user memory bank data.                                                                                     

	ImpinjOPSQTWrite(0x0000); // EPC 128 bit, TID Contains 96 bit Public EPC, 

	//| Step Name       | Description                         | Expected Result                                                                                                                   
	//| Step 4          | 4. Perform Inventory                | 4. Tag should return to the Query command with 96 bit Public EPC value and the PC word and CRC word should alsorelfelct the same. 
	//| Step 5          | 5. Trt to read the TID Memory Bank  | 5. Tag should return only 32 bit standard TID                                                                                     
	//| Step 6          | 6. Try to Read the User Memory Ban*/k | 6. Should return appropriate errorcode(Memory Location unavailable)                                                                                                                                                                                           

	ImpinjOPSQTWrite(0x4000);	// EPC 96 bit, TID Contains 32 bits and No USER MEMORY 
	ImpinjOPSQTWrite(0x0000);
	// Disconnect to the Reader
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}