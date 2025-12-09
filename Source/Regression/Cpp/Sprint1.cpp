#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[2];
#ifndef linux
//int TestRFID3AcceptConnection( UINT32 Port  )
//{
//	// Initialize Winsock.
//	WSADATA wsaData;
//	SOCKET m_ListenSocket,acceptSocket;
//
//	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
//	if (iResult != NO_ERROR)
//		fprintf(api3Data.fpLog,"\nError at WSAStartup()\n");
//
//	// Create a SOCKET for listening for incoming connection requests.
//	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (m_ListenSocket == INVALID_SOCKET)
//	{
//		wprintf(L"Error at socket(): %ld\n", WSAGetLastError());
//		WSACleanup();
//		return -1;
//	}
//
//	// The sockaddr_in structure specifies the address family,
//	// IP address, and port for the socket that is being bound.
//	sockaddr_in service;
//	service.sin_family = AF_INET;
//	service.sin_addr.s_addr = INADDR_ANY;
//	service.sin_port = htons( Port );
//
//	if (bind( m_ListenSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR )
//	{
//			wprintf(L"bind() failed.\n");
//			closesocket(m_ListenSocket);
//			return -1; 
//	}
//
//	// Listen for incoming connection requests
//	// on the created socket
//	if (listen( m_ListenSocket, 2 ) == SOCKET_ERROR)
//		wprintf(L"Error listening on socket.\n");
//	acceptSocket = accept( m_ListenSocket, NULL, NULL );
//	if (acceptSocket == INVALID_SOCKET)
//			return -1;
//	
//	SERVER_INFO serverInfo = { 2000,RFID_API3_5_1,0 };
//	api3Data.rfidStatus = RFID_AcceptConnection( &api3Data.hReader,acceptSocket,&serverInfo,NULL );
//	if( api3Data.rfidStatus != RFID_API_SUCCESS )
//	{
//		wprintf(L"\n API Result Description: %S",RFID_GetErrorDescription(api3Data.rfidStatus));
//	}
//	else
//	{
//		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
//		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
//		rfid3Sleep( 6000 );	
//		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );	
//		rfid3Sleep( 1000 );
//		
//		while( (api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
//		{		
//			printTagDataWithResults(pTagData);
//		}
//		RFID_DeallocateTag(api3Data.hReader,pTagData);
//	}
//
//	
//	
//	return 1;
//}
#endif
void getRSSIRange( int *minRSSI,int* maxRSSI )
{
	int rssiArray[100];
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, NULL,NULL, NULL );
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	UINT32 tagCount=0;
	
	while( RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pTagData))
	{		
		rssiArray[tagCount]	= pTagData->peakRSSI;
		tagCount++;
		if(tagCount==100) break;
	}

	int mxm,min;
	min=mxm= rssiArray[0];

	for (UINT32 i=0; i<tagCount; i++)
	{
		if (rssiArray[i] > mxm)
		{
			mxm = rssiArray[i];
		}
		if (rssiArray[i] < min)
		{
			min = rssiArray[i];
		}

	}
	
	*maxRSSI = mxm;*minRSSI=min;
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
}


