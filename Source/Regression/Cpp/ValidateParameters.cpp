#include "Common.h"
#include "Config.h"

extern struct RFID3DATA api3Data;

void TestPhysicalAntennaProperties( RFID_HANDLE32 hReader )
{
	int antennaIndexArray[] = { -9999,10,255,9999,65535 };
	UINT32 gain;
	SetLogText("RFID_GetPhysicalAntennaProperties()- FUN ");
	BOOLEAN antennaStatus;
	int antennaId;
	
	for( antennaId = 1;antennaId <= api3Data.rCaps.numAntennas ;antennaId ++ )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:RFID_GetPhysicalAntennaProperties():Test With valid values\n <br> Expected OutPut:APISUCCESS.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_GetPhysicalAntennaProperties( hReader, antennaId, &antennaStatus,&gain);
		if(RFID_API_SUCCESS == api3Data.rfidStatus)
		{
			SetLogResult( TRUE );
		}
		fprintf( api3Data.fpCurLog, "\n Result is: rfidStatus:%S antennaID:%d Status:%S gain:%d \n ",RFID_GetErrorDescription( api3Data.rfidStatus ), antennaId,antennaStatus?L"Connected":L"Not Connected",gain );
		wprintf(L"\n Result is: rfidStatus:%ls antennaID:%d Status:%ls gain:%d \n ",RFID_GetErrorDescription( api3Data.rfidStatus ), antennaId,antennaStatus?L"Connected":L"Not Connected",gain );
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-CONFIG");
	}
	
	for( int index =0 ; index <= 4; index ++ )
	{
		antennaId = antennaIndexArray[index];
		api3Data.rfidStatus = RFID_GetPhysicalAntennaProperties( hReader, antennaId, &antennaStatus,&gain);
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:RFID_GetPhysicalAntennaProperties() with Invalid antenna ID Value\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID_API_PARAM_OUT_OF_RANGE == api3Data.rfidStatus )
		{
			SetLogResult(TRUE );
		}

		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-CONFIG");
	}
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:RFID_GetPhysicalAntennaProperties() with Invalid Handle Value\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_GetPhysicalAntennaProperties( (RFID_HANDLE32)INVALID_HANDLE_VALUE, antennaId, &antennaStatus,&gain);
	logToFile( " RFID_GetPhysicalAntennaProperties() with Invalid Handle Value");
	if( RFID_API_INVALID_HANDLE == api3Data.rfidStatus)
	{
		SetLogResult(TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
}

void TestValidateParameters( RFID_HANDLE32 hReader )
{
	READER_CAPS rCaps;
	memset(&rCaps,0,sizeof(rCaps));
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetReaderCaps with Valid Params\n <br> Expected OutPut:API Success.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if(  ( api3Data.rfidStatus =  RFID_GetReaderCaps( hReader,&rCaps)) == RFID_API_SUCCESS  )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetReaderCaps with inValid Handle\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetReaderCaps( (RFID_HANDLE32)INVALID_HANDLE_VALUE ,&rCaps)) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetReaderCaps NULL value for READERCAPS\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetReaderCaps( hReader,NULL)) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	SetLogText("RFID_SetAntennaConfig()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a>CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetAntennaConfig with invalid handle value\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_SetAntennaConfig( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numAntennas + 255,NULL,NULL,NULL) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetAntennaConfig with invalid range for antenna\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_SetAntennaConfig( hReader, api3Data.rCaps.numAntennas + 255,NULL,NULL,NULL)) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	SetLogText("RFID_SetGPOState()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetGPOState with invalid GPO PIN\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_SetGPOState( hReader, api3Data.rCaps.numGPOs+2,TRUE) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetGPOState with invalid HANDLE\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_SetGPOState( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numGPOs+2,TRUE) ) != RFID_API_SUCCESS)
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	SetLogText("RFID_GetGPOState()- NEG ");
	BOOLEAN bVal;
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetGPOState with invalid range for GPO port\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetGPOState( hReader, api3Data.rCaps.numGPOs+2,&bVal) ) != RFID_API_SUCCESS)
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetGPOState with invalid handle value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetGPOState( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numGPOs,&bVal) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetGPOState with NULL value for Status Parameter\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetGPOState( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numGPOs,NULL ) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	GPI_PORT_STATE gpiState;
	SetLogText("RFID_GetGPIState()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetGPIState with invalid handle value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetGPIState( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numGPOs,&bVal,&gpiState) ) != RFID_API_SUCCESS)
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	// Doing the below making crash so commenting it .UnComment when it gets fixed - PIC 50046.
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetGPIState with NULL value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetGPIState( hReader, api3Data.rCaps.numGPOs,NULL,NULL ) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	SetLogText("RFID_EnableGPIPort()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a>CAPI-NEG-%d:\n\n<br>Description:Test RFID_EnableGPIPort with invalid handle value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_EnableGPIPort( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numGPIs -1 ,TRUE)) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_EnableGPIPort with invalid GPI portNumber value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_EnableGPIPort(hReader, api3Data.rCaps.numGPIs+10 ,TRUE)) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_EnableGPIPort with NULL value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_EnableGPIPort( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numGPOs,NULL ) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf(L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	SINGULATION_CONTROL singControl;
	SetLogText("RFID_GetSingulationControl()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetSingulationControl with invalid handle value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_GetSingulationControl( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numAntennas -1 ,&singControl) ) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a>CAPI-NEG-%d:\n\n<br>Description:Test RFID_GetSingulationControl with null value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetSingulationControl( hReader, api3Data.rCaps.numAntennas -1 ,NULL )) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetSingulationControl with invalid handle value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	singControl.session = SESSION_S0;
	if( ( api3Data.rfidStatus = RFID_SetSingulationControl( (RFID_HANDLE32)INVALID_HANDLE_VALUE, api3Data.rCaps.numAntennas -1 ,&singControl)) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetSingulationControl with null value \n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_SetSingulationControl( hReader, api3Data.rCaps.numAntennas -1 ,NULL )) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	SetLogText("RFID_DeallocateTag()- NEG ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_DeallocateTag with null value\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_DeallocateTag( hReader, NULL )) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_DeallocateTag with (RFID_HANDLE32)INVALID_HANDLE_VALUE value \n <br> Expected OutPut:Parameter Error..\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_DeallocateTag( (RFID_HANDLE32)INVALID_HANDLE_VALUE, NULL )) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	TAG_STORAGE_SETTINGS tgSettings = { 50000,4096,4096 };
	SetLogText("RFID_SetTagStorageSettings()- FUN ");
	for( int iteration = 0; iteration <= 2; iteration++ )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetTagStorageSettings  { 50000,4096,4096 }\n<br> Expected OutPut:API Success.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus =  RFID_SetTagStorageSettings( hReader, &tgSettings )) == RFID_API_SUCCESS )
		{		
			SetLogResult( TRUE );
		}
		wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-NEG");
	}
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetTagStorageSettings with MAX  Values\n<br> Expected OutPut:API Success.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	tgSettings.maxMemoryBankByteCount = tgSettings.maxTagCount = tgSettings.maxTagIDByteCount = 65536;
	if( ( api3Data.rfidStatus =  RFID_SetTagStorageSettings( hReader, &tgSettings )) == RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetTagStorageSettings with MIN  Values =0\n<br> Expected OutPut:API Success.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	tgSettings.maxMemoryBankByteCount = tgSettings.maxTagCount = tgSettings.maxTagIDByteCount = 0;
	if( ( api3Data.rfidStatus =  RFID_SetTagStorageSettings( hReader, &tgSettings )) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test RFID_SetTagStorageSettings with NULL = 0\n<br> Expected OutPut:API Success.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	tgSettings.maxMemoryBankByteCount = tgSettings.maxTagCount = tgSettings.maxTagIDByteCount = 1;
	if( ( api3Data.rfidStatus =  RFID_SetTagStorageSettings( hReader, NULL )) != RFID_API_SUCCESS )
	{		
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-NEG");

	if( api3Data.readerType == MC )
	{
		// duty cycle api RFID_GetDutyCycle
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test Duty cycle with invalid handle value = 0\n<br> Expected OutPut: invalid handle value.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		UINT16 index;
		if( ( api3Data.rfidStatus =  RFID_GetDutyCycle( (RFID_HANDLE32)INVALID_HANDLE_VALUE,&index, NULL )) != RFID_API_SUCCESS )
		{		
			SetLogResult( TRUE );
		}
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-NEG");

		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a>CAPI-NEG-%d:\n\n<br>Description:Test Duty cycle with invalid handle value = 0\n<br> Expected OutPut: invalid handle value.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus =  RFID_GetDutyCycle( hReader,NULL, NULL )) != RFID_API_SUCCESS )
		{		
			SetLogResult( TRUE );
		}
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-NEG");
		// duty cycle api
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-NEG-%d></a> CAPI-NEG-%d:\n\n<br>Description:Test Duty cycle with invalid handle value = 0\n<br> Expected OutPut: invalid handle value.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus =  RFID_SetDutyCycle( (RFID_HANDLE32)INVALID_HANDLE_VALUE,index, NULL )) != RFID_API_SUCCESS )
		{		
			SetLogResult( TRUE );
		}
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-NEG");
	}
}

