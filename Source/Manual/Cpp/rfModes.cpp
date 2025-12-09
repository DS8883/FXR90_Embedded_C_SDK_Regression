#include "ManualTests.h"

extern struct RFID3DATA api3Data;	


BOOLEAN ReadMemory( LPTAG_DATA pTagData, MEMORY_BANK mBank, UINT16 offSet,UINT16 length2Read)
{
	BOOLEAN bSuccess = FALSE;
	READ_ACCESS_PARAMS rParams = { mBank,offSet,length2Read,0 };

	wprintf(L"\nReading from Mem Bank:%ls  offSet:%d len:%d \n\n", wG2MEMBANK[mBank], offSet, length2Read);

	while ((api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(pTagData);
		if (pTagData->memoryBankDataLength == length2Read)
		{
			bSuccess = TRUE;
			break;
		}
	}

	return bSuccess;
}

void ReadAllMemoryBanks( RFID_VERSION rfidVersion  )
{
	TAG_STORAGE_SETTINGS tgSettings;
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	memset(&tgSettings, 0, sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader, &tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader, &tgSettings);

	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);

	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	// Read A22F TAG and B22F TAG

	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{
		// Check if the Tag is desired Tag for Regression
		if (pTagData->pTagID[0] == 0xBE)
		{
			ReadMemory(pTagData, MEMORY_BANK_EPC, 0, 4);
			ReadMemory(pTagData, MEMORY_BANK_EPC, 4, 8); 
			ReadMemory(pTagData, MEMORY_BANK_EPC, 0, 12); 
			ReadMemory(pTagData, MEMORY_BANK_EPC, 0, 16);
			ReadMemory(pTagData, MEMORY_BANK_EPC, 4, 12);

			// Read TID Memory Bank Data from different OFF Sets
			//
			ReadMemory(pTagData, MEMORY_BANK_TID, 0, 4);
			ReadMemory(pTagData, MEMORY_BANK_TID, 0, 8);
			ReadMemory(pTagData, MEMORY_BANK_TID, 0, 16);

			// Read RESERVED Memory Bank Data from different OFF Sets
			//
			ReadMemory(pTagData, MEMORY_BANK_RESERVED, 0, 2);
			ReadMemory(pTagData, MEMORY_BANK_RESERVED, 0, 4);
			ReadMemory(pTagData, MEMORY_BANK_RESERVED, 2, 4);

			// Read USER Memory Bank Data from different OFF Sets.Read entire memory bank first and then try reading four bytes
			// each time incrementing the off set and decrementing the length( this means that we are reading entire memory bank from
			// an offset).
			//
			ReadMemory(pTagData, MEMORY_BANK_USER, 0, 64);
			ReadMemory(pTagData, MEMORY_BANK_USER, 4, 60);
			ReadMemory(pTagData, MEMORY_BANK_USER, 8, 56);
			ReadMemory(pTagData, MEMORY_BANK_USER, 12, 52);
			ReadMemory(pTagData, MEMORY_BANK_USER, 16, 48);
			ReadMemory(pTagData, MEMORY_BANK_USER, 20, 44);
			ReadMemory(pTagData, MEMORY_BANK_USER, 24, 40);
			ReadMemory(pTagData, MEMORY_BANK_USER, 28, 36);
			ReadMemory(pTagData, MEMORY_BANK_USER, 32, 32);
			ReadMemory(pTagData, MEMORY_BANK_USER, 36, 28);
			ReadMemory(pTagData, MEMORY_BANK_USER, 40, 24);
			ReadMemory(pTagData, MEMORY_BANK_USER, 44, 20);
			ReadMemory(pTagData, MEMORY_BANK_USER, 48, 16);
			ReadMemory(pTagData, MEMORY_BANK_USER, 52, 12);
			ReadMemory(pTagData, MEMORY_BANK_USER, 56, 8);
			ReadMemory(pTagData, MEMORY_BANK_USER, 60, 4);
			break;
		}
	}

	RFID_DeallocateTag(api3Data.hReader, pTagData);
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
}