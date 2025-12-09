#include "Common.h"
#include <stdio.h>
#include "APITEST.h"
extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[2];


void TestLockOperations( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo )					
{
	UINT8 Data2Write[4] = { 0xDE,0xC0,0xDE,0xDD };
	UINT8 accessPwd[4]  = { 0x12,0x34,0x56,0x78 };
	
	SetLogText("TestLockOperations()- FUN ");
	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperations - Start ------------------\n\n");	
	logCurrentExecution( "TestLockOperations");

	HANDLE hStopAccess = CreateEvent(NULL, FALSE, FALSE, NULL);	// access stop event
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hStopAccess );

	TRIGGER_INFO triggerInfo;
	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
	//triggerInfo.stopTrigger.value.duration = 4000; // perform 3 rounds of inventory	
	
	// TEST with Simple Lock Operations without AnyFilters 
	// Read the first tag in the inventory list and lock and perform write with and without password
	//
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	
	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
	Sleep( 4000 );
	RFID_StopInventory( hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
	{	
		LOCK_ACCESS_PARAMS lockParams;

		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,accessPwd, 4, 0,0 };
		
		// Write Access Password and KILL Password to the MemoryBank
		//
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		
		wParams.byteOffset = 4;
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		
		lockParams.privilege[ LOCK_USER_MEMORY ] = LOCK_PRIVILEGE_READ_WRITE;
		lockParams.accessPassword = 0x00000000;			
		logToFile( "RFID_Lock() to all memory Banks without PASSWORD ");
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
		
		logToFile( "RFID_Lock() to all memory Banks with ACCESS PASSWORD ");
		lockParams.accessPassword = 0x12345678;
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	
		// Verify Write on a LOCKED MEMORY Bank USER without PASSWORD
		//
		{	
			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write, 4, 0,0 };

			logToFile("RFID_Write() on a locked memory bank : USER");
			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
			WaitForSingleObject( hStopAccess, 10000 );
			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				SetLogResult( TRUE );
			}

			logToResultFile(TEXT("LOCK-FUN") );
		}

		// Verify Write on a LOCKED MEMORY Bank USER with WRONG PASSWORD
		//
		{
			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write, 4, 0x12341234,0 };

			logToFile("RFID_Write() on a locked memory bank : USER");
			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
			WaitForSingleObject( hStopAccess, 10000 );
			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				SetLogResult( TRUE );
			}

			logToResultFile(TEXT("LOCK-FUN") );
		}

		// Verify Write on a LOCKED MEMORY Bank USER with PASSWORD
		//
		{
			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER, 0, Data2Write,4,0x12345678,0 };

			logToFile("RFID_Write() on a locked memory bank : USER");
			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
			WaitForSingleObject( hStopAccess, 10000 );
			if( api3Data.rfidStatus == RFID_API_SUCCESS )
			{
				SetLogResult( TRUE );
			}

			logToResultFile(TEXT("LOCK-FUN") );
		}

		//unlock all memory banks
		//
		lockParams.privilege[LOCK_USER_MEMORY] = LOCK_PRIVILEGE_UNLOCK;
		lockParams.accessPassword = 0x12345678;

		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		logToFile( "RFID_Lock() unlock to all memory Banks");
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}
	}

	{
		LOCK_ACCESS_PARAMS lockParams;
		
		lockParams.privilege[ LOCK_EPC_MEMORY ] = LOCK_PRIVILEGE_READ_WRITE;
				
		logToFile( "RFID_Lock() to EPC Memory Banks without PASSWORD ");
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
		
		logToFile( "RFID_Lock() to all memory Banks with ACCESS PASSWORD ");
		lockParams.accessPassword = 0x12345678;
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	
		// Verify Write on a LOCKED memory Bank EPC without PASSWORD
		//
		{	
			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_EPC,4,Data2Write, 4, 0,0 };

			logToFile("RFID_Write() on a locked memory bank : EPC");
			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
			WaitForSingleObject( hStopAccess, 10000 );
			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				SetLogResult( TRUE );
			}

			logToResultFile(TEXT("LOCK-FUN") );
		}

		// Verify Write on a LOCKED memory Bank EPC Wrong PASSWORD
		//
		{		
			WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_EPC,4,Data2Write, 4, 0x12341234,0 };

			logToFile("RFID_Write() on a locked memory bank : EPC");
			api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
			WaitForSingleObject( hStopAccess, 10000 );
			if( api3Data.rfidStatus != RFID_API_SUCCESS )
			{
				SetLogResult( TRUE );
			}

			logToResultFile(TEXT("LOCK-FUN") );
		}

					
		//unlock all memory banks
		//
		lockParams.privilege[LOCK_EPC_MEMORY]= LOCK_PRIVILEGE_UNLOCK;
		lockParams.accessPassword = 0x12345678;

		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		logToFile( "RFID_Lock() unlock to all memory Banks");
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(TEXT("LOCK-FUN") );
	}
	
	RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT );
	RFID_DeletePreFilter( hReader,0,0 );
	RFID_DeallocateTag( hReader,pTagData);	

	LogPassFailCount();
	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperations - End ------------------\n\n");	
}

