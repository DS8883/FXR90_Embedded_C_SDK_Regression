#include "ManualTests.h"

extern struct RFID3DATA api3Data;

#define MAX_CERT_KEY_FILE_SIZE		10240

wchar_t g_ClientCertFilePath[MAX_PATH];
wchar_t g_ClientKeyFilePath[MAX_PATH];
wchar_t g_RootCertFilePath[MAX_PATH];
wchar_t g_KeyPassword[MAX_PATH];
BYTE g_ClientCertContent[MAX_CERT_KEY_FILE_SIZE];
int	 g_ClientCertSize;
BYTE g_ClientKeyContent[MAX_CERT_KEY_FILE_SIZE];
int	g_ClientKeySize;
BYTE g_RootCertContent[MAX_CERT_KEY_FILE_SIZE];
int	 g_RootCertSize;
BYTE g_Password[MAX_PATH];


// secure mode accept connection
void StressSecureConnection( RFID_VERSION rfidVersion, int iterations)
{
	SEC_CONNECTION_INFO SecureInfo;
	memset(&SecureInfo,0,sizeof(SEC_CONNECTION_INFO));

#ifndef linux
	if(GetFileContent("C:\\FX7500\\CertDir\\client_crt.pem", g_ClientCertContent, &g_ClientCertSize) > 0)
#else
	if(GetFileContent("./client_crt.pem", g_ClientCertContent, &g_ClientCertSize) > 0)
#endif
	{
		SecureInfo.clientCertBuff = g_ClientCertContent;
		SecureInfo.sizeCertBuff = g_ClientCertSize;
	}
	
#ifndef linux
	if (GetFileContent("C:\\FX7500\\CertDir\\client_key.pem", g_ClientKeyContent, &g_ClientKeySize) > 0)
#else
	if (GetFileContent("./client_key.pem", g_ClientKeyContent, &g_ClientKeySize) > 0)
#endif
	{
		SecureInfo.clientKeyBuff = g_ClientKeyContent;
		SecureInfo.sizeKeyBuff = g_ClientKeySize;
	}
	
	SecureInfo.phraseBuff = (BYTE*)"abcd12345";
	SecureInfo.sizePhraseBuff = strlen((char *)"abcd12345");
#ifndef linux
	if (GetFileContent("C:\\FX7500\\CertDir\\cacert.pem", g_RootCertContent, &g_RootCertSize) > 0)
#else
	if (GetFileContent("./cacert.pem", g_RootCertContent, &g_RootCertSize) > 0)
#endif
	{
		SecureInfo.rootCertBuff = g_RootCertContent;
		SecureInfo.sizeRootCertBuff = g_RootCertSize;
	}
	
	SecureInfo.secureMode = TRUE;	
	SecureInfo.validatePeerCert = FALSE;
	for(int i=0;i<iterations;i++)
	{
		CONNECTION_INFO ConInfo = { rfidVersion,&SecureInfo, 0 };
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,10000,&ConInfo );
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, NULL,NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}
		RFID_DeallocateTag( api3Data.hReader, pTagData );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	}
}

