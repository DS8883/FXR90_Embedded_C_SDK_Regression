#include "ManualTests.h"
#include <list>
extern struct RFID3DATA api3Data;	

// Channel Masks for Nordic and Non Nordic Countries
// 
// 
static int SevenChannelMask[127][7] =
{
{0,0,0,0,0,0,1}, {0,0,0,0,0,1,0}, {0,0,0,0,0,1,1}, {0,0,0,0,1,0,0}, {0,0,0,0,1,0,1}, {0,0,0,0,1,1,0}, {0,0,0,0,1,1,1}, {0,0,0,1,0,0,0}, {0,0,0,1,0,0,1}, {0,0,0,1,0,1,0},
{0,0,0,1,0,1,1}, {0,0,0,1,1,0,0}, {0,0,0,1,1,0,1}, {0,0,0,1,1,1,0}, {0,0,0,1,1,1,1}, {0,0,1,0,0,0,0}, {0,0,1,0,0,0,1}, {0,0,1,0,0,1,0}, {0,0,1,0,0,1,1}, {0,0,1,0,1,0,0},
{0,0,1,0,1,0,1}, {0,0,1,0,1,1,0}, {0,0,1,0,1,1,1}, {0,0,1,1,0,0,0}, {0,0,1,1,0,0,1}, {0,0,1,1,0,1,0}, {0,0,1,1,0,1,1}, {0,0,1,1,1,0,0}, {0,0,1,1,1,0,1}, {0,0,1,1,1,1,0},
{0,0,1,1,1,1,1}, {0,1,0,0,0,0,0}, {0,1,0,0,0,0,1}, {0,1,0,0,0,1,0}, {0,1,0,0,0,1,1}, {0,1,0,0,1,0,0}, {0,1,0,0,1,0,1}, {0,1,0,0,1,1,0}, {0,1,0,0,1,1,1}, {0,1,0,1,0,0,0},
{0,1,0,1,0,0,1}, {0,1,0,1,0,1,0}, {0,1,0,1,0,1,1}, {0,1,0,1,1,0,0}, {0,1,0,1,1,0,1}, {0,1,0,1,1,1,0}, {0,1,0,1,1,1,1}, {0,1,1,0,0,0,0}, {0,1,1,0,0,0,1}, {0,1,1,0,0,1,0},
{0,1,1,0,0,1,1}, {0,1,1,0,1,0,0}, {0,1,1,0,1,0,1}, {0,1,1,0,1,1,0}, {0,1,1,0,1,1,1}, {0,1,1,1,0,0,0}, {0,1,1,1,0,0,1}, {0,1,1,1,0,1,0}, {0,1,1,1,0,1,1}, {0,1,1,1,1,0,0},
{0,1,1,1,1,0,1}, {0,1,1,1,1,1,0}, {0,1,1,1,1,1,1}, {1,0,0,0,0,0,0}, {1,0,0,0,0,0,1}, {1,0,0,0,0,1,0}, {1,0,0,0,0,1,1}, {1,0,0,0,1,0,0}, {1,0,0,0,1,0,1}, {1,0,0,0,1,1,0},
{1,0,0,0,1,1,1}, {1,0,0,1,0,0,0}, {1,0,0,1,0,0,1}, {1,0,0,1,0,1,0}, {1,0,0,1,0,1,1}, {1,0,0,1,1,0,0}, {1,0,0,1,1,0,1}, {1,0,0,1,1,1,0}, {1,0,0,1,1,1,1}, {1,0,1,0,0,0,0},
{1,0,1,0,0,0,1}, {1,0,1,0,0,1,0}, {1,0,1,0,0,1,1}, {1,0,1,0,1,0,0}, {1,0,1,0,1,0,1}, {1,0,1,0,1,1,0}, {1,0,1,0,1,1,1}, {1,0,1,1,0,0,0}, {1,0,1,1,0,0,1}, {1,0,1,1,0,1,0},
{1,0,1,1,0,1,1}, {1,0,1,1,1,0,0}, {1,0,1,1,1,0,1}, {1,0,1,1,1,1,0}, {1,0,1,1,1,1,1}, {1,1,0,0,0,0,0}, {1,1,0,0,0,0,1}, {1,1,0,0,0,1,0}, {1,1,0,0,0,1,1}, {1,1,0,0,1,0,0},
{1,1,0,0,1,0,1}, {1,1,0,0,1,1,0}, {1,1,0,0,1,1,1}, {1,1,0,1,0,0,0}, {1,1,0,1,0,0,1}, {1,1,0,1,0,1,0}, {1,1,0,1,0,1,1}, {1,1,0,1,1,0,0}, {1,1,0,1,1,0,1}, {1,1,0,1,1,1,0},
{1,1,0,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,0,0,0,1}, {1,1,1,0,0,1,0}, {1,1,1,0,0,1,1}, {1,1,1,0,1,0,0}, {1,1,1,0,1,0,1}, {1,1,1,0,1,1,0}, {1,1,1,0,1,1,1}, {1,1,1,1,0,0,0},
{1,1,1,1,0,0,1}, {1,1,1,1,0,1,0}, {1,1,1,1,0,1,1}, {1,1,1,1,1,0,0}, {1,1,1,1,1,0,1}, {1,1,1,1,1,1,0}, {1,1,1,1,1,1,1}
};

