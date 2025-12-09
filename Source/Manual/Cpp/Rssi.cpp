#include "ManualTests.h"

extern struct RFID3DATA api3Data;

void TestRSSI( RFID_VERSION version )
{
	CONNECTION_INFO ConInfo = { version, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort,0,&ConInfo );
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	RSSI_RANGE_FILTER rssiFilter;
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
	BOOLEAN bSuccess = FALSE;
	TAG_STORAGE_SETTINGS tgSettings = { 100,512,512,ALL_TAG_FIELDS,TRUE,TRUE,0};
	tgSettings.discardTagsOnInventoryStop = TRUE;
	RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings );
	
	UINT8 pattern1[2],pattern2[2];
	pattern1[0] = 0xA2;
	pattern1[1] = 0x2F;
	pattern2[0] = 0xB2;
	pattern2[1] = 0x2F;

	UINT8 TagMask[] = {0xFF,0xFF};
	POST_FILTER postFilter;
	memset(&postFilter,0,sizeof(POST_FILTER));

	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,pattern1,16,TagMask,16,0};
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,pattern2,16,TagMask,16,0}; 
	postFilter.lpTagPatternA = &tpA;
	postFilter.lpTagPatternB = &tpB;
	postFilter.matchPattern = NOTA_AND_B;
	postFilter.lpRSSIRangeFilter = &rssiFilter;
	postFilter.lpRSSIRangeFilter->peakRSSILowerLimit = -100;
	postFilter.lpRSSIRangeFilter->peakRSSIUpperLimit = 100;
	postFilter.lpRSSIRangeFilter->matchRange = WITHIN_RANGE; 
	TRIGGER_INFO triggerInfo;
	memset(&triggerInfo,0,sizeof(triggerInfo));
	triggerInfo.tagReportTrigger	= 1; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.lpTagEventReportInfo = NULL;
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	triggerInfo.lpTagEventReportInfo =  &tagEventInfo;
	// Read tags which within the  RSSI Range
	//
	/*RSSI_RANGE_FILTER rssiFilter;
	int lowerlimit,upperlimit,Range;
	wprintf(L"\n Min RSSI:" );
	wscanf (L"%d", &lowerlimit);
	wprintf(L"\n Max RSSI:" );
	wscanf (L"%d", &upperlimit);
	wprintf(L"\nWITHIN_RANGE = 0,OUTSIDE_RANGE = 1,GREATER_THAN_LOWER_LIMIT = 2,LOWER_THAN_UPPER_LIMIT = 3 : " );
	wscanf (L"%d", &Range);
	rssiFilter.peakRSSILowerLimit = lowerlimit;
	rssiFilter.peakRSSIUpperLimit = upperlimit;
	rssiFilter.matchRange = (MATCH_RANGE)Range;
	postfilter.lpRSSIRangeFilter = &rssiFilter;*/

	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	while( ( api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader, pTagData )) == RFID_API_SUCCESS )
	{
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}
	RFID_DeallocateTag( api3Data.hReader, pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}