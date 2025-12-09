// fx7500RMAPI.cpp : Test Application Install, Radio idle Mode time out, llrpSecureMode Config API
//
#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;	// RFID3 Data

void TestRadioControl( )
{
	api3Data.dwTestCaseNo = 01;
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.radioPowerControlSupported )
	{
		api3Data.dwSkipCount = 4;
		LogPassFailCount();
		return;
	}

	api3Data.itemTestCount = 4;

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestRadioControl");
	logCurrentExecution( "TestRadioControl");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Set Radio -- TRUE<br> Expected OutPut: Radio ON\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( (api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader,TRUE)) == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RADIO-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Get Radio State <br> Expected OutPut: Radio ON\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	BOOLEAN bRadioState;
	if( (api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader,&bRadioState)) == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	wprintf(L"\n Get Radio status:%ls",bRadioState?L"ON":L"OFF");
	fprintf( api3Data.fpCurLog, "\n Result is: %S  Radio State:%s\n ",RFID_GetErrorDescription( api3Data.rfidStatus ),bRadioState?"ON":"OFF");
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RADIO-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Set Radio -- FALSE<br> Expected OutPut: Radio OFF\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( (api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader,FALSE)) == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n <br> ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	logToResultFile(L"RADIO-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Inventory when Radio is OFF<br> Expected OutPut: Tags should be Read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");

	// Call perform Inventory and check if Radio will be turned ON and inventory happens
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,0,0,0,0);
	rfid3Sleep(6000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	UINT32 tagCount=0;
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	while(RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
	{
		printTagDataWithResults( pTagData);
		tagCount++;
	}
	if( tagCount > 0 )
	{
		SetLogResult( TRUE);
	}

	RFID_DeallocateTag(api3Data.hReader,pTagData);
	fprintf( api3Data.fpCurLog, "\n Number of Tags Read: %d \n <br> ",tagCount);
	logToResultFile(L"RADIO-FUN");
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));
}

void TestIdleModeTimeOut( )
{
	api3Data.dwTestCaseNo = 01;

	// Supported only in FX7500 Currently.and in API version 5_5
	//
	if( api3Data.apiVersion != RFID_API3_5_5 )//if( wcscmp(api3Data.rCaps.modelName,L"75004") == 0 )
	{
		api3Data.dwSkipCount = 06;
		LogPassFailCount();
		return;
	}

	api3Data.itemTestCount = 06;

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestIdleModeTimeOut");
	logCurrentExecution( "TestIdleModeTimeOut");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Get Radio idle time out -- 10Seconds<br> Expected OutPut: Currently Set Radio Idle Mode Time Out is \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	INT32 timeout =0;
	if( (api3Data.rfidStatus =  RFID_GetRadioIdleTimeout( api3Data.hReaderMgmt,&timeout)) == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf(api3Data.fpCurLog,"<br> Currently Set Idle Mode Time is : %d",timeout);
	wprintf(L"\n Currently Set idle time out  %ld",timeout);
	fprintf( api3Data.fpCurLog, "<br> Result is: %S \n <br>",RFID_GetErrorDescription( api3Data.rfidStatus ));
	logToResultFile(L"RADIO-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Set Radio idle time out -- 15 Seconds<br> Expected OutPut: Radio Idle Mode Time to be Set to 15 Seconds\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( (api3Data.rfidStatus = RFID_TurnOffRadioWhenIdle(  api3Data.hReaderMgmt,15 )) == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RADIO-FUN");
	rfid3Sleep(16000);	//  Wait for 15 Seconds to Radio turn OFF

	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Get Radio State <br> Expected OutPut: Radio OFF\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	BOOLEAN bRadioState;
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader,&bRadioState);
	if( (api3Data.rfidStatus == RFID_API_SUCCESS ) && !bRadioState )
	{
		SetLogResult(TRUE);
	}
	wprintf(L"\n Get Radio status:%ls",bRadioState?L"ON":L"OFF");
	fprintf( api3Data.fpCurLog, "\n Result is: %S  Radio State:%s\n ",RFID_GetErrorDescription( api3Data.rfidStatus ),bRadioState?"ON":"OFF");
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RADIO-FUN");

	// Do the inventory and verify if tags are Read, Since IdleMode turn off should turn on Radio if inventory is started
	// Call perform Inventory and check if Radio will be turned ON and inventory happens
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Get Radio State <br> Expected OutPut: Radio OFF\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,0,0,0,0);
	rfid3Sleep(4000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader,&bRadioState);
	if( (api3Data.rfidStatus == RFID_API_SUCCESS ) && bRadioState )
	{
		SetLogResult(TRUE);
	}
	wprintf(L"\n Get Radio status:%ls",bRadioState?L"ON":L"OFF");
	fprintf( api3Data.fpCurLog, "\n Result is: %S  Radio State:%s\n ",RFID_GetErrorDescription( api3Data.rfidStatus ),bRadioState?"ON":"OFF");
	logToResultFile(L"RADIO-FUN");

	// Set Idle Mode time to Negative Values, Which should not be allowed and should throw Parameter Error.
	//
	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Set Radio idle time out --> -10<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( (api3Data.rfidStatus = RFID_TurnOffRadioWhenIdle(  api3Data.hReaderMgmt,-10 )) != RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RADIO-FUN");
	
	// Set Radio time out to ZERO
	fprintf(api3Data.fpCurLog,"\n\n<a name=RADIO-FUN-%d></a> RADIO-FUN-%d:\n\n<br>Description:Set Radio idle time out --> 0<br> Expected OutPut: Setting Idle Time out to 0 \r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( (api3Data.rfidStatus = RFID_TurnOffRadioWhenIdle(  api3Data.hReaderMgmt,0 )) == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RADIO-FUN");

	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));
}