void TestResetConfigToDefaults( RFID_HANDLE32 hReader )
{	
	BOOLEAN bState;
	SetLogText("RFID_ResetConfigToFactoryDefaults()- FUN ");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Reset Config to defaults\n <br> Expected OutPut : API result should Show Success and Default the Config\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_ResetConfigToFactoryDefaults( hReader ) ) ==  RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
	if( api3Data.readerType == MC )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Reset Config with Radio OFF\n <br> Expected OutPut: Radio should be Turned ON\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetRadioPowerState( hReader,FALSE );
		api3Data.rfidStatus =  RFID_ResetConfigToFactoryDefaults( hReader );
		api3Data.rfidStatus = RFID_GetRadioPowerState( hReader, &bState );

		if( bState == TRUE )
		{
			SetLogResult( TRUE );
		}
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ", bState?L"Reset Config to Default turns on Radio:PASS":L"Reset Config to Default Does not turn on Radio:FAIL");
		wprintf( L"\n Result is: %ls \n ", bState?L"Reset Config to Default turns on Radio:PASS":L"Reset Config to Default Does not turn on Radio:FAIL");
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-CONFIG");
	}

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description:Reset Config with Invalid Hanlde Value\n <br> Expected OutPut: API should return Invalid Handle Value Param Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_ResetConfigToFactoryDefaults( (RFID_HANDLE32)INVALID_HANDLE_VALUE ) ) !=  RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is :%S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
}

