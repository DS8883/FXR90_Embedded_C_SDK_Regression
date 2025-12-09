#include "ManualTests.h"
#include "utils.h"
#include <stdio.h>

#define rfid_stprintf swprintf
extern struct RFID3DATA api3Data;
wchar_t CurrentTime[512];

// event count
static UINT32 numEventAll = 0;
static UINT32 numEventNewTag = 0;
static UINT32 numEventVisible = 0;
static UINT32 numEventInvisible = 0;
static UINT32 numEventMoving = 0;
static UINT32 numEventStationary = 0;
static UINT32 numMatchedTag = 0;
static bool bDisplayTagData = true;
static wchar_t CountryName[128];

int GetFileContent(char* filePath, BYTE *pBuffer, int *pLen)
{
	FILE * pFile;
	size_t result;

	pFile = fopen ( filePath , "r" );
	if (pFile==NULL) return 0;

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	*pLen = ftell (pFile);
	rewind (pFile);

	// copy the file into the buffer:
	result = fread (pBuffer, 1, *pLen, pFile);
	*pLen = result;

	/* the whole file is now loaded in the memory buffer. */
	fclose (pFile);
	return *pLen;
}
void DisplayLastErrorInfo( )
{
	ERROR_INFO errorInfo;
	api3Data.rfidStatus = RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
	wprintf( L"\n rfidStatusCode - %d <br>",errorInfo.rfidStatusCode);
	wprintf( L"\n statusDesc - %ls ",errorInfo.statusDesc);
	wprintf( L"\n vendorMessage - %ls", errorInfo.vendorMessage);
}
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
// Print Routine for tag data results.
//
void printTagDataWithResults(TAG_DATA *pTagData)
{
	wchar_t resultBuffer[4096] = {0,};
	char ansiBuffer[4096] = {0,};
	UINT8*pTagID = pTagData->pTagID;
	UINT8*pTagMBData = pTagData->pMemoryBankData;
	UINT32 epcLength =  pTagData->tagIDLength;
#ifdef WIN32
	TIME_ZONE_INFORMATION tzInfo;
#endif
	SYSTEMTIME localTime;

	wchar_t  memoryBankBuffer[128] = {0,};
	wchar_t  accessMBBuffer[2048] = {0,};
	wchar_t  AccessResultBuffer[128] = {0,};
	wchar_t  tagEventBuffer[128] = {0,};
	wchar_t  tagLocationBuffer[128] = {0,};
	wchar_t  AccessOpBuffer[128] = {0,};
	wchar_t tagBuffer[260] = {0,};
	wchar_t* pTagReportData = tagBuffer;
	unsigned int   index = 0;
#ifndef linux
	GetTimeZoneInformation(&tzInfo);
#endif
	for(index = 0; index < epcLength; index++)
	{
		if(0 < index && index%2 == 0)
		{
			//*pTagReportData++ = L'-';
		}
		rfid_swprintf(pTagReportData, 2048, L"%02X", pTagID[index]);
		while(*pTagReportData) pTagReportData++;
	}

	if(pTagData->lpLocation)
	{
		rfid_swprintf(tagLocationBuffer, 128, L"Relative-Distance = %d", pTagData->lpLocation->relativeDistance);
	}

	if(pTagData->tagEvent == NONE)
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->seenTime.utcTime.firstSeenTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->seenTime.utcTime.firstSeenTimeStamp, sizeof(SYSTEMTIME));
#endif
	}
	else
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->tagEventTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->tagEventTimeStamp, sizeof(SYSTEMTIME));
#endif
	}

	switch(pTagData->tagEvent)
	{
	case NONE:
		break;
	case UNKNOWN_STATE:
		break;
	case NEW_TAG_VISIBLE:
		rfid_swprintf(tagEventBuffer, 128, L"NEW\t");
		break;
	case TAG_BACK_TO_VISIBILITY:
		rfid_swprintf(tagEventBuffer, 128, L"BACK\t");
		break;
	case TAG_NOT_VISIBLE:
		rfid_swprintf(tagEventBuffer, 128, L"GONE\t");
		break;
	default:
		rfid_swprintf(tagEventBuffer, 128, L"NO TAGEVENT");
		break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE)
		switch(pTagData->opCode)
	{
		case  ACCESS_OPERATION_READ:
			rfid_swprintf(AccessOpBuffer, 128, L"Read");
			break;
		case ACCESS_OPERATION_WRITE:
			rfid_swprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_LOCK:
			rfid_swprintf(AccessOpBuffer, 128, L"Lock");
			break;
		case ACCESS_OPERATION_KILL:
			rfid_swprintf(AccessOpBuffer, 128, L"Kill");
			break;
		case ACCESS_OPERATION_BLOCK_WRITE:
			rfid_swprintf(AccessOpBuffer, 128, L"BlkWrite");
			break;
		case ACCESS_OPERATION_BLOCK_ERASE:
			rfid_swprintf(AccessOpBuffer, 128, L"BlkErase");
			break;
		case ACCESS_OPERATION_RECOMMISSION:
			rfid_swprintf(AccessOpBuffer, 128, L"Recommision");
			break;
		case ACCESS_OPERATION_BLOCK_PERMALOCK:
			rfid_swprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_NONE:
			rfid_swprintf(AccessOpBuffer, 128, L"None");
			break;
		default:
			rfid_swprintf(AccessOpBuffer, 128, L"ERROR!!!!!!!!!");
			break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE)
	switch(pTagData->opStatus)
	{
	case ACCESS_OPERATION_NONE:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: ACCESS_OPERATION_NONE", AccessOpBuffer);
		case ACCESS_SUCCESS:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Success", AccessOpBuffer);
			break;
		case ACCESS_TAG_NON_SPECIFIC_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag non-specific error", AccessOpBuffer);
			break;
		case ACCESS_READER_NON_SPECIFIC_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Reader non-specific error", AccessOpBuffer);
			break;
		case ACCESS_NO_RESPONSE_FROM_TAG:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: No response from Tag", AccessOpBuffer);
			break;
		case ACCESS_INSUFFICENT_POWER:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Insufficient power", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_LOCKED_ERROR	:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag memory locked", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_OVERRUN_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag memory overrun", AccessOpBuffer);
			break;
		case ACCESS_ZERO_KILL_PASSWORD_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Zero kill password error", AccessOpBuffer);
			break;
		default:
			rfid_swprintf(AccessResultBuffer, 128, L"ERROR!!!!!!!!!");
			break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE && ACCESS_SUCCESS == pTagData->opStatus)
	{

		switch(pTagData->memoryBank)
		{
		case MEMORY_BANK_RESERVED:
			rfid_swprintf(memoryBankBuffer, 128, L"-RSD-");
			break;
		case MEMORY_BANK_EPC:
			rfid_swprintf(memoryBankBuffer, 128, L"-EPC-");
			break;
		case MEMORY_BANK_TID:
			rfid_swprintf(memoryBankBuffer, 128, L"-TID-");
			break;
		case MEMORY_BANK_USER:
			rfid_swprintf(memoryBankBuffer, 128, L"-USR-");
			break;
		}

		wchar_t* pTagReportData = accessMBBuffer;
		for(index = 0; index < pTagData->memoryBankDataLength; index++)
		{
			if(0 < index && index%2 == 0)
			{
				//*pTagReportData++ = L'-';
			}
			rfid_swprintf(pTagReportData, 2048, L"%02X", pTagMBData[index]);
			while(*pTagReportData) pTagReportData++;
		}
	}
	
	rfid_swprintf(resultBuffer, 4096, L"%2d/%2d/%2d %02d:%02d:%2d:%03d  %ls  Antenna:%2d  PC:%5x RSSI:%04d\t%ls %ls %ls %ls %ls\tSeenCount:%d PhaseInfo: %3.2f\t ChanIndx:%02d \tTxPort:%02d\tFST:%2d/%2d/%2d %02d:%02d:%2d:%03d\tLST:%2d/%2d/%2d %02d:%02d:%2d:%03d NXP_Brand_Chk : %d \n",
		localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
		tagEventBuffer, pTagData->antennaID, pTagData->PC,
		pTagData->peakRSSI, tagBuffer,AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer,
		pTagData->tagSeenCount,(180.0 / 32767) * pTagData->phaseInfo,pTagData->channelIndex,pTagData->transmitPort,
		pTagData->seenTime.utcTime.firstSeenTimeStamp.wDay, pTagData->seenTime.utcTime.firstSeenTimeStamp.wMonth,
		pTagData->seenTime.utcTime.firstSeenTimeStamp.wYear, pTagData->seenTime.utcTime.firstSeenTimeStamp.wHour,
		pTagData->seenTime.utcTime.firstSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.firstSeenTimeStamp.wSecond,
		pTagData->seenTime.utcTime.firstSeenTimeStamp.wMilliseconds,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay,pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear,pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute,pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds,
		pTagData->brandValid
		
		);

	/*rfid_swprintf(resultBuffer, 4096, L"%2d/%2d/%2d %02d:%02d:%2d:%03d  %ls  Antenna:%2d  PC:%5x XPC:%5x RSSI:%04d\t%ls %ls %ls %ls %ls\tSeenCount:%d PhaseInfo: %d\t ChanIndx:%02d \tTxPort:%02d\tLST:%2d/%2d/%2d %02d:%02d:%2d:%03d\n",
		localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
		tagEventBuffer, pTagData->antennaID, pTagData->PC,
		pTagData->XPC, pTagData->peakRSSI, tagBuffer, AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer,
		pTagData->tagSeenCount, pTagData->phaseInfo, pTagData->channelIndex, pTagData->transmitPort,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wDay, pTagData->seenTime.utcTime.lastSeenTimeStamp.wMonth,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wYear, pTagData->seenTime.utcTime.lastSeenTimeStamp.wHour,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wMinute, pTagData->seenTime.utcTime.lastSeenTimeStamp.wSecond,
		pTagData->seenTime.utcTime.lastSeenTimeStamp.wMilliseconds
		);*/
	
	wprintf(L"%ls", resultBuffer);
	wcstombs(ansiBuffer,resultBuffer,4096);
}
#if !defined PIXIE
void convertTagDataToResults(TAG_DATA* pTagData, wchar_t* resultBuffer, bool bPhaseEnabled)
{
	UINT8* pTagID = pTagData->pTagID;
	UINT8* pTagMBData = pTagData->pMemoryBankData;
	UINT32 epcLength = pTagData->tagIDLength;
	SYSTEMTIME localTime;

	wchar_t  memoryBankBuffer[128] = { 0, };
	wchar_t  accessMBBuffer[128] = { 0, };
	wchar_t  AccessResultBuffer[128] = { 0, };
	wchar_t  tagEventBuffer[128] = { 0, };
	wchar_t  tagLocationBuffer[128] = { 0, };
	wchar_t  AccessOpBuffer[128] = { 0, };
	wchar_t tagBuffer[MAX_PATH] = { 0, };
	wchar_t* pTagReportData = tagBuffer;
	int   index = 0;
#ifndef linux
	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);