void TestRSSI( TRIGGER_INFO triggerInfo)
{
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
	int minRSSI,maxRSSI;
	getRSSIRange( &minRSSI,&maxRSSI );

	BOOLEAN bSuccess = FALSE;
	TAG_STORAGE_SETTINGS tgSettings = { 100,512,512,ALL_TAG_FIELDS,TRUE,TRUE,0};
	tgSettings.discardTagsOnInventoryStop = TRUE;
	RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings );
	
	// Read tags which within the  RSSI Range
	//
	POST_FILTER postFilter;
	RSSI_RANGE_FILTER rssiFilter = { minRSSI,maxRSSI,WITHIN_RANGE,{0} };
	postFilter.lpRSSIRangeFilter = &rssiFilter;
	postFilter.lpTagPatternA = postFilter.lpTagPatternB = NULL;
	postFilter.matchPattern = NOTA_AND_NOTB;
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Read Tags with RSSI Range min %d,max %d \n <br> Expected OutPut:Only Tags which match the RSSI range should be Read.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,minRSSI,maxRSSI );
	fprintf( api3Data.fpCurLog,"<br>");

	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult(TRUE);
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		if( pTagData->peakRSSI < minRSSI && pTagData->peakRSSI > maxRSSI )
		{
			SetLogResult( FALSE );
			break;
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	
	RFID_PurgeTags(api3Data.hReader,0);
	// Read tags which are out side the RSSI Range
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Read Tags outside RSSI Range min %d,max %d \n <br> Expected OutPut:Only Tags which are out side the RSSI range should be Read.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,minRSSI,maxRSSI );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.lpRSSIRangeFilter->matchRange = OUTSIDE_RANGE; 
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		if( pTagData->peakRSSI > minRSSI && pTagData->peakRSSI < maxRSSI )
		{
			SetLogResult( FALSE );
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");

	RFID_PurgeTags(api3Data.hReader,0);
	// Read Tags with RSSI greater than lower limit
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Read Tags with RSSI greater than lower limit %d \n <br> Expected OutPut: Tags greater than Lower limit should be read.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,minRSSI );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.lpRSSIRangeFilter->matchRange = GREATER_THAN_LOWER_LIMIT; 
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		if( pTagData->peakRSSI < minRSSI )
		{
			SetLogResult( FALSE );
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	
	RFID_PurgeTags(api3Data.hReader,0);
	// Read Tags with RSSI lower than upper limit
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Read Tags with RSSI Lower than upper limit %d \n <br> Expected OutPut: Tags lower than upper limit should be read.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,maxRSSI );
	fprintf( api3Data.fpCurLog,"<br>");
	
	postFilter.lpRSSIRangeFilter->matchRange = LOWER_THAN_UPPER_LIMIT; 
	RFID_PurgeTags(api3Data.hReader,0);
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		if( pTagData->peakRSSI > maxRSSI )
		{
			SetLogResult( FALSE );
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	RFID_PurgeTags( api3Data.hReader,0 );
	
	// Negative Tests for RSSI
	//
	RFID_PurgeTags( api3Data.hReader,0);
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Negative Test with range bet -127 and 127 \n <br> Expected OutPut: No tags be Read since its out side the Range.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.lpRSSIRangeFilter->peakRSSILowerLimit=-128;
	postFilter.lpRSSIRangeFilter->peakRSSIUpperLimit= 127;
	postFilter.lpRSSIRangeFilter->matchRange = OUTSIDE_RANGE; 
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );	
	bool berror = FALSE;
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		berror = true;
	}
	if(berror) SetLogResult(FALSE);
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	RFID_PurgeTags( api3Data.hReader,0 );
	
	// Negative Tests for RSSI
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Negative Test with range bet 0 and 127 \n <br> Expected OutPut: NO tags should be read.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.lpRSSIRangeFilter->peakRSSILowerLimit=0;
	postFilter.lpRSSIRangeFilter->peakRSSIUpperLimit=127;
	postFilter.lpRSSIRangeFilter->matchRange = WITHIN_RANGE; 
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	SetLogResult( TRUE );	
	berror = false;
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		berror = true;
	}
	if(berror) SetLogResult(FALSE);

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	RFID_PurgeTags( api3Data.hReader,0 );
	
	// Negative Tests for RSSI
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:Negative Test within range bet 0 and -1 \n <br> Expected OutPut: No tags be Read.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.lpRSSIRangeFilter->peakRSSIUpperLimit=0;
	postFilter.lpRSSIRangeFilter->peakRSSILowerLimit=-1;
	postFilter.lpRSSIRangeFilter->matchRange = WITHIN_RANGE; 
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	SetLogResult( TRUE );	
	
	berror = false;
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		berror = true;
	}
	if(berror) SetLogResult(FALSE);
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	RFID_PurgeTags( api3Data.hReader,0 );
	
	//  Read Tags with RSSI filter with TagPattern  with autonomous mode enabled and Pattern A and Not B
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:RSSI Filter with Tag Pattern \n <br> Expected OutPut:  Read Tags with RSSI filter with TagPattern  with autonomous mode enabled and Pattern A and Not B.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	RFID_PurgeTags( api3Data.hReader,0 );
	TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
	triggerInfo.lpTagEventReportInfo =  &tagEventInfo;

	UINT8 pattern1[2],pattern2[2];
	pattern1[0] = MyTags[0].epc[0];
	pattern1[1] = MyTags[0].epc[1];
	pattern2[0] = MyTags[1].epc[0];
	pattern2[1] = MyTags[1].epc[1];

	UINT8 TagMask[] = {0xFF,0xFF};

	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,pattern1,16,TagMask,16,{0}};
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,pattern2,16,TagMask,16,{0}}; 
	postFilter.lpTagPatternA = &tpA;
	postFilter.lpTagPatternB = &tpB;
	postFilter.matchPattern = A_AND_NOTB;
	postFilter.lpRSSIRangeFilter->peakRSSILowerLimit = -100;
	postFilter.lpRSSIRangeFilter->peakRSSIUpperLimit = 100;
	postFilter.lpRSSIRangeFilter->matchRange = WITHIN_RANGE; 
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		UINT8 temp[2]; memcpy( temp,pTagData->pTagID,2);
		if( memcmp(temp,pattern1,2) != 0 )
		{
			SetLogResult( FALSE );
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");

	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader,0 );

	//  Read Tags with RSSI filter with TagPattern  with autonomous mode enabled and Pattern B and Not A
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:RSSI Filter with Tag Pattern  \n <br> Expected OutPut:  Read Tags with RSSI filter with TagPattern  with autonomous mode enabled and Pattern B and Not A.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.matchPattern = NOTA_AND_B;
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		UINT8 temp[2]; memcpy( temp,pTagData->pTagID,2);
		if( memcmp(temp,pattern2,2) != 0 )
		{
			SetLogResult( FALSE );
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	
	RFID_PurgeTags( api3Data.hReader,0 );

	//  Read Tags with RSSI filter with TagPattern  with Pattern Not B and Not A
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:RSSI Filter with Tag Pattern  \n <br> Expected OutPut:  Read Tags with RSSI filter with TagPattern  with autonomous mode enabled and Pattern Not B and Not A.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.matchPattern = NOTA_AND_NOTB;
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	SetLogResult( TRUE );
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		UINT8 temp[2]; memcpy( temp,pTagData->pTagID,2);
		if( ( memcmp(temp,pattern2,2) == 0 ) || ( memcmp(temp,pattern1,2) == 0 ) )
		{
			SetLogResult( FALSE );
		}

		fprintf( api3Data.fpCurLog, "<br> TAG ID : %s RSSI: %d TAGEVENT: %s ANTENNA : %d",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,TAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
		wprintf(L"\nTAG ID : %ls RSSI: %d TAGEVENT: %ls ANTENNA : %d",Byte2HexWString( pTagData->pTagID,pTagData->tagIDLength),pTagData->peakRSSI,wTAGEVENTS[pTagData->tagEvent],pTagData->antennaID );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	
	RFID_PurgeTags( api3Data.hReader,0 );

	//  Read Tags with RSSI filter with TagPattern  with Pattern A and B
	//
	fprintf(api3Data.fpCurLog,"<br><a name=CAPI-RSSIFILTER-%d></a> CAPI-RSSIFILTER-%d:\n\n<br>Description:Description:RSSI Filter with Tag Pattern  \n <br> Expected OutPut:  Read Tags with RSSI filter with TagPattern  with autonomous mode enabled and Pattern A and B.\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	postFilter.matchPattern = A_AND_B;
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader,&postFilter,NULL,&triggerInfo,NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	if( RFID_NO_READ_TAGS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		SetLogResult( TRUE );	
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"CAPI-RSSIFILTER");
	
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData );
	
}

void TestRSSIFilter( )
{
	api3Data.dwTestCaseNo = 01;
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps);
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.rssiFilterSupported )
	{
		api3Data.dwSkipCount = 33;
		LogPassFailCount();
	}
	else
	{
		wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
		SetLogText("TestRSSIFILTER");
		logCurrentExecution( "TestRSSIFILTER");
		fprintf( api3Data.fpCurLog,"<html>\n");
		fprintf( api3Data.fpCurLog,"<body>\n");
		api3Data.itemTestCount = 33;
		TRIGGER_INFO triggerInfo;
		memset(&triggerInfo,0,sizeof(triggerInfo));
		triggerInfo.tagReportTrigger	= 0; // Report back all read tags after 3 rounds of inventory).
		triggerInfo.startTrigger.type	= START_TRIGGER_TYPE_IMMEDIATE;
		triggerInfo.stopTrigger.type	= STOP_TRIGGER_TYPE_IMMEDIATE;
		triggerInfo.lpTagEventReportInfo = NULL;
		TestRSSI(triggerInfo);
		TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000};
		triggerInfo.lpTagEventReportInfo = &tagEventInfo;
		TestRSSI(triggerInfo);
		tagEventInfo.reportTagBackToVisibilityEvent = tagEventInfo.reportTagInvisibleEvent = MODERATED;
		tagEventInfo.tagBackToVisibilityModeratedTimeoutMilliseconds = tagEventInfo.tagInvisibleEventModeratedTimeoutMilliseconds = 100;
		TestRSSI(triggerInfo);
		api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
		fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
		LogPassFailCount();
		wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
	}
}
