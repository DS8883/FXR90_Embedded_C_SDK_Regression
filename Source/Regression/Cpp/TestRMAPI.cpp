#include "Common.h"
#include "Config.h"
#include "APITest.h"
#include <stdio.h>

// global data for API
extern struct RFID3DATA api3Data;
 
void test_Login( BOOLEAN bSecure )
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));

	if( bSecure )
	{
		SetLogText("LOGIN-Secure Mode- FUN");
	}
	else
	{
		SetLogText("LOGIN- NON Secure Mode- FUN ");
	}

	LOGIN_INFO loginfo;
	RFID_HANDLE32 hReaderMgmt;
	wcscpy( loginfo.hostName,api3Data.szIPAddress);
	wcscpy( loginfo.password,L"a");
	wcscpy( loginfo.userName,L"a");
	
	if( api3Data.readerType != MC )
	{
		wcscpy(loginfo.hostName, L"255.255.255.0");
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Login with invalid reader IP- %ls\n <br> Expected OutPut:Login Should FAIL with ERROR\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,loginfo.hostName );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_Login( &hReaderMgmt,&loginfo,api3Data.readerType ,FALSE,0 );
		if( RFID_API_SUCCESS != api3Data.rfidStatus)
		{
			SetLogResult( TRUE );
		}	
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Login with invalid Handle Value\n <br> Expected OutPut:Login Should FAIL with ERROR\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api3Data.rfidStatus =  RFID_Login( NULL,&loginfo,api3Data.readerType,bSecure,0 ) ) != RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}	
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
	
	if( api3Data.readerType != MC )
	{
		wcscpy( loginfo.hostName,api3Data.szIPAddress);
		wcscpy( loginfo.password,L"administration");
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Login with invalid login information\n <br> Expected OutPut:Login Should FAIL with ERROR\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus =  RFID_Login( &hReaderMgmt,&loginfo,api3Data.readerType,bSecure,0 ) )!= RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}	
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	
 		wcscpy( loginfo.password,L"admin");
		wcscpy( loginfo.userName,L"111111122222222222222233333333333333ABCD");
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Login with Test with INCORRECT USERNAME\n <br> Expected OutPut:Login Should FAIL with ERROR\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus = RFID_Login( &hReaderMgmt,&loginfo,api3Data.readerType,bSecure,0 ))!= RFID_API_SUCCESS )
		{
			SetLogResult(TRUE);
		}	
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");

		wcscpy( loginfo.hostName,api3Data.szIPAddress);
		wcscpy( loginfo.password,L"admin");
		wcscpy( loginfo.userName,L"admin");

		wcscpy( loginfo.hostName,L"127.0.0.1.90.34");
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Login with Test with INCORRECT IP multi octet \n <br> Expected OutPut:Login Should FAIL with ERROR\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus = RFID_Login( &hReaderMgmt,&loginfo,api3Data.readerType,bSecure,0 )) != RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}	
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}
	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
}

BOOLEAN Login( void )
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	BOOLEAN rmSecureMode = FALSE;
	LOGIN_INFO loginfo;
	memset(&loginfo,0,sizeof(LOGIN_INFO));
	loginfo.version = api3Data.apiVersion;
	wcscpy( loginfo.hostName,api3Data.szIPAddress );
	wcscpy( loginfo.userName,L"admin");
	wcscpy( loginfo.password,L"Zebra@123");
	loginfo.forceLogin = TRUE;

	// Enable secure mode only if Version is 5_5.
	if( api3Data.bSecureMode && api3Data.apiVersion == RFID_API3_5_5)
	{
		wprintf(L"\n Secure Mode flag : TRUE\n");
		rmSecureMode = TRUE;
	}
	rmSecureMode = TRUE;
	wprintf(L"\n login info : ip %ls\tusename:%ls\tpwd:%ls\tforclelogin:%lsSecureMode:%ls",loginfo.hostName,loginfo.userName,loginfo.password,loginfo.forceLogin?L"TRUE":L"FALSE",rmSecureMode?L"TRUE":L"FALSE");
	int loginAttempts = 0;
	const int maxLoginAttempts = 3;
	while (loginAttempts < maxLoginAttempts) {
		api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,api3Data.readerType ,rmSecureMode,0 );
		if (api3Data.rfidStatus == RFID_API_SUCCESS) {
			break;
		} else {
			wprintf(L"\n RFID_Login failed (attempt %d): %ls", loginAttempts + 1, RFID_GetErrorDescription(api3Data.rfidStatus));
			DisplayLastErrorInfo();
			if (loginAttempts < maxLoginAttempts - 1) {
				wprintf(L"\n Retrying in 10 seconds...\n");
				rfid3Sleep(10000); // 10 seconds
			}
		}
		loginAttempts++;
	}
	if (api3Data.rfidStatus != RFID_API_SUCCESS) {
		wprintf(L"\n CRITICAL: RFID_Login failed after %d attempts. Aborting further operations.\n", maxLoginAttempts);
		return FALSE;
	}
	rfid3Sleep(5000); // Optional: wait after successful login
	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
	return TRUE;
}

