#include "Common.h"
#include "APITest.h"
#include "testNegative.h"
#include <stdio.h>

// global data for API
extern struct RFID3DATA api3Data;
 
void testRecommissionAPI()
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	UINT8 tagID[] = {0x12,0x33,0x12,0x33,0x12,0x33,0x12,0x33,0x12,0x33,0x12,0x33};
	logCurrentExecution("ReCommission API");
	SetLogText( "ReCommission API");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_PERMALOCK \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 7;
	RECOMMISSION_ACCESS_PARAMS rComParams = { 0xAAAABBBB, RECOMMISSION_DISABLE_PERMALOCK,0 };
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"CAPI-RECOM");
	
	// test with RECOMMISSION_DISABLE_USER_MEMORY
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_USER_MEMORY \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	rComParams.opCode = RECOMMISSION_DISABLE_USER_MEMORY;
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );	
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"RECOM-FUN");
	
	// test with RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	rComParams.opCode = RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION;
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );	
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"CAPI-RECOM");

	// test with RECOMMISSION_DISABLE_PASSWORD
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_PASSWORD \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	rComParams.opCode = RECOMMISSION_DISABLE_PASSWORD;
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );	
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"CAPI-RECOM");

	// test with RECOMMISSION_DISABLE_PERMALOCK_PASSWORD
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_PERMALOCK_PASSWORD \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	rComParams.opCode = RECOMMISSION_DISABLE_PERMALOCK_PASSWORD;
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );	
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"CAPI-RECOM");

	// test with RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	rComParams.opCode = RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD;
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );	
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"CAPI-RECOM");

	// test with RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RECOM-%d></a> CAPI-RECOM-%d:\n\n<br>Description: Recommission API-RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION \n <br> Expected OutPut :UnSupported\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	rComParams.opCode = RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION;
	api3Data.rfidStatus = RFID_Recommission( api3Data.hReader,tagID,12,&rComParams,NULL,NULL,0 );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n %S <br>\n", RFID_GetErrorDescription(api3Data.rfidStatus) );	
		SetLogResult(TRUE);
		wprintf(L"\n Status: %ls",RFID_GetErrorDescription(api3Data.rfidStatus) );
	}

	logToResultFile(L"CAPI-RECOM");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
