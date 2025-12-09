#ifndef _WIN64 
#ifndef linux
#include "rfid2Helper.h"
#include "TestRFID2API.h"
#include "common.h"

extern struct RFID2DATA api2Data;
extern struct RFID3DATA api3Data;

HANDLE hReadEvent;
HANDLE hReadThread;
extern MYTAGDATA MyTags[2];


void test_RFID2MemoryBank( void )					
{
	UINT8 mask[2];
	mask[0] = HIBYTE( MyTags[0].PC );
	mask[1] = LOBYTE( MyTags[0].PC );
	
	RFID2ReadTags( api2Data.hReader, 1,1);

	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S0_INV );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );  // Select Tags Inv Flag A
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S0 );
	//SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 2 );

	//%%%% Test 1 to 4
	{
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_RESERVED );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBank \n <br> Expected:Read RESERVED MEM Successfully\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,20,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}
	
	{
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBank \n <br> Expected:Read EPC MEM Successfully\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,20,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}

	{
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_TID );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBank \n <br> Expected:Read TID MEM Successfully\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,20,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}

	{
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_USER );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBank \n <br> Expected:Read USER MEM Successfully\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,20,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}	
}


void test_RFID2MemoryBankOFFSet( void )					
{

	UINT8 mask[] = { 0x30,0x00 };
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );  // Select Tags Inv Flag A
	api2Data.rfidStatus = RFID_DeletePreFilter( (RFID_HANDLE32)api2Data.hReader,0,0); // ensure cleanup ( API2 Call was not working
																			 // ao used API3 Call
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S0_INV );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S0 );
	//SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	
	//%%%% Test 1 to 4
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_RESERVED );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );	
	for(int offSet = 0; offSet < 4; offSet++ )
	{
		SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, offSet );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBankOFFSet \n <br> Expected:Read RES MEM from OFFSET\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,2,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}

	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_TID );
	for(int offSet = 0; offSet<4; offSet++ )
	{
		SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, offSet );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBankOFFSet \n <br> Expected:Read TID MEM from OFFSET\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,2,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}

	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_USER );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 7 );	
	for(int offSet = 0; offSet< 20; offSet++ )
	{
		SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, offSet );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_RFID2MemoryBankOFFSet \n <br> Expected:Read USER MEM from OFFSET\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,2,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");	
	}
}
#if 0
void testRFID2LockMask( void )
{
	SetLogText("testRFID2LockMask");
	fprintf( api3Data.fpLog,"\n\n----------------testRFID2LockMask - Start ------------------\n\n");	
	logCurrentExecution( " testRFID2LockMask");

	UINT8 mask[ 2 ] = { 0x30,0x00 };
	DWORD Status;

	api2Data.rfidStatus = RFID_DeletePreFilter( (RFID_HANDLE32)api2Data.hReader,0,0); // ensure cleanup ( API2 Call was not working
		
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S0_INV );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );  // Select Tags Inv Flag A
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S0 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );

	//Writing Access Passwords to the tags
	SetCapValue( api2Data.hReader,RFID_WRITECAP_TAGTYPE, RFID_TAG_TYPE_EPC_CLASSG2 );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 4 );// read entire Reserved Memory Bank
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_RESERVED );
	
	RFID2ReadTags( api2Data.hReader, 2 , 2 );
	// Write data to tags selected
	{
		UINT8 writeData[] = { 0x12,0x34,0x00,0x00,0x12,0x34,0x00,0x00 };
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 8, 3, 3, 5 );
	}

	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 2 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 2 ); //Kill PassWord

	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x12340000 ); // Access Password 
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_MASK, 0x00C0  );
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION, 0x0080 ); // Use Write Lock Option
	
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask------------",api3Data.dwTestCaseNo);
	Status = RFID_LockTag( api2Data.hReader,0,3, 5,0 ); // Length and the killcode are set to zero as it is not required
	if( Status == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile();	
	
	// Write data to tags selected With Access pwd correct to Res Mem
	{
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with correct PW------------",api3Data.dwTestCaseNo);	
		UINT8 writeData[] = { 0x12,0x34 };
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 2, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();
	}
	
	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x00000000 );// With Access pwd incorrect
	
	// Write data to tags selected With Access pwd INCORRECT to Res Mem
	{
		UINT8 writeData[] = { 0x56,0x78 };
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with incorrect PW------------",api3Data.dwTestCaseNo);
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 2, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();	
	}
	
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION, 0x0000 ); // Use Write UnLock Option
	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x12340000 ); // Access Password

	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask Unlock with correct PW------------",api3Data.dwTestCaseNo);
	Status = RFID_LockTag( api2Data.hReader,0,3, 5,0 ); // Length and the killcode are set to zero as it is not required
	if( Status == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile();	

	//***************EPC Memory Bank
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 ); //offset to EPC
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );

	RFID2ReadTags( api2Data.hReader, 2, 2 );
	
	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x12340000 ); // Access Password 
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_MASK, 0x0030  );
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION, 0x0020 ); // Use Write Lock Option

	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with Write lock correct PW------------",api3Data.dwTestCaseNo);
	Status = RFID_LockTag( api2Data.hReader,0,3, 5,0 ); // Length and the killcode are set to zero as it is not required
	if( Status == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile();	
	
	// Write data to tags selected With Access pwd correct to Res Mem
	{
		UINT8 writeData[] = { 0xAA,0xBB,0xCC,0xDD };
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with Correct PW------------",api3Data.dwTestCaseNo);
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 4, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();	
	}

	// Write data to tags selected With Access pwd INCORRECT to Res Mem
	{
		SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x00000000 ); // Access Password
		UINT8 writeData[] = { 0x56,0x78 };
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with IN Correct PW------------",api3Data.dwTestCaseNo);
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 2, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();
	}
	
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION, 0x0000 ); // Use Write UnLock Option
	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x12340000 ); // Access Password

	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with write unlock Correct PW------------",api3Data.dwTestCaseNo);
	Status = RFID_LockTag( api2Data.hReader,0,3, 5,0 ); // Length and the killcode are set to zero as it is not required
	if( Status == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile();

	// Write without PASSWORD should Succeed
	
	// Write data to tags selected With Access pwd INCORRECT to Res Mem
	{
		SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x00000000 ); // Access Password
		UINT8 writeData[] = { 0x12,0x34 };
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with IN Correct PW------------",api3Data.dwTestCaseNo);
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 2, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();
	}	
	
	//***************USER Memory Bank
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 ); //offset to USER
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 7 );	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_USER );
	RFID2ReadTags( api2Data.hReader, 2, 2 );
	
	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x12340000 ); // Access Password 
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_MASK, 0x0003  );
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION, 0x0002 ); // Use Write Lock Option

	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask Write Lock------------",api3Data.dwTestCaseNo);
	Status = RFID_LockTag( api2Data.hReader,0,3, 5,0 ); // Length and the killcode are set to zero as it is not required
	if( Status == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile();
	
	// Write data to tags selected With Access pwd correct to Res Mem
	{
		UINT8 writeData[] = { 0xBE,0xDD,0xED,0x00,0x00,0xBE,0xDD,0xED };
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 8, 3, 3, 5 );
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with Correct PW------------",api3Data.dwTestCaseNo);
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();
	}

	// Write data to tags selected With Access pwd INCORRECT to Res Mem
	{
		SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x00000000 ); // Access Password
		UINT8 writeData[] = { 0x56,0x78 };
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask with INCorrect PW------------",api3Data.dwTestCaseNo);
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 2, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();
	}
	
	SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION, 0x0000 ); // Use Write UnLock Option
	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x12340000 ); // Access Password

	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask Wrtie UnLock------------",api3Data.dwTestCaseNo);
	Status = RFID_LockTag( api2Data.hReader,0,3, 5,0 ); // Length and the killcode are set to zero as it is not required
	if( Status == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile();

	// Write without PASSWORD should Succeed
	
	// Write data to tags selected With Access pwd INCORRECT to USER Mem
	{
		SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD, 0x00000000 ); // Access Password
		UINT8 writeData[] = { 0x12,0x34 };
		fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,testRFID2LockMask Write to Tags------------",api3Data.dwTestCaseNo);
		Status = RFID_ProgramTags( api2Data.hReader, writeData, 2, 3, 3, 5 );
	
		if( Status == RFID_SUCCESS )
		{
			SetLogResult( TRUE );
		}
		logToResultFile();
	}	
	
	LogPassFailCount();
	fprintf( api3Data.fpLog,"\n\n----------------testRFID2LockMask - Start ------------------\n\n");
}
#endif
void ValidateRFID2LockAction( void )			
{
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2LockAction \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION,RFID_GEN2_LOCK_ACTION_EPC_PWD) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");	
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2LockAction \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION,RFID_GEN2_LOCK_ACTION_TID_PWD) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");	
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2LockAction \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueWORD( api2Data.hReader,RFID_WRITECAP_G2_LOCK_ACTION,RFID_GEN2_LOCK_ACTION_USER_PWD) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");	
}

