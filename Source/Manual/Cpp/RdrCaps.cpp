#include "ManualTests.h"

extern struct RFID3DATA api3Data;

void DisplayCapabilities()
{
	READER_CAPS readerCaps;
	wchar_t myCOMMNSTANDARD[][32] = { L"UNSPECIFIED",L"US_FCC_PART_15",L"ETSI_302_208",L"ETSI_300_220",L"AUSTRALIA_LIPD_1W",L"AUSTRALIA_LIPD_4W",L"JAPAN_ARIB_STD_T89",L"HONGKONG_OFTA_1049",L"TAIWAN_DGT_LP0002",L"KOREA_MIC_ARTICLE_5_2" };
	CONNECTION_INFO ConInfo = { RFID_API3_5_5,0, 0 };
	wprintf(L"\n  Display Capabilities ");

	// Get the Reader Capability
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &readerCaps);

	wprintf(L"\n...RFID Reader Capabilities...");
	wprintf(L"\n readerCaps.blockEraseSupported: %d", readerCaps.blockEraseSupported);
	wprintf(L"\n readerCaps.blockWriteSupported  : %d", readerCaps.blockWriteSupported);
	wprintf(L"\n readerCaps.canDoTagInventoryStateAwareSingulation  : %d", readerCaps.stateAwareSingulationSupported);
	wprintf(L"\n readerCaps.communicationStandard	: %ls", myCOMMNSTANDARD[readerCaps.communicationStandard]);
	wprintf(L"\n readerCaps.countryCode	: %d", readerCaps.countryCode);
	wprintf(L"\n readerCaps.hasUTCClockCapability : %d", readerCaps.utcClockSupported);
	wprintf(L"\n readerCaps.hoppingEnabled	: %d", readerCaps.hoppingEnabled);
	wprintf(L"\n readerCaps.maxNumOperationsInAccessSequence : %d", readerCaps.maxNumOperationsInAccessSequence);
	wprintf(L"\n readerCaps.maxNumPreFilters : %d", readerCaps.maxNumPreFilters);
	wprintf(L"\n readerCaps.modelName : %ls ", readerCaps.modelName);
	wprintf(L"\n readerCaps.numAntennaSupported : %d ", readerCaps.numAntennas);
	wprintf(L"\n readerCaps.numGPI : %d ", readerCaps.numGPIs);
	wprintf(L"\n readerCaps.numGPO : %d ", readerCaps.numGPOs);
	wprintf(L"\n readerCaps.pFirmwareVersion : %ls ", readerCaps.firmWareVersion);
	wprintf(L"\n readerCaps.readerID.type: %d ", readerCaps.readerID.type);
	wprintf(L"\n readerCaps.readerID.value: %ls ", readerCaps.readerID.value);
	wprintf(L"\n readerCaps.writeUMISupported: %d ", readerCaps.writeUMISupported);
	wprintf(L"\n readerCaps.AntennaRFConfigSupported: %d ", readerCaps.AntennaRFConfigSupported);
	wprintf(L"\n readerCaps.FujitsuCommandsSupported: %d ", readerCaps.FujitsuCommandsSupported);
	wprintf(L"\n readerCaps.PeriodicTagReportsSupported: %d ", readerCaps.PeriodicTagReportsSupported);
	wprintf(L"\n readerCaps.radioPowerControlSupported: %d ", readerCaps.radioPowerControlSupported);
	wprintf(L"\n readerCaps.TagPhaseReportingSupported: %d ", readerCaps.TagPhaseReportingSupported);
	wprintf(L"\n readerCaps.ZoneSuppported: %d ", readerCaps.ZoneSuppported);

	if (readerCaps.lpVersions != NULL)
	{
		for (int nVersions = 0; nVersions < readerCaps.lpVersions->numVersions; nVersions++)
		{
			wprintf(L"\n readerCaps.lpVersion: %d ", readerCaps.lpVersions->numVersions);
			wprintf(L"\n readerCaps.ModuleName and version: %ls : %ls ", readerCaps.lpVersions->pVersionList->moduleName, readerCaps.lpVersions->pVersionList->moduleVersion);
		}
	}

}

