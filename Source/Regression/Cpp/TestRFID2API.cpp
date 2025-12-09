#if defined X

#include "rfid2Helper.h"
#include "TestRFID2API.h"
#include "common.h"

extern struct RFID2DATA api2Data;
extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[2];

void initRFID2API( void )
{
	if( api3Data.hReader )
	{
		RFID_Disconnect( api3Data.hReader );
	}
	
	
	api3Data.rfidStatus = RFID_API_SUCCESS;
	wcscpy( api2Data.szIPAddress,api3Data.szIPAddress);
	api2Data.dwPort = api3Data.dwPort = 5084;
	api2Data.bContinue = FALSE;
	IntialiseReader();
	//ConfigureAntenna( api2Data.hReader,0,0 );
	//ConfigureTagSupport( api2Data.hReader,FALSE,FALSE,FALSE,TRUE );
	fprintf( api3Data.fpLog, "\n\n Read tags first and see any tags around the field\n");
	RFID2ReadTags( api2Data.hReader , 4, 1 );
}

void DeinitRFID2API( void )
{
	DWORD dwStatus;

	dwStatus = RFID_CloseReader( api2Data.hReader );
	dwStatus = RFID_Close( &api2Data.hReader );
}

void test_RFID2WriteTags( void )
{
	RFID_SetTraceLevel((RFID_HANDLE32)api2Data.hReader ,TRACE_LEVEL_ALL);
	SetLogText("test_RFID2WriteTags()- ");
	fprintf( api3Data.fpLog,"\n\n----------------test_RFID2WriteTags - Start ------------------\n\n");	
	logCurrentExecution( " test_RFID2WriteTags");
	
	// Delete all select records
	//
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
	api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader );	

	DWORD dwStatus;
	UINT8 wEPCData[32];
	UINT8 wRESData[8]; 
	UINT8 wUSRData[] = {
						0x11,0x11,0x22,0x22,0x33,0x33,0x44,0x44,0x55,0x55,0x66,0x66,0x77,0x77,0x88,0x88,
						0x11,0x11,0x22,0x22,0x33,0x33,0x44,0x44,0x55,0x55,0x66,0x66,0x77,0x77,0x88,0x88,
						0x11,0x11,0x22,0x22,0x33,0x33,0x44,0x44,0x55,0x55,0x66,0x66,0x77,0x77,0x88,0x88,
						0x11,0x11,0x22,0x22,0x33,0x33,0x44,0x44,0x55,0x55,0x66,0x66,0x77,0x77,0x88,0x88
						};

	RFID2ReadTags( api2Data.hReader, 2, 1 );

	//Writing Access Passwords to the tags
	//
	SetCapValue( api2Data.hReader,RFID_WRITECAP_TAGTYPE, RFID_TAG_TYPE_EPC_CLASSG2 );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 2 );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
	
	memset( wEPCData, 0xAB,28);
	dwStatus = RFID_ProgramTags( api2Data.hReader,wEPCData,28,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags to EPC with OFFSET 0 and Data-14 WORDS------------",api3Data.dwTestCaseNo); 
	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 1 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_RESERVED );
	memset( wRESData, 0xDE,8);
	dwStatus = RFID_ProgramTags( api2Data.hReader,wRESData,8,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags RES mem 4 WORDS------------",api3Data.dwTestCaseNo);
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	//
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_USER );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 4 );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 );

	// Wrtie to user from an offset 0 ---->4BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,4,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags Wrtie to user from an offset 0 ---->4BYTES------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 8 );
	// Wrtie to user from an offset 0 ---->8BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,8,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 12 );
	// Wrtie to user from an offset 0 ---->12BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,12,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 16 );
	// Wrtie to user from an offset 0 ---->16BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,16,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 20 );
	// Wrtie to user from an offset 0 ---->20BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,20,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 24 );
	// Wrtie to user from an offset 0 ---->24BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,24,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 28 );
	// Wrtie to user from an offset 0 ---->28BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,28,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 32 );
	// Wrtie to user from an offset 0 ---->32BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,32,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 48 );
	// Wrtie to user from an offset 0 ---->48BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,48,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	// Wrtie to user from an offset 0 ---->64BYTES
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,48,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 3 );
	// Wrtie to user from an offset 0 ---->3BYTES === ODD number of bytes
	dwStatus = RFID_ProgramTags( api2Data.hReader,wUSRData,3,3,3,3 );
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_RFID2WriteTags------------",api3Data.dwTestCaseNo);
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
	RFID2ReadTags( api2Data.hReader, 4, 2 );
	SetLogResult( 2 );logToResultFile();
		
	RFID_SetTraceLevel((RFID_HANDLE32)api2Data.hReader ,TRACE_LEVEL_OFF );
	LogPassFailCount();
	fprintf( api3Data.fpLog,"\n\n----------------test_RFID2WriteTags - END ------------------\n\n");	
}

