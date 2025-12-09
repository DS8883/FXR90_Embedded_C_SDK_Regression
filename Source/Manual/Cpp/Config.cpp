#include "ManualTests.h"

extern struct RFID3DATA api3Data;
extern wchar_t* PrintTime( void );

#ifdef linux
static sem_t gpiSemaphore;
static sem_t disconnectSemaphore;
static sem_t rdrExceptSemaphore;
static sem_t hhTEventSemaphore;
static void MonitorgpiEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&gpiSemaphore);
}
static void MonitorDisconEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&disconnectSemaphore);
}
static void RdrExceptEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&rdrExceptSemaphore);
}
static void hhTriggerEvent(RFID_HANDLE32 readerHandle, RFID_EVENT_TYPE eventType)
{
	sem_post(&hhTEventSemaphore);
}
#endif

void TestResetConfig( int Count ,RFID_VERSION version)
{
	wprintf( L"\n\n ===============================================================\n");
	wprintf( L"\n\n ===============Reset Config to Defaults - START======\n");
	wprintf( L"\n\n ===============================================================\n");
	BOOLEAN bPowerstate;
	UINT16 pRxIndex,pTxIndex,pTxFIndex;
	UINT32 pGain;
	BOOLEAN pStatus;
	LPTAG_DATA pTagData;
	ANTENNA_RF_CONFIG antRFConfig,gantRFConfig;

	for( int i = 0; i < Count; i++ )
	{
		CONNECTION_INFO ConInfo = { version, 0 };
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
		api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
		pTagData = RFID_AllocateTag( api3Data.hReader );
	//	api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,FALSE);
	//	wprintf(L"\n set Radio --> FALSE status:%ls",RFID_GetErrorDescription(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults( api3Data.hReader );
		wprintf(L"\n ResetConfig2Defauots--> API status:%ls",RFID_GetErrorDescription( api3Data.rfidStatus ));
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Get Radio status:%ls",bPowerstate?L"ON":L"OFF");

		api3Data.rfidStatus = RFID_GetAntennaRfConfig( api3Data.hReader,1,&antRFConfig);
		antRFConfig.antennaStopTrigger.stopTriggerType = ANTENNA_STOP_TRIGGER_TYPE_DURATION_MILLISECS;
		antRFConfig.antennaStopTrigger.stopTriggerValue = 4000;

		api3Data.rfidStatus = RFID_SetAntennaRfConfig( api3Data.hReader,0,&antRFConfig);
		for(int antIndex = 1;antIndex <= api3Data.rCaps.numAntennas;antIndex++)
		{
			api3Data.rfidStatus = RFID_GetAntennaRfConfig( api3Data.hReader,antIndex,&gantRFConfig);
			wprintf(L"\n Antenna RFCOnfig : antenna ID: %ld TXdbm:%d \t Tari %ld\t ReceivePort:%ld\tTxPort:%ld\tTxfreqIndx:%ld",gantRFConfig.transmitPort,api3Data.rCaps.transmitPowerLevelTable.pPowerValueList[gantRFConfig.transmitPowerIndex],gantRFConfig.tari,gantRFConfig.receivePort,antRFConfig.transmitPort,gantRFConfig.transmitFrequencyIndex);

		}
		api3Data.rfidStatus = RFID_GetAntennaConfig( api3Data.hReader,1,&pRxIndex,&pTxIndex,&pTxFIndex );
		wprintf(L"\n GetAntConfig ---> API status:%ls",RFID_GetErrorDescription( api3Data.rfidStatus ));
		api3Data.rfidStatus = RFID_SetAntennaConfig( api3Data.hReader,1,pRxIndex,pTxIndex,pTxFIndex);
		wprintf(L"\n SetAntConfig---> API status:%ls",RFID_GetErrorDescription( api3Data.rfidStatus ));
		api3Data.rfidStatus = RFID_GetPhysicalAntennaProperties(api3Data.hReader,1,&pStatus,&pGain);
		wprintf(L"\n PhySical AntProp---> API status:%ls",RFID_GetErrorDescription( api3Data.rfidStatus ));
		api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,TRUE);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,0,0,0,0);
		rfid3Sleep(6000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData ))
		{
			printTagDataWithResults( pTagData);
		}
		RFID_DeallocateTag( api3Data.hReader, pTagData );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	}
	
	printf( "\n\n ===============ResetConfig toDefaults -END======\n");
}


void StressRadioONOFF( int Count,RFID_VERSION version)
{
	wprintf(L"\n\n ===============================================================\n");
	wprintf(L"\n\n ===============Radion On --Inv---Radio OFF======\n");
	wprintf(L"\n\n ===============================================================\n");
	BOOLEAN bPowerstate = 0;
	
	CONNECTION_INFO ConInfo = { version, 0 };
	
	for( int i = 0; i < Count; i++ )
	{
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
		api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,TRUE);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n set Radio --> TRUE status:%ls",bPowerstate?L"ON":L"OFF");
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,0);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			printTagDataWithResults( pTagData );
		}
		api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,FALSE);
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
		RFID_DeallocateTag( api3Data.hReader, pTagData );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	}

	
	wprintf(L"\n\n ===============ResetConfig toDefaults -END======\n");

}