#endif

	for (index = 0; index < epcLength; index++)
	{
		if (0 < index && index % 2 == 0)
		{
			*pTagReportData++ = L'-';
		}
		rfid_stprintf(pTagReportData, MAX_PATH, L"%02X", pTagID[index]);
		while (*pTagReportData) pTagReportData++;
	}

	if (pTagData->lpLocation)
	{
		rfid_stprintf(tagLocationBuffer, 128, L"Relative-Distance = %d", pTagData->lpLocation->relativeDistance);
	}

	if (pTagData->tagEvent == NONE)
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->seenTime.utcTime.firstSeenTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->seenTime.utcTime.firstSeenTimeStamp, sizeof(SYSTEMTIME));
#endif
	}
	else
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->tagEventTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->tagEventTimeStamp, sizeof(SYSTEMTIME));
#endif
	}

	numEventAll++;
	switch (pTagData->tagEvent)
	{
	case NONE:
		break;
	case UNKNOWN_STATE:
		break;
	case NEW_TAG_VISIBLE:
		numEventNewTag++;
		rfid_stprintf(tagEventBuffer, 128, L"NEW\t");
		break;
	case TAG_BACK_TO_VISIBILITY:
		numEventVisible++;
		rfid_stprintf(tagEventBuffer, 128, L"BACK\t");
		break;
	case TAG_NOT_VISIBLE:
		numEventInvisible++;
		rfid_stprintf(tagEventBuffer, 128, L"GONE\t");
		break;
	case TAG_MOVING:
		numEventMoving++;
		rfid_stprintf(tagEventBuffer, 128, L"MOVING\t");
		break;
	case TAG_STATIONARY:
		numEventStationary++;
		rfid_stprintf(tagEventBuffer, 128, L"STATION\t");
		break;
	default:
		rfid_stprintf(tagEventBuffer, 128, L"ERROR!!!!!!!!!");
		break;
	}

	if (pTagData->opCode != ACCESS_OPERATION_NONE)
		switch (pTagData->opCode)
		{
		case  ACCESS_OPERATION_READ:
			rfid_stprintf(AccessOpBuffer, 128, L"Read");
			break;
		case ACCESS_OPERATION_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_LOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Lock");
			break;
		case ACCESS_OPERATION_KILL:
			rfid_stprintf(AccessOpBuffer, 128, L"Kill");
			break;
		case ACCESS_OPERATION_BLOCK_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_BLOCK_ERASE:
			rfid_stprintf(AccessOpBuffer, 128, L"Erase");
			break;
		case ACCESS_OPERATION_RECOMMISSION:
			rfid_stprintf(AccessOpBuffer, 128, L"Recommision");
			break;
		case ACCESS_OPERATION_BLOCK_PERMALOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_NONE:
			rfid_stprintf(AccessOpBuffer, 128, L"None");
			break;
		case 	ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu:Change Word Lock");
			break;
		case ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Change Block lock");
			break;
		case ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu:Read Block Lock");
			break;
		case ACCESS_OPERATION_FUJITSU_BURST_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Burst Write");
			break;
		case ACCESS_OPERATION_FUJITSU_BURST_ERASE:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Burst Erase");
			break;
		case ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Block/Area Grp Password");
			break;
		case ACCESS_OPERATION_FUJITSU_AREA_READLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Area Write Lock");
			break;
		case ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: WriteLock WO Password");
			break;
		case ACCESS_OPERATION_IMPINJ_QT_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Impinj: QT Write");
			break;
		case ACCESS_OPERATION_IMPINJ_QT_READ:
			rfid_stprintf(AccessOpBuffer, 128, L"Impinj: QT Read");
			break;
		case ACCESS_OPERATION_NXP_CHANGE_CONFIG:
			rfid_stprintf(AccessOpBuffer, 128, L"NXP: Chane Config");
			break;
		case ACCESS_OPERATION_AUTHENTICATE:
			rfid_stprintf(AccessOpBuffer, 128, L"G2V2: Authenticate");
			break;
		case ACCESS_OPERATION_READBUFFER:
			rfid_stprintf(AccessOpBuffer, 128, L"G2V2: ReadBuffer");
			break;
		case ACCESS_OPERATION_UNTRACEABLE:
			rfid_stprintf(AccessOpBuffer, 128, L"G2V2: Untraceable");
			break;
		default:
			rfid_stprintf(AccessOpBuffer, 128, L"ERROR!!!!!!!!!");
			break;
		}

	if (pTagData->opCode != ACCESS_OPERATION_NONE)
		switch (pTagData->opStatus)
		{
		case ACCESS_SUCCESS:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success", AccessOpBuffer);
			break;
		case ACCESS_TAG_NON_SPECIFIC_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Tag non-specific error", AccessOpBuffer);
			break;
		case ACCESS_READER_NON_SPECIFIC_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Reader non-specific error", AccessOpBuffer);
			break;
		case ACCESS_NO_RESPONSE_FROM_TAG:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: No response from Tag", AccessOpBuffer);
			break;
		case ACCESS_INSUFFICENT_POWER:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Insufficient power", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_LOCKED_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Tag memory locked", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_OVERRUN_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Tag memory overrun", AccessOpBuffer);
			break;
		case ACCESS_ZERO_KILL_PASSWORD_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Zero kill password error", AccessOpBuffer);
			break;
		case ACCESS_TAG_IN_PROCESS_STILL_WORKING:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: In Process - Still working", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_STORED_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Stored response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_STORED_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Stored response with length", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_SEND_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Send response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_SEND_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Send response with length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_STORED_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Stored response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_STORED_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Stored response with length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_SEND_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Send response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_SEND_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Send response with length", AccessOpBuffer);
			break;
		default:
			rfid_stprintf(AccessResultBuffer, 128, L"ERROR!!!!!!!!!");
			break;
		}

	if ((pTagData->opCode == ACCESS_OPERATION_AUTHENTICATE) || (pTagData->opCode == ACCESS_OPERATION_READBUFFER))
	{
		if (pTagData->lpAccessOperationResult != 0)
		{
			wchar_t* pTagReportData = accessMBBuffer;
			UINT16 receivedBitCount = pTagData->lpAccessOperationResult->AuthenticateReadBufferResult.receivedBitCount;
			UINT8* pReceivedBitData = pTagData->lpAccessOperationResult->AuthenticateReadBufferResult.pReceivedBitData;

			rfid_stprintf(memoryBankBuffer, 128, L":BitLength(%d):", receivedBitCount);
			for (index = 0; index < (receivedBitCount + 7) / 8; index++)
			{
				if (0 < index && index % 2 == 0)
				{
					*pTagReportData++ = L'-';
				}
				rfid_stprintf(pTagReportData, 128, L"%02X", pReceivedBitData[index]);
				while (*pTagReportData) pTagReportData++;
			}
		}
	}

	if (pTagData->opCode != ACCESS_OPERATION_NONE && ACCESS_SUCCESS == pTagData->opStatus)
	{

		switch (pTagData->memoryBank)
		{
		case MEMORY_BANK_RESERVED:
			rfid_stprintf(memoryBankBuffer, 128, L"-RSD-");
			break;
		case MEMORY_BANK_EPC:
			rfid_stprintf(memoryBankBuffer, 128, L"-EPC-");
			break;
		case MEMORY_BANK_TID:
			rfid_stprintf(memoryBankBuffer, 128, L"-TID-");
			break;
		case MEMORY_BANK_USER:
			rfid_stprintf(memoryBankBuffer, 128, L"-USR-");
			break;
		}

		wchar_t* pTagReportData = accessMBBuffer;
		for (index = 0; index < pTagData->memoryBankDataLength; index++)
		{
			if (0 < index && index % 2 == 0)
			{
				*pTagReportData++ = L'-';
			}
			rfid_stprintf(pTagReportData, 128, L"%02X", pTagMBData[index]);
			while (*pTagReportData) pTagReportData++;
		}
	}
	if (bPhaseEnabled) {
		rfid_stprintf(resultBuffer, MAX_PATH, L"%2d/%2d/%2d %02d:%02d:%2d:%03d\t%ls\tAntenna:%2d\tPC:%5x\tXPC:%5x\tRSSI:%04d\t%ls %ls %ls %ls %ls\tPhase:%3.2f",
			localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
			tagEventBuffer,
			pTagData->antennaID, pTagData->PC,
			pTagData->XPC, pTagData->peakRSSI, tagBuffer, AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer,(180.0/32767)* pTagData->phaseInfo);
	}
	else {
		rfid_stprintf(resultBuffer, MAX_PATH, L"%2d/%2d/%2d %02d:%02d:%2d:%03d\t%ls\tAntenna:%2d\tPC:%5x\tXPC:%5x\tRSSI:%04d\t%ls %ls %ls %ls %ls",
			localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
			tagEventBuffer,
			pTagData->antennaID, pTagData->PC,
			pTagData->XPC, pTagData->peakRSSI, tagBuffer, AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer);
		wprintf(L"%ls", resultBuffer);
	}

}
#endif

