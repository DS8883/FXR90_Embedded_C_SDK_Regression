#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;
static DWORD tagCount = 0;

// This source file contain automation of locationg and duty cycle.
//


//static void Print2Log( wchar_t* mylogData,UINT16 antID,UINT16 crc,UINT16 PC,UINT32 XPC, wchar_t* szIP, UINT8*pTagID, UINT32 tagIDLength )
//{
//        char  tagBuffer[512] = {0,};
//        char* pTagReportData = tagBuffer;
//        UINT16   index = 0;
//        for( index = 0; index < tagIDLength; index++ )
//        {
//            if(0 < index && index%2 == 0)
//            {
//                    *pTagReportData++ = '-';
//            }
//            sprintf(pTagReportData,"%02X", pTagID[index]);
//            while(*pTagReportData) pTagReportData++;
//        }
//
//		fprintf(api3Data.fpLog,"\n %S %S antennaID:%d CRC %X PC %X XPC %X Tag ID: %s Result: %S",mylogData,szIP, antID,crc,PC,XPC,tagBuffer,RFID_GetErrorDescription( api3Data.rfidStatus ));
//}

//static DWORD WINAPI ProcessOnlyTags( LPVOID tagEvent )
//{
//	DWORD dwStatus;
//	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
//
//	while( api3Data.bContinue )
//	{
//		dwStatus = WaitForSingleObject( (HANDLE)tagEvent, INFINITE );
//
//		switch( dwStatus )
//		{
//		case WAIT_OBJECT_0:
//		{
//			 while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
//			 {
//				 //if( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
//				 //{
//					  tagCount++;
//					 //Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);
//					 //RFID_PurgeTags( api3Data.hReader, 0 );
//					 //Print2Console( pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);							
//					 //Print2Log(L"TAG INFO"),pTagData->antennaID,pTagData->CRC,pTagData->PC,pTagData->XPC,api3Data.szIPAddress,pTagData->pTagID,pTagData->tagIDLength);
//					 //if( pTagData->lpLocation ) fprintf( api3Data.fpLog," \n Tag Relative distance : %d",pTagData->lpLocation->relativeDistance );
//					 //wprintf(L"\nantennaID:%d",pTagData->antennaID);
//					 printTagDataWithResults(pTagData);			 
//				// }
//			 }
//		}
//		break;
//		default:
//			;
//		}
//	}
//
//	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
//
//	return TRUE;
//}

void VerifyDutyCycle()
{

	SetLogText("VerifyDutyCycle");
	logCurrentExecution( "VerifyDutyCycle");	
	TAG_STORAGE_SETTINGS tagStorageSettings;
	UINT16 SetDutyIndex;
	UINT16 GetDutyIndex;
	
	// This is not supported in FX7400 or 7500..
	if(RFID_READER_FUNCTION_UNSUPPORTED == RFID_GetDutyCycle(api3Data.hReader, &GetDutyIndex, 0))
	{
		api3Data.dwSkipCount = 19;
		LogPassFailCount();
		wprintf(L"\n Not Supported -->Tag DutyCycle is supported only on HH Readers \n\n");
		return;
	}

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	
	api3Data.rfidStatus = RFID_GetDutyCycle( api3Data.hReader,&GetDutyIndex,0 );
	
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 19;

	// Set the tag storage settings
	//
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tagStorageSettings);
	tagStorageSettings.maxTagCount = 10000;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader,&tagStorageSettings);
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	// DO inventory with duty cycle Set and check if tags are read
	//
	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader, &api3Data.rCaps );
	SetDutyIndex = 0; //starts with index 0 
	while( SetDutyIndex !=  api3Data.rCaps.lpDutyCycleTable->numValues )
	{	
		fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-DUTYCYCLE-%d></a> CAPI-DUTYCYCLE-%d:\n\n<br>Description:Test Inventory with DutyCycle Index :%d \n <br> Expected OutPut :Tags Should be read\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,SetDutyIndex );
		fprintf( api3Data.fpCurLog,"<br>");
		RFID_STATUS rfidSetStatus = RFID_SetDutyCycle(api3Data.hReader,SetDutyIndex,0);
		RFID_STATUS rfidGetStatus =  RFID_GetDutyCycle(api3Data.hReader,&GetDutyIndex,0);

		if( ( api3Data.rfidStatus == RFID_API_SUCCESS ) && ( GetDutyIndex == SetDutyIndex ) )
		{
			fprintf( api3Data.fpCurLog,"\n Setting the Duty Cycle is Success--Index %d \n",SetDutyIndex );
			wprintf(L"\n Setting the Duty Cycle is Success--Index %d \n",SetDutyIndex );
		}
		else
		{
			fprintf( api3Data.fpCurLog,"Status Set Duty Cycle:%ls",RFID_GetErrorDescription(rfidSetStatus));
			wprintf(L"Status Get Duty Cycle:%ls",RFID_GetErrorDescription(rfidGetStatus));
		}
				
		UINT16 ant[] = { 1,2 };
		ANTENNA_INFO myAntInfo;
		myAntInfo.pAntennaList = ant;
		myAntInfo.length = 2;
		myAntInfo.pAntennaOpList = NULL ;
		api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL,NULL,NULL, NULL );
		rfid3Sleep(5000);	
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
		{
			printTagDataWithResults( pTagData);
			tagCount++;
		}
		if( tagCount != 0 )
		{
			SetLogResult( TRUE );
		}
		logToResultFile(L"CAPI-DUTYCYCLE");
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		
		fprintf( api3Data.fpCurLog, "\n Number of Tags Read for DutyCycle index %d is : %d \n ",SetDutyIndex,tagCount);
		wprintf(L"\n Number of Tags Read for DutyCycle index %d is : %d \n ",SetDutyIndex,tagCount);
		fprintf( api3Data.fpCurLog,"<br>");
		tagCount = 0;
		rfid3Sleep(3000);	
		SetDutyIndex++;
	}
	
	// negative tests for duty cycle
	//
	SetDutyIndex = 17;
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-DUTYCYCLE-%d></a> CAPI-DUTYCYCLE-%d:\n\n<br>Description:Test with invalid DutyCycle Index :%d \n <br> Expected OutPut :Parameter out of range\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,SetDutyIndex );
	fprintf( api3Data.fpCurLog,"<br>");
	RFID_STATUS rfidSetStatus = RFID_SetDutyCycle(api3Data.hReader,SetDutyIndex,0);
	if( rfidSetStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n API result is : %ls\n ", RFID_GetErrorDescription( rfidSetStatus ));
		SetLogResult(TRUE);
	}
	logToResultFile(L"CAPI-DUTYCYCLE");

	SetDutyIndex = 100;
	fprintf(api3Data.fpCurLog,"\n\n<a nameCAPI-DUTYCYCLE-%d></a> CAPI-DUTYCYCLE-%d:\n\n<br>Description:Test with invalid DutyCycle Index :%d \n <br> Expected OutPut :Parameter out of range\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,SetDutyIndex );
	fprintf( api3Data.fpCurLog,"<br>");
	api3Data.rfidStatus = RFID_SetDutyCycle(api3Data.hReader,SetDutyIndex,0);
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog, "\n API result is : %ls\n ", RFID_GetErrorDescription( api3Data.rfidStatus ));
		SetLogResult(TRUE);
	}
	logToResultFile(L"CAPI-DUTYCYCLE");

	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	tagStorageSettings.maxTagCount = 1000;
	api3Data.rfidStatus = RFID_SetTagStorageSettings(api3Data.hReader,&tagStorageSettings);
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