BOOLEAN LoginA(void)
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	BOOLEAN rmSecureMode = FALSE;
	LOGIN_INFOA loginfo;
	memset(&loginfo, 0, sizeof(LOGIN_INFOA));
	loginfo.version = api3Data.apiVersion;
	strcpy(loginfo.hostName, api3Data.mbcsIPAddress);
	strcpy(loginfo.userName, "admin");
	strcpy(loginfo.password, "Zebra@123");
	loginfo.forceLogin = TRUE;

	// Enable secure mode only if Version is 5_5.
	if (api3Data.bSecureMode && api3Data.apiVersion == RFID_API3_5_5)
	{
		wprintf(L"\n Secure Mode flag : TRUE\n");
		rmSecureMode = TRUE;
	}
	rmSecureMode = TRUE;
	printf("\n login info : ip %s\tusename:%s\tpwd:%s\tforclelogin:%s\t SecureMode:%s", loginfo.hostName, loginfo.userName, loginfo.password, loginfo.forceLogin ? "TRUE" : "FALSE", rmSecureMode ? "TRUE" : "FALSE");
	int loginAttempts = 0;
	const int maxLoginAttempts = 3;
	while (loginAttempts < maxLoginAttempts) {
		api3Data.rfidStatus = RFID_LoginA(&api3Data.hReaderMgmt, &loginfo, api3Data.readerType, rmSecureMode, 0);
		if (api3Data.rfidStatus == RFID_API_SUCCESS) {
			break;
		} else {
			wprintf(L"\n RFID_LoginA failed (attempt %d): %ls", loginAttempts + 1, RFID_GetErrorDescription(api3Data.rfidStatus));
			DisplayLastErrorInfo();
			if (loginAttempts < maxLoginAttempts - 1) {
				wprintf(L"\n Retrying in 10 seconds...\n");
				rfid3Sleep(10000); // 10 seconds
			}
		}
		loginAttempts++;
	}
	if (api3Data.rfidStatus != RFID_API_SUCCESS) {
		wprintf(L"\n CRITICAL: RFID_LoginA failed after %d attempts. Aborting further operations.\n", maxLoginAttempts);
		return FALSE;
	}
	rfid3Sleep(5000); // Optional: wait after successful login
	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
	return TRUE;
}
void test_EnableReadPoint( )
{
	int antennaIndexArray[] = { -9999,10,255,9999,65535 };
	SetLogText("test_EnableReadPoint()- FUN ");
	
	// Disblae Antennas one by One
	for( int antID = 1; antID <= api3Data.rCaps.numAntennas; antID++ )
	{
		api3Data.rfidStatus = RFID_EnableReadPoint( api3Data.hReaderMgmt, antID, FALSE );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_EnableReadPoint() for disabling the antenna\n <br> Expected OutPut:Should Disble the Read point.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if(RFID_API_SUCCESS == api3Data.rfidStatus && api3Data.readerType != MC )
		{
			SetLogResult(TRUE );
		}
		else if( RFID_API_SUCCESS != api3Data.rfidStatus && api3Data.readerType == MC )
		{
			SetLogResult(TRUE );
		}
		
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}

	// Enable Antennas one by One
	for( int antID=1;antID <= api3Data.rCaps.numAntennas;antID++ )
	{
		api3Data.rfidStatus = RFID_EnableReadPoint( api3Data.hReaderMgmt, antID, TRUE );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_EnableReadPoint() for enabling the antenna\n <br> Expected OutPut:Should enable the Read point.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if(RFID_API_SUCCESS == api3Data.rfidStatus && api3Data.readerType != MC )
		{
			SetLogResult(TRUE );
		}
		else if( RFID_API_SUCCESS != api3Data.rfidStatus && api3Data.readerType == MC )
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}
	
	// Negative Test Cases for Antenna Enable or DIsable.
	int antID = 1;
	api3Data.rfidStatus = RFID_EnableReadPoint((RFID_HANDLE32)INVALID_HANDLE_VALUE, antID, TRUE );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_EnableReadPoint() with Invalid Handle Value\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if(RFID_API_SUCCESS != api3Data.rfidStatus)
    {
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
	
	for( int index =0 ; index <= 4; index++ )
	{
		antID = antennaIndexArray[index];
		api3Data.rfidStatus = RFID_EnableReadPoint( api3Data.hReaderMgmt, antID, TRUE);
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_EnableReadPoint() to enable with Invalid antenna ID Value %d\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,antID );
		fprintf( api3Data.fpCurLog,"<br>");
		if(RFID_API_SUCCESS != api3Data.rfidStatus)
		{
			SetLogResult(TRUE );
		}

		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
		
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_EnableReadPoint() to Disblae with Invalid antenna ID Value %d\n <br> Expected OutPut:Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,antID );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_EnableReadPoint( api3Data.hReaderMgmt, antID, FALSE);
		if(RFID_API_SUCCESS != api3Data.rfidStatus)
		{
			SetLogResult(TRUE );
		}

		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}	
}

