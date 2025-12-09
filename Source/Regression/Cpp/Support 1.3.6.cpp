#include "Common.h"
#include <stdlib.h>
#include "NxpC.h"
#include "APITest.h"

// global data for API
extern struct RFID3DATA api3Data;


void verifyDllversionInfo( )
{
	VERSION_INFOA dllVersionInfo;
	api3Data.rfidStatus = RFID_GetDllVersionInfoA(&dllVersionInfo);
	printf("\n RDID DLL Version %s", dllVersionInfo.dllVersion);
}

void ValidateRFIDLoginMultiByteMode( )
{
	LOGIN_INFOA loginfo;
	USERAPP_LISTA userAppList;
	ZONE_CONFIGA zoneConfig;

	wcstombs(loginfo.userName, api3Data.szIPAddress, 256);
	strcpy(loginfo.userName, "admin");
	strcpy(loginfo.password, "change");
	loginfo.forceLogin = TRUE;
	loginfo.version = RFID_API3_5_5;

	api3Data.rfidStatus = RFID_LoginA(&api3Data.hReaderMgmt, &loginfo, FX, FALSE, 0);

	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		printf("\n Login Success");
		
		api3Data.rfidStatus = RFID_AddZoneA(api3Data.hReader, 1, &zoneConfig, NULL);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			printf("\n Antenna ount: %d", zoneConfig.antennaCount);
		}
		api3Data.rfidStatus = RFID_DeleteZone( api3Data.hReader,1,NULL );
		api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	}

}
void ReaderCapsInMultiByteMode()
{
	char ansiIPString[512];
	char pCommStandard[][32] = { "UNSPECIFIED","US_FCC_PART_15","ETSI_302_208","ETSI_300_220","AUSTRALIA_LIPD_1W","AUSTRALIA_LIPD_4W","JAPAN_ARIB_STD_T89","HONGKONG_OFTA_1049","TAIWAN_DGT_LP0002","KOREA_MIC_ARTICLE_5_2" };
	// If Secure Mode is enabled, initialize security Parameters. This Call fills the Security Parameters of 
	// api3Data.SecureConnectionInfo
	if (api3Data.bSecureMode == TRUE && api3Data.apiVersion == RFID_API3_5_5)
	{
		InitSecurityParameters();
		api3Data.conInfo.lpSecConInfo = &api3Data.SecureConnectionInfo;
	}
	
	//wcstombs(ansiIPString, api3Data.szIPAddress, 512);

	//api3Data.conInfo.version = RFID_API3_5_5;
	//api3Data.conInfo.lpSecConInfo = NULL;
	//if (api3Data.hReader != (RFID_HANDLE32)INVALID_HANDLE_VALUE)
	//{
	//	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	//}
	//api3Data.rfidStatus = RFID_ConnectA(&api3Data.hReader, ansiIPString, 5084, 0, &api3Data.conInfo);
	READER_CAPSA readerCaps;
	memset(&readerCaps, 0, sizeof(READER_CAPSA));
	printf("\n\n***InSide the Function Display Capabilities****");

	// Get the Reader Capability
	api3Data.rfidStatus = RFID_GetReaderCapsA(api3Data.hReader, &readerCaps);

		// printing s Console
	printf("\n...RFID Reader Capabilities...");
	printf("\n readerCaps.blockEraseSupported: %d", readerCaps.blockEraseSupported);
	printf("\n readerCaps.blockWriteSupported  : %d", readerCaps.blockWriteSupported);
	printf("\n readerCaps.canDoTagInventoryStateAwareSingulation  : %d", readerCaps.stateAwareSingulationSupported);
	printf("\n readerCaps.communicationStandard	: %s", pCommStandard[readerCaps.communicationStandard]);
	printf("\n readerCaps.countryCode	: %d", readerCaps.countryCode);
	printf("\n readerCaps.hasUTCClockCapability : %d", readerCaps.utcClockSupported);
	printf("\n readerCaps.hoppingEnabled	: %d", readerCaps.hoppingEnabled);
	printf("\n readerCaps.maxNumOperationsInAccessSequence : %d", readerCaps.maxNumOperationsInAccessSequence);
	printf("\n readerCaps.maxNumPreFilters : %d", readerCaps.maxNumPreFilters);
	printf("\n readerCaps.modelName : %s ", readerCaps.modelName);
	printf("\n readerCaps.numAntennaSupported : %d ", readerCaps.numAntennas);
	printf("\n readerCaps.numGPI : %d ", readerCaps.numGPIs);
	printf("\n readerCaps.numGPO : %d ", readerCaps.numGPOs);
	printf("\n readerCaps.pFirmwareVersion : %s ", readerCaps.firmWareVersion);
	printf("\n readerCaps.readerID.type: %d ", readerCaps.readerID.type);
	printf("\n readerCaps.readerID.value: %s ", readerCaps.readerID.value);
	printf("\n readerCaps.writeUMISupported: %d ", readerCaps.writeUMISupported);
	printf("\n readerCaps.writeUMISupported: %d ", readerCaps.writeUMISupported);

	printf("\n readerCaps.AntennaRFConfigSupported: %d ", readerCaps.AntennaRFConfigSupported);
	printf("\n readerCaps.FujitsuCommandsSupported: %d ", readerCaps.FujitsuCommandsSupported);
	printf("\n readerCaps.PeriodicTagReportsSupported: %d ", readerCaps.PeriodicTagReportsSupported);
	printf("\n readerCaps.radioPowerControlSupported: %d ", readerCaps.radioPowerControlSupported);
	printf("\n readerCaps.TagPhaseReportingSupported: %d ", readerCaps.TagPhaseReportingSupported);
	printf("\n readerCaps.ZoneSuppported: %d ", readerCaps.ZoneSuppported);

	if (readerCaps.lpVersions != NULL)
	{
		for (int nVersions = 0; nVersions < readerCaps.lpVersions->numVersions; nVersions++)
		{
			printf("\n readerCaps.lpVersion: %d ", readerCaps.lpVersions->numVersions);
			printf("\n readerCaps.ModuleName and version: %s : %s ", readerCaps.lpVersions->pVersionList->moduleVersion, readerCaps.lpVersions->pVersionList->moduleName);
		}
	}
	
	// Test ANSI API's
	
	wprintf(L"\n LoginA %ls",LoginA()?L"LoginA Success":L"LoginA FAIL");

	UPDATE_PARTITION_STATUSA updatePartitionA = { 0, };
	FILE_UPDATE_LISTA fileUpdateListA = { 0, };
	TEMPERATURE_STATUSA tempStatusA = { 0, };
	SYSLOG_SERVER_INFOA syslogServerInfoA = { 0, };
	TIME_SERVER_INFOA ntpServerInfoA = { 0, };
	LLRP_CONNECTION_STATUSA llrpConStatusA = { 0, };

	api3Data.rfidStatus = RFID_GetUpdatePartitionStatusA(api3Data.hReaderMgmt, &updatePartitionA);
	printf("\n RFID_GetUpdatePartitionStatusA Status:%s \n", RFID_GetErrorDescriptionA(api3Data.rfidStatus));

	api3Data.rfidStatus = RFID_GetFilesToUpdateA(api3Data.hReaderMgmt, &fileUpdateListA);
	printf("\n RFID_GetFilesToUpdateA Status:%s \n", RFID_GetErrorDescriptionA(api3Data.rfidStatus));

	api3Data.rfidStatus = RFID_GetTemperatureStatusA(api3Data.hReaderMgmt, &tempStatusA);
	printf("\n RFID_GetTemperatureStatusA Status:%s \n", RFID_GetErrorDescriptionA(api3Data.rfidStatus));

	api3Data.rfidStatus = RFID_GetSysLogServerA(api3Data.hReaderMgmt, &syslogServerInfoA);
	printf("\n RFID_GetSysLogServerA Status:%s \n", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
	printf("\n minSeverity: %d	remoteServerHostName:%s		remoteServerPort:%d\n", syslogServerInfoA.minSeverity, syslogServerInfoA.remoteServerHostName, syslogServerInfoA.remoteServerPort);

	
	api3Data.rfidStatus = RFID_GetNTPServerA(api3Data.hReaderMgmt, &ntpServerInfoA);
	printf("\n RFID_GetNTPServerA Status:%s \n", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
	printf("\n ntpServer: %s\n", ntpServerInfoA.sntpHostName);

	api3Data.rfidStatus = RFID_GetLLRPConnectionStatusA(api3Data.hReaderMgmt, &llrpConStatusA);
	printf("\n RFID_GetLLRPConnectionStatusA Status:%s \n", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
	printf("\n clientIPAddress: %s Status:%s\n", llrpConStatusA.clientIPAddress, llrpConStatusA.isConnected?"Connected":"NotConnected");
}

void ExportProfileMultiBye()
{
	char ansiIPString[512];
	LOGIN_INFOA loginfo;
	PROFILE_LISTA profilelist;
	READER_SYSTEM_INFOA readerSysInfo;
	TIME_ZONE_LISTA myTimeZoneList;

	wcstombs(ansiIPString, api3Data.szIPAddress, 512);

	strcpy(loginfo.hostName, ansiIPString);
	strcpy(loginfo.userName, "admin");
	strcpy(loginfo.password, "change");
	loginfo.forceLogin = TRUE;
	loginfo.version = RFID_API3_5_5;

	int loginAttempts = 0;
	const int maxLoginAttempts = 3;
	while (loginAttempts < maxLoginAttempts) {
		api3Data.rfidStatus = RFID_LoginA(&api3Data.hReaderMgmt, &loginfo, FX, FALSE, 0);
		if (api3Data.rfidStatus == RFID_API_SUCCESS) {
			printf("\n Login Success");
			break;
		} else {
			printf("\n Login failed (attempt %d): %s", loginAttempts + 1, RFID_GetErrorDescriptionA(api3Data.rfidStatus));
			if (loginAttempts < maxLoginAttempts - 1) {
				printf("\n Retrying in 10 seconds...\n");
				rfid3Sleep(10000); // 10 seconds
			}
		}
		loginAttempts++;
	}
	if (api3Data.rfidStatus != RFID_API_SUCCESS) {
		printf("\n CRITICAL: RFID_LoginA failed after %d attempts. Aborting further operations.\n", maxLoginAttempts);
		return;
	}
	// Optional: Wait 5 seconds after successful login if required
	rfid3Sleep(5000);
	
	api3Data.rfidStatus = RFID_GetProfileListA(api3Data.hReaderMgmt, &profilelist);
	printf("\n profile List\n");
	for (int index = 0; index < profilelist.numProfiles; index++)
	{
		printf("\n Profile Index %d  Profile Name : %s", index, profilelist.pProfileName[index]);
	}

	for (int index = 1; index < profilelist.numProfiles; index++)
	{
		char profilename[256];
		char absProfileName[256];
		api3Data.rfidStatus = RFID_ImportProfileFromReaderA(api3Data.hReaderMgmt, "Default.xml", "c:\\junk\\");
		sprintf(profilename, "%s%s%d%s", "c:\\junk\\", "YourProfile", index, ".xml");
		sprintf(absProfileName, "%s%d%s", "YourProfile", index, ".xml");
		rename("c:\\junk\\Default.xml", profilename);
		api3Data.rfidStatus = RFID_ExportProfileToReaderA(api3Data.hReaderMgmt,absProfileName, "c:\\junk\\", TRUE);
	}

	api3Data.rfidStatus = RFID_SetActiveProfileA(api3Data.hReaderMgmt, "YourProfile1.xml");
	api3Data.rfidStatus = RFID_DeleteProfileA(api3Data.hReaderMgmt, "YourProfile2.xml");
	api3Data.rfidStatus = RFID_GetSystemInfoA(api3Data.hReaderMgmt, &readerSysInfo);

	printf("\n cpuUsageForSystemProcesses : %d", readerSysInfo.cpuUsageForSystemProcesses);
	printf("\n cpuUsageForSystemProcesses : %d", readerSysInfo.cpuUsageForUserProcesses);
	printf("\n cpuUsageForSystemProcesses : %d", readerSysInfo.flashAvailable);
	printf("\n cpuUsageForSystemProcesses : %s", readerSysInfo.fPGAVersion);
	printf("\n cpuUsageForSystemProcesses : %s", readerSysInfo.radioFirmwareVersion);
	printf("\n cpuUsageForSystemProcesses : %d", readerSysInfo.ramAvailable);
	printf("\n cpuUsageForSystemProcesses : %d", readerSysInfo.ramTotal);
	printf("\n cpuUsageForSystemProcesses : %d", readerSysInfo.ramUsed);
	printf("\n cpuUsageForSystemProcesses : %s", readerSysInfo.serialNumber);
	printf("\n cpuUsageForSystemProcesses : %s", readerSysInfo.readerLocation);
	printf("\n cpuUsageForSystemProcesses : %s", readerSysInfo.readerName);
	printf("\n cpuUsageForSystemProcesses : %s", readerSysInfo.upTime);

	api3Data.rfidStatus = RFID_GetTimeZoneListA(api3Data.hReaderMgmt, &myTimeZoneList);

	for (int i = 0; i < myTimeZoneList.numTimeZones; i++)
	{
		printf("\n Time Zone : %d  Name: %s", i, myTimeZoneList.pTimeZone[i]);
	}

	RFID_Logout( api3Data.hReaderMgmt );

}

/*void DeRegisterTempAlarmEvent()
{
	// If Secure Mode is enabled, initialize security Parameters. This Call fills the Security Parameters of 
	// api3Data.SecureConnectionInfo
	if (api3Data.bSecureMode == TRUE && api3Data.apiVersion == RFID_API3_5_5)
	{
		InitSecurityParameters();
		api3Data.conInfo.lpSecConInfo = &api3Data.SecureConnectionInfo;
	}

	api3Data.conInfo.version = RFID_API3_5_5;

	//api3Data.rfidStatus = RFID_ConnectA(&api3Data.hReader, "10.17.131.111", 5084, 0, &api3Data.conInfo);
	// for Windows based
	HANDLE hTempAlarmEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// EAS alarm event
	api3Data.rfidStatus = RFID_RegisterEventNotification(api3Data.hReader, TEMPERATURE_ALARM_EVENT, hTempAlarmEvent);


	if (WAIT_OBJECT_0 == WaitForSingleObject(hTempAlarmEvent, 10000))
	{
		wprintf(L"\n-------Temp alarm Event is Signalled-------");
		TEMPERATURE_ALARM_DATA myTempEventData;
		api3Data.rfidStatus = RFID_GetEventData(api3Data.hReader,TEMPERATURE_ALARM_EVENT, &myTempEventData);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			printf("\n AlamrnSource: %s AlarmLevel: %s Temperature: %d ", myTempEventData.sourceName, myTempEventData.alarmLevel, myTempEventData.currentTemperature);
		}
	}

	api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, TEMPERATURE_ALARM_EVENT);
	CloseHandle(hTempAlarmEvent);
	//RFID_Disconnect(api3Data.hReader);
}*/


void TestRFID3ANSIAPI()
{
	//ValidateRFIDLoginMultiByteMode();
	ReaderCapsInMultiByteMode();
	//ExportProfileMultiBye();
	//DeRegisterTempAlarmEvent();
	verifyDllversionInfo();
}

