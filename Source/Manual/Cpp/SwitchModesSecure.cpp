#include "ManualTests.h"
extern struct RFID3DATA api3Data;


BOOLEAN Login( BOOLEAN Secure)
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	BOOLEAN rmSecureMode = Secure;
	LOGIN_INFO loginfo;
	memset(&loginfo, 0, sizeof(LOGIN_INFO));
	loginfo.version = api3Data.apiVersion;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;

	// Enable secure mode only if Version is 5_5.
	if (Secure && api3Data.apiVersion == RFID_API3_5_5)
	{
		wprintf(L"\n Secure Mode flag : TRUE\n");
		rmSecureMode = TRUE;
	}

	wprintf(L"\n login info : ip %ls\tusename:%ls\tpwd:%ls\tforclelogin:%lsSecureMode:%ls", loginfo.hostName, loginfo.userName, loginfo.password, loginfo.forceLogin ? L"TRUE" : L"FALSE", rmSecureMode ? L"TRUE" : L"FALSE");
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, api3Data.readerType, rmSecureMode, 0);

	
	// Display Error Info if Login is not Succesful
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n RFID_Login: %ls ", RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();
		return FALSE;
	}

	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
}
void SwitchWithSecureModes()
{
	/*READER_NETWORK_INFOA nwInfoA = { 0, };
	READER_NETWORK_INFOW nwInfoW = { 0, };
	Login(FALSE);
	api3Data.rfidStatus = RFID_GetNetworkInterfaceSettingsA(api3Data.hReaderMgmt, &nwInfoA);
	api3Data.rfidStatus = RFID_GetNetworkInterfaceSettingsW(api3Data.hReaderMgmt, &nwInfoW);
	nwInfoW.Interface
	RFID_SetNetworkInterfaceSettings()

	RFID_SetNetworkInterfaceSettingsW*/
}