void InitSingulationControl( SINGULATION_CONTROL* pSingControl,SESSION session,UINT16 tagPopulation,UINT16 tagTransitTimeMilliseconds,INVENTORY_STATE inventoryState,SL_FLAG slFlag)
{
	memset(pSingControl,0,sizeof(SINGULATION_CONTROL));
	pSingControl->session = session;
	pSingControl->tagPopulation  = tagPopulation;
	pSingControl->stateAwareSingulationAction.inventoryState = inventoryState;
	pSingControl->stateAwareSingulationAction.perform = TRUE;
	pSingControl->stateAwareSingulationAction.slFlag = slFlag;
}

void addanWareSelectRecord(RFID_HANDLE32 hReader, UINT8* pattern, MEMORY_BANK mBank, UINT16 bitoffset, UINT8 patternbitCount, FILTER_ACTION fAction, STATE_UNAWARE_ACTION selectParam)
{
	PRE_FILTER pf;
	memset(&pf, 0, sizeof(PRE_FILTER));
	UINT32 index;
	pf.memoryBank = mBank;
	pf.pTagPattern = pattern;
	pf.tagPatternBitCount = patternbitCount;
	pf.bitOffset = bitoffset;
	pf.filterAction = fAction;
	pf.filterActionParams.stateUnawareAction = selectParam;
	pf.truncateAction = TRUNCATE_ACTION_DO_NOT_TRUNCATE;
	api3Data.rfidStatus = RFID_AddPreFilter(hReader, 0, &pf, &index);
}

void addSelectRecord( RFID_HANDLE32 hReader, UINT8* pattern,MEMORY_BANK mBank,UINT8 bitoffset,UINT8 patternbitCount,STATE_AWARE_ACTION_PARAMS sParams )
{
	PRE_FILTER pf;
	memset(&pf,0,sizeof(PRE_FILTER));
	UINT32 index;
	pf.bitOffset = bitoffset ;pf.filterAction = FILTER_ACTION_STATE_AWARE;
	pf.filterActionParams.stateAwareParams= sParams;
	pf.memoryBank = mBank;pf.pTagPattern = pattern;pf.tagPatternBitCount = patternbitCount;
	api3Data.rfidStatus = RFID_AddPreFilter( hReader, 0,&pf,&index );
}
void Print2Console(UINT16 crc,UINT16 PC,UINT32 XPC, UINT8*pTagID, UINT32 tagIDLength)
{
        char  tagBuffer[512] = {0,};
        char* pTagReportData = tagBuffer;
        UINT16   index = 0;
        for( index = 0; index < tagIDLength; index++ )
        {
            if(0 < index && index%2 == 0)
            {
                    *pTagReportData++ = '-';
            }
            sprintf(pTagReportData,"%02X", pTagID[index]);
            while(*pTagReportData) pTagReportData++;
        }

        printf("\n CRC %X PC %X XPC %X Tag ID: %s", crc,PC,XPC,tagBuffer);
}
// Byte to Hex Wide Char String Conversion
//
wchar_t* Byte2HexWString(UINT8* pTagIDData,UINT32 tagIDLength )
{
	wchar_t wtagBuffer[4096] = {0,};
	char ptagBuffer[4096]={0,};
    char* pTagReportData = NULL;
 
	pTagReportData = ptagBuffer;
    for(UINT32 index = 0; index < tagIDLength; index++)
    {
        if(0 < index && index%2 == 0)
        {
                *pTagReportData++ = '-';
        }

        sprintf(pTagReportData,"%02X", pTagIDData[index]);
        while(*pTagReportData) pTagReportData++;
    }
	
	mbstowcs(wtagBuffer,ptagBuffer,4096);
	return wtagBuffer;
}
wchar_t* MultiByte2WideChar( const char* mbs )
{
	mbstowcs( api3Data.pRfid3WideCharString,mbs,512);
	return api3Data.pRfid3WideCharString;
}

#ifdef linux
tm SYSTEMTIME2tm(SYSTEMTIME *s)
{
	tm t;

	t.tm_year  = s->wYear - 1900;
	t.tm_mon   = s->wMonth- 1;
	t.tm_wday  = s->wDayOfWeek;
	t.tm_mday  = s->wDay;
	t.tm_yday  = 0;
	t.tm_hour  = s->wHour;
	t.tm_min   = s->wMinute;
	t.tm_sec   = s->wSecond;
	t.tm_isdst = 0;

	return t;
}


static SYSTEMTIME tm2SYSTEMTIME(tm *t,int milliSecs)
{
	SYSTEMTIME s;

	s.wYear      = t->tm_year + 1900;
	s.wMonth     = t->tm_mon  + 1;
	s.wDayOfWeek = t->tm_wday;
	s.wDay       = t->tm_mday;
	s.wHour      = t->tm_hour;
	s.wMinute    = t->tm_min;
	s.wSecond    = t->tm_sec;
	s.wMilliseconds = milliSecs;

	return s;
}


