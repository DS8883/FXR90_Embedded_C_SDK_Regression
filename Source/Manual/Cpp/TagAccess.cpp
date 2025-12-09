#include "ManualTests.h"

extern struct RFID3DATA api3Data;
extern wchar_t* PrintTime( void );

// [in] Count -- Number of times to be written to a tag
//
void SingleTagWrites( int Count,RFID_VERSION version)
{
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );

	UINT8 writeData[64];
	memset(writeData,0xCA,64);
	//memset(writeData, 0x44, 12);
	DWORD Iteration=0;
	BOOLEAN bSuccess = FALSE;
	RFID_ResetConfigToFactoryDefaults(api3Data.hReader);
	// TEST with Simple Write Operations without AnyFilters 
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	
	WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,writeData,64,0 };
	//WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_EPC, 4, writeData, 12, 0 };
	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 5000 );	// inventory for 4 Seconds
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	while( (api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader, pTagData ) ) == RFID_API_SUCCESS)
	{	
		if (pTagData->pTagID[0] == 0xBE)
		{
			int SucCount, FailCount; SucCount = FailCount = 0;
			for (int WriteCount = 0; WriteCount < Count; WriteCount++)
			{
				api3Data.rfidStatus = RFID_Write(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &wParams, NULL, NULL, 0);
				rfid3Sleep(500);
				if (api3Data.rfidStatus == RFID_API_SUCCESS)
				{
					wprintf(L"\n result is :%ls", RFID_GetErrorDescription(api3Data.rfidStatus));
					SucCount++;
				}
				else
				{
					wprintf(L"\n result is :%ls", RFID_GetErrorDescription(api3Data.rfidStatus));
					FailCount++;
					rfid3Sleep(500);
				}
				wprintf(L"\n No:Of Attempts :%d\t Success Count : %d \t Failure Count: %d", WriteCount, SucCount, FailCount);
			}
		}
	}

	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}

// Test Block perma lock
//
void TestBlockPermaLock(RFID_VERSION version )
{
	CONNECTION_INFO ConInfo = { version, 0 };
	//api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, L"10.17.130.229", 9180, 0, &ConInfo);

	wprintf(L"\n\n *********************Test Block Perma Lock Functionality****************\n");
	
	// Read the tag from the field for doing block perma lock operation
	//
	LPTAG_DATA pTagData  = RFID_AllocateTag( api3Data.hReader );
	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&api3Data.postFilter,NULL,NULL,0 );
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader, pTagData );
	
    UINT8 Mask[2] = { 0x80, 0x00};
	wprintf(L"\n Test Block Perma Lock on RPC Memory\n");

	// Block Permalock Higgs 3 tags taking each block aty a time
	//
	UINT16 temp  = 0x8000;
	for( int blockCount = 0;blockCount < 8; blockCount++ )
	{		
		wprintf(L"\n Perma Lock Block %d in the User memory\n\n", blockCount );
		wprintf(L"\n Block Mask: %4x\n", temp );
		// Read User-Bank of multiple tags
		BLOCK_PERMALOCK_ACCESS_PARAMS blkPermalockAccessParams = { MEMORY_BANK_USER,FALSE,0,1,0x00000001,Mask,2};
		
		api3Data.rfidStatus = RFID_BlockPermalock( api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &blkPermalockAccessParams, NULL, NULL, pTagData, NULL);
		rfid3Sleep( 5000 );
		if( RFID_API_SUCCESS == api3Data.rfidStatus )
		{
			wprintf(L"\n Perma Lock Success\n" );
		}
		else
		{
			wprintf(L"\n Perma Lock FAIL: Status: %ls\n",RFID_GetErrorDescription( api3Data.rfidStatus ));
		}
		
		// Read back the Mask from the Locked Tag
		//
		blkPermalockAccessParams.readLock = FALSE;
		api3Data.rfidStatus = RFID_BlockPermalock( api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &blkPermalockAccessParams, NULL, NULL, pTagData, NULL );
		
		wprintf(L"\n Read back Mask ..Status: %ls\n",RFID_GetErrorDescription( api3Data.rfidStatus ) );
		
		// Lets print the Tag ID and Memory Bank
		printTagDataWithResults(pTagData);
	
		temp = temp >> 1;
		Mask[0] = HIBYTE(temp);
		Mask[1] = LOBYTE(temp);
		int offset = blockCount * 4;	// because block is 4words for higgs,increment block by 4 every time
		//Try to write to the perma locked user memory
		//
		{
			BYTE writeData[] = {0x11,0x22,0x33,0x44};
			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,offset,writeData,4,0x00000001,0};
			api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,NULL,NULL );
			if(RFID_API_SUCCESS != api3Data.rfidStatus)
			{					
				wprintf(L"\n PermaLock Success: %S", RFID_GetErrorDescription( api3Data.rfidStatus ));
			}	
		}
	}

	// Cleanup 
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
// Access performance 
// [in] NoOfTagWrites -- Number of times the tag to be written and Read
//
void AccessSuccessFailCount( int NoOfTagWrites,RFID_VERSION version)
{
	TAG_STORAGE_SETTINGS tgSettings;

	UINT8 EPC1[] = {0xE2,0x00};
	UINT8 EPC2[] = {0x04,0x01};
	UINT8 TagMask[] = { 0xFF,0xFF };
	
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );
	//api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, TEXT("10.17.126.173"), 9180, 0, &ConInfo);

	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	tgSettings.discardTagsOnInventoryStop = FALSE;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	

	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,0,32,0,0 }; 
	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep(10000);
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader, pTagData );
	Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,pTagData->pTagID,pTagData->tagIDLength);

	int SucCount,FailCount;SucCount=FailCount = 0;
	printf("\n---- Access Read Operation----------");
	for( int i=0; i<NoOfTagWrites; i++)
	{
		api3Data.rfidStatus = RFID_Read( api3Data.hReader, pTagData->pTagID,pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0);
		
		if(api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SucCount++;
			Print2Console(pTagData->CRC, pTagData->PC, pTagData->XPC, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
		}
		else
		{
			FailCount++;
		}
		wprintf(L"\n Success Count : %d, Failure Count: %d, Iteration: %d",SucCount,FailCount,i);
	}

	wprintf( L"\n---- Access Write Operation----------");
	UINT8 Data2Write[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write,8,0 };
	FailCount;SucCount=FailCount = 0;
	for( int i=0;i<NoOfTagWrites; i++ )
	{
		api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,0,0);
		//Sleep(500);
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SucCount++;
		}
		else
		{
			FailCount++;
		}
		wprintf(L"\n Success Count : %d, Failure Count: %d, Iteration: %d",SucCount,FailCount,i);
	}

	// Cleanup 
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}