void TestllrpSecureConfig( )
{
	api3Data.dwTestCaseNo = 01;
	LLRP_CONNECTION_CONFIG llrpConnConfig,gllrpConnConfig;
	LLRP_CONNECTION_CONFIGA gllrpConnConfigA;

	// Supported only in FX7500 Currently.and in API version 5_5
	//
	if( api3Data.apiVersion != RFID_API3_5_5 )
	{
		api3Data.dwSkipCount = 06;
		LogPassFailCount();
		return;
	}

	api3Data.itemTestCount = 06;

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("llrpSecureConConfig");
	logCurrentExecution( "llrpSecureConConfig");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Get llrpConnection Config <br> Expected OutPut: llrpConnection Config\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	
	// get LLRP Connection Configuration
	api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&gllrpConnConfig);
	if (api3Data.rfidStatus != RFID_READER_FUNCTION_UNSUPPORTED)
	{
		fprintf(api3Data.fpCurLog, "<br>ServerIP:%S \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ", gllrpConnConfig.hostServerIP, gllrpConnConfig.isClient, gllrpConnConfig.port, gllrpConnConfig.pSecureModeConfig->secureMode, gllrpConnConfig.pSecureModeConfig->validatePeerCert);
		wprintf(L"\n ServerIP:%ls \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ", gllrpConnConfig.hostServerIP, gllrpConnConfig.isClient, gllrpConnConfig.port, gllrpConnConfig.pSecureModeConfig->secureMode, gllrpConnConfig.pSecureModeConfig->validatePeerCert);

		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			SetLogResult(TRUE);
		}
		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		//// get LLRP Connection Configuration MBCS
		api3Data.rfidStatus = RFID_GetLLRPConnectionConfigA(api3Data.hReaderMgmt, &gllrpConnConfigA);
		fprintf(api3Data.fpCurLog, "<br>ServerIP:%ls \t isClient:%d \t port:%d \t  ", gllrpConnConfigA.hostServerIP, gllrpConnConfigA.isClient, gllrpConnConfigA.port);
		printf("\n ServerIP:%s \t isClient:%d \t port:%d \t SecMode:%d \t ", gllrpConnConfigA.hostServerIP, gllrpConnConfigA.isClient, gllrpConnConfigA.port);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			SetLogResult(TRUE);
		}
		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		// Set LLRP Connection Configuration
		fprintf(api3Data.fpCurLog, "\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Set llrpConnection Config SecureMode = TRUE, ValidatePeer = TRUE <br> Expected OutPut: llrpConnection Config\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		gllrpConnConfig.pSecureModeConfig->secureMode = TRUE;
		gllrpConnConfig.pSecureModeConfig->validatePeerCert = TRUE;
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt, &gllrpConnConfig);
		rfid3Sleep(4000);
		api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt, &llrpConnConfig);
		fprintf(api3Data.fpCurLog, "<br>ServerIP:%S \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ", llrpConnConfig.hostServerIP, llrpConnConfig.isClient, llrpConnConfig.port, llrpConnConfig.pSecureModeConfig->secureMode, llrpConnConfig.pSecureModeConfig->validatePeerCert);
		wprintf(L"\n ServerIP:%ls \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ", llrpConnConfig.hostServerIP, llrpConnConfig.isClient, llrpConnConfig.port, llrpConnConfig.pSecureModeConfig->secureMode, llrpConnConfig.pSecureModeConfig->validatePeerCert);

		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (llrpConnConfig.pSecureModeConfig->secureMode && llrpConnConfig.pSecureModeConfig->validatePeerCert))
		{
			SetLogResult(TRUE);
		}
		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		// Set LLRP Connection Configuration
		fprintf(api3Data.fpCurLog, "\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Set llrpConnection Config Client = TRUE SecureMode = TRUE, ValidatePeer = TRUE <br> Expected OutPut: llrpConnection Config\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		gllrpConnConfig.pSecureModeConfig->secureMode = TRUE;
		gllrpConnConfig.pSecureModeConfig->validatePeerCert = TRUE;
		gllrpConnConfig.isClient = TRUE;
		wcscpy(gllrpConnConfig.hostServerIP, L"deca:2012::59f4:c7aa:8ac1:9b65");
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt, &gllrpConnConfig);
		RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt, &llrpConnConfig);
		fprintf(api3Data.fpCurLog, "<br>ServerIP:%S \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ", llrpConnConfig.hostServerIP, llrpConnConfig.isClient, llrpConnConfig.port, llrpConnConfig.pSecureModeConfig->secureMode, llrpConnConfig.pSecureModeConfig->validatePeerCert);
		wprintf(L"\n ServerIP:%ls \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ", llrpConnConfig.hostServerIP, llrpConnConfig.isClient, llrpConnConfig.port, llrpConnConfig.pSecureModeConfig->secureMode, llrpConnConfig.pSecureModeConfig->validatePeerCert);

		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (llrpConnConfig.isClient && llrpConnConfig.pSecureModeConfig->secureMode && llrpConnConfig.pSecureModeConfig->validatePeerCert))
		{
			SetLogResult(TRUE);
		}
		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		fprintf(api3Data.fpCurLog, "\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Set Invalid llrpConnConfig - Port as 0 <br> Expected OutPut: Parameter Error\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		gllrpConnConfig.port = 0;
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt, &gllrpConnConfig);

		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			DisplayLastErrorInfo();
			SetLogResult(TRUE);
		}
		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		// Setting invalid IpAddress
		fprintf(api3Data.fpCurLog, "\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Set Invalid llrpConnConfig - Port as 0 <br> Expected OutPut: Parameter Error\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		wcscpy(gllrpConnConfig.hostServerIP, L"255.255.255.255");
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt, &gllrpConnConfig);

		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			DisplayLastErrorInfo();
			SetLogResult(TRUE);
		}

		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		// Attempt a Secure Mode connection to the Reader and Verify if connection succeeds
		//.
		fprintf(api3Data.fpCurLog, "\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Secure Mode as TRUE and RFID_Connect() <br> Expected OutPut:  Config Set and Connect should succeed\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		gllrpConnConfig.isClient = FALSE;
		gllrpConnConfig.port = 5084;
		wcscpy(gllrpConnConfig.hostServerIP, L"::1");
		gllrpConnConfig.pSecureModeConfig->secureMode = TRUE;
		gllrpConnConfig.pSecureModeConfig->validatePeerCert = FALSE;
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt, &gllrpConnConfig);
		api3Data.conInfo.lpSecConInfo = &api3Data.SecureConnectionInfo;
		api3Data.conInfo.lpSecConInfo->secureMode = TRUE;
		if (api3Data.rfidStatus == RFID_API_SUCCESS && (RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &api3Data.conInfo) == RFID_API_SUCCESS))
		{
			SetLogResult(TRUE);
		}
		else
		{
			DisplayLastErrorInfo();
		}

		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");

		// Setting valid Configuration with llrp as client as FALSE.
		fprintf(api3Data.fpCurLog, "\n\n<a name=LLRP-CONFIG-%d></a> LLRP-CONFIG-%d:\n\n<br>Description:Set AllValid llrpConnConfig - Port as 0 <br> Expected OutPut:  Config is Set\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		gllrpConnConfig.isClient = FALSE;
		gllrpConnConfig.port = 5084;
		wcscpy(gllrpConnConfig.hostServerIP, L"::1");
		gllrpConnConfig.pSecureModeConfig->secureMode = gllrpConnConfig.pSecureModeConfig->validatePeerCert = FALSE;
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt, &gllrpConnConfig);

		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			SetLogResult(TRUE);
		}
		else
		{
			DisplayLastErrorInfo();
		}

		fprintf(api3Data.fpCurLog, "<br> Result is: %S \n ", RFID_GetErrorDescription(api3Data.rfidStatus));
		logToResultFile(L"LLRP-CONFIG");
	}

	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));

}

