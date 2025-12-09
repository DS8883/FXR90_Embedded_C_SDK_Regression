#include "ManualTests.h"

extern struct RFID3DATA api3Data;


void AntennaRFConfig( RFID_VERSION rfidVersion )
{
	ANTENNA_RF_CONFIG antRFConfig;
	TAG_STORAGE_SETTINGS tgSettings;
	UINT32 tagCount = 0;
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 20000;
	tInfo.tagReportTrigger = 0;
	ANTENNA_INFO* pantennaInfo = new ANTENNA_INFO();
	pantennaInfo->length = 4;
	pantennaInfo->pAntennaList = new UINT16[4];
	pantennaInfo->pAntennaOpList = new OPERATION_QUALIFIER[4];
	pantennaInfo->pAntennaList[0] = 1;
	pantennaInfo->pAntennaList[1] = 2;
	pantennaInfo->pAntennaList[2] = 3;
	pantennaInfo->pAntennaList[3] = 4;
	pantennaInfo->pAntennaOpList[0] = C1G2_OPERATION;
	pantennaInfo->pAntennaOpList[1] = C1G2_OPERATION;
	pantennaInfo->pAntennaOpList[2] = C1G2_OPERATION;
	pantennaInfo->pAntennaOpList[3] = C1G2_OPERATION;

	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );
	//api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, L"10.17.126.173",9180, 0, &ConInfo);
	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	tgSettings.maxTagCount = 20000;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings);
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
					
	for(int antIndex = 1;antIndex <= api3Data.rCaps.numAntennas;antIndex++)
	{
		tagCount =0;

		api3Data.rfidStatus = RFID_GetAntennaRfConfig( api3Data.hReader,antIndex,&antRFConfig);
		antRFConfig.antennaStopTrigger.stopTriggerType = ANTENNA_STOP_TRIGGER_TYPE_DURATION_MILLISECS;
		antRFConfig.antennaStopTrigger.stopTriggerValue = 4000;
		if(api3Data.rCaps.fixedFreqInfo.numFreq > 0 && antIndex <= api3Data.rCaps.fixedFreqInfo.numFreq  )
		{
			antRFConfig.transmitFrequencyIndex = antIndex;
		}
		//antRFConfig.rfModeTableIndex = 30;
		//api3Data.rfidStatus = RFID_SetAntennaRfConfig( api3Data.hReader,antIndex,NULL);
		api3Data.rfidStatus = RFID_SetAntennaRfConfig( api3Data.hReader,antIndex,&antRFConfig);
		wprintf(L"\n SetConfig: TXdbm:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld",api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antRFConfig.transmitPowerIndex],antRFConfig.tari,antRFConfig.receivePort,antRFConfig.transmitPort,antRFConfig.transmitFrequencyIndex);
		api3Data.rfidStatus = RFID_GetAntennaRfConfig( api3Data.hReader,antIndex,&antRFConfig);
		wprintf(L"\n GetConfig: TXdbm:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld",api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antRFConfig.transmitPowerIndex],antRFConfig.tari,antRFConfig.receivePort,antRFConfig.transmitPort,antRFConfig.transmitFrequencyIndex);
	}
	
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, pantennaInfo,&tInfo, NULL );
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	while( (api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData))== RFID_API_SUCCESS)
	{		
		printTagDataWithResults(pTagData);
		tagCount++;
	}
	wprintf(L"\n Number of Tags Read %ld,tagCount");
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader, 0 );
		
	RFID_DeallocateTag( api3Data.hReader,pTagData );

	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