//  Stress Bock Erase
// [in] Count -- Number of times the tag to be erased.
void StressBlockErase( int Count,RFID_VERSION version )
{
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );
	BLOCK_ERASE_ACCESS_PARAMS blkParams = { MEMORY_BANK_USER,0,32,0,0 };
	UINT8 writeData[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	DWORD Iteration=1;
	BOOLEAN bSuccess = FALSE;
	
	// TEST with Simple Write Operations without AnyFilters 
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	
	WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,writeData,8,0 };
	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 4000 );	// inventory for 4 Seconds
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		printTagDataWithResults(pTagData);
		
		int SucCount,FailCount;SucCount=FailCount = 0;
		if (pTagData->pTagID[0] == 0xBE)
		{
			for (int WriteCount = 0; WriteCount < Count; WriteCount++)
			{

				api3Data.rfidStatus = RFID_BlockErase(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &blkParams, NULL, NULL, 0);
				if (api3Data.rfidStatus == RFID_API_SUCCESS)
				{
					SucCount++;
				}
				else
				{
					FailCount++;
				}
				wprintf(L"\n No:Of Attempts :%d\t Success Count : %d \t Failure Count: %d", Iteration++, SucCount, FailCount);
			}
		}
	}

	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
//  Stress Bock Write
// [in] Count -- Number of times the tag to be written using block Write.
void StressBlockWrite( int Count,RFID_VERSION version )
{
	UINT8 writeData[64];
	memset(writeData,0xAA,64);
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );
	
	WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,writeData,64,0 };
	DWORD Iteration=1;
	BOOLEAN bSuccess = FALSE;
	
	// TEST with Simple Write Operations without AnyFilters 
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,NULL,NULL );
	rfid3Sleep( 10000 );	// inventory for 4 Seconds
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		int SucCount,FailCount;SucCount=FailCount = 0;
		if (pTagData->pTagID[0] == 0xBE)
		{
			printTagDataWithResults(pTagData);
			for (int WriteCount = 0; WriteCount < Count; WriteCount++)
			{
				api3Data.rfidStatus = RFID_BlockWrite(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &wParams, NULL, NULL, 0);
				if (api3Data.rfidStatus == RFID_API_SUCCESS)
				{
					SucCount++;
				}
				else
				{
					FailCount++;
				}
				wprintf(L"\n No:Of Attempts :%d\t Success Count : %d \t Failure Count: %d", Iteration++, SucCount, FailCount);
			}
		}
	}

	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );

}