#ifndef linux
#include <windows.h>
#endif
#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;

struct MYLOGIN
{
	LOGIN_INFO loginfo;
	FTPSERVER_INFO ftpinfo;
	READER_TYPE readerType;
	BOOLEAN bMode;
};
#ifndef linux
DWORD WINAPI Connect2Read( LPVOID lParam)
#else
void* Connect2Read( void* lParam)
#endif
{
	RFID_HANDLE32 hReader;
	TRIGGER_INFO triggerInfo;
	memset(&triggerInfo,0,sizeof(triggerInfo));

	READ_ACCESS_PARAMS rParams;
	wchar_t szIp[256];
	wcscpy(szIp,(wchar_t*)lParam);
	api3Data.rfidStatus = RFID_Connect( &hReader, szIp,5084,0,0 );
	
	ERROR_INFO eInfo;
	RFID_GetLastErrorInfo( hReader,&eInfo );
	wprintf(L"\nConnect Call on Reader %ls StatusCode %d\n",szIp,api3Data.rfidStatus );

	triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
	triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
	triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_DURATION;
	triggerInfo.stopTrigger.value.duration = 50000; 
	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		triggerInfo.lpTagEventReportInfo = NULL;
	}
	// TEST with Simple Write Operations without AnyFilters 
	
	LPTAG_DATA pTagData = RFID_AllocateTag( hReader );	
	
	api3Data.rfidStatus = RFID_PerformInventory(hReader,NULL,NULL,&triggerInfo,NULL );
	rfid3Sleep( 10000 );
	api3Data.rfidStatus = RFID_StopInventory( hReader );	
	
	while( RFID_API_SUCCESS == RFID_GetReadTag(hReader, pTagData))
	{
		api3Data.rfidStatus = RFID_Read( hReader,pTagData->pTagID,pTagData->tagIDLength,&rParams,NULL,NULL,pTagData,0);
		Print2Console(pTagData->antennaID,pTagData->CRC,pTagData->PC, pTagData->XPC,(wchar_t*)lParam,pTagData->pTagID,pTagData->tagIDLength );
	}
	if( pTagData != NULL ) RFID_DeallocateTag( hReader,pTagData );
	api3Data.rfidStatus = RFID_Disconnect( hReader );
	RFID_GetLastErrorInfo( hReader,&eInfo );
	wprintf(L"\nDisConnect Call on Reader %ls StatusCode %d\n",szIp,api3Data.rfidStatus );
#ifndef linux
	return TRUE;
