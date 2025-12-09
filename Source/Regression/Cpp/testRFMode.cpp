#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;

// Get thePower Index if the power value is given
int getPowerIndex( int PowerInDbm,READER_CAPS rCaps )
{
	// if no values present in the Reader Capabilities
	//
	if( rCaps.transmitPowerLevelTable.numValues <= 0 )
	{
		return 0;
	}
	else
	{
		for(int index = 0; index< rCaps.transmitPowerLevelTable.numValues;index++ )
		{
			if( rCaps.transmitPowerLevelTable.pPowerValueList[index] == PowerInDbm )
			{
				return index;
			}
		}
	}

	return 0;
}

void TestRFIndex( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("TestRFID3RFModeIndex");
	logCurrentExecution( "TestRFID3RFModeIndex");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;

	// Test for 1/4 Watt, 1/2 Watt and 1-Watt Power Levels.
	// The number of test cases depends on the total number of profiles available in a perticular region.
	// FX7400 - 9 profiles x 3 = 27 FX7500 - 32 profiles x 3 = 96 MCXXXX - 18 Profiles x 3 = 54
	// Note: This was extended to manual tests where it is done for every region. 
	//
	UINT16 PowerDbmLevels[ ] = { 2400,2700,3000 };
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader,&api3Data.rCaps);
	api3Data.itemTestCount = api3Data.rCaps.rfModes.pUHFTables->numEntries * 4;

	// Test RF Mode Index
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	api3Data.rfidStatus = RFID_DeletePreFilter(api3Data.hReader,0,0);

	// Verify if a modex index value can be set and do a corresponding get to verify the value
	//
	for(UINT32 modeIndex = 0;modeIndex < api3Data.rCaps.rfModes.pUHFTables->numEntries;modeIndex++)
	{		
		UINT32 getMode,tari;
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-RFINDEX-%d></a> CAPI-RFINDEX-%d:\n\n<br>Description:Set and Get RFMode\n <br> Expected OutPut:Set mode and getmode should be equal\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		api3Data.rfidStatus = RFID_SetRFMode( api3Data.hReader,0,modeIndex,0);
		api3Data.rfidStatus = RFID_GetRFMode( api3Data.hReader,1,&getMode,&tari);
		if( modeIndex == getMode )
		{
			SetLogResult(TRUE);
		}
		fprintf( api3Data.fpCurLog, "\nsetvalue: %d getValue %d \n ",modeIndex,getMode );
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"CAPI-RFINDEX");
	}
	

	// Check for antenna 1..Since 9190 has single antenna We do inventory on single antenna for all devices

	ANTENNA_INFO antennaInfo;
	memset(&antennaInfo,0,sizeof(ANTENNA_INFO));
	UINT16 antennaIDList[1] = {1};
	antennaInfo.pAntennaList = antennaIDList;
	antennaInfo.length = 1;
	antennaInfo.pAntennaOpList = NULL;

	UINT16 pRxIndex,pTxIndex,pTxFIndex;
	UINT16* pPowerValues = api3Data.rCaps.transmitPowerLevelTable.pPowerValueList;
	/*int Count;
	wprintf(L"\n Input the number of times you want to Run the Test:");
	wscanf(L"%ld", &Count);*/
	/*for (int i = 0; i < Count; i++)
	{
		fprintf(api3Data.fpCurLog, "\n*******Started Iteration - %d**************",Count);
		wprintf(L"\n******* Started Iteration - %d **************", i);*/
		// Modified Power iternation to One due to PA issue.
		// 
		for (int PowerInterations = 0; PowerInterations < 1/*3*/; PowerInterations++)
		{
			for (UINT32 modeIndex = 0; modeIndex < api3Data.rCaps.rfModes.pUHFTables->numEntries; modeIndex++)
			{
				fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-RFINDEX-%d></a> CAPI-RFINDEX-%d:\n\n<br>Description:Set RF Mode with Inventory with different Power Levels.\n <br> Expected OutPut:Should Read the Tags.\r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
				fprintf(api3Data.fpCurLog, "<br>");
				int PowerLevelIndex = getPowerIndex(PowerDbmLevels[PowerInterations], api3Data.rCaps);

				// Set antenna Config to all antennas with the Tx Power and check if a perituclr RFIndex with the 
				// Combination of Power level reads the tags or not.
				//
				if (api3Data.rCaps.hoppingEnabled) // Set the config only if hopping enabled.
				{
					api3Data.rfidStatus = RFID_SetAntennaConfig(api3Data.hReader, 0, 0, PowerLevelIndex, api3Data.rCaps.freqHopInfo.pFreqTables->hopTableID);
				}
				api3Data.rfidStatus = RFID_GetAntennaConfig(api3Data.hReader, 1, &pRxIndex, &pTxIndex, &pTxFIndex);
				fprintf(api3Data.fpCurLog, "\n pRXIndex :%d  pTXIndex:%d  pTXFIndex:%d", pRxIndex, pTxIndex, pTxFIndex);
				fprintf(api3Data.fpCurLog, "\n Set Antenna Config Status: %S", RFID_GetErrorDescription(api3Data.rfidStatus));

				api3Data.rfidStatus = RFID_SetRFMode(api3Data.hReader, 0, modeIndex, 0);

				api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
				rfid3Sleep(3000);
				//rfid3Sleep(15000);
				api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
				UINT32 tagCount = 0;
				while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
				{
					tagCount++;
				}
				api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
				if (tagCount > 0)
				{
					SetLogResult(TRUE);
				}

				//wprintf(L"\n  Modulation : %d FwdLinkModulation: %d SpectralMask: %d",api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].modulation,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].forwardLinkModulationType,api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].spectralMaskIndicator);
				fprintf(api3Data.fpCurLog, "\n CountryCode : %d CountryName : %S TrsPower dbm:%d Mode Index:%d Number of tags Read :%d", api3Data.rCaps.countryCode,GetCountryNamebyCode(api3Data.rCaps.countryCode),*(api3Data.rCaps.transmitPowerLevelTable.pPowerValueList), modeIndex, tagCount);
				wprintf(L"\n CountryCode : %d CountryName : %ls Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t MInd:%d \t tagRead :%d",
					api3Data.rCaps.countryCode, GetCountryNamebyCode(api3Data.rCaps.countryCode),
					wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].modulation],
					wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].forwardLinkModulationType],
					wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].spectralMaskIndicator],
					api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[PowerLevelIndex], modeIndex, tagCount);
				fprintf(api3Data.fpCurLog, "<br>");
				logToResultFile(L"CAPI-RFINDEX");
			}
		}
	//}
	
		api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.dwSkipCount = 0;
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