void TestGPIEvent( RFID_VERSION version,TRIGGER_INFO* ptriggerInfo )
{

	CONNECTION_INFO ConInfo = { version , 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	RFID_EVENT_TYPE rfideventType = GPI_EVENT ;
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	sem_init(&gpiSemaphore, 0, 0);		
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorgpiEvent,NULL,NULL);
#else
	// Create and Register tag read event handle
	//
	HANDLE hgpiEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,rfideventType,hgpiEvent);
#endif

	// Perform inventory to Start access and inventory
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,ptriggerInfo,NULL);
#ifdef linux
	if( sem_wait(&gpiSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( hgpiEvent, 20000 ) )
	{
#endif
		GPI_EVENT_DATA gpiEventData;
		while( RFID_API_SUCCESS == RFID_GetEventData(api3Data.hReader, GPI_EVENT, (STRUCT_HANDLE)&gpiEventData))
		{
			wprintf(L"\nTest GPI Event: PASS");
			wprintf(L" GPI State:%d on GPI Port:%d",gpiEventData.eventInfo,gpiEventData.port );
		}
	}
	else
	{
		wprintf(L"\n -------gpi Event is Not Signalled-------\n");	
	}
	
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,GPI_EVENT );
	CloseHandle( hgpiEvent );
#endif
}

void TestDisconEvent( RFID_VERSION version,TRIGGER_INFO* ptriggerInfo )
{

	CONNECTION_INFO ConInfo = { version , 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	RFID_EVENT_TYPE rfideventType = DISCONNECTION_EVENT ;
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	sem_init(&disconnectSemaphore, 0, 0);		
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,MonitorDisconEvent,NULL,NULL);
#else
	// Create and Register tag read event handle
	//
	HANDLE hDisconEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,rfideventType,hDisconEvent);
#endif

	// Perform inventory to Start access and inventory
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,NULL,NULL,ptriggerInfo,NULL);
#ifdef linux
	if( sem_wait(&disconnectSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( hDisconEvent, 20000 ) )
	{
#endif
		DISCONNECTION_EVENT_DATA disconEventData;
		wprintf(L"\n>>>>>Reader Disconnect Event is Signalled<<<<<<<<<");
		while(RFID_API_SUCCESS == RFID_GetEventData( api3Data.hReader, DISCONNECTION_EVENT,(STRUCT_HANDLE)&disconEventData))
		{
			if( disconEventData.eventInfo == 0 )
			{
				wprintf(L" \n Disconnet Event Data Reason : %ls",L"READER_INITIATED_DISCONNECTION:Disconnection notified from Reader");
			}
			else if( disconEventData.eventInfo == 1 )
			{
				wprintf(L" \n Disconnet Event Data Reason : %ls",L"READER_EXCEPTION:Reader has reported an exception");
			}
			else if( disconEventData.eventInfo == 2 )
			{
				wprintf(L" \n Disconnet Event Data Reason : %ls",L"CONNECTION_LOST:Reader has reported loss of connectivity");
			}
		}
	}
	else
	{
		wprintf(L"\n -------Disconnection Event is Not Signalled-------\n");	
	}
	
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,DISCONNECTION_EVENT );
	CloseHandle( hDisconEvent );
#endif
}

void TestHHEvent( RFID_VERSION version,TRIGGER_INFO* ptriggerInfo )
{

	CONNECTION_INFO ConInfo = { version , 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	RFID_EVENT_TYPE rfideventType = HANDHELD_TRIGGER_EVENT ;
	
#ifdef linux
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,NULL,NULL,NULL);
	sem_init(&hhTEventSemaphore, 0, 0);		
	RFID_RegisterEventNotificationCallback(api3Data.hReader,&rfideventType,1,hhTriggerEvent,NULL,NULL);
#else
	// Create and Register tag read event handle
	//
	HANDLE hHHTriggerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// tag read event handle
	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,rfideventType,hHHTriggerEvent);
#endif
	
#ifdef linux
	if( sem_wait(&hhTEventSemaphore) == 0 )
	{
#else
	if( WAIT_OBJECT_0 == WaitForSingleObject( hHHTriggerEvent, 20000 ) )
	{
#endif
		HANDHELD_TRIGGER_EVENT_DATA hhEvent;
		while( (api3Data.rfidStatus =RFID_GetEventData( api3Data.hReader,HANDHELD_TRIGGER_EVENT,&hhEvent) ) == RFID_API_SUCCESS )
		{
			wprintf(L"\n Hand Held trigger event is signalled");
			wprintf(L"\n HH event is: %ls",hhEvent.eventInfo?L"HANDHELD_TRIGGER_PRESSED":L"HANDHELD_TRIGGER_RELEASED");
		}
	}
	else
	{
		wprintf(L"\n -------HH Event is Not Signalled-------\n");	
	}
	
#ifndef linux
	api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,HANDHELD_TRIGGER_EVENT );
	CloseHandle( hHHTriggerEvent );
#endif
}

void TestRFIDEvents( )
{
	TestGPIEvent(RFID_API3_5_5,NULL);
	wprintf(L"\n Generate GPI Event on the Desired Port( for 10Sec) .. Check the Console window");
	rfid3Sleep(15000);

	wprintf(L"\n Initiate a Disconnect event from Reader ( in 10sec).. Check the Console window");
	TestDisconEvent(RFID_API3_5_5,NULL);
	rfid3Sleep(15000);
	TestHHEvent(RFID_API3_5_5,NULL);
	wprintf(L"\n Press HH TRIGGER ( in 10Secs ).. Check the Console window");
	rfid3Sleep(15000);
}