#include "ManualTests.h"
extern struct RFID3DATA api3Data;

static UINT8* pA22FTagEPC = new UINT8[12];
static UINT8* pA22FTagTID = new UINT8[12];
static UINT8* pA22FTagUSER = new UINT8[64];
static UINT8* pB22FTagEPC = new UINT8[12];
static UINT8* pB22FTagTID = new UINT8[12];
static UINT8* pB22FTagUSER = new UINT8[64];
static UINT8* pDadaTagEPC = new UINT8[12];
static UINT8* pDadaTagTID = new UINT8[12];
static UINT8* pDadaTagUSER = new UINT8[64];

BOOLEAN Read2Tags2Filtering()
{
	BOOLEAN bTag1, bTag2, bTag3;
	bTag1 = bTag2 = bTag3 = FALSE;
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} };
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	
	// Set maximum power for TID reads
	//DHVANISH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	UINT16 maxPowerIndex = api3Data.rCaps.transmitPowerLevelTable.numValues - 1;
	UINT16 maxSensitivityIndex = api3Data.rCaps.receiveSensitivtyTable.numValues - 1;
	api3Data.rfidStatus = RFID_SetAntennaConfig(api3Data.hReader, 1, maxSensitivityIndex, maxPowerIndex, 0);
	wprintf(L"\n Set Antenna Power to MAX: PowerIndex=%d, PowerValue=%d centi-dBm\n", 
		maxPowerIndex, api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[maxPowerIndex]);
	
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	// Read A22F TAG and B22F TAG

	{
		BOOLEAN buser = FALSE; BOOLEAN tid = FALSE;
		while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
		{
			// Check if the Tag is desired Tag for Regression
			int offset = 0;
			bool isA2 = false;
			if (pTagData->pTagID[0] == 0xA2) { isA2 = true; offset = 0; }
			else if (pTagData->tagIDLength > 4 && pTagData->pTagID[4] == 0xA2) { isA2 = true; offset = 4; }

			if (isA2 && !bTag1)
			{
				int epcLen = pTagData->tagIDLength - offset;
				if (epcLen > 12) epcLen = 12;
				memcpy(pA22FTagEPC, pTagData->pTagID + offset, epcLen);
				
				//print EPC
				printTagDataWithResults(pTagData);
				//get TID memory bank
				rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
				for (int iterCnt = 0; iterCnt < 5; iterCnt++)
				{
					wprintf(L"\n Attempt %d, Reading TID memory bank", iterCnt);
					RFID_STATUS rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID + offset, pTagData->tagIDLength - offset, &rParams, NULL, NULL, pTagData, 0);
					if (RFID_API_SUCCESS == rfidStatus)
					{
						tid = TRUE;
						break;
					}
					else
					{
						wprintf(L"\n Error Reading memory Bank: %ls", RFID_GetErrorDescription(rfidStatus));
					}
				}

				if (pTagData->memoryBank != MEMORY_BANK_TID)
				{
					wprintf(L"\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}
				memcpy(pA22FTagTID, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				printTagDataWithResults(pTagData);
				// User memory bank
				rParams.memoryBank = MEMORY_BANK_USER; rParams.byteOffset = 0; rParams.byteCount = 64;

				
				for (int iterCnt = 0; iterCnt < 5; iterCnt++)
				{
					wprintf(L"\n attempt %d Reading User memory bank", iterCnt);
					if (RFID_API_SUCCESS == RFID_Read(api3Data.hReader, pTagData->pTagID + offset, pTagData->tagIDLength - offset, &rParams, NULL, NULL, pTagData, 0))
					{
						buser = TRUE;
						break;
					}
				}


				if (pTagData->memoryBank != MEMORY_BANK_USER)
				{
					wprintf(L"\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}
				memcpy(pA22FTagUSER, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				printTagDataWithResults(pTagData);

				if (buser && tid)
				{
					bTag1 = TRUE;
				}
				else
				{
					bTag1 = FALSE;
				}
			}

			// Check if the Tag is desired Tag for Regression
			offset = 0;
			bool isB2 = false;
			if (pTagData->pTagID[0] == 0xB2) { isB2 = true; offset = 0; }
			else if (pTagData->tagIDLength > 4 && pTagData->pTagID[4] == 0xB2) { isB2 = true; offset = 4; }

			if (isB2 && !bTag2)
			{
				int epcLen = pTagData->tagIDLength - offset;
				if (epcLen > 12) epcLen = 12;
				memcpy(pB22FTagEPC, pTagData->pTagID + offset, epcLen);
				printTagDataWithResults(pTagData);

				// get TID memory bank			
				rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
				
				for (int iterCnt = 0; iterCnt < 5; iterCnt++)
				{
					wprintf(L"\n attempt %d Reading TID memory bank", iterCnt);
					if ((api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID + offset, pTagData->tagIDLength - offset, &rParams, NULL, NULL, pTagData, 0)) == RFID_API_SUCCESS)
					{
						tid = TRUE;
						break;
					}
					wprintf(L"\n rfidStatus: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
				}

				if (pTagData->memoryBank != MEMORY_BANK_TID)
				{
					wprintf(L"\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}

				memcpy(pB22FTagTID, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				printTagDataWithResults(pTagData);
				// User memory bank
				rParams.memoryBank = MEMORY_BANK_USER; rParams.byteOffset = 0; rParams.byteCount = 64;
				
				for (int iterCnt = 0; iterCnt < 5; iterCnt++)
				{
					wprintf(L"\n attempt : %d Reading User memory bank", iterCnt);
					if ((api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID + offset, pTagData->tagIDLength - offset, &rParams, NULL, NULL, pTagData, 0)) == RFID_API_SUCCESS)
					{
						buser = TRUE;
						break;
					}

					wprintf(L"\n rfidStatus: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
				}

				if (pTagData->memoryBank != MEMORY_BANK_USER)
				{
					wprintf(L"\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}
				memcpy(pB22FTagUSER, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				printTagDataWithResults(pTagData);
				if (buser && tid)
				{
					bTag2 = TRUE;
				}
				else bTag2 = FALSE;
			}
			// Check if the Tag is desired Tag for Regression
			//
			offset = 0;
			bool isDA = false;
			if (pTagData->pTagID[0] == 0xDA) { isDA = true; offset = 0; }
			else if (pTagData->tagIDLength > 4 && pTagData->pTagID[4] == 0xDA) { isDA = true; offset = 4; }

			if (isDA && !bTag3)
			{
				printTagDataWithResults(pTagData);
				int epcLen = pTagData->tagIDLength - offset;
				if (epcLen > 12) epcLen = 12;
				memcpy(pDadaTagEPC, pTagData->pTagID + offset, epcLen);

				// get TID memory bank			
				rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
				for (int iterCnt = 0; iterCnt < 5; iterCnt++)
				{
					wprintf(L"\n attempt %d Reading TID memory bank", iterCnt);

					api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID + offset, pTagData->tagIDLength - offset, &rParams, NULL, NULL, pTagData, 0);

					wprintf(L"\n rfidStatus: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));

					if (api3Data.rfidStatus == RFID_API_SUCCESS)
					{
						memcpy(pDadaTagTID, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
						printTagDataWithResults(pTagData);
						break;
					}
				}

			}

			if (bTag1 && bTag2 ) break;
		}

	}

	if (pTagData != NULL) RFID_DeallocateTag(api3Data.hReader, pTagData);
	return TRUE;

}
long SelectInventory()
{
	long TagCount = 0;
	LPTAG_DATA pInventoriedTag = RFID_AllocateTag(api3Data.hReader);
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	while ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pInventoriedTag)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(pInventoriedTag);
		TagCount++;
	}

	api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader,pInventoriedTag);
	return TagCount;
}
void TestRFID4PREFILTERS_STATE_UNAWARE()
{
	UINT16 TestCaseCount = 0;
	TAG_STORAGE_SETTINGS tgSettings;

	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	wprintf(L"\n getting 2 Tags for filtering.\n");

	if (!Read2Tags2Filtering())
	{
		wprintf(L"\n\n Could not Read Tags for Filtering \n\n");
	}
	
	// Inv A Not Inv B
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS  + MAY READ OTHER TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only matching Tags - A22F
	}
	
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_NOT_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS  MAY Read and Non Matching Should not Read ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld NO A22F TAGS ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld NO A22F TAGS + OTHER TAGS Should READ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_NOT_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld May Read A22F Other Tags Should be Read.------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	// Do for TID Memory BANK - DADACAFE TAG
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld DADACAFE TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}


	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld Should Read DADACAFE TAGS  + MAY READ OTHER TAGS ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_NOT_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld DADACAFE TAGS  MAY Read and Non Matching Should not Read ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld NO DADACAFE TAGS ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld NO DADACAFE TAGS + OTHER TAGS Should READ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_NOT_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld May Read DADACAFE Other Tags Should be Read.------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}
	
	// Test it with USER Memory BANK. A22FA22F
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagUSER, MEMORY_BANK_USER, 0, 16, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}
	

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagUSER, MEMORY_BANK_USER, 0, 64, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS  + MAY READ OTHER TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagUSER, MEMORY_BANK_USER, 0, 64, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_NOT_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS  MAY Read and Non Matching Should not Read ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagUSER, MEMORY_BANK_USER, 0, 64, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_UNSELECT);
		wprintf(L"\n\n --------- TestCase : %ld NO A22F TAGS ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagUSER, MEMORY_BANK_USER, 0, 64, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld NO A22F TAGS + OTHER TAGS Should READ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		addanWareSelectRecord(api3Data.hReader, pA22FTagUSER, MEMORY_BANK_USER, 0, 64, FILTER_ACTION_STATE_UNAWARE, STATE_UNAWARE_ACTION_NOT_SELECT);
		wprintf(L"\n\n --------- TestCase : %ld May Read A22F Other Tags Should be Read.------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}

	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);

}

void TestRFID4PREFILTERS_STATE_AWARE( )
{
	UINT16 TestCaseCount = 0;
	TAG_STORAGE_SETTINGS tgSettings;

	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	wprintf(L"\n getting 2 Tags for filtering.\n");

	if (!Read2Tags2Filtering())
	{
		wprintf(L"\n\n Could not Read Tags for Filtering \n\n");
	}

	// Inv A Not Inv B
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		STATE_AWARE_ACTION_PARAMS InvActionParams = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, InvActionParams);
	}
		
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only matching Tags - A22F
		
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------  TestCase : %ld TAGS OTHERTHAN A22F TAGS ONLY ---------------\n\n-", TestCaseCount++);
		SelectInventory();	// Only Non matching Tags - 
	}

	// Inv STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pB22FTagEPC, MEMORY_BANK_EPC, 32, 16, InvActionParams1);

		STATE_AWARE_ACTION_PARAMS InvActionParams = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL };
		addSelectRecord(api3Data.hReader, pB22FTagEPC, MEMORY_BANK_EPC, 32, 16, InvActionParams);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_FLAG_DEASSERTED);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------  TestCase : %ld B22F TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_FLAG_DEASSERTED);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------  TestCase : %ld TAGS OTHERTHAN B22F TAGS ONLY ---------------\n\n", TestCaseCount++);
		SelectInventory();	// Only Non matching Tags - 
	}

	// Inv A -> Matching should move to A | Non Matching may or may not move hence can be Read
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------  TestCase : %ld A22F Should be Read May read Other Tags--------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// Not B -> Non MAtching Tags to be moved to B
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld  only A22F Should be Read ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld TAGS other than A22F Should be Read ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}


	// STATE_AWARE_ACTION_INV_A2BB2A -> 
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A2BB2A };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 16, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------TestCase : %ld  A22F + All Tags ------------------------\n\n", TestCaseCount++);
		SelectInventory();	

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------TestCase : %ld Only A22F ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// With Session 2 Filters
	// 
	// Inv A Not Inv B
	wprintf(L"\n\n Testing FREE FILTERS with Session2 with TID memory BANK\n\n");
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		STATE_AWARE_ACTION_PARAMS InvActionParams = { TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_A_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ----------TestCase : %ld DADCAFE TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only matching Tags - A22F

	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ----------TestCase : %ld TAGS OTHERTHAN DADCAFE TAGS ONLY ---------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only Non matching Tags - 
	}

	// Inv STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_A_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams1);

		STATE_AWARE_ACTION_PARAMS InvActionParams = { TARGET_SL,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_A, SL_FLAG_ASSERTED);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld DADCAFE TAGS ONLY ------------------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only matching Tags - A22F

	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_B, SL_FLAG_DEASSERTED);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld TAGS OTHERTHAN DADCAFE TAGS ONLY ---------------\n\n-", TestCaseCount++);
		SelectInventory();	// Only Non matching Tags - 
	}

	// Inv A -> Matching should move to A | Non Matching may or may not move hence can be Read
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_A };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld DADCAFE Should be Read May read Other Tags------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// Not B -> Non MAtching Tags to be moved to B
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------  TestCase : %ld  only DADACAFE Should be Read ------------------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only matching Tags - A22F

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld TAGS other than DADACAFE Should be Read ------------------------\n\n", TestCaseCount++);
		SelectInventory( );	// Only matching Tags - A22F

	}


	// STATE_AWARE_ACTION_INV_A2BB2A -> 
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_A2BB2A };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld DADACAFE + All Tags ------------------------\n\n", TestCaseCount++);
		SelectInventory();

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld Only DADACAFE ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// STATE_AWARE_ACTION_INV_B_NOT_INV_A
	//
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S2,STATE_AWARE_ACTION_INV_B_NOT_INV_A };
		addSelectRecord(api3Data.hReader, pDadaTagTID, MEMORY_BANK_TID, 0, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld DADACAFE Tags Only ------------------------\n\n", TestCaseCount++);
		SelectInventory();

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S2, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld Tags other than DADACAFE  ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// MAtch All Bits of EPC - 96
	// Inv A Not Inv B
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);
		STATE_AWARE_ACTION_PARAMS InvActionParams = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld A22F TAGS ONLY - 96bitFilter ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld TAGS OTHERTHAN A22F TAGS ONLY - 96bitFilter---------------\n\n-", TestCaseCount++);
		SelectInventory();	// Only Non matching Tags - 
	}

	// Inv STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pB22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams1);

		STATE_AWARE_ACTION_PARAMS InvActionParams = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL };
		addSelectRecord(api3Data.hReader, pB22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_FLAG_DEASSERTED);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld B22F TAGS ONLY  - 96bitFilter------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_FLAG_DEASSERTED);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld TAGS OTHERTHAN B22F TAGS ONLY  - 96bitFilter---------------\n\n-", TestCaseCount++);
		SelectInventory();	// Only Non matching Tags - 
	}

	// Inv A -> Matching should move to A | Non Matching may or may not move hence can be Read
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------TestCase : %ld  A22F Should be Read May read Other Tags  - 96bitFilter------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// Not B -> Non MAtching Tags to be moved to B
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_NOT_INV_B };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld only A22F Should be Read - 96bitFilter ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld TAGS other than A22F Should be Read - 96bitFilter ------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}


	// STATE_AWARE_ACTION_INV_A2BB2A -> 
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A2BB2A };
		addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams1);
	}

	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_A, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n ---------TestCase : %ld  A22F + All Tags  - 96bitFilter------------------------\n\n", TestCaseCount++);
		SelectInventory();

	}
	{
		SINGULATION_CONTROL s;
		InitSingulationControl(&s, SESSION_S0, 100, 0, INVENTORY_STATE_B, SL_ALL);
		api3Data.rfidStatus = RFID_SetSingulationControl(api3Data.hReader, 0, &s);
		wprintf(L"\n\n --------- TestCase : %ld Only A22F  - 96bitFilter------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F

	}

	// STATE_AWARE_ACTION_INV_A2BB2A -> 
	{
		RFID_DeletePreFilter(api3Data.hReader, 0, 0);

		STATE_AWARE_ACTION_PARAMS InvActionParams1 = { TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_B };
		for (int Index = 0; Index < 32; Index++)
		{
			addSelectRecord(api3Data.hReader, pA22FTagEPC, MEMORY_BANK_EPC, 32, 96, InvActionParams1);
		}
		wprintf(L"\n\n --------- TestCase : %ld Adding 32 Filters------------------------\n\n", TestCaseCount++);
		SelectInventory();	// Only matching Tags - A22F
	}
	
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
}