void GetLocalTime(SYSTEMTIME * pSystemTime)
{
	timeval tv;
	struct timezone tz;
	tm *ptm;
	gettimeofday(&tv,&tz);
	ptm = localtime(&(tv.tv_sec));
	*pSystemTime = tm2SYSTEMTIME(ptm,tv.tv_usec/1000);
}
#endif
BOOLEAN ChangeRegion(RegionInfo *pInfo, ChannelSelectInfo* pChannelsInfo, BOOLEAN Hopping)
{
	ACTIVE_REGION_INFO activeRegion;
	CHANNEL_LIST ChannelTable;
	ERROR_INFO errorInfo;
	int ChannelIndex = 0;

	
	if (pChannelsInfo != NULL) // EU Region
	{
		if (pChannelsInfo->freqCount > 1)
		{
			ChannelTable.ChannelList = new UINT32[pChannelsInfo->freqCount];
		}

		ChannelTable.iNumChannels = pChannelsInfo->freqCount;

		if (pChannelsInfo->ChannelInfo[0] == 1)
		{
			ChannelTable.ChannelList[ChannelIndex++] = 1;
		}

		if (pChannelsInfo->ChannelInfo[1] == 1)
		{
			ChannelTable.ChannelList[ChannelIndex++] = 2;
		}

		if (pChannelsInfo->ChannelInfo[2] == 1)
		{
			ChannelTable.ChannelList[ChannelIndex++] = 3;
		}

		if (pChannelsInfo->freqCount > 3 && pChannelsInfo[0].ChannelInfo[3] == 1)
		{
			ChannelTable.ChannelList[ChannelIndex++] = 4;
		}
	}
	api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegion);
	if(wcscmp(activeRegion.strRegionName, pInfo->RegionName) != 0)
	{
		api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, pInfo->RegionName, pInfo->StandardName);
		wprintf(L"\n RFID_SetActiveRegion() %ls %ls", pInfo->RegionName, pInfo->StandardName);
	}
	if (pInfo->bEURegion)
	{
		api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, Hopping, &ChannelTable);
	}

	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n RFID_SetFrequencySetting() failure error Msg %ls", RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo));
		return FALSE;
	}

	return TRUE;
}


// Common Wrapper definition for getting SystemTime
//
void rfid3SystemTime( SYSTEMTIME* rfid3TIME )
{
#ifdef linux
	GetLocalTime( rfid3TIME );
#else
	GetLocalTime( rfid3TIME );
#endif
}