void VerifyLocationingSanity()
{
	api3Data.dwTestCaseNo = 01;
	SetLogText("VerifyLocationing");
	logCurrentExecution( "VerifyLocationing");
	//  First verify if the capability is supported or not. Skip if the capability is not Supported.
	if( !api3Data.rCaps.tagLocationingSupported)
	{
		api3Data.dwSkipCount = 2;
		LogPassFailCount();
		wprintf(L"\n Tag Locationing is supported only on HH Readers \n");
		return;
	}
	
	api3Data.itemTestCount = 2;
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	RFID_SetTraceLevel( api3Data.hReader,TRACE_LEVEL_OFF );
	int input = 1;
	OPERATION_QUALIFIER OpQ[] = { C1G2_OPERATION };
	UINT16 ant[] = { 1 };
	ANTENNA_INFO myAntInfo;
	memset(&myAntInfo,0,sizeof(ANTENNA_INFO));
	myAntInfo.pAntennaList = ant;
	myAntInfo.length = 1;
	myAntInfo.pAntennaOpList = OpQ ;

	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );
	api3Data.rfidStatus = RFID_PerformInventory( api3Data.hReader, NULL, &myAntInfo,NULL, NULL );
	rfid3Sleep( 3000 );
	api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-LOCATE-%d></a> CAPI-LOCATE-%d:\n\n<br>Description:Locate with antenna 1  \n <br> Expected OutPut :Should get relative distance\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	OpQ[0] = LOCATE_TAG;
	if( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
	{
		api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader, 0 );
		api3Data.rfidStatus = RFID_PerformTagLocationing( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&myAntInfo,NULL,NULL);
		rfid3Sleep(10000);
		api3Data.rfidStatus = RFID_StopTagLocationing( api3Data.hReader );			
	}
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		 printTagDataWithResults( pTagData);
		 if( pTagData->lpLocation )
		 {
			 wprintf(L"\nAntID:%d Tag Relative distance : %d",pTagData->antennaID,pTagData->lpLocation->relativeDistance );
			 fprintf(api3Data.fpCurLog,"<br>AntID:%d Tag Relative distance : %d",pTagData->antennaID,pTagData->lpLocation->relativeDistance );
		 }
	}	
	
	if( pTagData->lpLocation )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"CAPI-LOCATE");

	
	fprintf(api3Data.fpCurLog,"\n\n<a name=CAPI-LOCATE-%d></a> CAPI-LOCATE-%d:\n\n<br>Description:Locate with antenna 2 \n <br> Expected OutPut :Should get relative distance\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	(*myAntInfo.pAntennaList) = 2;
	api3Data.rfidStatus = RFID_PerformTagLocationing( api3Data.hReader,pTagData->pTagID,pTagData->tagIDLength,&myAntInfo,NULL,NULL);
	rfid3Sleep(10000);
	api3Data.rfidStatus = RFID_StopTagLocationing( api3Data.hReader );			
	
	while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader, pTagData ) )
	{
		 printTagDataWithResults( pTagData);
		 if( pTagData->lpLocation )
		 {
			 wprintf(L"\nAntID:%d Tag Relative distance : %d",pTagData->antennaID,pTagData->lpLocation->relativeDistance );
			 fprintf(api3Data.fpCurLog,"<br>AntID:%d Tag Relative distance : %d",pTagData->antennaID,pTagData->lpLocation->relativeDistance );
		 }
	}	
	
	if( pTagData->lpLocation )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"CAPI-LOCATE");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader, pTagData );
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
