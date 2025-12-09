#include "Common.h"
extern struct RFID3DATA api3Data;

void TestErrorInfo( RFID_HANDLE32 hReader )
{
	UINT16 errorArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 200, 201,202, 300, 301, 400, 401, 402, 403, 404, 405,
		406, 407, 420, 421, 422, 423, 500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523,
		524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 601, 602, 603, 604, 605, 700, 701, 702 };


	SetLogText( "RFID_GetErrorDescription()- FUN" );
	
	for (int i = 0; i < (int)(sizeof(errorArray) / sizeof(errorArray[0])); i++)
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test with correct Error Code: %d\n <br> Expected OutPut : Show Error Description for %s\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,errorArray[i],ERRORSTATUS[i] );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = ( RFID_STATUS )errorArray[i];
		if( RFID_GetErrorDescription( (RFID_STATUS)errorArray[i] ) != NULL )
		{
			SetLogResult( TRUE);
		}

		wprintf( L"\n Error Description  for ERRORCODE : %d \t %ls \n ",errorArray[i], RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-CONFIG");
	}
}

void TestErrorInfoA(RFID_HANDLE32 hReader)
{
	UINT16 errorArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 200, 201,202, 300, 301, 400, 401, 402, 403, 404, 405,
		406, 407, 420, 421, 422, 423, 500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523,
		524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 601, 602, 603, 604, 605, 700, 701, 702 };

	ACCESS_OPERATION_STATUS accessOperationStatus[] = { (ACCESS_OPERATION_STATUS)0, (ACCESS_OPERATION_STATUS)1, (ACCESS_OPERATION_STATUS)2, (ACCESS_OPERATION_STATUS)3, (ACCESS_OPERATION_STATUS)4, (ACCESS_OPERATION_STATUS)5, (ACCESS_OPERATION_STATUS)6, (ACCESS_OPERATION_STATUS)7 };


	SetLogText("RFID_GetErrorDescriptionA()- FUN");

	for (int i = 0; i < (int)(sizeof(errorArray) / sizeof(errorArray[0])); i++)
	{
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test with correct Error Code: %d\n <br> Expected OutPut : Show Error Description for %s\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, errorArray[i], ERRORSTATUS[i]);
		fprintf(api3Data.fpCurLog, "<br>");
		api3Data.rfidStatus = (RFID_STATUS)errorArray[i];
		if (RFID_GetErrorDescriptionA((RFID_STATUS)errorArray[i]) != NULL)
		{
			SetLogResult(TRUE);
		}
		printf("\n Error Description  for ERRORCODE : %d \t %s \n ", errorArray[i], RFID_GetErrorDescriptionA(api3Data.rfidStatus));
		fprintf(api3Data.fpCurLog, "\n Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf(api3Data.fpCurLog, "<br>");
		logToResultFile(L"CAPI-CONFIG");
	}
}

	
void TestGetLastErrorInfo( RFID_HANDLE32 hReader )
{
	ERROR_INFO	errorInfo;
	SetLogText( "RFID_GetLastErrorInfo()- FUN" );
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test With valid parameters\n <br> Expected OutPut : Show Error Description\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);	
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetLastErrorInfo( hReader,&errorInfo ) ) == RFID_API_SUCCESS  )
	{	
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test RFID_GetLastErrorInfo with Invalid Handle Value\n <br> Expected OutPut : Invalid Handle Value Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );	
	fprintf( api3Data.fpCurLog,"<br>");
	if(  ( api3Data.rfidStatus = RFID_GetLastErrorInfo( NULL,&errorInfo )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test RFID_GetLastErrorInfo with with NULL Buffer\n <br> Expected OutPut : Invalid Handle Value Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );	
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetLastErrorInfo( hReader,NULL )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
}

void TestSetTraceLevel( RFID_HANDLE32 hReader )
{
	DWORD dwTraceLevels = 7;
	TRACE_LEVEL traceArray[8] = {TRACE_LEVEL_OFF,TRACE_LEVEL_FATAL,TRACE_LEVEL_ERROR,TRACE_LEVEL_WARNING,TRACE_LEVEL_INFO,TRACE_LEVEL_VERBOSE,TRACE_LEVEL_ALL};
	
	SetLogText( "RFID_SetTraceLevel()- FUN" );
	while( dwTraceLevels )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test for TraceLevel Value: %d\n <br> Expected OutPut : API SUccess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,traceArray[ dwTraceLevels ] );	
		fprintf( api3Data.fpCurLog,"<br>");	
		if(( api3Data.rfidStatus = RFID_SetTraceLevel( hReader,traceArray[dwTraceLevels])) == RFID_API_SUCCESS  )
		{	
			SetLogResult(TRUE );
		}
		dwTraceLevels--;
		wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-CONFIG");
	}
		
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Test RFID_SetTraceLevel with Invalid handle value Handle \n <br> Expected OutPut : API SUccess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );	
	fprintf( api3Data.fpCurLog,"<br>");	
	if( ( api3Data.rfidStatus = RFID_SetTraceLevel( (RFID_HANDLE32)INVALID_HANDLE_VALUE,TRACE_LEVEL_ALL) ) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
}