void test_Session_RFID2API( void )
{
	UINT8 mask[2];
	mask[0] = HIBYTE(MyTags[0].PC);
	mask[1] = LOBYTE(MyTags[0].PC);
	DWORD Status;
	// Delete all select records
	//
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
	Status = RFID_RemoveTagMaskEX( api2Data.hReader );	
	RFID_ResetConfigToFactoryDefaults( (RFID_HANDLE32)api2Data.hReader);

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 1 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 2 );	
	RFID2ReadTags( api2Data.hReader, 4,1);
	//%%%%% Test 1 to 16
	
	for ( int Session= 0; Session < 4; Session++ )//For Sessions S0 to S3
	{		
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
		Status = RFID_RemoveTagMaskEX( api2Data.hReader );
		
		// For all tags set the session for matching tags to ASSERT 
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_ASRT_SL_NOT_DSRT_SL  );
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG );
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

		//%%%%% Test 1,5,9,13 
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, Session );
	
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );//Move Matching to A and non matching to B
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, Session ); //Modify sessions Inv Flag
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
		
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, Session );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Session_RFID2API \n <br> Expected: ONly %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
		
		//%%%%% Test 2,6,10,14
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, Session );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Session_RFID2API \n <br> Expected: Non %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
		

		//%%%%% Test 3,7,11,15
		// Clean up added select records
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
		Status = RFID_RemoveTagMaskEX( api2Data.hReader );	
		api2Data.rfidStatus = RFID_DeletePreFilter( (RFID_HANDLE32)api2Data.hReader,0,0);
		Status = RFID_ResetConfigToFactoryDefaults( ( RFID_HANDLE32) api2Data.hReader );
		
		// Move matching to DSL and non matching to ASL
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_DSRT_SL_NOT_ASRT_SL  );
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG );
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_B_NOT_INV_A  );//Move Matching to B and non matching to A
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, Session ); //Modify sessions Inv Flag
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, Session );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Session_RFID2API \n <br> Expected: Non %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");

		//%%%%% Test 4,8,12,16
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, Session );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Session_RFID2API \n <br> Expected: Only %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}
	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
	Status = RFID_RemoveTagMaskEX( api2Data.hReader );	
	RFID_DeletePreFilter( (RFID_HANDLE32)api2Data.hReader,0,0);
	RFID2ReadTags( api2Data.hReader, 4 , 1 );	
	Status = RFID_ResetConfigToFactoryDefaults( ( RFID_HANDLE32) api2Data.hReader );	
}
void test_SelectRecordIndexAPI2( void )
{
	UINT8 mask[2];
	mask[0] = HIBYTE(MyTags[0].PC);
	mask[1] = LOBYTE(MyTags[0].PC);
	RFID_ResetConfigToFactoryDefaults( (RFID_HANDLE32)api2Data.hReader);
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_SESSION_S3 );
	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 1 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 2 );	
		
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S3_INV );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	RFID2ReadTags( api2Data.hReader, 4,1);
	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 0 );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_SelectRecordIndexAPI2 \n <br> Expected: Test Add and remove select records\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader ) ) == 	RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");
	
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	mask[0] = mask[0] << 1;
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	mask[0] = mask[0] << 2;
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	//mask[0] = mask[0] << 3;
	//SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 3 );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_SelectRecordIndexAPI2 \n <br> Expected: Test Add and remove select records\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader ) ) == 	RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 2 );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_SelectRecordIndexAPI2 \n <br> Expected: Test Add and remove select records\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader ) ) == 	RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");
	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 1 );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_SelectRecordIndexAPI2 \n <br> Expected: Test Add and remove select records\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader ) ) == 	RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 0 );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_SelectRecordIndexAPI2 \n <br> Expected: Test Add and remove select records\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader ) ) == 	RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	mask[0] = mask[0] >> 1;
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	mask[0] = mask[0] >> 2;
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	//mask[0] = mask[0] >> 3;
	//SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	// NEgative test case for Select recode index 
	UINT8 selIndex  = 128;
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, selIndex );
	if( ( api2Data.rfidStatus = RFID_RemoveTagMaskEX( api2Data.hReader ) ) == 	RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_SelectRecordIndexAPI2 \n <br> Expected: RFID_SUCCESS RESULT:%S\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,RFID_GetCommandStatusText(api2Data.hReader,api2Data.rfidStatus) );
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");
}