// Common Wrapper definition for Sleep
//
void rfid3Sleep( int milliSeconds )
{
#ifdef linux
	sleep( milliSeconds /1000);
#else
	Sleep( milliSeconds );
#endif
}
wchar_t* GetCountryNamebyCode(int Code)
{
	switch (Code)
	{

		case 4:		wcscpy(CountryName, L"Afghanistan"); break;
		case 8:		wcscpy(CountryName, L"Albania"); break;
		case 10:	wcscpy(CountryName, L"	Antarctica"); break;
		case 12:	wcscpy(CountryName, L"	Algeria"); break;
		case 16:	wcscpy(CountryName, L"	American Samoa"); break;
		case 20:	wcscpy(CountryName, L"	Andorra"); break;
		case 24:	wcscpy(CountryName, L"	Angola"); break;
		case 28:	wcscpy(CountryName, L"	Antigua and Barbuda"); break;
		case 31:	wcscpy(CountryName, L"	Azerbaijan"); break;
		case 32:	wcscpy(CountryName, L"	Argentina"); break;
		case 36:	wcscpy(CountryName, L"	Australia"); break;
		case 40:	wcscpy(CountryName, L"	Austria"); break;
		case 44:	wcscpy(CountryName, L"	Bahamas (the)"); break;
		case 48:	wcscpy(CountryName, L"	Bahrain"); break;
		case 50:	wcscpy(CountryName, L"	Bangladesh"); break;
		case 51:	wcscpy(CountryName, L"	Armenia"); break;
		case 52:	wcscpy(CountryName, L"	Barbados"); break;
		case 56:	wcscpy(CountryName, L"	Belgium"); break;
		case 60:	wcscpy(CountryName, L"	Bermuda"); break;
		case 64:	wcscpy(CountryName, L"	Bhutan"); break;
		case 68:	wcscpy(CountryName, L"	Bolivia (Plurinational State of)"); break;
		case 70:	wcscpy(CountryName, L"	Bosnia and Herzegovina"); break;
		case 72:	wcscpy(CountryName, L"	Botswana"); break;
		case 74:	wcscpy(CountryName, L"	Bouvet Island"); break;
		case 76:	wcscpy(CountryName, L"	Brazil"); break;
		case 84:	wcscpy(CountryName, L"	Belize"); break;
		case 86:	wcscpy(CountryName, L"	British Indian Ocean Territory (the)"); break;
		case 90:	wcscpy(CountryName, L"	Solomon Islands"); break;
		case 92:	wcscpy(CountryName, L"	Virgin Islands (British)"); break;
		case 96:	wcscpy(CountryName, L"	Brunei Darussalam"); break;
		case 100:	wcscpy(CountryName, L"Bulgaria"); break; 
		case 104:	wcscpy(CountryName, L"Myanmar"); break;
		case 108:	wcscpy(CountryName, L"Burundi"); break;
		case 112:	wcscpy(CountryName, L"Belarus"); break;
		case 116:	wcscpy(CountryName, L"Cambodia"); break;
		case 120:	wcscpy(CountryName, L"Cameroon"); break;
		case 124:	wcscpy(CountryName, L"Canada"); break;
		case 132:	wcscpy(CountryName, L"Cabo Verde"); break;
		case 136:	wcscpy(CountryName, L"Cayman Islands (the)"); break;
		case 140:	wcscpy(CountryName, L"Central African Republic (the)"); break;
		case 144:	wcscpy(CountryName, L"Sri Lanka"); break;
		case 148:	wcscpy(CountryName, L"Chad"); break;
		case 152:	wcscpy(CountryName, L"Chile"); break;
		case 156:	wcscpy(CountryName, L"China"); break;
		case 158:	wcscpy(CountryName, L"Taiwan (Province of China)"); break;
		case 162:	wcscpy(CountryName, L"Christmas Island"); break;
		case 166:	wcscpy(CountryName, L"Cocos (Keeling) Islands (the)"); break;
		case 170:	wcscpy(CountryName, L"Colombia"); break;
		case 174:	wcscpy(CountryName, L"Comoros (the)"); break;
		case 175:	wcscpy(CountryName, L"Mayotte"); break;
		case 178:	wcscpy(CountryName, L"Congo (the)"); break;
		case 180:	wcscpy(CountryName, L"Congo (the Democratic Republic of the)"); break;
		case 184:	wcscpy(CountryName, L"Cook Islands (the)"); break;
		case 188:	wcscpy(CountryName, L"Costa Rica"); break;
		case 191:	wcscpy(CountryName, L"Croatia"); break;
		case 192:	wcscpy(CountryName, L"Cuba"); break;
		case 196:	wcscpy(CountryName, L"Cyprus"); break;
		case 203:	wcscpy(CountryName, L"Czechia"); break;
		case 204:	wcscpy(CountryName, L"Benin"); break;
		case 208:	wcscpy(CountryName, L"Denmark"); break;
		case 212:	wcscpy(CountryName, L"Dominica"); break;
		case 214:	wcscpy(CountryName, L"Dominican Republic (the)"); break;
		case 218:	wcscpy(CountryName, L"Ecuador"); break;
		case 222:	wcscpy(CountryName, L"El Salvador"); break;
		case 226:	wcscpy(CountryName, L"Equatorial Guinea"); break;
		case 231:	wcscpy(CountryName, L"Ethiopia"); break;
		case 232:	wcscpy(CountryName, L"Eritrea"); break;
		case 233:	wcscpy(CountryName, L"Estonia"); break;
		case 234:	wcscpy(CountryName, L"Faroe Islands (the)"); break;
		case 238:	wcscpy(CountryName, L"Falkland Islands (the) [Malvinas]"); break;
		case 239:	wcscpy(CountryName, L"South Georgia and the South Sandwich Islands"); break;
		case 242:	wcscpy(CountryName, L"Fiji"); break;
		case 246:	wcscpy(CountryName, L"Finland"); break;
		case 248:	wcscpy(CountryName, L"Aland Islands"); break;
		case 250:	wcscpy(CountryName, L"France"); break;
		case 254:	wcscpy(CountryName, L"French Guiana"); break;
		case 258:	wcscpy(CountryName, L"French Polynesia"); break;
		case 260:	wcscpy(CountryName, L"French Southern Territories (the)"); break;
		case 262:	wcscpy(CountryName, L"Djibouti"); break;
		case 266:	wcscpy(CountryName, L"Gabon"); break;
		case 268:	wcscpy(CountryName, L"Georgia"); break;
		case 270:	wcscpy(CountryName, L"Gambia (the)"); break;
		case 275:	wcscpy(CountryName, L"Palestine, State of"); break;
		case 276:	wcscpy(CountryName, L"Germany"); break;
		case 288:	wcscpy(CountryName, L"Ghana"); break;
		case 292:	wcscpy(CountryName, L"Gibraltar"); break;
		case 296:	wcscpy(CountryName, L"Kiribati"); break;
		case 300:	wcscpy(CountryName, L"Greece"); break;
		case 304:	wcscpy(CountryName, L"Greenland"); break;
		case 308:	wcscpy(CountryName, L"Grenada"); break;
		case 312:	wcscpy(CountryName, L"Guadeloupe"); break;
		case 316:	wcscpy(CountryName, L"Guam"); break;
		case 320:	wcscpy(CountryName, L"Guatemala"); break;
		case 324:	wcscpy(CountryName, L"Guinea"); break;
		case 328:	wcscpy(CountryName, L"Guyana"); break;
		case 332:	wcscpy(CountryName, L"Haiti"); break;
		case 334:	wcscpy(CountryName, L"Heard Island and McDonald Islands"); break;
		case 336:	wcscpy(CountryName, L"Holy See (the)"); break;
		case 340:	wcscpy(CountryName, L"Honduras"); break;
		case 344:	wcscpy(CountryName, L"Hong Kong"); break;
		case 348:	wcscpy(CountryName, L"Hungary"); break;
		case 352:	wcscpy(CountryName, L"Iceland"); break;
		case 356:	wcscpy(CountryName, L"India"); break;
		case 360:	wcscpy(CountryName, L"Indonesia"); break;
		case 364:	wcscpy(CountryName, L"Iran (Islamic Republic of)"); break;
		case 368:	wcscpy(CountryName, L"Iraq"); break;
		case 372:	wcscpy(CountryName, L"Ireland"); break;
		case 376:	wcscpy(CountryName, L"Israel"); break;
		case 380:	wcscpy(CountryName, L"Italy"); break;
		case 384:	wcscpy(CountryName, L"Ivory Coast"); break;
		case 388:	wcscpy(CountryName, L"Jamaica"); break;
		case 392:	wcscpy(CountryName, L"Japan"); break;
		case 398:	wcscpy(CountryName, L"Kazakhstan"); break;
		case 400:	wcscpy(CountryName, L"Jordan"); break;
		case 404:	wcscpy(CountryName, L"Kenya"); break;
		case 408:	wcscpy(CountryName, L"Korea (the Democratic People's Republic of)"); break;
		case 410:	wcscpy(CountryName, L"Korea (the Republic of)"); break;
		case 414:	wcscpy(CountryName, L"Kuwait"); break;
		case 417:	wcscpy(CountryName, L"Kyrgyzstan"); break;
		case 418:	wcscpy(CountryName, L"Lao People's Democratic Republic (the)"); break;
		case 422:	wcscpy(CountryName, L"Lebanon"); break;
		case 426:	wcscpy(CountryName, L"Lesotho"); break;
		case 428:	wcscpy(CountryName, L"Latvia"); break;
		case 430:	wcscpy(CountryName, L"Liberia"); break;
		case 434:	wcscpy(CountryName, L"Libya"); break;
		case 438:	wcscpy(CountryName, L"Liechtenstein"); break;
		case 440:	wcscpy(CountryName, L"Lithuania"); break;
		case 442:	wcscpy(CountryName, L"Luxembourg"); break;
		case 446:	wcscpy(CountryName, L"Macao"); break;
		case 450:	wcscpy(CountryName, L"Madagascar"); break;
		case 454:	wcscpy(CountryName, L"Malawi"); break;
		case 458:	wcscpy(CountryName, L"Malaysia"); break;
		case 462:	wcscpy(CountryName, L"Maldives"); break;
		case 466:	wcscpy(CountryName, L"Mali"); break;
		case 470:	wcscpy(CountryName, L"Malta"); break;
		case 474:	wcscpy(CountryName, L"Martinique"); break;
		case 478:	wcscpy(CountryName, L"Mauritania"); break;
		case 480:	wcscpy(CountryName, L"Mauritius"); break;
		case 484:	wcscpy(CountryName, L"Mexico"); break;
		case 492:	wcscpy(CountryName, L"Monaco"); break;
		case 496:	wcscpy(CountryName, L"Mongolia"); break;
		case 498:	wcscpy(CountryName, L"Moldova (the Republic of)"); break;
		case 499:	wcscpy(CountryName, L"Montenegro"); break;
		case 500:	wcscpy(CountryName, L"Montserrat"); break;
		case 504:	wcscpy(CountryName, L"Morocco"); break;
		case 508:	wcscpy(CountryName, L"Mozambique"); break;
		case 512:	wcscpy(CountryName, L"Oman"); break;
		case 516:	wcscpy(CountryName, L"Namibia"); break;
		case 520:	wcscpy(CountryName, L"Nauru"); break;
		case 524:	wcscpy(CountryName, L"Nepal"); break;
		case 528:	wcscpy(CountryName, L"Netherlands (the)"); break;
		case 531:	wcscpy(CountryName, L"Curacao"); break;
		case 533:	wcscpy(CountryName, L"Aruba"); break;
		case 534:	wcscpy(CountryName, L"Sint Maarten (Dutch part)"); break;
		case 535:	wcscpy(CountryName, L"Bonaire, Sint Eustatius and Saba"); break;
		case 540:	wcscpy(CountryName, L"New Caledonia"); break;
		case 548:	wcscpy(CountryName, L"Vanuatu"); break;
		case 554:	wcscpy(CountryName, L"New Zealand"); break;
		case 558:	wcscpy(CountryName, L"Nicaragua"); break;
		case 562:	wcscpy(CountryName, L"Niger (the)"); break;
		case 566:	wcscpy(CountryName, L"Nigeria"); break;
		case 570:	wcscpy(CountryName, L"Niue"); break;
		case 574:	wcscpy(CountryName, L"Norfolk Island"); break;
		case 578:	wcscpy(CountryName, L"Norway"); break;
		case 580:	wcscpy(CountryName, L"Northern Mariana Islands (the)"); break;
		case 581:	wcscpy(CountryName, L"United States Minor Outlying Islands (the)"); break;
		case 583:	wcscpy(CountryName, L"Micronesia (Federated States of)"); break;
		case 584:	wcscpy(CountryName, L"Marshall Islands (the)"); break;
		case 585:	wcscpy(CountryName, L"Palau"); break;
		case 586:	wcscpy(CountryName, L"Pakistan"); break;
		case 591:	wcscpy(CountryName, L"Panama"); break;
		case 598:	wcscpy(CountryName, L"Papua New Guinea"); break;
		case 600:	wcscpy(CountryName, L"Paraguay"); break;
		case 604:	wcscpy(CountryName, L"Peru"); break;
		case 608:	wcscpy(CountryName, L"Philippines (the)"); break;
		case 612:	wcscpy(CountryName, L"Pitcairn"); break;
		case 616:	wcscpy(CountryName, L"Poland"); break;
		case 620:	wcscpy(CountryName, L"Portugal"); break;
		case 624:	wcscpy(CountryName, L"Guinea-Bissau"); break;
		case 626:	wcscpy(CountryName, L"Timor-Leste"); break;
		case 630:	wcscpy(CountryName, L"Puerto Rico"); break;
		case 634:	wcscpy(CountryName, L"Qatar"); break;
		case 638:	wcscpy(CountryName, L"Reunion"); break;
		case 642:	wcscpy(CountryName, L"Romania"); break;
		case 643:	wcscpy(CountryName, L"Russian Federation (the)"); break;
		case 646:	wcscpy(CountryName, L"Rwanda"); break;
		case 652:	wcscpy(CountryName, L"Saint Barthelemy"); break;
		case 654:	wcscpy(CountryName, L"Saint Helena, Ascension and Tristan da Cunha"); break;
		case 659:	wcscpy(CountryName, L"Saint Kitts and Nevis"); break;
		case 660:	wcscpy(CountryName, L"Anguilla"); break;
		case 662:	wcscpy(CountryName, L"Saint Lucia"); break;
		case 663:	wcscpy(CountryName, L"Saint Martin (French part)"); break;
		case 666:	wcscpy(CountryName, L"Saint Pierre and Miquelon"); break;
		case 670:	wcscpy(CountryName, L"Saint Vincent and the Grenadines"); break;
		case 674:	wcscpy(CountryName, L"San Marino"); break;
		case 678:	wcscpy(CountryName, L"Sao Tome and Principe"); break;
		case 682:	wcscpy(CountryName, L"Saudi Arabia"); break;
		case 686:	wcscpy(CountryName, L"Senegal"); break;
		case 688:	wcscpy(CountryName, L"Serbia"); break;
		case 690:	wcscpy(CountryName, L"Seychelles"); break;
		case 694:	wcscpy(CountryName, L"Sierra Leone"); break;
		case 702:	wcscpy(CountryName, L"Singapore"); break;
		case 703:	wcscpy(CountryName, L"Slovakia"); break;
		case 704:	wcscpy(CountryName, L"VietNam"); break;
		case 705:	wcscpy(CountryName, L"Slovenia"); break;
		case 706:	wcscpy(CountryName, L"Somalia"); break;
		case 710:	wcscpy(CountryName, L"South Africa"); break;
		case 716:	wcscpy(CountryName, L"Zimbabwe"); break;
		case 724:	wcscpy(CountryName, L"Spain"); break;
		case 728:	wcscpy(CountryName, L"South Sudan"); break;
		case 729:	wcscpy(CountryName, L"Sudan (the)"); break;
		case 732:	wcscpy(CountryName, L"Western Sahara"); break;
		case 740:	wcscpy(CountryName, L"Suriname"); break;
		case 744:	wcscpy(CountryName, L"Svalbard and Jan Mayen"); break;
		case 748:	wcscpy(CountryName, L"Eswatini"); break;
		case 752:	wcscpy(CountryName, L"Sweden"); break;
		case 756:	wcscpy(CountryName, L"Switzerland"); break;
		case 760:	wcscpy(CountryName, L"Syrian Arab Republic"); break;
		case 762:	wcscpy(CountryName, L"Tajikistan"); break;
		case 764:	wcscpy(CountryName, L"Thailand"); break;
		case 768:	wcscpy(CountryName, L"Togo"); break;
		case 772:	wcscpy(CountryName, L"Tokelau"); break;
		case 776:	wcscpy(CountryName, L"Tonga"); break;
		case 780:	wcscpy(CountryName, L"Trinidad and Tobago"); break;
		case 784:	wcscpy(CountryName, L"United Arab Emirates (the)"); break;
		case 788:	wcscpy(CountryName, L"Tunisia"); break;
		case 792:	wcscpy(CountryName, L"Turkey"); break;
		case 795:	wcscpy(CountryName, L"Turkmenistan"); break;
		case 796:	wcscpy(CountryName, L"Turks and Caicos Islands (the)"); break;
		case 798:	wcscpy(CountryName, L"Tuvalu"); break;
		case 800:	wcscpy(CountryName, L"Uganda"); break;
		case 804:	wcscpy(CountryName, L"Ukraine"); break;
		case 807:	wcscpy(CountryName, L"Republic of North Macedonia"); break;
		case 818:	wcscpy(CountryName, L"Egypt"); break;
		case 826:	wcscpy(CountryName, L"United Kingdom of Great Britain and Northern Ireland (the)"); break;
		case 831:	wcscpy(CountryName, L"Guernsey"); break;
		case 832:	wcscpy(CountryName, L"Jersey"); break;
		case 833:	wcscpy(CountryName, L"Isle of Man"); break;
		case 834:	wcscpy(CountryName, L"Tanzania, United Republic of"); break;
		case 840:	wcscpy(CountryName, L"United States of America (the)"); break;
		case 850:	wcscpy(CountryName, L"Virgin Islands (U.S.)"); break;
		case 854:	wcscpy(CountryName, L"Burkina Faso"); break;
		case 858:	wcscpy(CountryName, L"Uruguay"); break;
		case 860:	wcscpy(CountryName, L"Uzbekistan"); break;
		case 862:	wcscpy(CountryName, L"Venezuela (Bolivarian Republic of)"); break;
		case 876:	wcscpy(CountryName, L"Wallis and Futuna"); break;
		case 882:	wcscpy(CountryName, L"Samoa"); break;
		case 887:	wcscpy(CountryName, L"Yemen"); break;
		case 894:	wcscpy(CountryName, L"Zambia"); break;
		default:
			wcscpy(CountryName, L"UnKnown Country Code"); break;
			break;
	}
	return CountryName;
}
wchar_t* PrintTime( void )
{
	SYSTEMTIME systime;
	rfid3SystemTime(&systime);
	wchar_t day[16];
	// Determine day of the week.

	switch (systime.wDayOfWeek)
	{
	case 0:
	   wcscpy( day,L"Sunday" );
	   break;
	case 1:
	   wcscpy( day,L"Monday" );
	   break;
	case 2:
	   wcscpy( day,L"Tuesday" );
	   break;
	case 3:
	   wcscpy( day,L"Wednesday" );
	   break;
	case 4:
	   wcscpy( day,L"Thursday" );
	   break;
	case 5:
	   wcscpy( day,L"Friday" );
	   break;
	case 6:
	   wcscpy( day,L"Saturday" );
	   break;
	}

	// Show the time in a message box.
	//
	rfid_swprintf( CurrentTime, 512, L"%ls %04d-%02d-%02d  %02d:%02d:%02d:%03d", day,systime.wYear, systime.wMonth, systime.wDay,systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds );
	return CurrentTime;
}
char G2MEMBANK[][32] = {"MEMORY_BANK_RESERVED", "MEMORY_BANK_EPC", "MEMORY_BANK_TID", "MEMORY_BANK_USER"};
char SESSIONS[][16] = {"SESSION_S0","SESSION_S1","SESSION_S2","SESSION_S3"};
char SLFLAGS[][32] = {"SL_FLAG_ASSERTED","SL_FLAG_DEASSERTED"};
char INVENTORYSTATES[][32] = {"INVENTORY_STATE_A","INVENTORY_STATE_B"};
char TAGEVENTS[][32] = {"UNKNOWN_STATE", "NEW_TAG_VISIBLE", "TAG_NOT_VISIBLE", "TAG_BACK_TO_VISIBILITY","TAG_MOVING","TAG_STATIONARY","NONE"};
char UNAWAREACTION[][64] = {"STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT","STATE_UNAWARE_ACTION_SELECT","STATE_UNAWARE_ACTION_NOT_UNSELECT","STATE_UNAWARE_ACTION_UNSELECT","STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT","STATE_UNAWARE_ACTION_NOT_SELECT"};
char ACCESSSTATUS[][64] = {"ACCESS_SUCCESS","ACCESS_TAG_NON_SPECIFIC_ERROR","ACCESS_READER_NON_SPECIFIC_ERROR","ACCESS_NO_RESPONSE_FROM_TAG","ACCESS_INSUFFIFICENT_POWER","ACCESS_INSUFFICENT_POWER","ACCESS_TAG_MEMORY_LOCKED_ERROR","ACCESS_TAG_MEMORY_OVERRUN_ERROR","ACCESS_ZERO_KILL_PASSWORD_ERROR"};
char RECOMOPCODES[][64] = {"RECOMMISSION_DISABLE_PERMALOCK","RECOMMISSION_DISABLE_USER_MEMORY","RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION","RECOMMISSION_DISABLE_PASSWORD","RECOMMISSION_DISABLE_PERMALOCK_PASSWORD","RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD","RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION"};	
char LOCKPRIVILEGE[][32] = {"LOCK_PRIVILEGE_NONE","LOCK_PRIVILEGE_READ_WRITE","LOCK_PRIVILEGE_READ_WRITE","LOCK_PRIVILEGE_PERMA_LOCK","LOCK_PRIVILEGE_PERMA_UNLOCK","LOCK_PRIVILEGE_UNLOCK"};
char LOCKDATAFIELD[][32] = {"LOCK_KILL_PASSWORD","LOCK_ACCESS_PASSWORD","LOCK_EPC_MEMORY","LOCK_TID_MEMORY","LOCK_USER_MEMORY"};
char COMMNSTANDARD[][64] = {"UNSPECIFIED","US_FCC_PART_15","ETSI_302_208","ETSI_300_220","AUSTRALIA_LIPD_1W","AUSTRALIA_LIPD_4W","JAPAN_ARIB_STD_T89","HONGKONG_OFTA_1049","TAIWAN_DGT_LP0002","KOREA_MIC_ARTICLE_5_2"};
char ERRORSTATUS[][64] = {
							"RFID_API_SUCCESS","RFID_API_COMMAND_TIMEOUT","RFID_API_PARAM_ERROR","RFID_API_PARAM_OUT_OF_RANGE","RFID_API_CANNOT_ALLOC_MEM","RFID_API_UNKNOWN_ERROR","RFID_API_INVALID_HANDLE","RFID_API_BUFFER_TOO_SMALL",
							"RFID_READER_FUNCTION_UNSUPPORTED","RFID_RECONNECT_FAILED","RFID_COMM_OPEN_ERROR","RFID_COMM_CONNECTION_ALREADY_EXISTS","RFID_COMM_RESOLVE_ERROR","RFID_COMM_SEND_ERROR",
							"RFID_COMM_RECV_ERROR","RFID_COMM_NO_CONNECTION","RFID_INVALID_SOCKET","RFID_READER_REGION_NOT_CONFIGURED","RFID_READER_REINITIALIZING","RFID_CONFIG_GET_FAILED",
							"RFID_CONFIG_SET_FAILED","RFID_CAP_NOT_SUPPORTED","RFID_CAP_GET_FAILED","RFID_FILTER_NO_FILTER","RFID_FILTER_INVALID_INDEX","RFID_FILTER_MAX_FILTERS_EXCEEDED","RFID_NO_READ_TAGS","RFID_NO_REPORTED_EVENTS",
							"RFID_INVENTORY_MAX_TAGS_EXCEEDED","RFID_INVENTORY_IN_PROGRESS","RFID_NO_INVENTORY_IN_PROGRESS","RFID_TAG_LOCATING_IN_PROGRESS","RFID_NO_TAG_LOCATING_IN_PROGRESS","RFID_NXP_EAS_SCAN_IN_PROGRESS",
							"RFID_NO_NXP_EAS_SCAN_IN_PROGRESS","RFID_PORTAL_DIRECTIONALITY_OPERTATION_IN_PROGRESS","RFID_NO_PORTAL_DIRECTIONALITY_OPERTATION_IN_PROGRESS","RFID_ACCESS_IN_PROGRESS","RFID_NO_ACCESS_IN_PROGRESS",
							"RFID_ACCESS_TAG_READ_FAILED","RFID_ACCESS_TAG_WRITE_FAILED","RFID_ACCESS_TAG_LOCK_FAILED","RFID_ACCESS_TAG_KILL_FAILED","RFID_ACCESS_TAG_BLOCK_ERASE_FAILED","RFID_ACCESS_TAG_BLOCK_WRITE_FAILED",
							"RFID_ACCESS_TAG_NOT_FOUND","RFID_ACCESS_SEQUENCE_NOT_INITIALIZED","RFID_ACCESS_SEQUENCE_EMPTY","RFID_ACCESS_SEQUENCE_IN_USE","RFID_ACCESS_SEQUENCE_MAX_OP_EXCEEDED","RFID_ACCESS_TAG_RECOMMISSION_FAILED","RFID_ACCESS_TAG_BLOCK_PERMALOCK_FAILED","RFID_ACCESS_NXP_TAG_SET_EAS_FAILED","RFID_ACCESS_NXP_TAG_READ_PROTECT_FAILED","RFID_ACCESS_FUJITSU_CHANGE_WORDLOCK_FAILED",
							"RFID_ACCESS_FUJITSU_CHANGE_BLOCKLOCK_FAILED" ,"RFID_ACCESS_FUJITSU_READ_BLOCKLOCK_FAILED","RFID_ACCESS_FUJITSU_BURST_WRITE_FAILED","RFID_ACCESS_FUJITSU_BURST_ERASE_FAILED","RFID_ACCESS_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_FAILED","RFID_ACCESS_FUJITSU_AREA_READLOCK_FAILED","RFID_ACCESS_FUJITSU_AREA_WRITELOCK_FAILED",
							"RFID_ACCESS_FUJITSU_AREA_WRITELOCK_WOPASSWORD_FAILED","RFID_ACCESS_NXP_CHANGE_CONFIG_FAILED","RFID_ACCESS_IMPINJ_QT_READ_FAILED","RFID_ACCESS_IMPINJ_QT_WRITE_FAILED","RFID_RM_INVALID_USERNAME_PASSWORD","RFID_RM_NO_UPDATION_IN_PROGRESS","RFID_RM_UPDATION_IN_PROGRESS","RFID_RM_COMMAND_FAILED","RFID_INVALID_ERROR_CODE"
						};