static int SixChannelMask[63][6] =
{
{0,0,0,0,0,1}, {0,0,0,0,1,0}, {0,0,0,0,1,1}, {0,0,0,1,0,0}, {0,0,0,1,0,1}, {0,0,0,1,1,0}, {0,0,0,1,1,1}, {0,0,1,0,0,0}, {0,0,1,0,0,1}, {0,0,1,0,1,0},
{0,0,1,0,1,1}, {0,0,1,1,0,0}, {0,0,1,1,0,1}, {0,0,1,1,1,0}, {0,0,1,1,1,1}, {0,1,0,0,0,0}, {0,1,0,0,0,1}, {0,1,0,0,1,0}, {0,1,0,0,1,1}, {0,1,0,1,0,0},
{0,1,0,1,0,1}, {0,1,0,1,1,0}, {0,1,0,1,1,1}, {0,1,1,0,0,0}, {0,1,1,0,0,1}, {0,1,1,0,1,0}, {0,1,1,0,1,1}, {0,1,1,1,0,0}, {0,1,1,1,0,1}, {0,1,1,1,1,0},
{0,1,1,1,1,1}, {1,0,0,0,0,0}, {1,0,0,0,0,1}, {1,0,0,0,1,0}, {1,0,0,0,1,1}, {1,0,0,1,0,0}, {1,0,0,1,0,1}, {1,0,0,1,1,0}, {1,0,0,1,1,1}, {1,0,1,0,0,0},
{1,0,1,0,0,1}, {1,0,1,0,1,0}, {1,0,1,0,1,1}, {1,0,1,1,0,0}, {1,0,1,1,0,1}, {1,0,1,1,1,0}, {1,0,1,1,1,1}, {1,1,0,0,0,0}, {1,1,0,0,0,1}, {1,1,0,0,1,0},
{1,1,0,0,1,1}, {1,1,0,1,0,0}, {1,1,0,1,0,1}, {1,1,0,1,1,0}, {1,1,0,1,1,1}, {1,1,1,0,0,0}, {1,1,1,0,0,1}, {1,1,1,0,1,0}, {1,1,1,0,1,1}, {1,1,1,1,0,0},
{1,1,1,1,0,1}, {1,1,1,1,1,0}, {1,1,1,1,1,1}
};

// Channel Mask for India ,EU etc
//
static int FourChannelMask[15][4] =
{
	{0,0,0,1}, {0,0,1,0}, {0,0,1,1}, {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1}, {1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1}, {1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}
};

static int ThreeChannelMask[7][3] =
{
	{0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}
};

// Get Tag EPC starting with BEDD for Write Operations.
//
BOOLEAN getTag2PerformAccessOperations(LPTAG_DATA pTag2Write )
{
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,NULL,NULL);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	if( (api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader,pTag2Write ) ) == RFID_API_SUCCESS )
	{
		wprintf(L"\n Successfully acquired pTag2Write");
		return TRUE;
	}
	else
	{
		wprintf(L"\n Fail 2 acquire pTag2Write");
		return FALSE;
	}
	return FALSE;
}

long DoInventory( )
{
	READER_CAPS rCaps;
	long TagCount = 0;
	LPTAG_DATA pInventoriedTag = RFID_AllocateTag(api3Data.hReader);
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &rCaps);
	wprintf(L"\n CountryCode : %ld\t CountryName: %ls\n", rCaps.countryCode, GetCountryNamebyCode(rCaps.countryCode));
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	while((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pInventoriedTag)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(pInventoriedTag);
		TagCount++;
	}
	api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader, pInventoriedTag);
	return TagCount;
}

BOOLEAN TestAccessModesPX(FILE* fp, int PowerWattage)
{
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	UINT32 indexInPowerTable;
	UINT32 gModeIndex, gTari;
	UINT16 pRxIndex, pTxIndex, pTxFIndex;

	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	// Set to Half Watt and Do the access operations on all readers
	indexInPowerTable = getPowerIndex(PowerWattage, api3Data.rCaps);

	for (UINT32 modeIndex = 0; modeIndex < api3Data.rCaps.rfModes.pUHFTables->numEntries; modeIndex++)
	{

		//Set antenna Config to all antennas with the Tx Power and check if a perituclr RFIndex with the 
		// Combination of Power level reads the tags or not.
		//
		if (api3Data.rCaps.hoppingEnabled) // Set the config only if hopping enabled.
		{
			api3Data.rfidStatus = RFID_SetAntennaConfig(api3Data.hReader, 0, 0, indexInPowerTable, api3Data.rCaps.freqHopInfo.pFreqTables->hopTableID);
		}
		api3Data.rfidStatus = RFID_GetAntennaConfig(api3Data.hReader, 1, &pRxIndex, &pTxIndex, &pTxFIndex);
		wprintf(L"\n pRXIndex :%ld  pTXIndex:%ld  pTXFIndex:%ld", pRxIndex, pTxIndex, pTxFIndex);

		api3Data.rfidStatus = RFID_SetRFMode(api3Data.hReader, 0, modeIndex, 0);
		api3Data.rfidStatus = RFID_GetRFMode(api3Data.hReader, 1, &gModeIndex, &gTari);
		wprintf(L"\n Set Index :%ld \t getIndex:%ld", modeIndex, gModeIndex);

		api3Data.rfidStatus = RFID_GetRFMode(api3Data.hReader, 1, &gModeIndex, &gTari);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			UINT16 TagCount = DoInventory();
			//wprintf(L"\n  Modulation : %d FwdLinkModulation: %d SpectralMask: %d",api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].modulation,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].forwardLinkModulationType,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].spectralMaskIndicator);
			wprintf(L"\n CountryCode : %ld Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t ModeIndex:%d\t maxTari: %d minTari: %d \tTagsRead: %ld",api3Data.rCaps.countryCode,
				wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].modulation],
				wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].forwardLinkModulationType],
				wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].spectralMaskIndicator],
				api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[indexInPowerTable], gModeIndex, api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].maxTariValue, api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].minTariValue, TagCount);

			fwprintf(fp,L"\n CountryCode : %ld Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t ModeIndex:%d\t maxTari: %d minTari: %d \tTagsRead: %ld", api3Data.rCaps.countryCode,
				wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].modulation],
				wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].forwardLinkModulationType],
				wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].spectralMaskIndicator],
				api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[indexInPowerTable], gModeIndex, api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].maxTariValue, api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].minTariValue, TagCount);	
		}
		else
		{
			wprintf(L"\n  APIRESULT:%ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		}
	}

	return TRUE;
}

