#include "ManualTests.h"
extern struct RFID3DATA api3Data;

// Test the newly added API list.
//RFID_SaveLlrpConfig
//RFID_GetSaveLlrpConfigStatus
//RfidEventCallbackFunction
//RFID_RegisterEventNotificationCallback
//RFID_SetEventNotificationParams
//RFID_StartRFSurvey
//RFID_StopRFSurvey
//RFID_GetRFSurvey
//
//RFID_AcquireLicenseOnLine - FX readers new functionality.
//
//RFID_UpdateRadioConfig - Not Supported for FXand XR readers.
//
//RFID_SetNTPServer
//RFID_GetNTPServerW
//RFID_SetSysLogServerW
//RFID_GetSysLogServerW
//
//time.google.com
//
//time2.google.com
//
//RFID_SetWirelessNetworkW
//RFID_GetWirelessConfigParametersW
//
//RFID_GetTemperatureStatusW


void rfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	switch (eventType)
	{
		case RF_SURVEY_START_EVENT:
			wprintf(L"RF_SURVEY_START_EVENT Triggered");
			break;
		case RF_SURVEY_STOP_EVENT:
			wprintf(L"RF_SURVEY_STOP_EVENT Triggered");
			break;
		case RF_SURVEY_DATA_READ_EVENT:
			wprintf(L"RF_SURVEY_DATA_READ_EVENT Triggered");
			LPRFSURVEY_DATA pSurveyData = RFID_AllocateRFSurvey(api3Data.hReader);
			api3Data.rfidStatus = RFID_GetEventData(api3Data.hReader, RF_SURVEY_DATA_READ_EVENT, (STRUCT_HANDLE)pSurveyData);
			wprintf(L"AVGpeakRSSI: %ld peakRSSI: %ld bandWidth:%ld frequency:%ld",pSurveyData->averagePeakRSSI, pSurveyData->peakRSSI, pSurveyData->bandWidth, pSurveyData->frequency);
			break;
	}
}



void TestNewAPIadditions()
{
	LOGIN_INFO loginfo = { L"10.17.130.229",L"admin",L"change",RFID_API3_5_5,TRUE,0,0 };
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	BOOLEAN pSaveStatus;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);

	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
	api3Data.rfidStatus = RFID_SaveLlrpConfig(api3Data.hReader, NULL);
	wprintf(L"\n RFID_SaveLlrpConfig: %ls \n", RFID_GetErrorDescription(api3Data.rfidStatus));
	api3Data.rfidStatus = RFID_GetSaveLlrpConfigStatus(api3Data.hReader, &pSaveStatus);
	wprintf(L"\n RFID_GetSaveLlrpConfigStatus: %ls \n", pSaveStatus ? L"YES" : L"NO");
	

	/*RfidEventCallbackFunction
	RFID_RegisterEventNotificationCallback
	RFID_SetEventNotificationParams*/
	wprintf(L"\n RFID_ACCESS_G2V2_AUTHENTICATE_FAILED : %ls", RFID_GetErrorDescription(RFID_ACCESS_G2V2_AUTHENTICATE_FAILED));

	RFSURVEY_STOP_TRIGGER rfSurveyStopTrigger;
	//LPRFSURVEY_DATA pSurveyData = RFID_AllocateRFSurvey(api3Data.hReader);
	rfSurveyStopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	rfSurveyStopTrigger.value.duration = 5000;
	RFID_EVENT_TYPE rfidEvents[18] = { GPI_EVENT,TAG_DATA_EVENT,TAG_READ_EVENT,BUFFER_FULL_WARNING_EVENT,ANTENNA_EVENT,INVENTORY_START_EVENT,INVENTORY_STOP_EVENT,	ACCESS_START_EVENT,	ACCESS_STOP_EVENT,DISCONNECTION_EVENT,BUFFER_FULL_EVENT,NXP_EAS_ALARM_EVENT,READER_EXCEPTION_EVENT,DEBUG_INFO_EVENT,TEMPERATURE_ALARM_EVENT,
	RF_SURVEY_DATA_READ_EVENT,RF_SURVEY_START_EVENT,RF_SURVEY_STOP_EVENT };// , DEBUG_INFO_EVENT, TEMPERATURE_ALARM_EVENT, AUTONOMOUS_EVENT
	
	api3Data.rfidStatus = RFID_RegisterEventNotificationCallback(api3Data.hReader, rfidEvents, 18, (RfidEventCallbackFunction)rfidEventCallback, NULL, NULL);

	for (UINT16 antenna=4; antenna < api3Data.rCaps.numAntennas; antenna++)
	{
		api3Data.rfidStatus = RFID_StartRFSurvey(api3Data.hReader, antenna, &rfSurveyStopTrigger, 860000, 928000, NULL);
		rfid3Sleep(10000);
		api3Data.rfidStatus = RFID_StopRFSurvey(api3Data.hReader);

		//api3Data.rfidStatus = RFID_GetRFSurvey(api3Data.hReader, pSurveyData);
		//wprintf(L"API Result is: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		//wprintf(L"Antenna: %ldAVGpeakRSSI: %ld peakRSSI: %ld bandWidth:%ld frequency:%ld", antenna, pSurveyData->averagePeakRSSI, pSurveyData->peakRSSI, pSurveyData->bandWidth, pSurveyData->frequency);

	}
	
	api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, RF_SURVEY_START_EVENT);
	api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, RF_SURVEY_DATA_READ_EVENT);
	api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, RF_SURVEY_STOP_EVENT);

	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);




}