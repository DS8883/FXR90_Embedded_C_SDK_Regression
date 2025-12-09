#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;

// Call back function for for notifications.
// Interested Events are 
// RF_SURVEY_START_EVENT
// RF_SURVEY_STOP_EVENT
// RF_SURVEY_DATA_READ_EVENT
//
static void rfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	switch (eventType)
	{
	case GPI_EVENT:
		fprintf(api3Data.fpCurLog, "\n GPI_EVENT: Triggered <br>\n");
		wprintf(L"\nGPI_EVENT Triggered");
		break;
	case TAG_DATA_EVENT:
		fprintf(api3Data.fpCurLog, "\n TAG_DATA_EVENT: Triggered <br>\n");
		wprintf(L"\nTAG_DATA_EVENT Triggered");
		break;
	case BUFFER_FULL_WARNING_EVENT:
		fprintf(api3Data.fpCurLog, "\n BUFFER_FULL_WARNING_EVENT: Triggered <br>\n");
		wprintf(L"BUFFER_FULL_WARNING_EVENT Triggered");
		break;
	case ANTENNA_EVENT:
		fprintf(api3Data.fpCurLog, "\n ANTENNA_EVENT: Triggered <br>\n");
		wprintf(L"ANTENNA_EVENT Triggered");

		break;
	case INVENTORY_START_EVENT:
		fprintf(api3Data.fpCurLog, "\n INVENTORY_START_EVENT: Triggered <br>\n");
		wprintf(L"INVENTORY_START_EVENT Triggered");
		break;
	case INVENTORY_STOP_EVENT:
		fprintf(api3Data.fpCurLog, "\n INVENTORY_STOP_EVENT: Triggered <br>\n");
		wprintf(L"INVENTORY_STOP_EVENT Triggered");
		break;
	case ACCESS_START_EVENT:
		fprintf(api3Data.fpCurLog, "\n ACCESS_START_EVENT: Triggered <br>\n");
		wprintf(L"ACCESS_START_EVENT Triggered");
		break;
	case ACCESS_STOP_EVENT:
		fprintf(api3Data.fpCurLog, "\n ACCESS_STOP_EVENT: Triggered <br>\n");
		wprintf(L"ACCESS_STOP_EVENT Triggered");
		break;
	case DISCONNECTION_EVENT:
		fprintf(api3Data.fpCurLog, "\n DISCONNECTION_EVENT: Triggered <br>\n");
		wprintf(L"DISCONNECTION_EVENT Triggered");
		break;
	case BUFFER_FULL_EVENT:
		fprintf(api3Data.fpCurLog, "\n BUFFER_FULL_EVENT: Triggered <br>\n");
		wprintf(L"BUFFER_FULL_EVENT Triggered");
		break;
	case NXP_EAS_ALARM_EVENT:
		fprintf(api3Data.fpCurLog, "\n NXP_EAS_ALARM_EVENT: Triggered <br>\n");
		wprintf(L"NXP_EAS_ALARM_EVENT Triggered");
		break;
	case READER_EXCEPTION_EVENT:
		fprintf(api3Data.fpCurLog, "\n READER_EXCEPTION_EVENT: Triggered <br>\n");
		wprintf(L"READER_EXCEPTION_EVENT Triggered");
		break;
	case DEBUG_INFO_EVENT:
		fprintf(api3Data.fpCurLog, "\n DEBUG_INFO_EVENT: Triggered <br>\n");
		wprintf(L"DEBUG_INFO_EVENT Triggered");
		break;
	case TEMPERATURE_ALARM_EVENT:
		fprintf(api3Data.fpCurLog, "\n TEMPERATURE_ALARM_EVENT: Triggered <br>\n");
		wprintf(L"TEMPERATURE_ALARM_EVENT Triggered");
		break;
	case RF_SURVEY_START_EVENT:
		fprintf(api3Data.fpCurLog, "\n RF_SURVEY_START_EVENT: Triggered <br>\n");
		wprintf(L"\nRF_SURVEY_START_EVENT Triggered");
		break;
	case RF_SURVEY_STOP_EVENT:
		fprintf(api3Data.fpCurLog, "\n RF_SURVEY_STOP_EVENT: Triggered <br>\n");
		wprintf(L"\nRF_SURVEY_STOP_EVENT Triggered");
		break;
	case RF_SURVEY_DATA_READ_EVENT:
	{
		wprintf(L"\nRF_SURVEY_DATA_READ_EVENT Triggered");
		fprintf(api3Data.fpCurLog, "\n RF_SURVEY_DATA_READ_EVENT: Triggered <br>\n");
		LPRFSURVEY_DATA pSurveyData = RFID_AllocateRFSurvey(api3Data.hReader);
		api3Data.rfidStatus = RFID_GetEventData(api3Data.hReader, RF_SURVEY_DATA_READ_EVENT, (STRUCT_HANDLE)pSurveyData);
		wprintf(L"\n Event Data : AVGpeakRSSI: %ld peakRSSI: %ld bandWidth:%ld frequency:%ld", pSurveyData->averagePeakRSSI, pSurveyData->peakRSSI, pSurveyData->bandWidth, pSurveyData->frequency);
		fprintf(api3Data.fpCurLog, "Event Data : AVGpeakRSSI: %ld peakRSSI: %ld bandWidth:%ld frequency:%ld", pSurveyData->averagePeakRSSI, pSurveyData->peakRSSI, pSurveyData->bandWidth, pSurveyData->frequency);
	}
		break;
	case AUTONOMOUS_EVENT:
		wprintf(L"AUTONOMOUS_EVENT Triggered");
		fprintf(api3Data.fpCurLog, "\n AUTONOMOUS_EVENT: Triggered <br>\n");

		break;
	default:
		break;
	}
}
// Test Antenna Config 
//
void TestAntennaConfig( )
{
	ANTENNA_RF_CONFIG antSetRFConfig, antGetRFConfig;
	LOGIN_INFO loginfo = { L"10.17.130.229",L"admin",L"change",RFID_API3_5_5,TRUE,0,0 };
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	RFSURVEY_STOP_TRIGGER rfSurveyStopTrigger;
	LPRFSURVEY_DATA pSurveyData = RFID_AllocateRFSurvey(api3Data.hReader);
	rfSurveyStopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	rfSurveyStopTrigger.value.duration = 10000;
	RFID_EVENT_TYPE rfidEvents[18] = { GPI_EVENT,TAG_DATA_EVENT,TAG_READ_EVENT,BUFFER_FULL_WARNING_EVENT,ANTENNA_EVENT,INVENTORY_START_EVENT,INVENTORY_STOP_EVENT,	ACCESS_START_EVENT,	ACCESS_STOP_EVENT,DISCONNECTION_EVENT,BUFFER_FULL_EVENT,NXP_EAS_ALARM_EVENT,READER_EXCEPTION_EVENT,DEBUG_INFO_EVENT,TEMPERATURE_ALARM_EVENT,
	RF_SURVEY_DATA_READ_EVENT,RF_SURVEY_START_EVENT,RF_SURVEY_STOP_EVENT };

	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("Test_AntennaConfig ");
	logCurrentExecution("Test_AntennaConfig");

	fprintf(api3Data.fpCurLog, "<html>\n");
	fprintf(api3Data.fpCurLog, "<body>\n");
	//api3Data.itemTestCount = 44;
	api3Data.dwTestCaseNo = 01;

	// Set the power value by reducing 20 from the Max index.
	// 
	UINT16 rfPowerMaxIndexValue = api3Data.rCaps.transmitPowerLevelTable.numValues -1;
	
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);

	for (UINT16 antIndex = 1; antIndex <= api3Data.rCaps.numAntennas; antIndex++)
	{
		// for the power table levels.
		for (int Iteration = 0; Iteration < 5; Iteration++)
		{
			fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-RFCONFIG-API-%d></a>CAPI-RFCONFIG-API-%d:\n\n<br>Description: AntennaConfig for Antenna %ld \n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, antIndex);
			fprintf(api3Data.fpCurLog, "<br>");
			fprintf(api3Data.fpCurLog, "\n Result is : \n ");
			
			if (api3Data.rfidStatus == RFID_API_INVALID_HANDLE)
			{
				CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
				api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
			}
			api3Data.rfidStatus = RFID_GetAntennaRfConfig(api3Data.hReader, antIndex, &antGetRFConfig);
			antSetRFConfig = antGetRFConfig;
			antSetRFConfig.transmitPowerIndex = rfPowerMaxIndexValue - Iteration * 20;
			api3Data.rfidStatus = RFID_SetAntennaRfConfig(api3Data.hReader, antIndex, &antSetRFConfig);
			wprintf(L"\n SetConfig: TXdbm:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld", api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antSetRFConfig.transmitPowerIndex], antSetRFConfig.tari, antSetRFConfig.receivePort, antSetRFConfig.transmitPort, antSetRFConfig.transmitFrequencyIndex);
			fprintf(api3Data.fpCurLog,"\n SetConfig: TXdbm:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld <br>\n", api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antSetRFConfig.transmitPowerIndex], antSetRFConfig.tari, antSetRFConfig.receivePort, antSetRFConfig.transmitPort, antSetRFConfig.transmitFrequencyIndex);
			api3Data.rfidStatus = RFID_GetAntennaRfConfig(api3Data.hReader, antIndex, &antGetRFConfig);
			wprintf(L"\n GetConfig: TXdbm:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld", api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antGetRFConfig.transmitPowerIndex], antGetRFConfig.tari, antGetRFConfig.receivePort, antGetRFConfig.transmitPort, antGetRFConfig.transmitFrequencyIndex);
			fprintf(api3Data.fpCurLog, "\n GetConfig: TXdbm:%ld \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld <br>\n", api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antGetRFConfig.transmitPowerIndex], antGetRFConfig.tari, antGetRFConfig.receivePort, antGetRFConfig.transmitPort, antGetRFConfig.transmitFrequencyIndex);

			if (antSetRFConfig.transmitPowerIndex == antGetRFConfig.transmitPowerIndex)
			{
				SetLogResult(TRUE);
			}
			else
			{
				SetLogResult(FALSE);
			}

			logToResultFile(L"CAPI-RFCONFIG");
		}
	}

	// Verify Save LLRP Config
	//
	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-LLRPCONFIG-API-%d></a>CAPI-LLRPCONFIG-API-%d:\n\n<br>Description: RFID_SaveLlrpConfig()\n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : \n ");
	api3Data.rfidStatus = RFID_SaveLlrpConfig(api3Data.hReader, NULL);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}
	logToResultFile(L"CAPI-RFCONFIG");


	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-LLRPCONFIG-API-%d></a>CAPI-LLRPCONFIG-API-%d:\n\n<br>RFID_GetSaveLlrpConfigStatus \n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : \n ");
	BOOLEAN pSaveStatus;
	api3Data.rfidStatus = RFID_GetSaveLlrpConfigStatus(api3Data.hReader, &pSaveStatus);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}
	fprintf(api3Data.fpCurLog, "\n RFID_GetSaveLlrpConfigStatus: %s <br>\n", pSaveStatus ? "YES" : "NO");
	logToResultFile(L"CAPI-RFCONFIG");

	// RF Surveey API
	// RFID_StartRFSurvey
	// RFID_StopRFSurvey
	// RFID_GetRFSurvey
	// Do the RF Survey on Antennas connected frequency Range: 860MHz to 930 MHz
	//

	api3Data.rfidStatus = RFID_RegisterEventNotificationCallback(api3Data.hReader, rfidEvents, 18, (RfidEventCallbackFunction)rfidEventCallback, NULL, NULL);

	for (UINT16 antenna = 1; antenna <= api3Data.rCaps.numAntennas; antenna++)
	{
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-RFSURVEY-API-%d></a>CAPI-RFSURVEY-API-%d:\n\n<br>RFSurvery on Antenna %ld \n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, antenna);
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : \n ");

		api3Data.rfidStatus = RFID_StartRFSurvey(api3Data.hReader, antenna, &rfSurveyStopTrigger, 860000, 928000, NULL);
		rfid3Sleep(12000);
		api3Data.rfidStatus = RFID_StopRFSurvey(api3Data.hReader);

		api3Data.rfidStatus = RFID_GetRFSurvey(api3Data.hReader, pSurveyData);
		wprintf(L"API Result is: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			SetLogResult(TRUE);
		}
		else
		{
			SetLogResult(FALSE);
		}

		logToResultFile(L"CAPI-RFSURVEY");
	}

	//  De-Register Events one by one. as there is no API to deregister all at once.
	// 
	for (int eventIndex = 0; eventIndex < 18; eventIndex++)
	{
		api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, rfidEvents[eventIndex]);
	}
	

	// Set NTP Server
	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-NTPCONFIG-API-%d></a>CAPI-NTPCONFIG-API-%d:\n\n<br>RFID_SetNTPServer:time.google.com \n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : \n ");
	TIME_SERVER_INFO ntpServerInfo = { L"time.google.com",{0} };
	api3Data.rfidStatus = RFID_SetNTPServer(api3Data.hReaderMgmt, &ntpServerInfo);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}
	fprintf(api3Data.fpCurLog, "\n RFID_SetNTPServer: time.google.com <br>\n");
	logToResultFile(L"CAPI-NTPCONFIG");
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, FALSE, NULL);
	api3Data.rfidStatus = RFID_GetNTPServer(api3Data.hReaderMgmt, &ntpServerInfo);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	fprintf(api3Data.fpCurLog, "\n RFID_GetNTPServer:%ls <br>\n",ntpServerInfo.sntpHostName);
	logToResultFile(L"CAPI-NTPCONFIG");

	TEMPERATURE_STATUSW tempStatus = { 0,0 };
	api3Data.rfidStatus = RFID_GetTemperatureStatus(api3Data.hReaderMgmt, &tempStatus);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	fprintf(api3Data.fpCurLog, "\n RFID_GetTemperatureStatus: paTemp:%f ambientTemp:%f <br>\n", tempStatus.paTemp,tempStatus.ambientTemp);
	logToResultFile(L"CAPI-TEMPSTATUS");


	SYSLOG_SERVER_INFO sysLogServer;
	api3Data.rfidStatus = RFID_GetSysLogServer(api3Data.hReaderMgmt, &sysLogServer);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}
	fprintf(api3Data.fpCurLog, "\n RFID_GetSysLogServer: ServerName:%ls Port:%ld Severity:%ld <br>\n", sysLogServer.remoteServerHostName, sysLogServer.remoteServerPort, sysLogServer.minSeverity);
	logToResultFile(L"CAPI-SYSLOGSERVER");

	wcscpy(sysLogServer.remoteServerHostName, L"10.17.131.48");
	sysLogServer.remoteServerPort = 514;
	sysLogServer.minSeverity = 1;
	api3Data.rfidStatus = RFID_SetSysLogServer(api3Data.hReaderMgmt, &sysLogServer);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}
	fprintf(api3Data.fpCurLog, "\n RFID_SetSysLogServer: ServerName:%ls Port:%ld Severity:%ld <br>\n", sysLogServer.remoteServerHostName, sysLogServer.remoteServerPort, sysLogServer.minSeverity);
	logToResultFile(L"CAPI-SYSLOGSERVER");

	fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ", MultiByte2WideChar(__FUNCTION__));
}