void TestLockOperationsOnResMemory( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo )
{
	LOCK_ACCESS_PARAMS lockParams;
	UINT8 Data2Write[4] = { 0xDE,0xC0,0xDE,0xDD };
	UINT8 accessPwd[4]  = { 0x12,0x34,0x56,0x78 };
	HANDLE hStopAccess = CreateEvent(NULL, FALSE, FALSE, NULL);	// access stop event
	api3Data.rfidStatus = RFID_RegisterEventNotification( hReader,ACCESS_STOP_EVENT,hStopAccess );
	
	SetLogText("TestLockOperationsOnResMemory()- FUN ");
	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperationsOnResMemory - Start ------------------\n\n");	
	logCurrentExecution( "TestLockOperationsOnResMemory");

			
	TRIGGER_INFO triggerInfo;
	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
		
	// TEST with Simple Lock Operations without AnyFilters 
	// Read the first tag in the inventory list and lock and perform write with and without password
	//
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	

	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
	Sleep( 4000 );
	RFID_StopInventory( hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
	{
		logToFile( "RFID_Lock() to all memory Banks without PASSWORD ");
		
		lockParams.privilege[ LOCK_KILL_PASSWORD ] = LOCK_PRIVILEGE_READ_WRITE;
		lockParams.privilege[ LOCK_ACCESS_PASSWORD ] = LOCK_PRIVILEGE_READ_WRITE;

		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,accessPwd, 4, 0,0 };
		
		// Write Access Password and KILL Password to the MemoryBank
		//
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		wParams.byteOffset = 4;
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
		
		logToFile( "RFID_Lock() to all memory Banks with ACCESS PASSWORD ");
		lockParams.accessPassword = 0x12345678;
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}


	// Verify Write on a LOCKED MEMORY BAnk RES( killpw) without PASSWORD
	//
	{		
		SetLogText( "RFID_Write() on a locked memory bank RES( KILLPW )");	
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,Data2Write,4,0,0 };

		logToFile("RFID_Write() on a locked memory bank : RES( KILLPW )");
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}

	// Verify Write on a LOCKED MEMORY BAnk RES( killpw) with WRONG PASSWORD
	//
	{
		SetLogText( "RFID_Write() on a locked memory bank RES( KILLPW )");	
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,0,Data2Write,4, 0x12341234,0 };

		logToFile("RFID_Write() on a locked memory bank : RES( KILLPW )");
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}


	// Verify Write on a LOCKED MEMORY BAnk RES( access pw) without PASSWORD
	//
	{
		SetLogText( "RFID_Write() on a locked memory bank RES( ACCESSPW )");	
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,4,Data2Write, 4, 0,0 };

		logToFile("RFID_Write() on a locked memory bank : RES( ACCESSPW )");
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}

	// Verify Write on a LOCKED MEMORY BAnk RES( access pw) with WRONG PASSWORD
	//
	{
		SetLogText( "RFID_Write() on a locked memory bank RES( ACCESSPW )");	
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED,4,Data2Write, 4, 0x12341234,0 };

		logToFile("RFID_Write() on a locked memory bank : RES( ACCESSPW )");
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}

	
	// Verify Write on a LOCKED MEMORY BAnk RES( killpw) with PASSWORD
	//
	{
		SetLogText( "RFID_Write() on a locked memory bank RES( KILLPW )");	
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 0, accessPwd,4, 0x1234578,0 };

		logToFile("RFID_Write() on a locked memory bank : RES( KILLPW )");
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}

	// Verify Write on a LOCKED MEMORY BAnk RES( access pw) with PASSWORD
	//
	{
		SetLogText( "RFID_Write() on a locked memory bank RES( ACCESSPW )");	
		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 4, accessPwd,4, 0x12345678,0 };

		logToFile("RFID_Write() on a locked memory bank : RES( ACCESSPW )");
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,pInfo,0);
		WaitForSingleObject( hStopAccess, 10000 );
		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}

		logToResultFile(TEXT("LOCK-FUN") );
	}

	//unlock all memory banks
	//
	lockParams.privilege[LOCK_KILL_PASSWORD] = LOCK_PRIVILEGE_UNLOCK;
	lockParams.privilege[LOCK_ACCESS_PASSWORD]= LOCK_PRIVILEGE_UNLOCK;
	lockParams.accessPassword = 0x12345678;

	api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL, pInfo,NULL );
	WaitForSingleObject( hStopAccess, 10000 );
	logToFile( "RFID_Lock() unlock to all memory Banks");
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	//logToResultFile(TEXT("LOCK-FUN") );

	RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT );
	RFID_DeletePreFilter( hReader,0,0 );
	RFID_DeallocateTag( hReader,pTagData);	
	
	LogPassFailCount();
	fprintf( api3Data.fpLog,"\n\n----------------TestLockOperationsOnResMemory - End ------------------\n\n");	
	
}
void TestRFID3KillTag( RFID_HANDLE32 hReader,UINT8* tPattern,ANTENNA_INFO* pInfo )
{
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( hReader );
	UINT8 accessPwd[4] = { 0x12,0x34,0x56,0x78 };
	fprintf(api3Data.fpLog," \n >>> KILL OPERATIONS TESTS.<< \n");
	SetLogText("RFID_Kill()- FUN");
			
	TRIGGER_INFO triggerInfo;

	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_DURATION;
	triggerInfo.stopTrigger.value.duration = 5000; // perform 3 rounds of inventory	
	
	// TEST with Simple Lock Operations without AnyFilters 
	// Read the first tag in the inventory list and lock and perform write with and without password
	//
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	addanWareSelectRecord( hReader,tPattern,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);

	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
	Sleep( 6000 );
	
	if( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
	{
		SetLogText( "RFID_Lock() to PERMALOCK USER and PERMAUNLOCK EPC without ACCESS PASSWORD");

		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 4, accessPwd,4,0 };
		
		// Write Access Password and KILL Password to the MemoryBank
		//
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		wParams.byteOffset = 0;
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		KILL_ACCESS_PARAMS killParams = { 0x12345678,0 }; 
		api3Data.rfidStatus = RFID_Kill( hReader,pTagData->pTagID,pTagData->tagIDLength,&killParams, 0, pInfo, 0 );

		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			fprintf( api3Data.fpLog, "\n\n Kill Success ");
		}
	}
}

