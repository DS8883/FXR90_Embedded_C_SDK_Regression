// Config.cpp : Tests Configuration Parameters of RFID3, Connect and DisConnet
// 
#include "Common.h"
#include "Config.h"

// global data for API
extern struct RFID3DATA api3Data;

// Test Get antenna Configuration API
//
int TestRFID_GetAntennaConfigAPI( RFID_HANDLE32 hReader )
{
	char szStatus[128];
	SetLogText("RFID_GetAntennaConfig()- FUN ");	
	UINT16 nTxpIndex,nTxfIndex,nRxIndex;

	for(int nAntennaIndex = 1;nAntennaIndex <= api3Data.rCaps.numAntennas;nAntennaIndex++)
	{
		if( ( api3Data.rfidStatus = RFID_GetAntennaConfig( hReader,nAntennaIndex,&nRxIndex,&nTxpIndex,&nTxfIndex) ) == RFID_API_SUCCESS )
		{	
			SetLogResult( TRUE );
		}

		if( IsAntennaConnected( hReader, nAntennaIndex ) )
		{
			strcpy( szStatus,"Connected");
		}
		else
		{
			strcpy( szStatus,"Not Connected");
		}

		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description: GetAntennaConfig for antenna Index: %d \n <br> Expected OutPut :Connected or Not Connected\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,nAntennaIndex );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf(api3Data.fpCurLog,"\n\n rfidStatus:%S AntennaStatus: %s nRxIndex:%d  nTxpIndex:%d   nTxfIndex:%d ",RFID_GetErrorDescription( api3Data.rfidStatus ),szStatus,nRxIndex,nTxpIndex,nTxfIndex);
		logToResultFile(L"CAPI-CONFIG");	

	}

	SetLogText("RFID_GetAntennaConfig()- NEG ");	

	// Test negative values for the Antenna Config
	//	
	fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-CONFIG-%d></a> TestCaseNo:CAPI-CONFIG-%d:\n\n<br>Description: GetAntennaConfig for antenna Index: 255 \n <br> Expected OutPut : Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is : \n ");
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_GetAntennaConfig(hReader,255,&nRxIndex,&nTxpIndex,&nTxfIndex);

	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-CONFIG");
	fprintf(api3Data.fpCurLog,"\n\n rfidStatus:%S ", RFID_GetErrorDescription( api3Data.rfidStatus ) );
	
	int antennaIndexArray[] = { -9999,10,255,9999,65535 };
	int antID;
	for( int index =0 ; index <= 4; index++ )
	{
		antID = antennaIndexArray[ index ];
		api3Data.rfidStatus = RFID_GetAntennaConfig( hReader, antID, &nRxIndex, &nTxpIndex, &nTxfIndex );
		fprintf(api3Data.fpCurLog,"\n\n<a name=TestCaseNo:CAPI-CONFIG-%d></a> TestCaseNo:CAPI-CONFIG-%d:\n\n<br>Description: GetAntennaConfig with Invalid Index: %ld \n <br> Expected OutPut : Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,antID );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result is : \n ");
		fprintf( api3Data.fpCurLog,"<br>");
		if(RFID_API_SUCCESS != api3Data.rfidStatus)
		{
			SetLogResult(TRUE );
		}
		logToResultFile(L"GETCONFIG-FUN");
		fprintf(api3Data.fpCurLog,"\n\n rfidStatus:%S ", RFID_GetErrorDescription( api3Data.rfidStatus ) );
	}

	return TRUE;
}
// Test Connect API
int TestRFID_ConnectAPI(  void )
{
	RFID_HANDLE32 hReader;
	CONNECTION_INFO conInfo = { api3Data.apiVersion, 0 };
	if( api3Data.hReader != (RFID_HANDLE32)INVALID_HANDLE_VALUE )
	{
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
		api3Data.hReader = (RFID_HANDLE32)INVALID_HANDLE_VALUE;
	}
	
	SetLogText( "RFID_Connect()- FUN" );
	fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_Connect with Valid Parameters\n <br> Expected OutPut:API Success with valid handle\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,NULL) ) == RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE);
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
		api3Data.hReader = (RFID_HANDLE32)INVALID_HANDLE_VALUE;
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CONNECT-FUN");	

	// Test for negative inputs
	fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_Connect:Test with Invalid IP = 255.255.255.0 \n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");

	if( ( api3Data.rfidStatus = RFID_Connect( &hReader,L"255.255.255.0", api3Data.dwPort,0,&conInfo) ) != RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE);
	}
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CONNECT-FUN");

	if( api3Data.readerType != MC )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_Connect:Test NULL buffer for IP address  \n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		wchar_t* pszIP = NULL;
		if( ( api3Data.rfidStatus = RFID_Connect( &hReader,pszIP, api3Data.dwPort,1000,&conInfo) ) != RFID_API_SUCCESS )
		{	
			SetLogResult( TRUE);
		}

		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CONNECT-FUN");
	}		
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_Connect: Test with different port = 1111 \n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Connect( &hReader,api3Data.szIPAddress,1111,10,&conInfo ) ) != RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE);
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CONNECT-FUN");

	return TRUE;
}

int TestRFID_DisConnectAPI( RFID_HANDLE32 hReader )
{	
	SetLogText( "TestRFID_DisConnectAPI- NEG" );
	fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_DisConnect: Test RFID_Disconnect with Neg Handle val = -1 \n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Disconnect( (RFID_HANDLE32)-1 ) ) != RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CONNECT-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_DisConnect: Test RFID_Disconnect with 0\n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Disconnect( 0 ) ) != RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CONNECT-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CONNECT-FUN-%d></a> CONNECT-FUN-%d:\n\n<br>Description:RFID_DisConnect: Test RFID_Disconnect with INVALID_HANDLE_VALUE\n <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_Disconnect( (RFID_HANDLE32)INVALID_HANDLE_VALUE ) ) != RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CONNECT-FUN");

	return TRUE;
}

void Test_RFID_GetDllVersionInfo( RFID_HANDLE32 hReader )
{
	VERSION_INFO vinfo;	
	SetLogText( "RFID_GetDllVersionInfo-FUN" );
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description: RFID_GetDllVersionInfo with Valid Version Info Struct\n <br> Expected OutPut:Version Info should be filled\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetDllVersionInfo(&vinfo ) ) == RFID_API_SUCCESS )
	{	
		fprintf(api3Data.fpLog,"Version info = %S",vinfo.dllVersion);
		SetLogResult( TRUE );
	}	
	logToResultFile(L"CAPI-CONFIG");

	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-CONFIG-%d></a> CAPI-CONFIG-%d:\n\n<br>Description: RFID_GetDllVersionInfo with NULL value for Version Info Struct\n <br> Expected OutPut:Prameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus = RFID_GetDllVersionInfo( NULL ) ) != RFID_API_SUCCESS )
	{	
		SetLogResult( TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-CONFIG");
}