void test_GetReadPointStatus (void)
{
	int antennaIndexArray[] = { -9999,10,255,9999,65535 };
	SetLogText("test_GetReadPointStatus()- FUN ");
	BOOLEAN antennaStatus;
	int antennaId;

	//api3Data.rfidStatus = RFID_EnableReadPoint(api3Data.hReaderMgmt, 330, FALSE);
	//api3Data.rfidStatus = RFID_GetReadPointStatus(api3Data.hReaderMgmt, 330, &antennaStatus);
	wprintf(L"\n Result is: %ls ", RFID_GetErrorDescription(api3Data.rfidStatus));

	if( api3Data.readerType != MC )
	{
		for( antennaId = 1;antennaId <= api3Data.rCaps.numAntennas ;antennaId ++ )
		{
			api3Data.rfidStatus = RFID_GetReadPointStatus(api3Data.hReaderMgmt, antennaId, &antennaStatus);
			fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:test_GetReadPointStatus() for antenna %d\n <br> Expected OutPut:Should Return Antenna Status.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,antennaId );
			fprintf( api3Data.fpCurLog,"<br>");
			if(RFID_API_SUCCESS == api3Data.rfidStatus)
			{
				SetLogResult(TRUE );
			}
			wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
			fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
			fprintf( api3Data.fpCurLog,"<br>");
			logToResultFile(L"RMAPI-FUN");
		}
	}
	else
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:test_GetReadPointStatus() on HANDHELD READER\n <br> Expected OutPut:UNSUPPORTED.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		if( ( api3Data.rfidStatus = RFID_GetReadPointStatus( api3Data.hReaderMgmt, 1, &antennaStatus) ) == RFID_READER_FUNCTION_UNSUPPORTED )
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}

	for( int index =0 ; index <= 4; index ++ )
	{
		antennaId = antennaIndexArray[index];
		api3Data.rfidStatus = RFID_GetReadPointStatus( api3Data.hReaderMgmt, antennaId, &antennaStatus);
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: RFID_GetReadPointStatus() with Invalid antenna ID Value %d\n <br> Expected OutPut:UNSUPPORTED.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,antennaId );
		fprintf( api3Data.fpCurLog,"<br>");
		if( RFID_API_SUCCESS != api3Data.rfidStatus )
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}

	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: test_GetReadPointStatus() with Invalid Handle Value\n <br> Expected OutPut: Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_GetReadPointStatus((RFID_HANDLE32)INVALID_HANDLE_VALUE, antennaId, &antennaStatus);
	if(RFID_API_SUCCESS != api3Data.rfidStatus)
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
	
	api3Data.rfidStatus = RFID_GetReadPointStatus((RFID_HANDLE32)INVALID_HANDLE_VALUE, antennaId, NULL );
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:test_GetReadPointStatus() with Invalid buffer for antenna status\n <br> Expected OutPut: Parameter Error.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if(RFID_API_SUCCESS != api3Data.rfidStatus)
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
}