#endif
}
//
//DWORD WINAPI ThreadRMAPI( LPVOID lParam)
//{
//	RFID_HANDLE32 hReader;
//		
//	MYLOGIN* pLoginInfo = ( MYLOGIN* ) lParam;
//	api3Data.rfidStatus = RFID_Login( &hReader,&(pLoginInfo->loginfo),pLoginInfo->readerType,pLoginInfo->bMode,0);
//	ERROR_INFO eInfo;
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\nLogin Call on Reader %S StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//	rfid3Sleep( 15000 );
//	BOOLEAN antennaStatus = TRUE;
//	UINT16 antennaID;
//	api3Data.rfidStatus = RFID_EnableReadPoint( hReader, 1, antennaStatus);
//	api3Data.rfidStatus = RFID_EnableReadPoint( hReader, 2, antennaStatus);
//	api3Data.rfidStatus = RFID_EnableReadPoint( hReader, 3, antennaStatus);
//	api3Data.rfidStatus = RFID_EnableReadPoint( hReader, 4, antennaStatus);
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_EnableReadPoint StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	api3Data.rfidStatus = RFID_GetReadPointStatus( hReader, 1, &antennaStatus);
//	api3Data.rfidStatus = RFID_GetReadPointStatus( hReader, 2, &antennaStatus);
//	api3Data.rfidStatus = RFID_GetReadPointStatus( hReader, 3, &antennaStatus);
//	api3Data.rfidStatus = RFID_GetReadPointStatus( hReader, 4, &antennaStatus);
//	
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_GetReadPointStatus StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	ANTENNA_MODE mode;
//	api3Data.rfidStatus = RFID_GetAntennaMode( hReader,&mode );
//	
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_GetAntennaMode StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	mode = ANTENNA_MODE_MONOSTATIC;
//	api3Data.rfidStatus = RFID_SetAntennaMode( hReader, mode);
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_GetAntennaMode StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	api3Data.rfidStatus = RFID_UpdateSoftware( hReader, &(pLoginInfo->ftpinfo) );
//	
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_UpdateSoftware StatusCode %d\n",pLoginInfo->ftpinfo.hostName,api3Data.rfidStatus );
//
//	UPDATE_STATUS upgradeStatus;
//	api3Data.rfidStatus = RFID_GetUpdateStatus( hReader, &upgradeStatus);
//	
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_GetUpdateStatus StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//	
//	READER_SYSTEM_INFO SysInfo;
//	api3Data.rfidStatus = RFID_GetSystemInfo( hReader, &SysInfo);
//	
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_GetSystemInfo StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	api3Data.rfidStatus = RFID_ExportProfileToReader( hReader,L"Default1.xml",L"C:\\Temp",TRUE);
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_ExportProfileToReader StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	PROFILE_LIST ProfileList ;
//	api3Data.rfidStatus = RFID_GetProfileList( hReader, &ProfileList);
//	api3Data.rfidStatus = RFID_ImportProfileFromReader( hReader,L"Default.xml",L"C:\\Temp" );
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_ImportProfileFromReader StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	BOOLEAN pState;
//	api3Data.rfidStatus = RFID_GetRadioPowerState( hReader,&pState );
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_GetRadioPowerState StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus );
//
//	api3Data.rfidStatus = RFID_SetRadioPowerState( hReader,TRUE);
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_SetRadioPowerState StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus);
//
//	wchar_t pFileName;
//    api3Data.rfidStatus = RFID_UpdateRadioFirmware( hReader,&pFileName);
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Call on Reader %S RFID_UpdateRadioFirmware StatusCode %d\n",pLoginInfo->loginfo.hostName,api3Data.rfidStatus);
//
//	/*Status = RFID_Restart( hReader );
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n ReStart Call on Reader %S StatusCode %d\n",pLoginInfo->loginfo.hostName,eInfo.rfidStatusCode );*/
//
//	api3Data.rfidStatus = RFID_Logout( hReader );
//
//	RFID_GetLastErrorInfo( hReader,&eInfo );
//	wprintf(L"\n Logout Call on Reader %S RFID_UpdateSoftware StatusCode %d\n",pLoginInfo->ftpinfo.hostName,api3Data.rfidStatus );
//
//	return TRUE;
//}
//
void TestThreadSafe( void )
{
#ifndef linux
	HANDLE h1,h2,h3,h4,h5,h6; DWORD dwThreadID;
	h1 = CreateThread(NULL,0,Connect2Read,(LPVOID)L"157.235.206.31",0,&dwThreadID);
	h2 = CreateThread(NULL,0,Connect2Read,(LPVOID)L"157.235.206.113",0,&dwThreadID);
	h3 = CreateThread(NULL,0,Connect2Read,(LPVOID)L"192.168.2.56",0,&dwThreadID);
#else
	 pthread_t thread1, thread2,thread3;
	 int  iret1, iret2,iret3;
	 iret1 = pthread_create( &thread1, NULL, Connect2Read, (void*) L"157.235.206.113");
     iret2 = pthread_create( &thread2, NULL, Connect2Read, (void*) L"157.235.206.31");
     iret3 = pthread_create( &thread3, NULL, Connect2Read, (void*) L"192.168.2.56");
	 
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
 
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL);
	 pthread_join( thread3, NULL);
#endif
	//h4 = CreateThread(NULL,0,Connect2Read,(LPVOID)L"157.235.206.29"),0,&dwThreadID);
	//h5 = CreateThread(NULL,0,Connect2Read,(LPVOID)L"157.235.208.20"),0,&dwThreadID);
	//h6 = CreateThread(NULL,0,Connect2Read,(LPVOID)L"192.168.2.176"),0,&dwThreadID); 


}
//
//void TestThreadSafeRMAPI( void )
//{
//	HANDLE h1,h2,h3,h4,h5,h6; DWORD dwThreadID;
//	
//	// Thread 1
//	wprintf(L"\n\n THREAD1 RMAPI\n");
//	{
//		LOGIN_INFO loginfo = { L"157.235.206.13",L"admin",L"change",RFID_API3_5_1,TRUE,0,0};
//		FTPSERVER_INFO ftpinfo = { L"157.235.206.13",L"admin",L"change",0};
//		MYLOGIN info = { loginfo,ftpinfo, FX, FALSE };
//		
//		h1 = CreateThread(NULL,0,ThreadRMAPI,(LPVOID)(&info),0,&dwThreadID);
//	}
//	
//	// Thread 2
//	wprintf(L"\n\n THREAD2 RMAPI\n");
//	{
//		LOGIN_INFO loginfo = { L"157.235.206.130",L"admin",L"change"};
//		MYLOGIN info = { loginfo, FX, FALSE };
//		h2 = CreateThread(NULL,0,ThreadRMAPI,(LPVOID)(&info),0,&dwThreadID);
//	}
//	
//	// Thread 3
//	/*wprintf(L"\n\n THREAD2 RMAPI\n");
//	{
//		LOGIN_INFO loginfo = { L"157.235.206.29"),L"admin"),L"admin")};
//		MYLOGIN info = { loginfo, XR, FALSE };
//		h3 = CreateThread(NULL,0,ThreadRMAPI,(LPVOID)(&info),0,&dwThreadID);
//	}*/
//	
//}
