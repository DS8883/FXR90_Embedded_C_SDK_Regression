#include "Common.h"
#include "fujitsu64K.h"

// global data for API
extern struct RFID3DATA api3Data;

void fujitsu8KChangeAreaGroupPassword( )
{
	

}

void fujitsu8KAreaReadLock( )
{

}

void fujitsu8KAreaWriteLock( )
{

}

void fujitsu8KAreaWriteLockWithoutPassword( )
{

}

bool getfujitsu8KTag()
{
	UINT8 beddTag[] = { 0xF8,0x00 };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,0 };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,0 };
	POST_FILTER postFilter ;postFilter.lpTagPatternA = &tpA;postFilter.lpTagPatternB = &tpB;
	postFilter.matchPattern = A_AND_NOTB;
	postFilter.lpRSSIRangeFilter = NULL;

	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,(LPPOST_FILTER)&postFilter,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	if( (api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader,pTagData ) ) == RFID_API_SUCCESS )
	{
		memcpy( api3Data.pfujitsu8Ktag->pTagID,pTagData->pTagID,pTagData->tagIDLength);	
		api3Data.pfujitsu8Ktag->tagIDLength = pTagData->tagIDLength;
	}
	
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );

	if(api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		return TRUE;
	}
	else
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		return FALSE;
	}
}

void Fujitsu8KTest( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	SetLogText("Fujitsu8KTest");
	logCurrentExecution( "Fujitsu8KTest");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	
	if(!getfujitsu8KTag( ) )
	{
		wprintf(L"\n Could not get fujitsu 8K tag, test cannot proceed");
		fprintf(api3Data.fpCurLog,"\n <br>Could not get fujitsu 8K tag, test cannot proceed");
	}
	
	
	LogPassFailCount();
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	
	wprintf(L"\n Executing %ls ",MultiByte2WideChar( __FUNCTION__));
}