void test_GetAntennaMode(void)
{
	ANTENNA_MODE antennaMode;
	SetLogText("test_GetAntennaMode()- FUN ");
	if( api3Data.readerType != MC )
	{
		for( int antIndex = 1 ; antIndex <= api3Data.rCaps.numAntennas; antIndex++ )
		{
			fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: RFID_GetAntennaMode()  with valid antenna index\n <br> Expected OutPut: RFIDAPI SUCCESS.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
			fprintf( api3Data.fpCurLog,"<br>");
			if( (api3Data.rfidStatus = RFID_GetAntennaMode(api3Data.hReaderMgmt,&antennaMode) ) == RFID_API_SUCCESS )
			{
				SetLogResult(TRUE );
			}
			else
			{
				DisplayLastErrorInfo();
			}
			fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
			fprintf(api3Data.fpCurLog,"\n Antenna Mode is = %S",antennaMode?L"ANTENNA_MODE_MONOSTATIC":L"ANTENNA_MODE_BISTATIC");
			wprintf(L"\n Antenna Mode is = %ls",antennaMode?L"ANTENNA_MODE_MONOSTATIC":L"ANTENNA_MODE_BISTATIC");
			fprintf( api3Data.fpCurLog,"<br>");
			logToResultFile(L"RMAPI-FUN");
		}
	}
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: RFID_GetAntennaMode()  with invalid Handle Value <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( (api3Data.rfidStatus = RFID_GetAntennaMode((RFID_HANDLE32)INVALID_HANDLE_VALUE, &antennaMode) ) != RFID_API_SUCCESS )
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: RFID_GetAntennaMode()  with invalid buffer <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_GetAntennaMode(api3Data.hReaderMgmt,NULL);
 	if( RFID_API_SUCCESS != api3Data.rfidStatus || RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus )
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	
}