static void rfidEventCallback(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	switch (eventType)
	{
	case GPI_EVENT:
		wprintf(L"\nGPI_EVENT Triggered\n");
		break;
	case TAG_DATA_EVENT:
		wprintf(L"\nTAG_DATA_EVENT Triggered\n");
		break;
	case BUFFER_FULL_WARNING_EVENT:
		wprintf(L"\nBUFFER_FULL_WARNING_EVENT Triggered\n");
		break;
	case ANTENNA_EVENT:
		wprintf(L"\nANTENNA_EVENT Triggered\n");
		break;
	case INVENTORY_START_EVENT:
		wprintf(L"\nINVENTORY_START_EVENT Triggered\n");
		break;
	case INVENTORY_STOP_EVENT:
		wprintf(L"\nINVENTORY_STOP_EVENT Triggered\n");
		break;
	case ACCESS_START_EVENT:
		wprintf(L"\nACCESS_START_EVENT Triggered\n");
		break;
	case ACCESS_STOP_EVENT:
		wprintf(L"\nACCESS_STOP_EVENT Triggered\n");
		break;
	case DISCONNECTION_EVENT:
		wprintf(L"\nDISCONNECTION_EVENT Triggered\n");
		break;
	case BUFFER_FULL_EVENT:
		wprintf(L"\nBUFFER_FULL_EVENT Triggered\n");
		break;
	case NXP_EAS_ALARM_EVENT:
		wprintf(L"\nNXP_EAS_ALARM_EVENT Triggered\n");
		break;
	case READER_EXCEPTION_EVENT:
		wprintf(L"\nREADER_EXCEPTION_EVENT Triggered\n");
		break;
	case DEBUG_INFO_EVENT:
		wprintf(L"\nDEBUG_INFO_EVENT Triggered\n");
		break;
	case TEMPERATURE_ALARM_EVENT:
		wprintf(L"\nTEMPERATURE_ALARM_EVENT Triggered\n");
		break;
	case RF_SURVEY_START_EVENT:
		wprintf(L"\nRF_SURVEY_START_EVENT Triggered\n");
		break;
	case RF_SURVEY_STOP_EVENT:
		wprintf(L"\nRF_SURVEY_STOP_EVENT Triggered\n");
		break;
	case RF_SURVEY_DATA_READ_EVENT:
	{
		wprintf(L"\nRF_SURVEY_DATA_READ_EVENT Triggered\n");
	}
	break;
	case AUTONOMOUS_EVENT:
	{
		wprintf(L"\nAUTONOMOUS_EVENT Triggered\n");
	}
	break;
	default:
		break;
	}
}

void rfid4events()
{
	TAG_STORAGE_SETTINGS tgSettings;
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} };
	RFID_EVENT_TYPE rfidEvents[18] = { GPI_EVENT,TAG_DATA_EVENT,TAG_READ_EVENT,BUFFER_FULL_WARNING_EVENT,ANTENNA_EVENT,INVENTORY_START_EVENT,INVENTORY_STOP_EVENT,	ACCESS_START_EVENT,	ACCESS_STOP_EVENT,DISCONNECTION_EVENT,BUFFER_FULL_EVENT,NXP_EAS_ALARM_EVENT,READER_EXCEPTION_EVENT,DEBUG_INFO_EVENT,TEMPERATURE_ALARM_EVENT,
	RF_SURVEY_DATA_READ_EVENT,RF_SURVEY_START_EVENT,RF_SURVEY_STOP_EVENT };
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	DisplayCapabilities();
	api3Data.rfidStatus = RFID_RegisterEventNotificationCallback(api3Data.hReader, rfidEvents, 18, (RfidEventCallbackFunction)rfidEventCallback, NULL, NULL);
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	tgSettings.maxTagCount = 5;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);
	// Inv Start and Stop
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	if(RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		//print EPC
		printTagDataWithResults(pTagData);
		//get TID memory bank
		rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
		for (int iterCnt = 0; iterCnt < 2; iterCnt++)
		{
			wprintf(L"\n Attempt %d, Reading TID memory bank", iterCnt);
			RFID_STATUS rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0);
		}
	}

	for (int eventIndex = 0; eventIndex < 18; eventIndex++)
	{
		api3Data.rfidStatus = RFID_DeregisterEventNotification(api3Data.hReader, rfidEvents[eventIndex]);
	}

	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
}