void ValidateRFID2AccessPassword( void )
{
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2AccessPassword \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD,0x00000000 ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2AccessPassword \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD,0x12340000 ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2AccessPassword \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD,0xFFFFFFFF ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_ACCESS_PASSWORD,0x00000000 );//Capability Reset	
}


void ValidateRFID2KillPassword( void )
{	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2KillPassword \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_KILL_PASSWORD,0x00000000 ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2KillPassword \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_KILL_PASSWORD,0x12340000 ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2KillPassword \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_KILL_PASSWORD,0xFFFFFFFF ) )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	SetCapValueDWORD( api2Data.hReader,RFID_TAGCAP_G2_KILL_PASSWORD,0x00000000 );//Capability Reset	
}
/////////////////TODO////////////////////////////////////
void ValidateRFID2TagTypes( void )			
{

}
// Tag Read event thread for autonomous mode reading
//
DWORD WINAPI ReadTagEventThread( LPVOID pvarg )
{
	DWORD dwEvent;
	DWORD dwCounter = 0;
	DWORD dwStartTick = 0;
	HANDLE hReader = (HANDLE)pvarg;
	int numEvents = 0;

	while( api2Data.bContinue )
	{
		dwEvent = WaitForSingleObject( hReadEvent, INFINITE );
		if( dwEvent == WAIT_OBJECT_0 )
		{
			DWORD dwNumItems= 1;
			char  tagBuffer[512] = {0,};
			char* pTagReportData = tagBuffer;
			UINT16   index = 0;
			DWORD dwStatus = RFID_SUCCESS;
			TYPE_TAG_EX Tag;
			memset(&Tag, 0, sizeof(Tag));
			TYPE_TAG_EX1_INIT(Tag);
			if(RFID_SUCCESS == RFID_GetTagIDEX(hReader, &Tag) )
			{
				for( index = 0; index < Tag.dataLength; index++ )
				{
					if(0 < index && index%2 == 0)
					{
							*pTagReportData++ = '-';
					}

					sprintf(pTagReportData,"%02X", Tag.tagID[index]);
					while(*pTagReportData) pTagReportData++;
				}

				fprintf(api3Data.fpLog,"\n Tag ID: %s", tagBuffer);
			}
		}
	};

	return(0);
};