void test_Taget_RFID2API( void )
{
	UINT8 mask[2];
	mask[0] = HIBYTE(MyTags[0].PC);
	mask[1] = LOBYTE(MyTags[0].PC);
	DWORD Status;
	Status = RFID_ResetConfigToFactoryDefaults( ( RFID_HANDLE32) api2Data.hReader );
	
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S3 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	//RFID2ReadTags( api2Data.hReader, 4, 1 );

	//%%%%% Test 1
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
	Status = RFID_RemoveTagMaskEX( api2Data.hReader );	
	
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_DSRT_SL_NOT_ASRT_SL  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S3_INV );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Taget_RFID2API \n <br> Expected: Only %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	//%%%%% Test 2
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Taget_RFID2API \n <br> Expected: Non %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	//%%%%% Test 3
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
	Status = RFID_RemoveTagMaskEX( api2Data.hReader );
	
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_DSRT_SL_NOT_ASRT_SL  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_B  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S0_INV );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Taget_RFID2API \n <br> Expected: Non %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	
	//%%%%% Test 4
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Taget_RFID2API \n <br> Expected: Only %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");
	
	//Remove all select records
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 );
	Status = RFID_RemoveTagMaskEX( api2Data.hReader );

	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_A_NOT_INV_B  );//Move Matching to A and non matching to B
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S0_INV ); //Modify sessions Inv Flag
}



void test_Multiple_SelectRecords_RFID2API( void )
{
	DWORD Status;
	UINT8 pattern1[2];
	pattern1[0] = HIBYTE(MyTags[0].PC);
	pattern1[1] = LOBYTE(MyTags[0].PC);
	UINT8 pattern2[2];
	pattern2[0] = HIBYTE(MyTags[1].PC); 
	pattern2[1] = LOBYTE(MyTags[1].PC);
	
	// query params
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
	SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
	SetCapValueWORD( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 0 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
		
	{
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 ); // Remove All select Records
		Status = RFID_RemoveTagMaskEX( api2Data.hReader );
		
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_B_NOT_INV_A );
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S0_INV  );
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, pattern1 ); // Adding Select Record

		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_ASRT_SL_NOT_DSRT_SL );
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG  );
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, pattern1 ); // Adding Select Record

	}

	{
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_B_NOT_INV_A );
		SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S1_INV  );
		SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, pattern2 ); // Adding Select Record

	}

	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );	
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S0 );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected: %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );	
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S0 );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
		
	}
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected:Non %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );	
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S1 );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	}

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected:%X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[1].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");
	
	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );	
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S1 );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
	}
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected:Non %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[1].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");


	{
		// Test Multiple Select Records.
		{
			SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX, 255 ); // Remove All select Records
			Status = RFID_RemoveTagMaskEX( api2Data.hReader );
		
			SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_B_NOT_INV_A );
			SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S2_INV  );
			SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, pattern1 ); // Adding Select Record
		}

		{
			SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_ASRT_SL_NOT_DSRT_SL );
			SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG  );
			SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, pattern1 ); // Adding Select Record
		}
		{
			SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_INV_B );
			SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_S2_INV  );
			SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, pattern2 ); // Adding Select Record
		}
	}

	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );	
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S2 );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
	}
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected:Only %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");


	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );	
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S2 );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	}

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected:Only %X Tags shoud be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[1].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );	
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S2 );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	}
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:test_Multiple_SelectRecords_RFID2API \n <br> Expected:Other Than %X, %X Tags shoud be Read \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC ,MyTags[1].PC );
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RFID2API-FUN");

}