char ACCESSOPERATIONCODES[][64] = { "ACCESS_OPERATION_READ","ACCESS_OPERATION_WRITE","ACCESS_OPERATION_LOCK","ACCESS_OPERATION_KILL","ACCESS_OPERATION_BLOCK_WRITE","ACCESS_OPERATION_BLOCK_ERASE",
									"ACCESS_OPERATION_RECOMMISSION","ACCESS_OPERATION_BLOCK_PERMALOCK","ACCESS_OPERATION_NXP_SET_EAS","ACCESS_OPERATION_NXP_READ_PROTECT","ACCESS_OPERATION_NXP_RESET_READ_PROTECT",
									"ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK","ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK","ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK","ACCESS_OPERATION_FUJITSU_BURST_WRITE",
									"ACCESS_OPERATION_FUJITSU_BURST_ERASE","ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD","ACCESS_OPERATION_FUJITSU_AREA_READLOCK","ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK",
									"ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD","ACCESS_OPERATION_IMPINJ_QT_WRITE","ACCESS_OPERATION_IMPINJ_QT_READ","ACCESS_OPERATION_NXP_CHANGE_CONFIG"
								  };
char TAGEVENTREPORTTRIGGER[][32] = { "NEVER","IMMEDIATE","MODERATED" };

// Wide char versions ..To support on Linux
//
wchar_t wG2MEMBANK[][32] = {L"MEMORY_BANK_RESERVED", L"MEMORY_BANK_EPC", L"MEMORY_BANK_TID", L"MEMORY_BANK_USER"};
wchar_t wSESSIONS[][16] = {L"SESSION_S0",L"SESSION_S1",L"SESSION_S2",L"SESSION_S3"};
wchar_t wSLFLAGS[][32] = {L"SL_FLAG_ASSERTED",L"SL_FLAG_DEASSERTED"};
wchar_t wINVENTORYSTATES[][32] = {L"INVENTORY_STATE_A",L"INVENTORY_STATE_B"};
wchar_t wTAGEVENTS[][32] = {L"UNKNOWN_STATE", L"NEW_TAG_VISIBLE", L"TAG_NOT_VISIBLE", L"TAG_BACK_TO_VISIBILITY",L"TAG_MOVING",L"TAG_STATIONARY",L"NONE"};
wchar_t wUNAWAREACTION[][64] = {L"STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT",L"STATE_UNAWARE_ACTION_SELECT",L"STATE_UNAWARE_ACTION_NOT_UNSELECT",L"STATE_UNAWARE_ACTION_UNSELECT",L"STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT",L"STATE_UNAWARE_ACTION_NOT_SELECT"};
wchar_t wACCESSSTATUS[][64] = {L"ACCESS_SUCCESS",L"ACCESS_TAG_NON_SPECIFIC_ERROR",L"ACCESS_READER_NON_SPECIFIC_ERROR",L"ACCESS_NO_RESPONSE_FROM_TAG",L"ACCESS_INSUFFIFICENT_POWER",L"ACCESS_INSUFFICENT_POWER",L"ACCESS_TAG_MEMORY_LOCKED_ERROR",L"ACCESS_TAG_MEMORY_OVERRUN_ERROR",L"ACCESS_ZERO_KILL_PASSWORD_ERROR"};
wchar_t wRECOMOPCODES[][64] = {L"RECOMMISSION_DISABLE_PERMALOCK",L"RECOMMISSION_DISABLE_USER_MEMORY",L"RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION",L"RECOMMISSION_DISABLE_PASSWORD",L"RECOMMISSION_DISABLE_PERMALOCK_PASSWORD",L"RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD",L"RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION"};	
wchar_t wLOCKPRIVILEGE[][32] = {L"LOCK_PRIVILEGE_NONE",L"LOCK_PRIVILEGE_READ_WRITE",L"LOCK_PRIVILEGE_READ_WRITE",L"LOCK_PRIVILEGE_PERMA_LOCK",L"LOCK_PRIVILEGE_PERMA_UNLOCK",L"LOCK_PRIVILEGE_UNLOCK"};
wchar_t wLOCKDATAFIELD[][32] = {L"LOCK_KILL_PASSWORD",L"LOCK_ACCESS_PASSWORD",L"LOCK_EPC_MEMORY",L"LOCK_TID_MEMORY",L"LOCK_USER_MEMORY"};
wchar_t wCOMMNSTANDARD[][64] = {L"UNSPECIFIED",L"US_FCC_PART_15",L"ETSI_302_208",L"ETSI_300_220",L"AUSTRALIA_LIPD_1W",L"AUSTRALIA_LIPD_4W",L"JAPAN_ARIB_STD_T89",L"HONGKONG_OFTA_1049",L"TAIWAN_DGT_LP0002",L"KOREA_MIC_ARTICLE_5_2"};
wchar_t wERRORSTATUS[][64] = {
							L"RFID_API_SUCCESS",L"RFID_API_COMMAND_TIMEOUT",L"RFID_API_PARAM_ERROR",L"RFID_API_PARAM_OUT_OF_RANGE",L"RFID_API_CANNOT_ALLOC_MEM",L"RFID_API_UNKNOWN_ERROR",L"RFID_API_INVALID_HANDLE",L"RFID_API_BUFFER_TOO_SMALL",
							L"RFID_READER_FUNCTION_UNSUPPORTED",L"RFID_RECONNECT_FAILED",L"RFID_COMM_OPEN_ERROR",L"RFID_COMM_CONNECTION_ALREADY_EXISTS",L"RFID_COMM_RESOLVE_ERROR",L"RFID_COMM_SEND_ERROR",
							L"RFID_COMM_RECV_ERROR",L"RFID_COMM_NO_CONNECTION",L"RFID_INVALID_SOCKET",L"RFID_READER_REGION_NOT_CONFIGURED",L"RFID_READER_REINITIALIZING",L"RFID_CONFIG_GET_FAILED",
							L"RFID_CONFIG_SET_FAILED",L"RFID_CAP_NOT_SUPPORTED",L"RFID_CAP_GET_FAILED",L"RFID_FILTER_NO_FILTER",L"RFID_FILTER_INVALID_INDEX",L"RFID_FILTER_MAX_FILTERS_EXCEEDED",L"RFID_NO_READ_TAGS",L"RFID_NO_REPORTED_EVENTS",
							L"RFID_INVENTORY_MAX_TAGS_EXCEEDED",L"RFID_INVENTORY_IN_PROGRESS",L"RFID_NO_INVENTORY_IN_PROGRESS",L"RFID_TAG_LOCATING_IN_PROGRESS",L"RFID_NO_TAG_LOCATING_IN_PROGRESS",L"RFID_NXP_EAS_SCAN_IN_PROGRESS",
							L"RFID_NO_NXP_EAS_SCAN_IN_PROGRESS",L"RFID_PORTAL_DIRECTIONALITY_OPERTATION_IN_PROGRESS",L"RFID_NO_PORTAL_DIRECTIONALITY_OPERTATION_IN_PROGRESS",L"RFID_ACCESS_IN_PROGRESS",L"RFID_NO_ACCESS_IN_PROGRESS",
							L"RFID_ACCESS_TAG_READ_FAILED",L"RFID_ACCESS_TAG_WRITE_FAILED",L"RFID_ACCESS_TAG_LOCK_FAILED",L"RFID_ACCESS_TAG_KILL_FAILED",L"RFID_ACCESS_TAG_BLOCK_ERASE_FAILED",L"RFID_ACCESS_TAG_BLOCK_WRITE_FAILED",
							L"RFID_ACCESS_TAG_NOT_FOUND",L"RFID_ACCESS_SEQUENCE_NOT_INITIALIZED",L"RFID_ACCESS_SEQUENCE_EMPTY",L"RFID_ACCESS_SEQUENCE_IN_USE",L"RFID_ACCESS_SEQUENCE_MAX_OP_EXCEEDED",L"RFID_ACCESS_TAG_RECOMMISSION_FAILED",L"RFID_ACCESS_TAG_BLOCK_PERMALOCK_FAILED",L"RFID_ACCESS_NXP_TAG_SET_EAS_FAILED",L"RFID_ACCESS_NXP_TAG_READ_PROTECT_FAILED",L"RFID_ACCESS_FUJITSU_CHANGE_WORDLOCK_FAILED",
							L"RFID_ACCESS_FUJITSU_CHANGE_BLOCKLOCK_FAILED" ,L"RFID_ACCESS_FUJITSU_READ_BLOCKLOCK_FAILED",L"RFID_ACCESS_FUJITSU_BURST_WRITE_FAILED",L"RFID_ACCESS_FUJITSU_BURST_ERASE_FAILED",L"RFID_ACCESS_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_FAILED",L"RFID_ACCESS_FUJITSU_AREA_READLOCK_FAILED",L"RFID_ACCESS_FUJITSU_AREA_WRITELOCK_FAILED",
							L"RFID_ACCESS_FUJITSU_AREA_WRITELOCK_WOPASSWORD_FAILED",L"RFID_ACCESS_NXP_CHANGE_CONFIG_FAILED",L"RFID_ACCESS_IMPINJ_QT_READ_FAILED",L"RFID_ACCESS_IMPINJ_QT_WRITE_FAILED",L"RFID_RM_INVALID_USERNAME_PASSWORD",L"RFID_RM_NO_UPDATION_IN_PROGRESS",L"RFID_RM_UPDATION_IN_PROGRESS",L"RFID_RM_COMMAND_FAILED",L"RFID_INVALID_ERROR_CODE"
						};
