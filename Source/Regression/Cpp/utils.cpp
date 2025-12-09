#include "stdafx.h"
#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[2];

DWORD ReadTags( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT16 offSet,ANTENNA_INFO* pInfo,TRIGGER_INFO* tInfo,TAGS2READ tags2Read )
{
	UINT16 tagCount = 0;
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );
	api3Data.rfidStatus = RFID_PurgeTags( hReader, 0 );
	api3Data.rfidStatus = RFID_PerformInventory(hReader,NULL,pInfo,tInfo,NULL );
	rfid3Sleep( 5000 );	
	api3Data.rfidStatus = RFID_StopInventory( hReader );	

	bool bSuccess = TRUE;
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{		
		tagCount++;

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d SeenCount: %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID,pTagData->tagSeenCount );
		wprintf(L"\n TAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d SeenCount: %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID,pTagData->tagSeenCount );
		switch(tags2Read)
		{
		case PATTERN_ONE_TAGS:
			//read only matching tags , failure is any non matching tag is read.
			//
			if( (pTagData->pTagID[0] != MyTags[0].epc[0]) || (pTagData->pTagID[1] != MyTags[0].epc[1]) || (pTagData->pTagID[2] != MyTags[0].epc[2]) || (pTagData->pTagID[3] != MyTags[0].epc[3]) )
			{
				bSuccess = FALSE;
			}
			break;
		case PATTERN_TWO_TAGS:
			// read only non matching tags, failure if a matching tag is read.
			if( (pTagData->pTagID[0] != MyTags[1].epc[0]) || (pTagData->pTagID[1] != MyTags[1].epc[1]) || (pTagData->pTagID[2] != MyTags[1].epc[2]) || (pTagData->pTagID[3] != MyTags[1].epc[3]) )
			{
				bSuccess = FALSE;
			}
			break;
		case TAGS_OTHERTHAN_PATTERN_ONE:
			// read only non matching tags, failure if a matching tag is read.
			if( (pTagData->pTagID[0] == MyTags[0].epc[0]) && (pTagData->pTagID[1] == MyTags[0].epc[1]) && (pTagData->pTagID[2] == MyTags[0].epc[2]) && (pTagData->pTagID[3] == MyTags[0].epc[3]) )
			{
				bSuccess = FALSE;
			}
			break;
		case TAGS_OTHERTHAN_PATTERN_TWO:
			// read only non matching tags, failure if a matching tag is read.
			if( (pTagData->pTagID[0] == MyTags[1].epc[0]) && (pTagData->pTagID[1] == MyTags[1].epc[1]) && (pTagData->pTagID[2] == MyTags[1].epc[2]) && (pTagData->pTagID[3] == MyTags[1].epc[3]) )
			{
				bSuccess = FALSE;
			}
			break;
		case ALL_TAGS:
			bSuccess = TRUE;
			break;
		default:
			bSuccess = TRUE;
			break;
		}

		if(bSuccess == FALSE)
		{
			tagCount = 0;
			break;
		}
	}

	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData );
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	RFID_DeletePreFilter( hReader,0,0);
	
	return tagCount;
}