// Verify this on a Single Tag in the field and kill it in the final part of the tes to verify the KILL OPeration.
// PLACE a SINGLE tag in the field with PC+EPC  as 3000FFFF and verify the test  for EPC and USER MEMORY 
void TestPermaLockANDPermaUNLock( RFID_HANDLE32 hReader,UINT8* tPattern, ANTENNA_INFO* pInfo)					
{
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( hReader );
	UINT8 accessPwd[4] = { 0x12,0x34,0x56,0x78 };
	SetLogText("TestPermaLockANDPermaUNLock()- FUN ");
	fprintf( api3Data.fpLog,"\n\n----------------TestPermaLockANDPermaUNLock - Start ------------------\n\n");	
	logCurrentExecution( "TestPermaLockANDPermaUNLock");
			
	TRIGGER_INFO triggerInfo;

	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
	
	// TEST with Simple Lock Operations without AnyFilters 
	// Read the first tag in the inventory list and lock and perform write with and without password
	//
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	addanWareSelectRecord( hReader,tPattern,MEMORY_BANK_EPC,32,16,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);

	RFID_PerformInventory( hReader, NULL, pInfo, &triggerInfo, NULL );
	Sleep( 6000 );
	RFID_StopInventory( hReader );

	if( RFID_API_SUCCESS == RFID_GetReadTag( hReader, pTagData ) )
	{
		logToFile( "RFID_Lock() to PERMALOCK USER and PERMAUNLOCK EPC without ACCESS PASSWORD");

		WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_RESERVED, 4, accessPwd,4,0 };
		
		// Write Access Password and KILL Password to the MemoryBank
		//
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
		wParams.byteOffset = 0;
		api3Data.rfidStatus = RFID_Write( hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams, 0, pInfo, 0 );
	
		LOCK_ACCESS_PARAMS lockParams;

		lockParams.privilege[ LOCK_USER_MEMORY ] = LOCK_PRIVILEGE_PERMA_LOCK;
		lockParams.privilege[ LOCK_EPC_MEMORY ] = LOCK_PRIVILEGE_PERMA_UNLOCK;
		
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );

		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}		
		logToResultFile(TEXT("LOCK-FUN") );

		logToFile( "RFID_Lock() to PERMALOCK USER and PERMAUNLOCK EPC with ACCESS PASSWORD");

		lockParams.accessPassword = 0x12345678;
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );

		if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(TEXT("LOCK-FUN") );
				
		logToFile( "RFID_Lock() try unLOCK on a PERMA LOCKEd MEM");
		lockParams.privilege[ LOCK_USER_MEMORY ] = LOCK_PRIVILEGE_UNLOCK;
		lockParams.privilege[ LOCK_EPC_MEMORY ] = LOCK_PRIVILEGE_PERMA_LOCK;

		lockParams.accessPassword = 0x12345678;
		api3Data.rfidStatus = RFID_Lock( hReader,pTagData->pTagID,pTagData->tagIDLength,&lockParams,NULL,pInfo,NULL );

		if( api3Data.rfidStatus != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}		

		logToResultFile(TEXT("LOCK-FUN") );
	}
	api3Data.rfidStatus = RFID_DeletePreFilter( hReader,0,0 );
	api3Data.rfidStatus = RFID_DeallocateTag( hReader, pTagData );
	
	LogPassFailCount();
	fprintf( api3Data.fpLog,"\n\n----------------TestPermaLockANDPermaUNLock - End ------------------\n\n");	
}