void TestAccessAPIWithNegValues( RFID_HANDLE32 hReader )
{
	ACCESS_FILTER aFilter;
	memset(&aFilter,0,sizeof(ACCESS_FILTER));
	ANTENNA_INFO aInfo;
	memset(&aInfo,0,sizeof(ANTENNA_INFO));
	aInfo.pAntennaList = NULL;
	aInfo.length=0;
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));

	tInfo.lpTagEventReportInfo = NULL;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );;
	READ_ACCESS_PARAMS rParams;
	WRITE_ACCESS_PARAMS wParams;
	
	SetLogText("TestAccessAPIWithNegValues()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_PerformInventory():Invalid HANDLE\n <br> Expected OutPut:Should throw invalid handle value Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_PerformInventory( (RFID_HANDLE32)INVALID_HANDLE_VALUE,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_PerformInventory():Test with Null Buffer\n <br> Expected OutPut:Should Show API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,&aInfo,&tInfo,0)) == RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_StopInventory() with Invalid HAndle Value\n <br> Expected OutPut:Invalid handle value Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_StopInventory( (RFID_HANDLE32)INVALID_HANDLE_VALUE )) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_GetReadTag():Test with Invalid HANDLE\n <br> Expected OutPut:Invalid handle value Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetReadTag( (RFID_HANDLE32)INVALID_HANDLE_VALUE,NULL )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_GetReadTag():Test with Invalid buffer \n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader,NULL ) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid HANDLE  \n <br> Expected OutPut:Invalid Handle Value\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( (RFID_HANDLE32)INVALID_HANDLE_VALUE,NULL,0,NULL,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid values\n <br> Expected OutPut:Parameter Errors\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,0,NULL,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid lenngth for tag id\n <br> Expected OutPut:Tag Not found or Error return\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( api3Data.hReader,NULL,65535,NULL,NULL,NULL,NULL,0) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid Values\n <br> Expected OutPut:Tag Not found or Error return\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,12,NULL,NULL,NULL,NULL,0) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid values\n <br> Expected OutPut:Tag Not found or Error return\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,12,&rParams,NULL,NULL,NULL,0) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Iinvalid values\n <br> Expected OutPut:Tag Not found or Error return\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,12,&rParams,&aFilter,NULL,NULL,0) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid lenngth for tag id\n <br> Expected OutPut:Tag Not found or Error return\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Read( api3Data.hReader,pTagData->pTagID,12,&rParams,&aFilter,&aInfo,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Write():Test with NULL Write Data\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");	
	if( ( api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Write():Test with NULL Write Data\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	wParams.pWriteData = NULL;
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Write():Test with NULL Write Data\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Write( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Lock():Test with Invalid Lock Parameters\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Lock( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_Lock():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Lock( (RFID_HANDLE32 )INVALID_HANDLE_VALUE,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_BlockWrite():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_BlockWrite( (RFID_HANDLE32)INVALID_HANDLE_VALUE,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_BlockWrite():Test With Null value Write Parameter\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_BlockWrite( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_BlockWrite():Test With Null value Write access Parameter\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_BlockWrite( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&wParams,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_BlockErase():Test with Invalid Handle\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_BlockErase( (RFID_HANDLE32)INVALID_HANDLE_VALUE,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_BlockWrite():Test NULL value for Block write Parameters\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_BlockWrite( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_BlockErase():Test NULL value for Block eRase Parameter\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_BlockErase( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,NULL,NULL,NULL,0)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_InitializeAccessSequence():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_InitializeAccessSequence( (RFID_HANDLE32)INVALID_HANDLE_VALUE ) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_AddOperationToAccessSequence():Test with InvalidHandle\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_AddOperationToAccessSequence( (RFID_HANDLE32)INVALID_HANDLE_VALUE,NULL,NULL )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_AddOperationToAccessSequence():NULL for OpCode Params\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader,NULL,NULL ) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	OP_CODE_PARAMS opParams;
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_AddOperationToAccessSequence():Null for OpCode Index\n <br> Expected OutPut:Parameter Errorr\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_AddOperationToAccessSequence( api3Data.hReader,&opParams,NULL )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_DeleteOperationFromAccessSequence():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( (RFID_HANDLE32)INVALID_HANDLE_VALUE,65535)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid Op Index\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence( api3Data.hReader,65535)) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_Read():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_DeinitializeAccessSequence( (RFID_HANDLE32)INVALID_HANDLE_VALUE )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_StopAccessSequence():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_StopAccessSequence( (RFID_HANDLE32)INVALID_HANDLE_VALUE ) ) != RFID_API_SUCCESS)
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:RFID_PerformAccessSequence():Test with Invalid Handle Value\n <br> Expected OutPut:Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_PerformAccessSequence( (RFID_HANDLE32)INVALID_HANDLE_VALUE ,NULL,NULL,NULL,0 )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-NEG-%d></a> TestCaseNo:CAPI-NEG-%d:\n\n<br>Description:RFID_PerformAccessSequence():Null Values for Parameters\n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_PerformAccessSequence( api3Data.hReader,NULL,NULL,NULL,0 )) != RFID_API_SUCCESS )
	{	
		SetLogResult(TRUE );
	}	
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
}