void TestCableLossCompensation( )
{
	api3Data.dwTestCaseNo = 01;
	CABLE_LOSS_COMPENSATION* pCableLoss[8];	// max 8 antennas
	CABLE_LOSS_COMPENSATION gCableLossCompensation;

	// Supported only in FX7500 Currently.and in API version 5_5
	//
	if( api3Data.apiVersion != RFID_API3_5_5 )
	{
		api3Data.dwSkipCount = 06;
		LogPassFailCount();
		return;
	}

	api3Data.itemTestCount = 04;
	for( int antennaIndex = 0; antennaIndex < api3Data.rCaps.numAntennas;antennaIndex++)
	{
		pCableLoss[antennaIndex] = new CABLE_LOSS_COMPENSATION;
		pCableLoss[antennaIndex]->antennaID = antennaIndex+1;
		pCableLoss[antennaIndex]->cableLengthInFt = antennaIndex+1;
		pCableLoss[antennaIndex]->cableLossPer100Ft = antennaIndex+1;
	}

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("CableLossCompensation");
	logCurrentExecution( "CableLossCompensation");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CABLE-LOSS-%d></a> CABLE-LOSS-%d:\n\n<br>Description:Set Cable Loss Compensation <br> Expected OutPut: Cable Loss Compensation values\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	
	// Set Cable Loss Compensation
	api3Data.rfidStatus = RFID_SetCableLossCompensation( api3Data.hReaderMgmt,api3Data.rCaps.numAntennas,pCableLoss );
	
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "<br> Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	logToResultFile(L"LLRP-CONFIG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CABLE-LOSS-%d></a> CABLE-LOSS-%d:\n\n<br>Description:Get Cable Loss Compensation <br> Expected OutPut: Cable Loss Compensation values\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	
	// get LLRP Connection Configuration
	for( int antennaIndex = 0; antennaIndex < api3Data.rCaps.numAntennas;antennaIndex++)
	{
		gCableLossCompensation.antennaID = antennaIndex+1;
		api3Data.rfidStatus = RFID_GetCableLossCompensation( api3Data.hReaderMgmt,&gCableLossCompensation);
		fprintf(api3Data.fpCurLog,"<br>Antenna ID : %d\t CableLength:%d  CableLoss:%lf <br>",gCableLossCompensation.antennaID,gCableLossCompensation.cableLengthInFt,gCableLossCompensation.cableLossPer100Ft);
	}

	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}

	fprintf( api3Data.fpCurLog, "<br> Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	logToResultFile(L"LLRP-CONFIG");
	
	// Set Cable loss with negative values
	fprintf(api3Data.fpCurLog,"\n\n<a name=CABLE-LOSS-%d></a> CABLE-LOSS-%d:\n\n<br>Description:Set Cable Loss Compensation with Negative Values<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	
	// Set Cable Loss Compensation
	pCableLoss[0]->cableLossPer100Ft = -10;
	api3Data.rfidStatus = RFID_SetCableLossCompensation( api3Data.hReaderMgmt,1,pCableLoss );
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	fprintf( api3Data.fpCurLog, "<br> Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	logToResultFile(L"LLRP-CONFIG");
	fprintf(api3Data.fpCurLog,"\n\n<a name=CABLE-LOSS-%d></a> CABLE-LOSS-%d:\n\n<br>Description:Set Cable Loss Compensation with Negative Values<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	
	// Set Cable loss for all antennas to 0 
	pCableLoss[0]->antennaID = pCableLoss[0]->cableLengthInFt = pCableLoss[0]->cableLossPer100Ft = 0;
	 
	api3Data.rfidStatus = RFID_SetCableLossCompensation( api3Data.hReaderMgmt,0,pCableLoss );
	
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult(TRUE);
	}
	else
	{
		DisplayLastErrorInfo();
	}

	fprintf( api3Data.fpCurLog, "<br> Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	logToResultFile(L"LLRP-CONFIG");
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));
}

void Fx7500RMAPI( )
{
	// New RM and Radio Control Test Cases.
	//
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	
	TestRadioControl();

	if( Login())
	{
		TestIdleModeTimeOut();
		TestCableLossCompensation();
		TestllrpSecureConfig();
	}

	
	wprintf(L"\n Executing %ls Ended ",MultiByte2WideChar( __FUNCTION__));
}