// Validates if autonomous mode Read the Tags in Mc3090 . This is backward compatiability test
//
void ValidateRFID2ReadMode( void )			
{
	fprintf( api3Data.fpLog,"\n\n Read Tags in OnDemand Mode\n");
	if( RFID_SUCCESS == SetCapValueDWORD( api2Data.hReader,RFID_READCAP_READMODE_ONDEMAND,RFID_READCAP_READMODE ) )
	{
		RFID2ReadTags( api2Data.hReader,4,2);
	}

	fprintf( api3Data.fpLog,"\n\n Read Tags in Autonomous Mode\n");
	
	hReadEvent = CreateEvent( NULL, TRUE, FALSE, L"TagReadEvent" );	
	ConfigureReadTagEvent( api2Data.hReader, L"TagReadEvent");
	api2Data.bContinue = TRUE;
	hReadThread = CreateThread( NULL, 0, ReadTagEventThread,(LPVOID)api2Data.hReader, 0, (unsigned long *)0);
	rfid3Sleep( 10000 );
	

	if( RFID_SUCCESS == SetCapValueDWORD( api2Data.hReader,RFID_READCAP_READMODE,RFID_READCAP_READMODE_AUTONOMOUS ) )
	{
		rfid3Sleep(10000);
		api2Data.bContinue = FALSE;
	}
	
	SetCapValueDWORD( api2Data.hReader,RFID_READCAP_READMODE,RFID_READCAP_READMODE_AUTONOMOUS );
	SetCapValueDWORD( api2Data.hReader,RFID_READCAP_READMODE,RFID_READCAP_READMODE_AUTONOMOUS );
	SetCapValueDWORD( api2Data.hReader,RFID_READCAP_READMODE_ONDEMAND,RFID_READCAP_READMODE );
}