// secure mode accept connection
void TestSecureNonSecure( RFID_VERSION rfidVersion)
{
	LPTAG_DATA pTagData;
	SEC_CONNECTION_INFO SecureInfo;
	CONNECTION_INFO ConInfo = { rfidVersion,&SecureInfo, 0 };
	
	memset(&SecureInfo,0,sizeof(SEC_CONNECTION_INFO));
	char tempStr[MAX_PATH];
	if(GetFileContent("C:\\FX7500\\CertDir\\client_crt.pem", g_ClientCertContent, &g_ClientCertSize) > 0)
	{
		SecureInfo.clientCertBuff = g_ClientCertContent;
		SecureInfo.sizeCertBuff = g_ClientCertSize;
	}
		
	if (GetFileContent("C:\\FX7500\\CertDir\\client_key.pem", g_ClientKeyContent, &g_ClientKeySize) > 0)
	{
		SecureInfo.clientKeyBuff = g_ClientKeyContent;
		SecureInfo.sizeKeyBuff = g_ClientKeySize;
	}
	
	SecureInfo.phraseBuff = (BYTE*)"abcd12345";
	SecureInfo.sizePhraseBuff = strlen((char *)"abcd12345");
	
	if (GetFileContent("C:\\FX7500\\CertDir\\cacert.pem", g_RootCertContent, &g_RootCertSize) > 0)
	{
		SecureInfo.rootCertBuff = g_RootCertContent;
		SecureInfo.sizeRootCertBuff = g_RootCertSize;
	}

	SecureInfo.secureMode = TRUE;	
	SecureInfo.validatePeerCert = TRUE;
	for(;;)
	{
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,10000,&ConInfo );
		pTagData = RFID_AllocateTag( api3Data.hReader );
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, NULL,NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}
		RFID_DeallocateTag( api3Data.hReader, pTagData );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );

		// RMLogin and Change the Secure --> Non Secure and Read
		LOGIN_INFO loginfo;
		wcscpy( loginfo.hostName,api3Data.szIPAddress);
		wcscpy( loginfo.userName,L"admin" );
		wcscpy( loginfo.password,L"change" );
		loginfo.forceLogin = TRUE;
		loginfo.version = rfidVersion;
		ConInfo.lpSecConInfo = NULL;
		LLRP_CONNECTION_CONFIG llrpConnConfig;
		memset( &llrpConnConfig,0,sizeof(LLRP_CONNECTION_CONFIG));
		api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
		api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&llrpConnConfig);
		SEC_LLRP_CONFIG SecConfig = { 0,0,0};	
		llrpConnConfig.pSecureModeConfig = &SecConfig;
		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig( api3Data.hReaderMgmt,&llrpConnConfig);

		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,10000,&ConInfo );
		pTagData = RFID_AllocateTag( api3Data.hReader );
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, NULL,NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}
		RFID_DeallocateTag( api3Data.hReader, pTagData );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	}
}

void SecureLLRPConnectionConfig( RFID_VERSION rfidVersion)
{
	LOGIN_INFO loginfo;
	wcscpy( loginfo.hostName,api3Data.szIPAddress);
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	HEALTH_STATUS sts;
	READER_SYSTEM_INFO sysInfo;
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	LLRP_CONNECTION_CONFIG llrpConnConfig,gllrpConnConfig;
	SEC_LLRP_CONFIG SecureConfig;
	SecureConfig.secureMode = TRUE;
	SecureConfig.validatePeerCert = TRUE;
	
	for( int i=0;i<1000;i++)
	{
		api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
		api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&llrpConnConfig);
		
		if( i % 2 == 0)
		{
			wcscpy(llrpConnConfig.hostServerIP,L"157.235.206.154");
			llrpConnConfig.isClient = TRUE;
			llrpConnConfig.port = 5085;	
			SecureConfig.secureMode = TRUE;
			SecureConfig.validatePeerCert = TRUE;
			llrpConnConfig.pSecureModeConfig = &SecureConfig;
		}
		else
		{
			llrpConnConfig.isClient = FALSE;
			llrpConnConfig.port = 5084;
			SecureConfig.secureMode = FALSE;
			SecureConfig.validatePeerCert = FALSE;
			llrpConnConfig.pSecureModeConfig = &SecureConfig;
		}

		api3Data.rfidStatus = RFID_SetLLRPConnectionConfig( api3Data.hReaderMgmt,&llrpConnConfig);
		api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,RM,&sts,0);
		wprintf(L"\n RM Status %ls",sts?L"YES":L"NO");
		api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,LLRP_SERVER,&sts,0);
		wprintf(L"\n LLRP Status %ls",sts?L"YES":L"NO");
		api3Data.rfidStatus = RFID_GetSystemInfo( api3Data.hReaderMgmt,&sysInfo);
		api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&gllrpConnConfig);
		if(gllrpConnConfig.pSecureModeConfig != NULL )
		{
		//	wprintf(L"\n ServerIP:%ls \t isClient:%d \t port:%d \t SecMode:%d \t ValidatePeer:%d ",gllrpConnConfig.hostServerIP,gllrpConnConfig.isClient,gllrpConnConfig.port,gllrpConnConfig.pSecureModeConfig->secureMode,gllrpConnConfig.pSecureModeConfig->validatePeerCert);
		}
		else
		{
		//	wprintf(L"\n ServerIP:%ls \t isClient:%d \t port:%d ",gllrpConnConfig.hostServerIP,gllrpConnConfig.isClient,gllrpConnConfig.port);
			
		}
		api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt);
	}

}