wchar_t wACCESSOPERATIONCODES[][64] = { L"ACCESS_OPERATION_READ",L"ACCESS_OPERATION_WRITE",L"ACCESS_OPERATION_LOCK",L"ACCESS_OPERATION_KILL",L"ACCESS_OPERATION_BLOCK_WRITE",L"ACCESS_OPERATION_BLOCK_ERASE",
									L"ACCESS_OPERATION_RECOMMISSION",L"ACCESS_OPERATION_BLOCK_PERMALOCK",L"ACCESS_OPERATION_NXP_SET_EAS",L"ACCESS_OPERATION_NXP_READ_PROTECT",L"ACCESS_OPERATION_NXP_RESET_READ_PROTECT",
									L"ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK",L"ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK",L"ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK",L"ACCESS_OPERATION_FUJITSU_BURST_WRITE",
									L"ACCESS_OPERATION_FUJITSU_BURST_ERASE",L"ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD",L"ACCESS_OPERATION_FUJITSU_AREA_READLOCK",L"ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK",
									L"ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD",L"ACCESS_OPERATION_IMPINJ_QT_WRITE",L"ACCESS_OPERATION_IMPINJ_QT_READ",L"ACCESS_OPERATION_NXP_CHANGE_CONFIG"
								  };
wchar_t wTAGEVENTREPORTTRIGGER[][32] = { L"NEVER",L"IMMEDIATE",L"MODERATED" };
wchar_t wRFIDEVENTS[][32] = { L"GPI_EVENT",L"TAG_DATA_EVENT",L"BUFFER_FULL_WARNING_EVENT",L"ANTENNA_EVENT",L"INVENTORY_START_EVENT",L"INVENTORY_STOP_EVENT",L"ACCESS_START_EVENT",L"ACCESS_STOP_EVENT",L"BUFFER_FULL_EVENT",L"NXP_EAS_ALARM_EVENT",L"READER_EXCEPTION_EVENT",L"HANDHELD_TRIGGER_EVENT"};
wchar_t wMODULATION[][16] = { L"MV_FM0",L"MV_2",L"MV_4",L"MV_8"};
wchar_t wFORWARD_LINK_MODULATION[][32] = {L"FORWARD_LINK_MODULATION_PR_ASK",L"FORWARD_LINK_MODULATION_SSB_ASK",L"FORWARD_LINK_MODULATION_DSB_ASK"};
wchar_t wSPECTRAL_MASK_INDICATOR[][16] = { L"SMI_UNKNOWN",L"SMI_SI",L"SMI_MI",L"SMI_DI"};
