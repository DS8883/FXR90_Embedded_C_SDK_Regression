#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;

void ReadMemory( MEMORY_BANK mBank,UINT8 offSet,UINT16 dataLength,ANTENNA_INFO* pInfo  )					
{
	BOOLEAN bSuccess = FALSE;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	READ_ACCESS_PARAMS rParams = { mBank,offSet,dataLength,0 };

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-READALLMEMORY-%d></a>CAPI-READALLMEMORY-%d:\n\n<br>Description: Reading from Mem Bank:%s  offSet:%d len:%d \n <br> Expected OutPut :Should Read the Memory Bank \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,G2MEMBANK[mBank],offSet,dataLength );
	wprintf(L"\nReading from Mem Bank:%ls  offSet:%d len:%d ",wG2MEMBANK[mBank],offSet,dataLength);
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	
	RFID_PerformInventory( api3Data.hReader,NULL,pInfo,NULL,NULL );
	rfid3Sleep( 5000 );
	RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		if(( api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&rParams,NULL,pInfo,pTagData,0) ) == RFID_API_SUCCESS )
		{
			printTagDataWithResults( pTagData);
			if(pTagData->memoryBankDataLength == dataLength )
			{
				bSuccess = TRUE;
				break;
			}
		}
	}

	if ( bSuccess )
	{
		SetLogResult( TRUE );
	}
	
	logToResultFile(L"CAPI-READALLMEMORY");
	
	RFID_PurgeTags( api3Data.hReader, 0 );
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
}

void TestReadAllMemoryBanks( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestReadAllMemoryBanks");
	logCurrentExecution( "TestReadAllMemoryBanks");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 27;
	// Read EPC Memory Bank Data from different OFF Sets
	//
	ReadMemory( MEMORY_BANK_EPC,0,4,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_EPC,4,8,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_EPC,0,12,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_EPC,0,16,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_EPC,4,12,&api3Data.pInfo );

	// Read TID Memory Bank Data from different OFF Sets
	//
	ReadMemory( MEMORY_BANK_TID,0,4,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_TID,0,8,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_TID,0,16,&api3Data.pInfo );

	// Read RESERVED Memory Bank Data from different OFF Sets
	//
	ReadMemory( MEMORY_BANK_RESERVED,0,2,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_RESERVED,0,4,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_RESERVED,2,4,&api3Data.pInfo );

	// Read USER Memory Bank Data from different OFF Sets.Read entire memory bank first and then try reading four bytes
	// each time incrementing the off set and decrementing the length( this means that we are reading entire memory bank from
	// an offset).
	//
	ReadMemory( MEMORY_BANK_USER,0,64,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,4,60,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,8,56,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,12,52,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,16,48,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,20,44,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,24,40,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,28,36,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,32,32,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,36,28,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,40,24,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,44,20,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,48,16,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,52,12,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,56,8,&api3Data.pInfo );
	ReadMemory( MEMORY_BANK_USER,60,4,&api3Data.pInfo );

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