void test_SL_RFID2API( void )
{
	UINT8 mask[2];
	mask[0] = HIBYTE( MyTags[0].PC );
	mask[1] = LOBYTE( MyTags[0].PC );
	RFID_ResetConfigToFactoryDefaults( (RFID_HANDLE32)api2Data.hReader);

	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_ACTION, RFID_GEN2_SELECT_DSRT_SL_NOT_ASRT_SL  );
	SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_SELECT_TARGET, RFID_GEN2_SELECT_MOD_SL_FLAG );
	SetTAG_MASK_EX( api2Data.hReader,16, 16, 01, 0, mask ); // Adding Select Record
	
	for( UINT8 Session = 0; Session < 4;Session++ )
	{
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, Session );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_START_Q, 4 );
		SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:SELECT TAGS Whose Select bit DisAsserted in Session:%d \n <br> Expected:PC %X Tags Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Session,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:SELECT TAGS Whose Select bit Asserted in Session:%d \n <br> Expected:Non-%X Tags Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Session,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:SELECT TAGS Whose INV FLAG is A and SL Assertedin Session:%d \n <br> Expected:%X Tags Should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,Session,MyTags[0].PC );
		fprintf( api3Data.fpCurLog,"<br>");
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");

		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:SELECT TAGS Whose INV FLAG is B and SL not Set\n <br> Expected:No tags\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );
		if( RFID2ReadTags( api2Data.hReader,4,1) == 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");

		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:SELECT TAGS Whose INV FLAG is B and SL Set\n <br> Expected:Non PC -%X tags\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,MyTags[0].PC);
		fprintf( api3Data.fpCurLog,"<br>");
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_SET );
		SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_B );
		if( RFID2ReadTags( api2Data.hReader,4,1) > 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"RFID2API-FUN");
	}

	/*fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_SL_RFID2API------------",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpLog,"\n\n###SELECT TAGS Whose Session S1 inv A and SL not Set**** ");
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_TARGET, RFID_GEN2_INVENTORIED_FLAG_A );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SEL, RFID_GEN2_SEL_SL_NOT_SET );
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S1 );
	RFID2ReadTags( api2Data.hReader,4,1 );
	SetLogResult( 2 );logToResultFile();
	
	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_SL_RFID2API------------",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpLog,"\n\n###SELECT TAGS Whose Session S2**** Inv A and SL not Set ");
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S2 );
	RFID2ReadTags( api2Data.hReader,4,1 );
	SetLogResult( 2 );logToResultFile();

	fprintf( api3Data.fpLog,"\n---------TestCaseNo:%d,test_SL_RFID2API------------",api3Data.dwTestCaseNo);
	fprintf(api3Data.fpLog,"\n\n###SELECT TAGS Whose Session S3 Inv A and SL not Set**** ");
	SetCapValue( api2Data.hReader,RFID_READCAP_G2_SESSION, RFID_GEN2_SESSION_S3 );
	RFID2ReadTags( api2Data.hReader,4,1 );
	SetLogResult( 2 );logToResultFile();*/
	
}

