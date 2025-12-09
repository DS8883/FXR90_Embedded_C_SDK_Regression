#include "ManualTests.h"

extern struct RFID3DATA api3Data;

void TestTagFocus( RFID_VERSION version)
{
	// Note : for the Last Mask the Tags does not result in EPC+TID and only EPC. The behaviour is expected.
	CONNECTION_INFO ConInfo = { version, 0 };
	SINGULATION_CONTROL s; 
	
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	wprintf(L"\n Connect %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	
	UINT8 TagFocusMask[][4] ={{0xE2,0x80,0x1B,0x30},{0xC5,0x00,0x36,0x60},{0x8A,0x00,0x6C,0xC0},{0x14,0x00,0xD9,0x81},{0x28,0x01,0xB3,0x02},{0x50,0x03,0x66,0x04},{0xA0,0x06,0xCC,0x08},{0x40,0x0D,0x98,0x10}};
	for( int mask = 0; mask < 8;mask++)
	{
		wprintf(L"\n Result for Mask index: %ld \t Mask: %ls", mask,Byte2HexWString(TagFocusMask[mask],4));
		RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		TAG_STORAGE_SETTINGS tgSettings;
		memset(&tgSettings,0,sizeof(TAG_STORAGE_SETTINGS));
		api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
		wprintf(L"\n TagStorage %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		tgSettings.tagFields = ALL_TAG_FIELDS;
		api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings);
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S0,STATE_AWARE_ACTION_INV_A};
			addSelectRecord( api3Data.hReader,TagFocusMask[mask],MEMORY_BANK_TID,mask,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_ASRT_SL };
			addSelectRecord( api3Data.hReader,TagFocusMask[mask],MEMORY_BANK_TID,mask,32,params);
		}
		
		InitSingulationControl( &s,SESSION_S0,100,0,INVENTORY_STATE_A,SL_ALL );
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,0,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData );
		}
		RFID_DeletePreFilter(api3Data.hReader,0,0);
	}
	
	// cleanup
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}
void FastID( RFID_VERSION version)
{
	CONNECTION_INFO ConInfo = { version, 0 };
	SINGULATION_CONTROL s; 
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );

	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	 
	UINT8 FastIDMask[][4] ={{0xE2,0x80,0x11,0x30},{0xE2,0x80,0x1D,0x30},{0xC5,0x00,0x3A,0x60},{0x8A,0x00,0x74,0xC0},{0x14,0x00,0xE9,0x81},{0x28,0x01,0xD3,0x02},{0x50,0x03,0xA6,0x04},{0xA0,0x07,0x4C,0x08},{0x40,0x0E,0x98,0x10}};
	for( int mask = 0; mask < 8;mask++)
	{
		wprintf(L"\n Result for Mask index: %ld \t Mask: %ls", mask,Byte2HexWString(FastIDMask[mask],4));
		RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		TAG_STORAGE_SETTINGS tgSettings;
		memset(&tgSettings,0,sizeof(TAG_STORAGE_SETTINGS));
		api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
		tgSettings.tagFields = ALL_TAG_FIELDS;
		api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings);
		{
			STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_A};
			addSelectRecord( api3Data.hReader,FastIDMask[mask],MEMORY_BANK_TID,0,32,params);
		}

		{
			STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_ASRT_SL };
			addSelectRecord( api3Data.hReader,FastIDMask[mask],MEMORY_BANK_TID,0,32,params);
		}
		
		InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
		api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,0,0);
		rfid3Sleep( 5000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData );
		}
		RFID_DeletePreFilter(api3Data.hReader,0,0);
	}
	
	// cleanup
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
}

void TestNewStateAwareFilters( RFID_VERSION version,TRIGGER_INFO* tInfo)
{
	CONNECTION_INFO ConInfo = { version, 0 };
	SINGULATION_CONTROL s; 
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );

	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	 
	UINT8 TagFocusMask[]= {0xA2,0x2F};
	
	
	RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
	TAG_STORAGE_SETTINGS tgSettings;
	memset(&tgSettings,0,sizeof(TAG_STORAGE_SETTINGS));
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader, &tgSettings);
	{
		STATE_AWARE_ACTION_PARAMS params = {TARGET_INVENTORIED_STATE_S1,STATE_AWARE_ACTION_INV_A_NOT_INV_B};
		addSelectRecord( api3Data.hReader,TagFocusMask,MEMORY_BANK_EPC,32,16,params);
	}

	{
		STATE_AWARE_ACTION_PARAMS params = { TARGET_SL,STATE_AWARE_ACTION_ASRT_SL_NOT_DSRT_SL };
		addSelectRecord( api3Data.hReader,TagFocusMask,MEMORY_BANK_EPC,32,16,params);
	}
	
	
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_ALL );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n ----------------Tags A22F should only be reported-----------------------\n");
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_ALL );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n ----------------Tags other than A22F should be reported---------------------------\n");
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_A,SL_FLAG_ASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n --------------------Tags A22F should only be reported---------------------------\n");
	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_B,SL_FLAG_DEASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	
	wprintf(L"\n --------------------Tags other than A22F should be reported---------------\n");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_ASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	wprintf(L"\n ------------------Tags A22F should only be reported--------------------\n");

	// S1 -->ABFILP-->SLALL
	InitSingulationControl( &s,SESSION_S1,100,0,INVENTORY_STATE_AB_FLIP,SL_FLAG_DEASSERTED );
	api3Data.rfidStatus = RFID_SetSingulationControl( api3Data.hReader, 0,&s);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,tInfo,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );

	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		printTagDataWithResults( pTagData );
	}
	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	wprintf(L"\n ------------------All Tags be reported--------------------\n");

}