// Validating boundary and negative tests for Select Action parameter
//
void ValidateRFID2SelectAction( void )			
{
	UINT8 SelectAction;
	for( SelectAction = 8; SelectAction < 255; SelectAction++ )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:ValidateRFID2SelectAction Value:%d \n <br> Expected:failure due to neg test\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,SelectAction );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID_SUCCESS != SetCapValue( api2Data.hReader, RFID_WRITECAP_G2_SELECT_ACTION, SelectAction ) )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}
	// Capability reset
	//
	SetCapValue( api2Data.hReader, RFID_WRITECAP_G2_SELECT_ACTION, 0x00 );
}

void GetRFID2Stats( void )
{
	DWORD dwStatus;
	RFID_STATS pStats;
	ZeroMemory(&pStats,sizeof(pStats));

	dwStatus = RFID_GetStats( api2Data.hReader, &pStats);
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: GetRFID2Stats \n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if (dwStatus == RFID_SUCCESS) 
	{
		fprintf( api3Data.fpLog,"dwVersion = %d\n", pStats.dwVersion);
		fprintf( api3Data.fpLog,"dwTotalTX = %d\n", pStats.dwTotalTX);
		fprintf( api3Data.fpLog,"dwTotalRX = %d\n", pStats.dwTotalRX);
		fprintf( api3Data.fpLog,"dwPacketsTX = %d\n", pStats.dwPacketsTX);
		fprintf( api3Data.fpLog,"dwPacketsRX = %d\n", pStats.dwPacketsRX);
		fprintf( api3Data.fpLog,"dwIncompleteTX = %d\n", pStats.dwIncompleteTX);
		fprintf( api3Data.fpLog,"dwPacketsFragmented = %d\n", pStats.dwPacketsFragmented);
		fprintf( api3Data.fpLog,"dwPacketsCRCError = %d\n", pStats.dwPacketsCRCError);
		fprintf( api3Data.fpLog,"dwSessionIDError = %d\n", pStats.dwSessionIDError);
		fprintf( api3Data.fpLog,"dwTotalReads = %d\n", pStats.dwTotalReads);
		fprintf( api3Data.fpLog,"dwTotalTagCRCError = %d\n", pStats.dwTotalTagCRCError);
		fprintf( api3Data.fpLog,"dwTotalTagCollisions = %d\n", pStats.dwTotalTagCollisions);
		fprintf( api3Data.fpLog,"dwTimeouts = %d\n", pStats.dwTimeouts);
		fprintf( api3Data.fpLog,"dwTotal_10MSOnTime = %d\n", pStats.dwTotal_10MSOnTime);
		fprintf( api3Data.fpLog,"dwTotalMSRFOnTime = %d\n", pStats.dwTotalMSRFOnTime);
		fprintf( api3Data.fpLog,"dwNoTagErr = %d\n", pStats.dwNoTagErr);
		fprintf( api3Data.fpLog,"dwEraseFailErr = %d\n", pStats.dwEraseFailErr);
		fprintf( api3Data.fpLog,"dwProgFailErr = %d\n", pStats.dwProgFailErr);
		fprintf( api3Data.fpLog,"dwTagLockErr = %d\n", pStats.dwTagLockErr);
		fprintf( api3Data.fpLog,"dwKillFailErr = %d\n", pStats.dwKillFailErr);
		fprintf( api3Data.fpLog,"dwHardwareErr = %d\n", pStats.dwHardwareErr);
		fprintf( api3Data.fpLog,"dwDataSizeErr = %d\n", pStats.dwDataSizeErr);
		fprintf( api3Data.fpLog,"dwReadTime_10MS = %d\n", pStats.dwReadTime_10MS);
		fprintf( api3Data.fpLog,"dwReadAttempts = %d\n", pStats.dwReadAttempts);
		fprintf( api3Data.fpLog,"dwReadSuccess = %d\n", pStats.dwReadSuccess);
		fprintf( api3Data.fpLog,"dwProgramAttempts = %d\n", pStats.dwProgramAttempts);
		fprintf( api3Data.fpLog,"dwProgramSuccess = %d\n", pStats.dwProgramSuccess);
		fprintf( api3Data.fpLog,"dwEraseAttempts = %d\n", pStats.dwEraseAttempts);
		fprintf( api3Data.fpLog,"dwEraseSuccess = %d\n", pStats.dwEraseSuccess);
		fprintf( api3Data.fpLog,"dwLockAttempts = %d\n", pStats.dwLockAttempts);
		fprintf( api3Data.fpLog,"dwLockSuccess = %d\n", pStats.dwLockSuccess);
		fprintf( api3Data.fpLog,"dwKillAttempts = %d\n", pStats.dwKillAttempts);
		fprintf( api3Data.fpLog,"dwKillSuccess = %d\n", pStats.dwKillSuccess);
		fprintf( api3Data.fpLog,"dwLockFailErr = %d\n", pStats.dwLockFailErr);
		for (int i = 0; i < RFID_TAG_TYPE_MAX; i++)
			fprintf( api3Data.fpLog,"dwTagTypeReadTotal[%d] = %d, \n", i, pStats.dwTagTypeReadTotal[i]);
		fprintf( api3Data.fpLog,"dwTagUnderrunErrors = %d\n", pStats.dwTagUnderrunErrors);
		fprintf( api3Data.fpLog,"dwDroppedTagEvents = %d\n\n", pStats.dwDroppedTagEvents);
	}

	if( dwStatus == RFID_SUCCESS )
	{
		SetLogResult(TRUE);
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");
}

void DisplayRFID2Capabilities( void )
{
	RFID_CAPS Caps;
	DWORD dwStatus;
	ZeroMemory(&Caps, sizeof(Caps));
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: DisplayRFID2Capabilities \n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	dwStatus = RFID_GetCaps( api2Data.hReader, &Caps);
	fprintf( api3Data.fpLog,"\nCalling the RFID_GetCaps function - Status :%S",RFID_GetCommandStatusText(api2Data.hReader, dwStatus));	
	fprintf( api3Data.fpLog,"\n...RFID Reader Capabilities...");
	fprintf( api3Data.fpLog,"\n\n API Version : %S",Caps.szAPIVersionString);
	fprintf( api3Data.fpLog,"\n\n Firmware Version : %S",Caps.szFirmwareVersion );
	fprintf( api3Data.fpLog,"\n\n Reader Number  : %d",Caps.ReaderNumber);
	fprintf( api3Data.fpLog,"\n\n Serial Info	: %S",Caps.szSerialInfo);
	fprintf( api3Data.fpLog,"\n\n Antenna	: %d",Caps.Antenna );
	fprintf( api3Data.fpLog,"\n\n Power State : %d",Caps.bPowerState);
	fprintf( api3Data.fpLog,"\n\n Port Open	: %d",Caps.bPortOpen );
	fprintf( api3Data.fpLog,"\n\n Reader Type : %d",Caps.dwReaderType );
	fprintf( api3Data.fpLog,"\n\n Current Port : %d",Caps.dwCurrentPort );
	fprintf( api3Data.fpLog,"\n\n Current Baud Rate : %d\n\n",Caps.dwCurrentBaud );
	if( dwStatus == RFID_SUCCESS )
	{
		SetLogResult(TRUE);
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");	
}
#endif
#endif