void testRfidAPI2GPIO( void )
{
	HANDLE hNotifyEvent  = CreateEvent(NULL, TRUE, FALSE, L"NotifyChangeEvent");
	DWORD status = RFID_SUCCESS;
	byte inputPINvalue = 0;
	byte outputPINvalue = 0; 
	
	//SetLogText( "RFID_GetGPIOValue Test");

	// gets the current status of input and output port
	if( RFID_SUCCESS == RFID_GetGPIOValue( api2Data.hReader, &inputPINvalue, &outputPINvalue) )
	{
		wprintf(L"\n  RFID_GetGPIOValue :  PASS");
		//SetLogResult( TRUE );
	}

	//logToResultFile( );

	// Port 4, 5 & 6 to turn on
	//byte outputPINs = 0x38; // 0011 1000 (interested PINs)
	//byte outputPINvalue = 0x38; //0011 1000 (PIN value)
	byte outputPINs = 0xC0; // 1100 0000 (interested PINs)
	outputPINvalue = 0xC0; //1100 0000 (PIN value)
	if( RFID_SUCCESS == RFID_SetGPOValue( api2Data.hReader, outputPINs, outputPINvalue) )
	{
		wprintf(L"\n  RFID_GetGPIOValue :  PASS");
		//SetLogResult( TRUE );
	}
	//logToResultFile( );

	rfid3Sleep(3000);

	// Port 4, 5  to turn off
	outputPINs = 0x80;
	outputPINvalue = 0x00;
	if( RFID_SUCCESS == RFID_SetGPOValue( api2Data.hReader, outputPINs, outputPINvalue) )
	{
		wprintf(L"\n  RFID_GetGPIOValue :  PASS");
		//SetLogResult( TRUE );
	}
	//logToResultFile( );

	rfid3Sleep(3000);

	
	// Set GPI notification 
	BYTE bInterval=0xff;
	byte inputPINMask = 0xff;
	// last parameter interval is not used in RFIDAPI3
	status = RFID_SetGPIDetection( api2Data.hReader, hNotifyEvent, inputPINMask, bInterval);
	if( RFID_SUCCESS == status )
	{
		wprintf(L"\n  RFID_GetGPIOValue :  PASS");
		//SetLogResult( TRUE );
	}

	//logToResultFile( );

	DWORD dwEvent = WaitForSingleObject( hNotifyEvent, INFINITE );

	if( dwEvent == WAIT_OBJECT_0 )
	{
		// Get GPI Event value on signalling the event
		byte changedPINMask = 0x00;
		byte currentPINValue = 0x00;

		status = RFID_GetGPIEventValue( api2Data.hReader, &changedPINMask, &currentPINValue, false);
		
	}	
}

void TestRFID2API( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestRFID2API");
	logCurrentExecution( "TestRFID2API");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 370;
				
	DeInitRFID();	// Cleanup if there is any pending resources.
				
	if( api3Data.hReader )
	{
		RFID_Disconnect( api3Data.hReader );
		api3Data.hReader = (RFID_HANDLE32)INVALID_HANDLE_VALUE;
	}

	initRFID2API( );
	
	DisplayRFID2Capabilities( );
	GetRFID2Stats();

	//testRfidAPI2GPIO( );
	test_SL_RFID2API( );
	test_Session_RFID2API();
	
	test_SelectRecordIndexAPI2();
	test_Taget_RFID2API( );
	test_Multiple_SelectRecords_RFID2API( );
	ValidateRFID2LockAction( );
	ValidateRFID2AccessPassword( );
	ValidateRFID2KillPassword( );
	ValidateRFID2SelectAction( );
	test_RFID2MemoryBank();
	test_RFID2MemoryBankOFFSet();
	//testRFID2LockMask( );
	//test_RFID2WriteTags( );
	//////test_RFID2ThreadSafety( );
	ValidateRFID2ReadMode();
	testRFID2Capabilities();
	DeinitRFID2API( );

	if( api3Data.hReader == (RFID_HANDLE32)INVALID_HANDLE_VALUE )
	{
		RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,0 );
	}
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
#endif