// Antenna RF Config : Tags will be read as per the stop trigger criteria starting from the first antenna  to the last antenna list in the RFConfig.
// Verify if the Tags are reported as per switching between the antennas listed and stop criteria.
// 
void AntennaRFConfigWithRFModes( RFID_VERSION rfidVersion )
{
#ifndef linux
	UINT16 PowerDbmLevels[] = { 2400,2700,3000 };
	ANTENNA_RF_CONFIG antRFConfig;
	TAG_STORAGE_SETTINGS tgSettings;
	UINT32 tagCount = 0;
	TRIGGER_INFO tInfo;
	memset(&tInfo,0,sizeof(tInfo));
	tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
	tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
	tInfo.stopTrigger.value.duration = 5000;
	//tInfo.stopTrigger.value.numAttempts.timeoutMilliseconds = 5000;
	tInfo.tagReportTrigger = 0;
	ANTENNA_INFO* pantennaInfo = new ANTENNA_INFO();
	pantennaInfo->length = 4;
	pantennaInfo->pAntennaList = new UINT16[4];
	pantennaInfo->pAntennaOpList = new OPERATION_QUALIFIER[4];
	pantennaInfo->pAntennaList[0] = 1;
	pantennaInfo->pAntennaList[1] = 2;
	pantennaInfo->pAntennaList[2] = 3;
	pantennaInfo->pAntennaList[3] = 4;
	pantennaInfo->pAntennaOpList[0] = C1G2_OPERATION;
	pantennaInfo->pAntennaOpList[1] = C1G2_OPERATION;
	pantennaInfo->pAntennaOpList[2] = C1G2_OPERATION;
	pantennaInfo->pAntennaOpList[3] = C1G2_OPERATION;

	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	tgSettings.maxTagCount = 20000;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings);
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	HANDLE hInvStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,INVENTORY_START_EVENT,hInvStartEvent);
	HANDLE hInvStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,INVENTORY_STOP_EVENT,hInvStopEvent);
	HANDLE hnxpAlarmEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,NXP_EAS_ALARM_EVENT,hnxpAlarmEvent);

	// doing only for 3 power levels 1/4 Watt, 1/2 Watt, 1 Watt
	//
	for(int PowerInterations = 0;PowerInterations < 3;PowerInterations++)	
	{
		for(UINT32 modeIndex = 0;modeIndex < api3Data.rCaps.rfModes.pUHFTables->numEntries;modeIndex++)
		{
			tagCount =0;
					
			for(int antIndex = 1;antIndex <= api3Data.rCaps.numAntennas;antIndex++)
			{
				api3Data.rfidStatus = RFID_GetAntennaRfConfig( api3Data.hReader,antIndex,&antRFConfig);
				antRFConfig.antennaStopTrigger.stopTriggerType = ANTENNA_STOP_TRIGGER_TYPE_DURATION_MILLISECS;
				antRFConfig.antennaStopTrigger.stopTriggerValue = 4000;
				antRFConfig.tari = api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].maxTariValue;
				antRFConfig.rfModeTableIndex = modeIndex;
				antRFConfig.transmitPowerIndex = getPowerIndex(PowerDbmLevels[PowerInterations],api3Data.rCaps);
				if(api3Data.rCaps.fixedFreqInfo.numFreq > 0 && antIndex <= api3Data.rCaps.fixedFreqInfo.numFreq  )
				{
					antRFConfig.transmitFrequencyIndex = antIndex;
				}

				api3Data.rfidStatus = RFID_SetAntennaRfConfig( api3Data.hReader,antIndex,&antRFConfig);
				wprintf(L"\n SetConfig: TXdbm:%d \t MInd:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld",api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antRFConfig.transmitPowerIndex],modeIndex,antRFConfig.tari,antRFConfig.receivePort,antRFConfig.transmitPort,antRFConfig.transmitFrequencyIndex);
				api3Data.rfidStatus = RFID_GetAntennaRfConfig( api3Data.hReader,antIndex,&antRFConfig);
				wprintf(L"\n GetConfig: TXdbm:%d \t MInd:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld",api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antRFConfig.transmitPowerIndex],modeIndex,antRFConfig.tari,antRFConfig.receivePort,antRFConfig.transmitPort,antRFConfig.transmitFrequencyIndex);
			}
			
			api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, pantennaInfo,&tInfo, NULL );

			if( WAIT_OBJECT_0 == WaitForSingleObject( hInvStartEvent, 20000 ) )
			{
				wprintf(L"\n Inv Start event time :%ls\n",PrintTime());
				
			}
			
			if( WAIT_OBJECT_0 == WaitForSingleObject( hnxpAlarmEvent, 20000 ) )
			{
				wprintf(L"\n NXP alarm Start event time :%ls\n",PrintTime());
				NXP_EAS_ALARM_DATA myEASevent;
				api3Data.rfidStatus = RFID_GetEventData( api3Data.hReader,NXP_EAS_ALARM_EVENT,&myEASevent );
				if( api3Data.rfidStatus == RFID_API_SUCCESS )
				{
					wprintf(L"\n Alarm code %lld Antenna ID: %d ",myEASevent.alarmCode,myEASevent.antennaID );
				}
			}
			if( WAIT_OBJECT_0 == WaitForSingleObject( hInvStopEvent, 20000 ) )
			{
				wprintf(L"\n Inv Stop event time :%ls\n",PrintTime());
				
			}
			while( (api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData))== RFID_API_SUCCESS)
			{		
				printTagDataWithResults(pTagData);
				tagCount++;
			}

			wprintf(L"\n Modulation : %ls FwdLinkModulation: %ls SpectralMask: %ls TXdbm:%d \t MInd:%d \t tagRead :%d",
				wMODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].modulation],
				wFORWARD_LINK_MODULATION[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].forwardLinkModulationType],
				wSPECTRAL_MASK_INDICATOR[api3Data.rCaps.rfModes.pUHFTables->pTablesEntries[modeIndex].spectralMaskIndicator],
				api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[antRFConfig.transmitPowerIndex],modeIndex,tagCount);
			api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader, 0 );
		}
	}
	RFID_DeallocateTag( api3Data.hReader,pTagData );

	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
#endif
}