// Test Access Reader: Write : Block Write  : Block Erase Operations through iterating through Profiles at certain Power Level
//
BOOLEAN TestAccessModesFX( FILE* fp, int PowerWattage )
{
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	UINT32 indexInPowerTable;
	UINT32 gModeIndex, gTari;

	
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
	
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	// Set to Half Watt and Do the access operations on all readers
	indexInPowerTable = getPowerIndex(PowerWattage, api3Data.rCaps);

	if (api3Data.rCaps.hoppingEnabled) // Set the config only if hopping enabled.
	{
		api3Data.rfidStatus = RFID_SetAntennaConfig(api3Data.hReader, 0, 0, indexInPowerTable, api3Data.rCaps.freqHopInfo.pFreqTables->hopTableID);
	}

	api3Data.rfidStatus = RFID_GetRFMode(api3Data.hReader, 1, &gModeIndex, &gTari);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		UINT16 TagCount = DoInventory();

		//wprintf(L"\n  Modulation : %d FwdLinkModulation: %d SpectralMask: %d",api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].modulation,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].forwardLinkModulationType,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].spectralMaskIndicator);
		wprintf(L"\n Time : %ls CountryCode : %ld Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t ModeIndex:%d\t TagsRead: %ld", PrintTime(),api3Data.rCaps.countryCode,
			wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].modulation],
			wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].forwardLinkModulationType],
			wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].spectralMaskIndicator],
			api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[indexInPowerTable], gModeIndex, TagCount);
		fwprintf(fp,L"\n Time : %ls CountryCode : %ld Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t ModeIndex:%d\t TagsRead: %ld", PrintTime(), api3Data.rCaps.countryCode,
			wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].modulation],
			wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].forwardLinkModulationType],
			wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].spectralMaskIndicator],
			api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[indexInPowerTable], gModeIndex, TagCount);
	}
	else
	{
		wprintf(L"\n  APIRESULT:%ls", RFID_GetErrorDescription(api3Data.rfidStatus));
	}

	//if (getTag2PerformAccessOperations(pTagData))
	//{
	//	UINT16 pRxIndex, pTxIndex, pTxFIndex;
	//	BLOCK_ERASE_ACCESS_PARAMS blkEraseParams = { MEMORY_BANK_USER,0,8,0,0 };
	//	UINT8 Data2Write[] = { 0xDA,0xDA,0xCA,0xFE,0xDA,0xDA,0xCA,0xFE };
	//	WRITE_ACCESS_PARAMS wParams = { MEMORY_BANK_USER,0,Data2Write,8,0 };
	//	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_USER,0,8,0 };
	//	RFID_STATUS blkEraseStatus, blkWriteStatus, rfidReadStatus;

	//	

	//	//for (UINT32 modeIndex = 0; modeIndex < api3Data.rCaps.rfModes.pUHFTables->numEntries; modeIndex++)
	//	//{
	//	//	//Set antenna Config to all antennas with the Tx Power and check if a perituclr RFIndex with the 
	//	//	// Combination of Power level reads the tags or not.
	//	//	//
	//	//	if (api3Data.rCaps.hoppingEnabled) // Set the config only if hopping enabled.
	//	//	{
	//	//		api3Data.rfidStatus = RFID_SetAntennaConfig(api3Data.hReader, 0, 0, indexInPowerTable, api3Data.rCaps.freqHopInfo.pFreqTables->hopTableID);
	//	//	}
	//	//	api3Data.rfidStatus = RFID_GetAntennaConfig(api3Data.hReader, 1, &pRxIndex, &pTxIndex, &pTxFIndex);
	//	//	wprintf(L"\n pRXIndex :%ld  pTXIndex:%ld  pTXFIndex:%ld", pRxIndex, pTxIndex, pTxFIndex);

	//	//	api3Data.rfidStatus = RFID_SetRFMode(api3Data.hReader, 0, modeIndex, 0);
	//		api3Data.rfidStatus = RFID_GetRFMode(api3Data.hReader, 1, &gModeIndex, &gTari);
	//	//	wprintf(L"\n Set Index :%ld \t getIndex:%ld", modeIndex, gModeIndex);

	//		blkEraseStatus = RFID_BlockErase(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &blkEraseParams, NULL, NULL, 0);
	//		blkWriteStatus = RFID_BlockWrite(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &wParams, NULL, NULL, 0);;
	//		rfidReadStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0);
	//		printTagDataWithResults(pTagData);

	//		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);

	//		//wprintf(L"\n  Modulation : %d FwdLinkModulation: %d SpectralMask: %d",api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].modulation,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].forwardLinkModulationType,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].spectralMaskIndicator);
	//		wprintf(L"\n Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t ModeIndex:%d\t blkErase %ls\t blkWrite: %ls\t read:%ls",
	//			wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].modulation],
	//			wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].forwardLinkModulationType],
	//			wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].spectralMaskIndicator],
	//			api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[indexInPowerTable], gModeIndex, blkEraseStatus ? L":FAIL":L":SUCCESS",blkWriteStatus ? L":FAIL" : L":SUCCESS",rfidReadStatus ? L":FAIL" : L":SUCCESS");

	//			// Log the Failure to read the tags in any RFMode setting

	//			fwprintf(fp, L"\n Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t MInd:%d \t  blkErase %ld\t blkWrite: %ld\t read:%ld",
	//				wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].modulation],
	//				wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].forwardLinkModulationType],
	//				wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[gModeIndex].spectralMaskIndicator],
	//				api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[indexInPowerTable], gModeIndex, blkEraseStatus, blkWriteStatus, rfidReadStatus);


	//	/*}*/
	//}
	//else
	//{
	//	wprintf(L"\n Error getting the Tag to Wrtie");
	//}

	
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	

	return TRUE;
}