void test_SetAntennaMode(void)
{
	SetLogText("test_SetAntennaMode()- FUN ");
	if( api3Data.readerType != MC )
	{
		api3Data.rfidStatus = RFID_SetAntennaMode(api3Data.hReaderMgmt, ANTENNA_MODE_MONOSTATIC );
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: RFID_SetAntennaMode() with Mode as ANTENNA_MODE_MONOSTATIC <br> Expected OutPut: Mode to be Set as Mono Static\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		if(RFID_API_SUCCESS == api3Data.rfidStatus)
		{
			SetLogResult(TRUE );
		}
		else
		{
			DisplayLastErrorInfo();
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
		
		// This Mode is not supported on MERLIN Readers.
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:  RFID_SetAntennaMode() with Mode as ANTENNA_MODE_BISTATIC <br> Expected OutPut: Mode to be Set as BI Static\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetAntennaMode( api3Data.hReaderMgmt, ANTENNA_MODE_BISTATIC);
		if(RFID_API_SUCCESS == api3Data.rfidStatus)
		{
			SetLogResult(TRUE );
		}
		else
		{
			DisplayLastErrorInfo();
		}
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetAntennaMode() with Mode as INVALID_HANDLE_VALUE <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetAntennaMode((RFID_HANDLE32)INVALID_HANDLE_VALUE, ANTENNA_MODE_BISTATIC);
		if(RFID_API_SUCCESS != api3Data.rfidStatus)
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}
	else
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetAntennaMode() with Mode as INVALID_HANDLE_VALUE <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetAntennaMode((RFID_HANDLE32)INVALID_HANDLE_VALUE, ANTENNA_MODE_BISTATIC);
		if( RFID_API_SUCCESS != api3Data.rfidStatus || RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus )
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetAntennaMode() with Mode as ANTENNA_MODE_BISTATIC <br> Expected OutPut: UNSUPPORTED\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetAntennaMode(api3Data.hReaderMgmt, ANTENNA_MODE_BISTATIC );
		if( RFID_API_SUCCESS != api3Data.rfidStatus || RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus )
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
		
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetAntennaMode() with Mode as ANTENNA_MODE_MONOSTATIC <br> Expected OutPut: UNSUPPORTED\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetAntennaMode(api3Data.hReaderMgmt, ANTENNA_MODE_MONOSTATIC );
		if( RFID_API_SUCCESS != api3Data.rfidStatus || RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus )
		{
			SetLogResult(TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");	
	}
}


void test_GetSystemInfo(void)
{
	READER_SYSTEM_INFO readerSystemInfo;
	memset(&readerSystemInfo,0,sizeof(READER_SYSTEM_INFO));
    SetLogText("test_GetSystemInfo()- FUN ");
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetSystemInfo() with valid info <br> Expected OutPut: API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");	
	api3Data.rfidStatus = RFID_GetSystemInfo( api3Data.hReaderMgmt, &readerSystemInfo );
	if(RFID_API_SUCCESS == api3Data.rfidStatus && api3Data.readerType != MC )
	{
		SetLogResult(TRUE );
	}
	else if( RFID_API_SUCCESS != api3Data.rfidStatus && api3Data.readerType == MC )
	{
		SetLogResult(TRUE );
		DisplayLastErrorInfo();
		
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
    fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	

	fprintf(api3Data.fpLog,"\n Reader System Info: flashAvailable :%-d",readerSystemInfo.flashAvailable);
	fprintf(api3Data.fpLog,"\n Reader System Info: fPGAVersion :%-S",readerSystemInfo.fPGAVersion );
	fprintf(api3Data.fpLog,"\n Reader System Info: radioFirmwareVersion: %-S",readerSystemInfo.radioFirmwareVersion );
	fprintf(api3Data.fpLog,"\n Reader System Info: ramAvailable: %-d",readerSystemInfo.ramAvailable );
	fprintf(api3Data.fpLog,"\n Reader System Info: readerLocation: %-S",readerSystemInfo.readerLocation );
	fprintf(api3Data.fpLog,"\n Reader System Info: readerName: %-S",readerSystemInfo.readerName );
	fprintf(api3Data.fpLog,"\n Reader System Info: upTime: %-S",readerSystemInfo.upTime );
	fprintf(api3Data.fpLog,"\n Reader System Info: cpuUsageForSystemProcesses: %-d",readerSystemInfo.cpuUsageForSystemProcesses);
	fprintf(api3Data.fpLog,"\n Reader System Info: cpuUsageForUserProcesses: %-d",readerSystemInfo.cpuUsageForUserProcesses);
	fprintf(api3Data.fpLog,"\n Reader System Info: cpuUsageForUserProcesses: %-d",readerSystemInfo.ramTotal);
	fprintf(api3Data.fpLog,"\n Reader System Info: ramUsed: %-d",readerSystemInfo.ramUsed);
	fprintf(api3Data.fpLog,"\n Reader System Info: serialNumber: %-S",readerSystemInfo.serialNumber);

	wprintf(L"\n Reader System Info: flashAvailable :%d bytes",readerSystemInfo.flashAvailable);
	wprintf(L"\n Reader System Info: fPGAVersion :%ls",readerSystemInfo.fPGAVersion );
	wprintf(L"\n Reader System Info: radioFirmwareVersion: %ls",readerSystemInfo.radioFirmwareVersion );
	wprintf(L"\n Reader System Info: ramAvailable: %d bytes",readerSystemInfo.ramAvailable );
	wprintf(L"\n Reader System Info: readerLocation: %ls",readerSystemInfo.readerLocation );
	wprintf(L"\n Reader System Info: readerName: %ls",readerSystemInfo.readerName );
	wprintf(L"\n Reader System Info: upTime: %ls",readerSystemInfo.upTime );
	wprintf(L"\n Reader System Info: cpuUsageForSystemProcesses: %-d",readerSystemInfo.cpuUsageForSystemProcesses);
	wprintf(L"\n Reader System Info: cpuUsageForUserProcesses: %-d",readerSystemInfo.cpuUsageForUserProcesses);
	wprintf(L"\n Reader System Info: cpuUsageForUserProcesses: %-d",readerSystemInfo.ramTotal);
	wprintf(L"\n Reader System Info: ramUsed: %-d",readerSystemInfo.ramUsed);
	wprintf(L"\n Reader System Info: serialNumber: %-ls",readerSystemInfo.serialNumber);

	wprintf(L"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description: RFID_GetSystemInfo() with INVALID_HANDLE_VALUE <br> Expected OutPut: FAIL\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");	
	api3Data.rfidStatus =  RFID_GetSystemInfo((RFID_HANDLE32)INVALID_HANDLE_VALUE,&readerSystemInfo);
	if(RFID_API_SUCCESS != api3Data.rfidStatus)
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:  RFID_GetSystemInfo() with Invalid buffer value <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");	
	api3Data.rfidStatus =  RFID_GetSystemInfo( api3Data.hReaderMgmt, 0 );
	if(RFID_API_SUCCESS != api3Data.rfidStatus)
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	
}

void test_GetRadioPowerState( void )
{
	BOOLEAN pState;
	SetLogText("test_GetRadioPowerState()- FUN ");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetRadioPowerState() with vaild parameters <br> Expected OutPut: API Success( FOR MC UNSUPPORTED)\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( api3Data.rCaps.radioPowerControlSupported )
	{
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader,&pState );
		wprintf(L"\n Radio Power State is = %ls",pState?L"ON":L"OFF");
		if(RFID_API_SUCCESS == api3Data.rfidStatus)
		{
			SetLogResult(TRUE);
		}	
		else
		{
			DisplayLastErrorInfo();
		}
	}
	else
	{
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader,&pState );
		if( ( api3Data.readerType == XR || api3Data.readerType == FX ) & ( RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus ) )
		{
			SetLogResult(TRUE );
		}
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetRadioPowerState() with Invalid Handle <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_GetRadioPowerState( (RFID_HANDLE32)INVALID_HANDLE_VALUE,&pState );
	if( RFID_API_INVALID_HANDLE == api3Data.rfidStatus ||  RFID_API_SUCCESS != api3Data.rfidStatus )
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	

	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetRadioPowerState() with Invalid Powerstate <br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_GetRadioPowerState(api3Data.hReader,NULL);
	if( api3Data.rfidStatus == RFID_API_PARAM_ERROR || RFID_API_SUCCESS != api3Data.rfidStatus )
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");	

}
void testMCgetandSetRadioState( void )
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("testMCgetandSetRadioState()- FUN ");
	BOOLEAN bState;
	api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bState );
	for( int i = 0; i < 10;i++ )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:Calling get and Set sequentially :Radio State OFF<br> Expected OutPut: RFID API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader, FALSE );
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bState );
		if( bState ==  FALSE )
		{
			SetLogResult( TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");

		fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:Calling get and Set sequentially:Radio State ON <br> Expected OutPut: RFID API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
		fprintf( api3Data.fpCurLog,"<br>");
	
		api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader, TRUE );
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bState );
		if( bState ==  TRUE  )
		{
			SetLogResult( TRUE );
		}
		wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RMAPI-FUN");
	}
	RFID_SetRadioPowerState(api3Data.hReader , TRUE );
	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
}
void test_GetSledBatteryStatus()
{
	SetLogText("test_GetSledBatteryStatus()- FUN ");
	SLED_BATTERY_STATUS rfidSledStatus = {0,};

	if (api3Data.rCaps.SledBatteryStatusSupported)
	{
		fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetSledBatteryStatus( ) with vaild parameters<br> Expected OutPut: RFID API Success\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		api3Data.rfidStatus = RFID_GetSledBatteryStatus(api3Data.hReader, &rfidSledStatus);
		if (rfidSledStatus.status == BATTERY_CHARGING)
		{
			fprintf(api3Data.fpCurLog, "\n rfidSled- BatteryLevel:%d BatteryStatus:%s", rfidSledStatus.batteryLevel, "BATTERY_CHARGING");
		}
		else if (rfidSledStatus.status == BATTERY_DISCHARGING)
		{
			fprintf(api3Data.fpCurLog, "\n rfidSled- BatteryLevel:%d BatteryStatus:%s", rfidSledStatus.batteryLevel, "BATTERY_DISCHARGING");
		}
		else if (rfidSledStatus.status == BATTERY_LEVEL_CRITICAL)
		{
			fprintf(api3Data.fpCurLog, " \nrfidSled- BatteryLevel:%d BatteryStatus:%s", rfidSledStatus.batteryLevel, "BATTERY_LEVEL_CRITICAL");
		}
		else if (rfidSledStatus.status == BATTERY_STATUS_UNKNOWN)
		{
			fprintf(api3Data.fpCurLog, "\n rfidSled- BatteryLevel:%d BatteryStatus:%s", rfidSledStatus.batteryLevel, "BATTERY_STATUS_UNKNOWN");
		}
		fprintf(api3Data.fpCurLog, "<br>");
		SetLogResult(TRUE);

	}
	else
	{
		api3Data.rfidStatus = RFID_GetSledBatteryStatus(api3Data.hReader, &rfidSledStatus);
		if (api3Data.rfidStatus == RFID_READER_FUNCTION_UNSUPPORTED)
		{
			fprintf(api3Data.fpCurLog, "\n rfidSled- RFID_READER_FUNCTION_UNSUPPORTED");
			fprintf(api3Data.fpCurLog, "<br>");
			SetLogResult(TRUE);
		}

	}

	logToResultFile(L"RMAPI-FUN");

}

