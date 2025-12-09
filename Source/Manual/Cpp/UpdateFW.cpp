#include "ManualTests.h"

extern struct RFID3DATA api3Data;
extern wchar_t* PrintTime( void );
#define MAX_BUF 512

void test_GetUpdateStatus(void)
{
	UPDATE_STATUS upgradeStatus;
    
	for( int i= 0;i<500;i++ )
	{
		api3Data.rfidStatus = RFID_GetUpdateStatus(api3Data.hReaderMgmt, &upgradeStatus);
		wprintf(L"\n API Result: %ls \t parition Name: %ls \t Percentage Complete:%d ",RFID_GetErrorDescription(api3Data.rfidStatus),upgradeStatus.updateInfo,upgradeStatus.percentage);
		rfid3Sleep(1000 );
		if( upgradeStatus.percentage == 100 ) break;
	}
}

void UpdateFWonPXDevices(RFID_VERSION version)
{
	int Iteration = 0;
	char buf[MAX_BUF] = { 0, };
	wchar_t Path2file1[] = L"C:\\Pixie(S2F)\\Firmwares\\3.0.6\\CAAFBS00-001-N08D0.DAT";
	wchar_t Path2file2[] = L"C:\\Pixie(S2F)\\Firmwares\\3.0.7\\CAAFBS00-001-N09D0.DAT";
	//wchar_t Path2file1[] = L"/usr/tmp/CAAFBS00-001-N08D0.DAT";
	//wchar_t Path2file2[] = L"/usr/tmp/CAAFBS00-001-N09D0.DAT";
	wchar_t* Path2file;
	wprintf(L"Executing test_SoftwareUpdate()\n");
	LOGIN_INFO loginfo;

	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = version;
	wprintf(L"\n RMLogin - user : %ls \t password : %ls \t hostIP : %ls", loginfo.userName, loginfo.password, loginfo.hostName);
	CONNECTION_INFO ConInfo = { version, 0 };
	for (;;)
	{
		wprintf(L"\n Iteration %ld", Iteration++);

		api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
#ifdef PIXIE
		api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, PX, FALSE, 0);
#endif // PIXIE

		LPTAG_DATA pInventoriedTag = RFID_AllocateTag(api3Data.hReader);
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);

		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		while ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pInventoriedTag)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(pInventoriedTag);
		}

		api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
		wprintf(L"\n Version Info %ls", api3Data.rCaps.firmWareVersion);
		if (wcscmp(L"3.0.7.0", api3Data.rCaps.firmWareVersion) == 0)
		{
			Path2file = Path2file1;
		}
		else
		{
			Path2file = Path2file2;
		}

		wprintf(L"\n Updating to Version--> %ls \n", Path2file);

		wprintf(L"\n RFID_Login: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();

		api3Data.rfidStatus = RFID_UpdateRadioFirmware(api3Data.hReaderMgmt, Path2file);
		if (RFID_API_SUCCESS != api3Data.rfidStatus)
		{
			wprintf(L"\n API Result Description: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
			DisplayLastErrorInfo();
		}

		//test_GetUpdateStatus();
		api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
		api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
		rfid3Sleep(2000);
	}
	
}

void UpdateFWonFXDevices( RFID_VERSION version)
{
	char buf[MAX_BUF]={0,};
	wprintf(L"Executing test_SoftwareUpdate()\n");
	LOGIN_INFO loginfo;
	
	wprintf(L"\n Reader IP:");
	wscanf (L"%ls", &loginfo.hostName);
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = version;
	wprintf(L"\n RMLogin - user : %ls \t password : %ls \t hostIP : %ls",loginfo.userName,loginfo.password,loginfo.hostName);
	api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
	wprintf(L"\n RFID_Login: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	DisplayLastErrorInfo();

	FTPSERVER_INFO ftploginInfo;
	wprintf(L"\n FTP/SCP username:");
	wscanf (L"%ls", &ftploginInfo.userName);
	wprintf(L"\n FTP/SCP password:");
	wscanf (L"%ls", &ftploginInfo.password);
	wprintf(L"\n FTP/SCP Path:");
	wscanf (L"%ls", &ftploginInfo.hostName);
	//wprintf(L"\n Enter the ftp/scp/ftps Link for the Image: \n");
	//wcscpy(loginInfo.hostName,L"ftps://192.168.2.104/ftproot/");
	//wcscpy(loginInfo.hostName,L"ftps://..235.206.150/ftproot/FX7500/Build-fx7500_dev-1.1.41.0/");

	api3Data.rfidStatus = RFID_UpdateSoftware( api3Data.hReaderMgmt, &ftploginInfo);
    if(RFID_API_SUCCESS != api3Data.rfidStatus)
	{
		wprintf(L"\n API Result Description: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();
	}
	test_GetUpdateStatus();
}

// test cable loss compensation
//
void CallAllCleanUPFunctions( RFID_VERSION rfidVersion )
{
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	RFID_EVENT_TYPE rfideventType[] = { BUFFER_FULL_WARNING_EVENT,GPI_EVENT,TAG_DATA_EVENT,ANTENNA_EVENT,INVENTORY_START_EVENT,INVENTORY_STOP_EVENT,ACCESS_START_EVENT,ACCESS_STOP_EVENT,TEMPERATURE_ALARM_EVENT};
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	wprintf(L"\n RFID_DeletePreFilter: %ls",RFID_GetErrorDescription(RFID_DeletePreFilter(api3Data.hReader,0,0)));
	wprintf(L"\n RFID_DeletePreFilter: %ls",RFID_GetErrorDescription(RFID_DeletePreFilter(api3Data.hReader,0,1)));
	wprintf(L"\n RFID_DeleteOperationFromAccessSequence: %ls",RFID_GetErrorDescription(RFID_DeleteOperationFromAccessSequence( api3Data.hReader,0)));
	wprintf(L"\n RFID_RegisterEventNotificationCallback: %ls",RFID_GetErrorDescription( RFID_RegisterEventNotificationCallback(api3Data.hReader,rfideventType,9,NULL,NULL,NULL)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,BUFFER_FULL_WARNING_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,GPI_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,TAG_DATA_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,ANTENNA_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,INVENTORY_START_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_START_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,ACCESS_STOP_EVENT)));
	wprintf(L"\n RFID_DeregisterEventNotification: %ls",RFID_GetErrorDescription( RFID_DeregisterEventNotification( api3Data.hReader,TEMPERATURE_ALARM_EVENT)));
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}