BOOLEAN accessWithRFModesFX(RFID_VERSION rfidVersion)
{
	REGION_LIST regionList;
	STANDARD_INFO_LIST StandardInfo;
	ACTIVE_REGION_INFO activeRegionInfo;
	STANDARD_REGION_INFO standardRgnInfo;
	UINT32 ChannelCount = 0;
	LOGIN_INFO loginfo;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;

	FILE* fp = fopen("accessInModes.txt", "w");
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, FALSE, 0);
	api3Data.rfidStatus = RFID_GetSupportedRegionList(api3Data.hReaderMgmt, &regionList);
	for (int i = 0; i < regionList.numRegions; i++)
	{
		wprintf(L"\n\n %d. RegionName : %ls \n", i, regionList.RegionNamesList[i]);
	}
	// Iterate through all the regions and verify RF Modes.
	// execute it on single region for all modes.
	for (int regionIndex = 8; regionIndex < regionList.numRegions; regionIndex++)
	{
		api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, FALSE, 0);
		api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);
		
		for (int standards = 0; standards < StandardInfo.numStds; standards++)
		{
			api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);

			ChannelCount = StandardInfo.StdsList[standards].iNumChannels;
			if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 3)
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
				
				for (int MaskIndex = 0; MaskIndex < 7; MaskIndex++)
				{
					CHANNEL_LIST ChannelTable;
					int ChannelIndex = 0;
					
					ChannelTable.iNumChannels = ThreeChannelMask[MaskIndex][0] + ThreeChannelMask[MaskIndex][1] + ThreeChannelMask[MaskIndex][2];
					
					
					ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];
					
					if (ThreeChannelMask[MaskIndex][0] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 1;
					}

					if (ThreeChannelMask[MaskIndex][1] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 2;
					}

					if (ThreeChannelMask[MaskIndex][2] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 3;
					}
					
					if (ChannelTable.iNumChannels > 1)
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
					}
					else
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
					}

					api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);

					for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
					{
						wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
					}

					wprintf(L"\n Time: %ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					fwprintf(fp, L"\n Time : %ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					
					TestAccessModesFX(fp,2400); //Wattage of Power to read
					
					delete ChannelTable.ChannelList;
				}
								
			}
			else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 4)
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

				for (int MaskIndex = 0; MaskIndex < 15; MaskIndex++)
				{
					CHANNEL_LIST ChannelTable;
					int ChannelIndex = 0;

					ChannelTable.iNumChannels = FourChannelMask[MaskIndex][0] + FourChannelMask[MaskIndex][1] + FourChannelMask[MaskIndex][2] + FourChannelMask[MaskIndex][3];


					ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

					if (FourChannelMask[MaskIndex][0] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 1;
					}

					if (FourChannelMask[MaskIndex][1] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 2;
					}

					if (FourChannelMask[MaskIndex][2] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 3;
					}

					if (FourChannelMask[MaskIndex][3] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 4;
					}

					if (ChannelTable.iNumChannels > 1)
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
					}
					else
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
					}

					api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
										
					for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
					{
						wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
					}

					wprintf(L"\n Time:%ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					fwprintf(fp, L"\n Time :%ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

					TestAccessModesFX(fp, 2700); //Wattage of Power to read

					delete ChannelTable.ChannelList;
				}

			}
			else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 6)
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

				for (int MaskIndex = 0; MaskIndex < 63; MaskIndex++)
				{
					CHANNEL_LIST ChannelTable;
					int ChannelIndex = 0;

					ChannelTable.iNumChannels = SixChannelMask[MaskIndex][0] + SixChannelMask[MaskIndex][1] + SixChannelMask[MaskIndex][2] + SixChannelMask[MaskIndex][3]+ SixChannelMask[MaskIndex][4] + SixChannelMask[MaskIndex][5];


					ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

					if (SixChannelMask[MaskIndex][0] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 1;
					}

					if (SixChannelMask[MaskIndex][1] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 2;
					}

					if (SixChannelMask[MaskIndex][2] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 3;
					}

					if (SixChannelMask[MaskIndex][3] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 4;
					}
					if (SixChannelMask[MaskIndex][4] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 5;
					}
					if (SixChannelMask[MaskIndex][5] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 6;
					}

					if (ChannelTable.iNumChannels > 1)
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
					}
					else
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
					}

					api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);

					for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
					{
						wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
					}

					wprintf(L"\n Time : %ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					fwprintf(fp, L"\n Time : %ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

					TestAccessModesFX(fp, 2700); //Wattage of Power to read

					delete ChannelTable.ChannelList;
				}
			}
				// Hopping enabled and frequencies selected 
			else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 7)
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

				for (int MaskIndex = 0; MaskIndex < 127; MaskIndex++)
				{
					CHANNEL_LIST ChannelTable;
					int ChannelIndex = 0;

					ChannelTable.iNumChannels = SevenChannelMask[MaskIndex][0] + SevenChannelMask[MaskIndex][1] + SevenChannelMask[MaskIndex][2] + SevenChannelMask[MaskIndex][3] + SevenChannelMask[MaskIndex][4] + SevenChannelMask[MaskIndex][5] + SevenChannelMask[MaskIndex][6];


					ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

					if (SevenChannelMask[MaskIndex][0] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 1;
					}

					if (SevenChannelMask[MaskIndex][1] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 2;
					}

					if (SevenChannelMask[MaskIndex][2] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 3;
					}

					if (SevenChannelMask[MaskIndex][3] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 4;
					}
					if (SevenChannelMask[MaskIndex][4] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 5;
					}
					if (SevenChannelMask[MaskIndex][5] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 6;
					}
					if (SevenChannelMask[MaskIndex][6] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 7;
					}

					if (ChannelTable.iNumChannels > 1)
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
					}
					else
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
					}

					api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);

					for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
					{
						wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
					}

					wprintf(L"\n time : %ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					fwprintf(fp, L"\n time %ls Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

					TestAccessModesFX(fp, 2700); //Wattage of Power to read

					delete ChannelTable.ChannelList;
				}


			}
			else
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

				wprintf(L"\n RFID_SetActiveRegion Status:%ls RegionName:%ls StandardName:%ls", RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
				for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
				{
					wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
				}
				wprintf(L"\n Time: %ls RFID_GetActiveRegionInfo Status:%ls RegionName:%ls StandardName:%ls", PrintTime(),RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				fwprintf(fp, L"\n time %ls \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				TestAccessModesFX(fp,2400);
			}
		}
				
	}

	fflush(fp);
	fclose(fp);

	return TRUE;
}
#ifdef PIXIE


