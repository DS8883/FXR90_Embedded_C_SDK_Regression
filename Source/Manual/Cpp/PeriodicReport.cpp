//
//#include "ManualTests.h"
//
//extern struct RFID3DATA api3Data;
//
//static DWORD WINAPI ProcessMyEvents( LPVOID lpParam )
//{
//	HANDLE evntHandle = (RFID_HANDLE32*)lpParam;
//	DWORD dwStatus;
//	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
//	while( api3Data.bContinue )
//	{
//		dwStatus = WaitForSingleObject(evntHandle, INFINITE );
//
//		switch(dwStatus)
//		{
//		case WAIT_OBJECT_0: // gpi event
//			{
//				wprintf(L"\n>> tagevent");	
//				
//				while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
//				{
//					printTagDataWithResults( pTagData );
//				}
//			}
//			
//			break;
//		default:
//			break;
//		}
//	}
//	api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,pTagData );
//	return TRUE;
//}
//
//void TestPeriodicReport( RFID_VERSION version)
//{
//	DWORD dwThreadID;
//	CONNECTION_INFO ConInfo = { version, 0 };
//	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
//
//	RFID_HANDLE32 hTagEvent = CreateEvent(NULL, FALSE, FALSE, NULL);  // Reader exception event
//	
//	api3Data.rfidStatus = RFID_RegisterEventNotification( api3Data.hReader,TAG_READ_EVENT,hTagEvent);
//	api3Data.bContinue = TRUE;
//	HANDLE hThread = CreateThread(NULL, 0, ProcessMyEvents, (LPVOID)(hTagEvent), 0, &dwThreadID );
//	REPORT_TRIGGERS report;
//	memset(&report,0,sizeof(report));
//	TRIGGER_INFO trInfo;
//	memset(&trInfo,0,sizeof(trInfo));
//	report.periodicReportDuration = 5;
//
//	trInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
//	trInfo.stopTrigger.type = STOP_TRIGGER_TYPE_IMMEDIATE;
//	trInfo.tagReportTrigger = 1;
//	trInfo.lpReportTriggers = &report;
//	
//	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,0,0,&trInfo,0);
//	rfid3Sleep( 60000);
//	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
//
//	api3Data.bContinue = FALSE;
//	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
//}