void test_NetworkInterfaceSettings()
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("test_GetNetworkInterfaceSettings()- FUN ");
	READER_NETWORK_INFOA nwInfoA = { 0, };
	READER_NETWORK_INFOW nwInfoW = { 0, };
	WIRELESS_CONFIGURED_PARAMETERSA wlConfigA = { 0, };
	WIRELESS_CONFIGURED_PARAMETERSW wlConfigW = { 0, };

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetNetworkInterfaceSettings( ) with vaild parameters<br> Expected OutPut: RFID API Success\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	api3Data.rfidStatus = RFID_GetNetworkInterfaceSettingsA(api3Data.hReaderMgmt, &nwInfoA);
	api3Data.rfidStatus = RFID_GetNetworkInterfaceSettingsW(api3Data.hReaderMgmt, &nwInfoW);
	fprintf(api3Data.fpCurLog, "\n  DNSServer:%s EnableRAPAckets:%s Gateway:%s Interface:%s IPAddress:%s IPV6Address:%s IPV6DNS:%s IPV6GateWay:%s", nwInfoA.DNSServer, nwInfoA.EnableRAPAckets?"RAPktEnabled":"RAPktNOTEnabled", nwInfoA.Gateway, nwInfoA.Interface, nwInfoA.IPAddress, nwInfoA.IPV6Address, nwInfoA.IPV6DNS, nwInfoA.IPV6GateWay);
	wprintf(L"\n DNSServer:%ls EnableRAPAckets:%ls Gateway:%ls Interface:%ls IPAddress:%ls IPV6Address:%ls IPV6DNS:%ls IPV6GateWay:%ls", nwInfoW.DNSServer, nwInfoW.EnableRAPAckets ? L"RAPktEnabled" : L"RAPktNOTEnabled", nwInfoW.Gateway, nwInfoW.Interface, nwInfoW.IPAddress, nwInfoW.IPV6Address, nwInfoW.IPV6DNS, nwInfoW.IPV6GateWay);
	fprintf(api3Data.fpCurLog, "\n IPV6Suffix:%s IPVersion:%s isCoreConfig:%s isDHCPEnabled:%s isDHCPv6Enabled:%s MACAddress:%s SubnetMask:%s", nwInfoA.IPV6Suffix, nwInfoA.IPVersion, nwInfoA.isCoreConfig?"CORECONFIG:YES":"CoreConfig:NO",nwInfoA.isDHCPEnabled ? "isDHCPEnabled:YES" : "isDHCPEnabled:NO", nwInfoA.isDHCPv6Enabled ? "isDHCPv6Enabled:YES" : "isDHCPv6Enabled:NO", nwInfoA.MACAddress, nwInfoA.SubnetMask);
	wprintf(L"\n IPV6Suffix : %ls IPVersion : %ls isCoreConfig : %ls isDHCPEnabled : %ls isDHCPv6Enabled : %ls MACAddress : %ls SubnetMask : %ls", nwInfoW.IPV6Suffix, nwInfoW.IPVersion, nwInfoW.isCoreConfig ? L"CORECONFIG : YES" : L"CoreConfig : NO", nwInfoW.isDHCPEnabled ? L"isDHCPEnabled : YES" : L"isDHCPEnabled : NO", nwInfoW.isDHCPv6Enabled ? L"isDHCPv6Enabled : YES" : L"isDHCPv6Enabled : NO", nwInfoW.MACAddress, nwInfoW.SubnetMask);

	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	logToResultFile(L"RMAPI-FUN");
	fprintf(api3Data.fpCurLog, "<br>");

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetNetworkInterfaceSettings( ) with NULL parameters<br> Expected OutPut: RFID API Success\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	api3Data.rfidStatus = RFID_GetNetworkInterfaceSettingsW(api3Data.hReaderMgmt, NULL);
	if (api3Data.rfidStatus == RFID_API_PARAM_ERROR)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	fprintf(api3Data.fpCurLog, "\n ErrorDescription %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
	logToResultFile(L"RMAPI-FUN");
	fprintf(api3Data.fpCurLog, "<br>");

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetWirelessConfigParameters( ) with valid parameters<br> Expected OutPut: RFID API Success\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	api3Data.rfidStatus = RFID_GetWirelessConfigParametersW(api3Data.hReaderMgmt,&wlConfigW);
	api3Data.rfidStatus = RFID_GetWirelessConfigParametersA(api3Data.hReaderMgmt, &wlConfigA);

	fprintf(api3Data.fpCurLog, "\n  wlConfigA.autoconnect :  %s wlConfigA.essid : %s, wlConfigA.passKey : %s", wlConfigA.autoconnect ? "autoConnect: YES" : "autoConnect: NO", wlConfigA.essid, wlConfigA.passKey);
	wprintf(L"\n  wlConfigA.autoconnect :  %ls wlConfigA.essid : %ls, wlConfigA.passKey : %ls", wlConfigW.autoconnect ? L"autoConnect: YES" : L"autoConnect: NO", wlConfigW.essid, wlConfigW.passKey);

	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	logToResultFile(L"RMAPI-FUN");
	fprintf(api3Data.fpCurLog, "<br>");

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_GetWirelessConfigParameters( ) with NULL parameters<br> Expected OutPut: RFID API Success\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	api3Data.rfidStatus = RFID_GetWirelessConfigParameters(api3Data.hReaderMgmt, NULL);
	if (api3Data.rfidStatus == RFID_API_PARAM_ERROR)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	fprintf(api3Data.fpCurLog, "\n ErrorDescription %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
	logToResultFile(L"RMAPI-FUN");
	fprintf(api3Data.fpCurLog, "<br>");
	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
}

void test_SetRadioPowerState(void)
{
	SetLogText("test_SetRadioPowerState()- FUN ");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetRadioPowerState() with vaild parameters<br> Expected OutPut: RFID API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader,FALSE);
	if( !api3Data.rCaps.radioPowerControlSupported && (RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus) )
	{
		SetLogResult(TRUE);
	}	
	else if( api3Data.rCaps.radioPowerControlSupported && (api3Data.rfidStatus == RFID_API_SUCCESS) )
	{
		SetLogResult(TRUE );
	}

	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetRadioPowerState() with vaild parameters<br> Expected OutPut: RFID API Success\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader,TRUE);
	if( !api3Data.rCaps.radioPowerControlSupported && (RFID_READER_FUNCTION_UNSUPPORTED == api3Data.rfidStatus) )
	{
		SetLogResult(TRUE);
	}	
	else if( api3Data.rCaps.radioPowerControlSupported && (api3Data.rfidStatus == RFID_API_SUCCESS) )
	{
		SetLogResult(TRUE );
	}

	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
   
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetRadioPowerState()Test with Invalid Handle<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_SetRadioPowerState( (RFID_HANDLE32)INVALID_HANDLE_VALUE,TRUE );
	if( RFID_API_INVALID_HANDLE == api3Data.rfidStatus ||  RFID_API_SUCCESS != api3Data.rfidStatus )
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_SetRadioPowerState()with Invalid Powerstate<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,NULL);
	if( api3Data.rfidStatus == RFID_API_PARAM_ERROR || RFID_API_SUCCESS != api3Data.rfidStatus )
	{
		SetLogResult(TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
}

void test_Logout(void)
{
	SetLogText("test_Logout()- FUN ");
    api3Data.rfidStatus = RFID_Logout((RFID_HANDLE32)INVALID_HANDLE_VALUE);
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Logout() with Invalid Handle<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	if( RFID_API_INVALID_HANDLE == api3Data.rfidStatus ||  RFID_API_SUCCESS != api3Data.rfidStatus )
	{
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br>Description:RFID_Logout() with Valid Parameters<br> Expected OutPut: Parameter Error\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo);
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt );
	if(RFID_API_SUCCESS == api3Data.rfidStatus)
	{
		SetLogResult( TRUE );
	}
	wprintf( L"\n Result is: %ls ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog, "\n Result is: %S \n ",RFID_GetErrorDescription( api3Data.rfidStatus ));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RMAPI-FUN");
}