BOOLEAN RegulatoryTesting(RFID_VERSION rfidVersion)
{
	REGION_LIST regionList;
	wchar_t pMyRegionList[96][128];
	STANDARD_INFO_LIST StandardInfo;
	ACTIVE_REGION_INFO activeRegionInfo;
	STANDARD_REGION_INFO standardRgnInfo;
	BOOLEAN bexit = FALSE;
	UINT32 ChannelCount = 0;
	LOGIN_INFO loginfo;
	UINT16 numRegions;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	char charArray[2];
	int numChannels;
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	FILE* fp = fopen("accessInModes.txt", "w");
	//api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, PX, FALSE, 0);
	//api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, L"UNITED ARAB EMIRATES", L"UNITED ARAB EMIRATES");
	api3Data.rfidStatus = RFID_GetSupportedRegionList(api3Data.hReaderMgmt, &regionList);
	numRegions = regionList.numRegions;
	for (int i = 0; i < numRegions; i++)
	{
		memcpy(pMyRegionList[i], regionList.RegionNamesList[i], 128);
		wprintf(L"\n\n %d. RegionName : %ls \n", i, pMyRegionList[i]);
	}
	while (!bexit)
	{
		//api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
		//api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);

		// Iterate through all the regions and verify RF Modes.
		// execute it on single region for all modes.
		int regionIndex = 0;
		int standardIndex = 0;
		wprintf(L"\n Select the Country Code you want to Test:");
		wscanf(L"%ld", &regionIndex);
		wprintf(L"\n regionIndex:%ld", regionIndex);
		
		api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);
		if (StandardInfo.numStds == 1)
		{
			api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], StandardInfo.StdsList[0].strStandardName);
		}
		else if (StandardInfo.numStds > 1)
		{
			for (int standardlist = 0; standardlist < StandardInfo.numStds; standardlist++)
			{
				wprintf(L"\n Standard %ld %ls", standardlist, StandardInfo.StdsList[standardlist].strStandardName);
			}
			
			wprintf(L"\n Select a  Satandard you want to Set:");
			wscanf(L"%ld", &standardIndex);

			api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], StandardInfo.StdsList[standardIndex].strStandardName);
		}

		api3Data.rfidStatus = RFID_GetActiveRegionStandardInfo( api3Data.hReaderMgmt,&standardRgnInfo);


		if (standardRgnInfo.bIsHoppingConfigurable)
		{
			wprintf(L"\n Select the Channels you want: ");
			for (int chIndex = 0; chIndex < standardRgnInfo.iNumChannels; chIndex++)
			{
				wprintf(L"\nChannel %d Frequency %d MHz", standardRgnInfo.piChannelInfo[chIndex], standardRgnInfo.piChannelValueInfo[chIndex]);
			}
			wprintf(L"\n");
			CHANNEL_LIST ChannelTable;
			wprintf(L"\n Input Number of Channels you want to Select:\n");
			wscanf(L"%ld", &numChannels);
			ChannelTable.iNumChannels = numChannels;
			ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

			for (int ChannelList = 0; ChannelList < numChannels; ChannelList++)
			{
				wprintf(L"\n Select Channel %ld:", ChannelList);
				wscanf(L"%ld", &ChannelTable.ChannelList[ChannelList]);
			}
			wprintf(L"\n Channels you Selected\n");

			for (int i = 0; i < ChannelTable.iNumChannels; i++)
			{
				wprintf(L"\n Channel %ld\n", ChannelTable.ChannelList[i]);
			}


			if (ChannelTable.iNumChannels > 1)
			{
				api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
			}
			else
			{
				api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
			}
										
		}
		
		
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			wprintf(L"\n Region Set Successfully \n");
		}
		//DoInventory();
		/*api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
		rfid3Sleep(10000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);*/
	}
	return TRUE;
}
BOOLEAN accessWithRFModesPXI(RFID_VERSION rfidVersion)
{
	REGION_LIST regionList;
	wchar_t pMyRegionList[96][128];
	STANDARD_INFO_LIST StandardInfo;
	ACTIVE_REGION_INFO activeRegionInfo;
	BOOLEAN bexit = FALSE;
	UINT32 ChannelCount = 0;
	LOGIN_INFO loginfo;
	UINT16 numRegions;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	FILE* fp = fopen("accessInModes.txt", "w");
	//api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, PX, FALSE, 0); 
	//api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, L"UNITED ARAB EMIRATES", L"UNITED ARAB EMIRATES");
	api3Data.rfidStatus = RFID_GetSupportedRegionList(api3Data.hReaderMgmt, &regionList);
	numRegions = regionList.numRegions;
	for (int i = 0; i < numRegions; i++)
	{
		memcpy(pMyRegionList[i], regionList.RegionNamesList[i], 128);
		wprintf(L"\n\n %d. RegionName : %ls \n", i, pMyRegionList[i]);
	}
	
	
		//api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
		//api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);

		// Iterate through all the regions and verify RF Modes.
		// execute it on single region for all modes.
		/*int regionIndex = 0;
		wprintf(L"\n Select the Country Code you want to Test:");
		wscanf(L"%ld", &regionIndex);*/
		for (int regionIndex = 0; regionIndex < regionList.numRegions; regionIndex++)
		{
			api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);

			for (int standards = 0; standards < StandardInfo.numStds; standards++)
			{
				api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);

				ChannelCount = StandardInfo.StdsList[standards].iNumChannels;
				if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 3)
				{
					api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

					for (int MaskIndex = 0; MaskIndex < 7; MaskIndex++)
					{
						CHANNEL_LIST ChannelTable;
						int ChannelIndex = 0;

						ChannelTable.iNumChannels = ThreeChannelMask[MaskIndex][0] + ThreeChannelMask[MaskIndex][1] + ThreeChannelMask[MaskIndex][2];


						ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

						if (ThreeChannelMask[MaskIndex][0] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 1;
						}

						if (ThreeChannelMask[MaskIndex][1] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 2;
						}

						if (ThreeChannelMask[MaskIndex][2] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 3;
						}

						if (ChannelTable.iNumChannels > 1)
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
						}
						else
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
						}

						api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
						wprintf(L"\n --------- Lsit of frequencies ----------- \n");
						for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
						{
							wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
						}
						wprintf(L"\n----------- Lsit of frequencies ---------- \n");
						api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
						wprintf(L"\n Time: %ls CountryCode : %ld Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
						fwprintf(fp, L"\n Time : %ls CountryCode : %ld  Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

						TestAccessModesPX(fp, 2400); //Wattage of Power to read

						delete ChannelTable.ChannelList;
					}

				}
				else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 4)
				{
					api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

					for (int MaskIndex = 0; MaskIndex < 15; MaskIndex++)
					{
						CHANNEL_LIST ChannelTable;
						int ChannelIndex = 0;

						ChannelTable.iNumChannels = FourChannelMask[MaskIndex][0] + FourChannelMask[MaskIndex][1] + FourChannelMask[MaskIndex][2] + FourChannelMask[MaskIndex][3];


						ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

						if (FourChannelMask[MaskIndex][0] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 1;
						}

						if (FourChannelMask[MaskIndex][1] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 2;
						}

						if (FourChannelMask[MaskIndex][2] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 3;
						}

						if (FourChannelMask[MaskIndex][3] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 4;
						}

						if (ChannelTable.iNumChannels > 1)
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
						}
						else
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
						}

						api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
						wprintf(L"\n --------- Lsit of frequencies ----------- \n");
						for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
						{
							wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
						}
						wprintf(L"\n----------- Lsit of frequencies ---------- \n");
						api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
						wprintf(L"\n Time: %ls CountryCode : %ld Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
						fwprintf(fp, L"\n Time : %ls CountryCode : %ld  Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

						TestAccessModesPX(fp, 2400); //Wattage of Power to read

						delete ChannelTable.ChannelList;
					}

				}
				else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 6)
				{
					api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

					for (int MaskIndex = 0; MaskIndex < 63; MaskIndex++)
					{
						CHANNEL_LIST ChannelTable;
						int ChannelIndex = 0;

						ChannelTable.iNumChannels = SixChannelMask[MaskIndex][0] + SixChannelMask[MaskIndex][1] + SixChannelMask[MaskIndex][2] + SixChannelMask[MaskIndex][3] + SixChannelMask[MaskIndex][4] + SixChannelMask[MaskIndex][5];


						ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

						if (SixChannelMask[MaskIndex][0] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 1;
						}

						if (SixChannelMask[MaskIndex][1] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 2;
						}

						if (SixChannelMask[MaskIndex][2] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 3;
						}

						if (SixChannelMask[MaskIndex][3] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 4;
						}
						if (SixChannelMask[MaskIndex][4] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 5;
						}
						if (SixChannelMask[MaskIndex][5] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 6;
						}

						if (ChannelTable.iNumChannels > 1)
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
						}
						else
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
						}

						api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
						wprintf(L"\n --------- Lsit of frequencies ----------- \n");
						for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
						{
							wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
						}
						wprintf(L"\n----------- Lsit of frequencies ---------- \n");

						api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
						wprintf(L"\n Time: %ls CountryCode : %ld Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
						fwprintf(fp, L"\n Time : %ls CountryCode : %ld  Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

						TestAccessModesPX(fp, 2400); //Wattage of Power to read

						delete ChannelTable.ChannelList;
					}
				}
				// Hopping enabled and frequencies selected 
				else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount == 7)
				{
					api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

					for (int MaskIndex = 0; MaskIndex < 127; MaskIndex++)
					{
						CHANNEL_LIST ChannelTable;
						int ChannelIndex = 0;

						ChannelTable.iNumChannels = SevenChannelMask[MaskIndex][0] + SevenChannelMask[MaskIndex][1] + SevenChannelMask[MaskIndex][2] + SevenChannelMask[MaskIndex][3] + SevenChannelMask[MaskIndex][4] + SevenChannelMask[MaskIndex][5] + SevenChannelMask[MaskIndex][6];


						ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

						if (SevenChannelMask[MaskIndex][0] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 1;
						}

						if (SevenChannelMask[MaskIndex][1] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 2;
						}

						if (SevenChannelMask[MaskIndex][2] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 3;
						}

						if (SevenChannelMask[MaskIndex][3] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 4;
						}
						if (SevenChannelMask[MaskIndex][4] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 5;
						}
						if (SevenChannelMask[MaskIndex][5] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 6;
						}
						if (SevenChannelMask[MaskIndex][6] == 1)
						{
							ChannelTable.ChannelList[ChannelIndex++] = 7;
						}

						if (ChannelTable.iNumChannels > 1)
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
						}
						else
						{
							api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
						}

						api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
						wprintf(L"\n --------- Lsit of frequencies ----------- \n");
						for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
						{
							wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
						}
						wprintf(L"\n----------- Lsit of frequencies ---------- \n");

						api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
						wprintf(L"\n Time: %ls CountryCode : %ld Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
						fwprintf(fp, L"\n Time : %ls CountryCode : %ld  Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

						TestAccessModesPX(fp, 2400); //Wattage of Power to read

						delete ChannelTable.ChannelList;
					}
				}
				else
				{
					api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

					wprintf(L"\n RFID_SetActiveRegion Status:%ls RegionName:%ls StandardName:%ls", RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
					for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
					{
						wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
					}
					api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
					wprintf(L"\n Time: %ls CountryCode : %ld  \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					fwprintf(fp, L"\n Time : %ls CountryCode : %ld  \t APIRESULT:%ls RegionName:%ls StandardName:%ls", PrintTime(), api3Data.rCaps.countryCode, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

					TestAccessModesPX(fp, 2400);
				}
			}

		}

	
		/*api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, pMyRegionList[regionIndex], &StandardInfo);
		wprintf(L"\n  APIRESULT:%ls RegionName:%ls StandardName:%ls", RFID_GetErrorDescription(api3Data.rfidStatus),StandardInfo.StdsList[0].strRegionName, StandardInfo.StdsList[0].strStandardName);
		for (int standards = 0; standards < StandardInfo.numStds; standards++)
		{
			//api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);
			api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, pMyRegionList[regionIndex], &StandardInfo);
			ChannelCount = StandardInfo.StdsList[standards].iNumChannels;

			api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
			api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
			wprintf(L"\n  CountryCode : %ld SetActiveRegion- Result:%ls RegionName:%ls StandardName:%ls", api3Data.rCaps.countryCode,RFID_GetErrorDescription(api3Data.rfidStatus), StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
			
			api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
			wprintf(L"\n  APIRESULT:%ls RegionName:%ls StandardName:%ls",  RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
			for (int ChannelCount = 0; ChannelCount < activeRegionInfo.iNumChannels; ChannelCount++)
			{
				wprintf(L"\n Channel %ld Frequency %ld", ChannelCount, activeRegionInfo.piChannelInfo[ChannelCount]);
			}

			TestAccessModesPX(fp, 2700);
			//api3Data.rfidStatus = RFID_Logout(&api3Data.hReaderMgmt);
			//api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);*
		}*/

	fflush(fp);
	fclose(fp);
	
	api3Data.rfidStatus = RFID_Logout(&api3Data.hReaderMgmt);
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);

	return TRUE;
}

