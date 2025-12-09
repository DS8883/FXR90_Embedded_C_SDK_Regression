
#include "common.h"
#include "APITest.h"
//#include "rfid3Events.h"

extern struct RFIDDATA api3Data;

static HANDLE hList[3];

static DWORD WINAPI ProcessMyEvents( LPVOID lpParam )
{
	DWORD dwStatus;
	while( api3Data.bContinue )
	{
		dwStatus = WaitForMultipleObjects( 3,hList,0,INFINITE );
		//dwStatus = WaitForSingleObject(hList[0],INFINITE );

		switch(dwStatus)
		{
		case WAIT_OBJECT_0: // gpi event
			{
				DISCONNECTION_EVENT_DATA disconEventData;
				if(RFID_API_SUCCESS == RFID_GetEventData( api3Data.hReader, DISCONNECTION_EVENT,(STRUCT_HANDLE)&disconEventData))
				{
					if( disconEventData.eventInfo == 0 )
					{
						wprintf(L"\n Disconnet Event Data Reason : %ls",L"READER_INITIATED_DISCONNECTION:Disconnection notified from Reader");
					}
					else if( disconEventData.eventInfo == 1 )
					{
						wprintf(L"\n Disconnet Event Data Reason : %ls",L"READER_EXCEPTION:Reader has reported an exception");
					}
					else if( disconEventData.eventInfo == 2 )
					{
						wprintf(L" \n Disconnet Event Data Reason : %ls",L"CONNECTION_LOST:Reader has reported loss of connectivity");
					}
				}
				
			}
			break;
		
		case WAIT_OBJECT_0 + 1:	// antenna Connect or DisConnect Event
			{
				CONNECTION_INFO conInfo = { api3Data.apiVersion, 0 };
				// Sled Connect			
				wprintf(L"\n Sled Connected\n");
				api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084, 0, &conInfo );
				api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,0,0);
				Sleep( 5000 );
				api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader);
				LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
				while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
				{
					printTagDataWithResults( pTagData );
				}
				RFID_DeallocateTag(api3Data.hReader,pTagData);
			}
			break;
		
		case WAIT_OBJECT_0 + 2:	// Reader DisConnect Event
			{
				wprintf(L"\n Sled Dis Connected\n");
				
			}
			break;
		}
	}
	return TRUE;
}
void TestSledEvent()

{
	RFID_STATUS rfidStatus;
	DWORD dwThreadID;
	api3Data.bContinue = TRUE;

	// Create events that can be processed in a different thread
	//
	 hList[0] = CreateEvent(NULL, FALSE, FALSE, NULL);	
	 hList[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	 hList[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	// Register for all events 
	//
	 rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,DISCONNECTION_EVENT,hList[0]);
	 rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,SLED_CONNECTED_EVENT,hList[1]);
	 rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,SLED_DISCONNECTED_EVENT,hList[2]);


	 HANDLE hThread = CreateThread(NULL, 0, ProcessMyEvents, NULL, 0, &dwThreadID );
	 Sleep( 100000);
	 api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,DISCONNECTION_EVENT );
	 api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,SLED_CONNECTED_EVENT );
	 api3Data.rfidStatus = RFID_DeregisterEventNotification( api3Data.hReader,SLED_DISCONNECTED_EVENT );
	
	api3Data.bContinue = FALSE;
}