void ValidateRMAPI( RFID_HANDLE32 hReader )
{
	//test_EnableReadPoint();
	test_GetReadPointStatus();
	test_GetAntennaMode();
	test_SetAntennaMode();			
	test_GetRadioPowerState();
	test_SetRadioPowerState();                                                 
	test_GetSystemInfo();
	test_GetSledBatteryStatus();
	test_NetworkInterfaceSettings();

	if (api3Data.readerType != XR && api3Data.readerType != FX)
	{
		testMCgetandSetRadioState();
	}

	test_Login( FALSE );
	test_Login( TRUE );
	RFID_Logout( api3Data.hReaderMgmt );
}

void TestRFID3ConfigParameters( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	DisplayCapabilities( api3Data.hReader );
	fflush(api3Data.fpLog);
	logCurrentExecution( "TestRFID3ConfigParameters");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 242;
	TestPhysicalAntennaProperties( api3Data.hReader );
	TestRFID_GetAntennaConfigAPI( api3Data.hReader );
	TestResetConfigToDefaults( api3Data.hReader );
	TestAccessAPIWithNegValues( api3Data.hReader );
	TestErrorInfo( api3Data.hReader );
	TestErrorInfoA(api3Data.hReader);
	TestGetLastErrorInfo( api3Data.hReader );
	TestSetTraceLevel( api3Data.hReader );
	Test_RFID_GetDllVersionInfo( api3Data.hReader );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();

	SetLogText("ValidateRFID3Parameters");
	logCurrentExecution( "ValidateParameters");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 30;
	if(api3Data.readerType == FX)
	{
		api3Data.itemTestCount = 27;
	}

	TestValidateParameters( api3Data.hReader );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
}
void ValidaterRFID3rmapi( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("ValidateRFID3RMAPI");
	logCurrentExecution( "ValidateRFID3RMAPI");

	// This is not supported in FX7400 or 7500..
	if(api3Data.readerType == MC )
	{
		api3Data.dwSkipCount = 61;
		LogPassFailCount();
		return;
	}
	
	
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 61;
	
	if( Login())
	{
		ValidateRMAPI( api3Data.hReader );
	}

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}