/*
BOOLEAN accessWithRFModesPX(RFID_VERSION rfidVersion)
{
	REGION_LIST regionList;
	STANDARD_INFO_LIST StandardInfo;
	ACTIVE_REGION_INFO activeRegionInfo;
	UINT32 ChannelCount = 0;
	LOGIN_INFO loginfo;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion; 
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	//api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	FILE* fp = fopen("accessInModes.txt", "w");
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, PX, FALSE, 0);
	api3Data.rfidStatus = RFID_GetSupportedRegionList(api3Data.hReaderMgmt, &regionList);

	// Iterate through all the regions and verify RF Modes.
	// execute it on single region for all modes.
	for (int regionIndex = 0; regionIndex < regionList.numRegions; regionIndex++)
	{
		//api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, FALSE, 0);
		api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);

		for (int standards = 0; standards < StandardInfo.numStds; standards++)
		{
			api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);

			ChannelCount = StandardInfo.StdsList[standards].iNumChannels;
			if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount < 5)
			{
				
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
				api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
				for (int MaskIndex = 0; MaskIndex < 15; MaskIndex++)
				{
					CHANNEL_LIST ChannelTable;
					int ChannelIndex = 0;
					if (ChannelCount < 4)
					{
						ChannelTable.iNumChannels = Mask[MaskIndex][0] + Mask[MaskIndex][1] + Mask[MaskIndex][2];
					}
					else
					{
						ChannelTable.iNumChannels = Mask[MaskIndex][0] + Mask[MaskIndex][1] + Mask[MaskIndex][2] + Mask[MaskIndex][3];
					}

					ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

					if (Mask[MaskIndex][0] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 1;
					}

					if (Mask[MaskIndex][1] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 2;
					}

					if (Mask[MaskIndex][2] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 3;
					}

					if (ChannelCount > 3 && Mask[MaskIndex][3] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 4;
					}

					if (ChannelTable.iNumChannels > 1)
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
					}
					else
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
					}

					RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
					for (int i = 0; i < activeRegionInfo.iNumChannels; i++)
					{
						wprintf(L" Active Channel Index: %ld", activeRegionInfo.piChannelInfo[i]);
					}

					wprintf(L"\n Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
					fwprintf(fp, L"\n Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);

					TestAccessModesPX(fp, 2700);

					if ((ChannelCount < 4) && MaskIndex == 6)
					{
						break;
					}

					delete ChannelTable.ChannelList;
				}

			}
			// Hopping enabled and frequencies selected 
			else if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && ChannelCount > 5)
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);

				for (int MaskIndex = 0; MaskIndex < 126; MaskIndex++)
				{
					CHANNEL_LIST ChannelTable;

					int ChannelIndex = 0;

					if (ChannelCount < 7)
					{
						ChannelTable.iNumChannels = SwissMask[MaskIndex][0] + SwissMask[MaskIndex][1] + SwissMask[MaskIndex][2] + SwissMask[MaskIndex][3] + SwissMask[MaskIndex][4] + SwissMask[MaskIndex][5];
					}
					else
					{
						ChannelTable.iNumChannels = SwissMask[MaskIndex][0] + SwissMask[MaskIndex][1] + SwissMask[MaskIndex][2] + SwissMask[MaskIndex][3] + SwissMask[MaskIndex][4] + SwissMask[MaskIndex][5] + SwissMask[MaskIndex][6];
					}

					ChannelTable.ChannelList = new UINT32[ChannelTable.iNumChannels];

					if (SwissMask[MaskIndex][0] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 1;
					}
					if (SwissMask[MaskIndex][1] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 2;
					}
					if (SwissMask[MaskIndex][2] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 3;
					}
					if (SwissMask[MaskIndex][3] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 4;
					}
					if (SwissMask[MaskIndex][4] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 5;
					}
					if (SwissMask[MaskIndex][5] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 6;
					}
					if (ChannelCount > 6 && SwissMask[MaskIndex][6] == 1)
					{
						ChannelTable.ChannelList[ChannelIndex++] = 7;
					}

					if (ChannelTable.iNumChannels > 1)
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
					}
					else
					{
						api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
					}

					api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
					wprintf(L"\n Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls No.Channels: %ld", MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName, activeRegionInfo.iNumChannels);

					for (int i = 0; i < activeRegionInfo.iNumChannels; i++)
					{
						wprintf(L" Active Channel Index: %ld", activeRegionInfo.piChannelInfo[i]);
					}

					fwprintf(fp, L"\n Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls,No.Channels: %ld", MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName, activeRegionInfo.iNumChannels);
					TestAccessModesPX(fp, 2700);

					delete ChannelTable.ChannelList;
				}


			}
			else
			{
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
				RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
				wprintf(L"\n APIRESULT:%ls RegionName:%ls StandardName:%ls", RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				fwprintf(fp, L"\n APIRESULT:%ls RegionName:%ls StandardName:%ls", RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				TestAccessModesPX(fp, 2700);
			}
		}

	}

	fflush(fp);
	fclose(fp);

	return TRUE;
}
*/
#endif // PIXIE