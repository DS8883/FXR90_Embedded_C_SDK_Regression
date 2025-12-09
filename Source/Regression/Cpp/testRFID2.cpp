#ifndef _WIN64 
#ifndef linux

#include "rfid2Helper.h"
#include "TestRFID2API.h"
#include "common.h"

extern struct RFID2DATA api2Data;
extern struct RFID3DATA api3Data;

void testCapabilityDefaults( )
{
	{
		UINT8 CapID[] = { RFID_READCAP_G2_SEL,RFID_READCAP_G2_SESSION,RFID_READCAP_G2_TARGET,RFID_READCAP_G2_START_Q,
						  RFID_TAGCAP_G2_SELECT_MEM_BANK,RFID_TAGCAP_G2_MEM_BANK,RFID_TAGCAP_G2_WORD_POINTER,
						  RFID_WRITECAP_G2_WRITE_OPTIONS,RFID_WRITECAP_G2_LOCK_OPTIONS,
						  RFID_WRITECAP_G2_LOCK_MASK,RFID_WRITECAP_G2_LOCK_ACTION,RFID_TAGCAP_G2_ACCESS_PASSWORD,
						  RFID_TAGCAP_G2_KILL_PASSWORD,RFID_TAGCAP_G2_SELECT_MEM_BANK,255 
					    };
		DWORD dwNumItems=0;DWORD dwSize=0;
		BYTE capValue = 0;
		DWORD dwStatusSet,dwStatusGet;
		api2Data.rfidStatus = RFID_SetCapDflts( api2Data.hReader );
		int i=0;
		for( CapID[i]; CapID[i] != 255; CapID[i++] )
		{
			fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description:RFID2Capability Defaults\n <br> Expected OutPut:Set and get capability should be same\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
			fprintf( api3Data.fpCurLog,"<br>");
			dwStatusSet = RFID_SetCapDfltValue( api2Data.hReader,CapID[i] );
			//dwStatusGet = RFID_GetCapDfltValue( api2Data.hReader,CapID[i],&dwNumItems,dwSize,&capValue );
			if( dwStatusSet == RFID_SUCCESS || RFID_CAPNOTSUPPORTED )
			{
				SetLogResult( TRUE );
			}
			fprintf( api3Data.fpCurLog, "\nsetvalue: %d getValue %d \n ",CapID[i],capValue );
			fprintf( api3Data.fpCurLog,"<br>");
			logToResultFile(L"RFID2API-FUN");
		}
	}

}

void test_WriteOptions( )
{
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: Test RFID_WRITECAP_G2_WRITE_OPTIONS with RFID_GEN2_WRITE_OPTION_NONE\n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_WRITE_OPTIONS,RFID_GEN2_WRITE_OPTION_NONE ) ) == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: Test RFID_WRITECAP_G2_WRITE_OPTIONS with RFID_GEN2_WRITE_OPTION_BLOCK_WRITE\n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_WRITE_OPTIONS,RFID_GEN2_WRITE_OPTION_BLOCK_WRITE )) == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");
	
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: Test RFID_WRITECAP_G2_WRITE_OPTIONS with RFID_GEN2_WRITE_OPTION_BLOCK_ERASE_FIRST\n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_WRITE_OPTIONS,RFID_GEN2_WRITE_OPTION_BLOCK_ERASE_FIRST )) == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");

	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: Test RFID_WRITECAP_G2_WRITE_OPTIONS with RFID_GEN2_WRITE_OPTION_PROGRAM_EPC\n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	if( ( api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_WRITECAP_G2_WRITE_OPTIONS,RFID_GEN2_WRITE_OPTION_PROGRAM_EPC )) == RFID_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");						
}

void test_EnabledTagTypes( )
{	
	UINT8 enablecheck[]={ 0, 0, 0, 0, 1 };
	UINT8 uchartemp[5];
	DWORD dwStatusSet,dwStatusGet;
	DWORD dwNumItems = 5;
	DWORD dwSize = dwNumItems*sizeof( enablecheck[0] ); 
	dwStatusSet = RFID_SetCapCurrValue( api2Data.hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize,enablecheck); 
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: Calling the SetCapCurrVal function for RFID_TAGCAP_ENABLED_TYPES\n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	dwStatusGet = RFID_GetCapCurrValue( api2Data.hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize, &uchartemp); 
	
	if( (dwStatusSet == RFID_SUCCESS) && (dwStatusGet == RFID_SUCCESS) )
	{
		SetLogResult( TRUE );
	}
	fprintf( api3Data.fpCurLog,"Result is :%S",RFID_GetCommandStatusText( api2Data.hReader, dwStatusSet));
	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");

	// neg test
	{
		UINT8 negativeCheck[] = { 5,5,5,5,5 };
		dwStatusSet = RFID_SetCapCurrValue( api2Data.hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize,negativeCheck); 
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: Calling the SetCapCurrVal function for RFID_TAGCAP_ENABLED_TYPES\n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");

		dwStatusGet = RFID_GetCapCurrValue( api2Data.hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize, &uchartemp); 
		
		if( (dwStatusSet == RFID_SUCCESS) && (dwStatusGet == RFID_SUCCESS) )
		{
			SetLogResult( TRUE );
		}

		fprintf( api3Data.fpCurLog,"Result is :%S",RFID_GetCommandStatusText( api2Data.hReader, dwStatusSet));
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RFID2API-FUN");
	}

	api2Data.rfidStatus = RFID_GetCapCurrValue( api2Data.hReader, RFID_TAGCAP_SUPPORTED_TYPES, &dwNumItems, dwSize, &uchartemp ); 
	UINT8 comp[5]={ 0x00,0x00,0x00,0x00,0x01 };
	
	for (int i=0; i<5; i++)
	{
		if(comp[i]!=uchartemp[i])
		{
			fprintf( api3Data.fpLog ,"\nRFID_TAGCAP_SUPPORTED_TYPES Get %d",uchartemp[i] );
			break;
		}
	}
	
	{
		dwNumItems=44;
		dwSize=176;
		BYTE uchartemp[176];
		api2Data.rfidStatus = RFID_GetCapCurrValue( api2Data.hReader, RFID_INFCAP_SUPPORTEDCAPS, &dwNumItems, dwSize, &uchartemp);  
		
		if( api2Data.rfidStatus == RFID_SUCCESS )
		{
			fprintf(api3Data.fpLog,"\nThe list of capability are ");
			for(int i=0;i<176;i++)
			{
				
			  fprintf(api3Data.fpLog,"0x%2X,",uchartemp[i]);
			  i=i+3;
			}
		}
	}

}



void testRFID2_Read_ATTENUATION()				//2
{	
	
	UINT32 arrayList[] = { 255,0,127,1000,655535 };

	for( UINT8 i=0; i<5; i++ )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: testRFID2_Read_ATTENUATION %d \n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,arrayList[i] );
		fprintf( api3Data.fpCurLog,"<br>");
		if((api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_READCAP_RF_ATTENUATION,arrayList[i] )) == RFID_SUCCESS )
		{
			if( RFID2ReadTags( api2Data.hReader,4 ,1 ) )
			{
				SetLogResult( TRUE );
			}
			fprintf( api3Data.fpCurLog,"Result is :%S",RFID_GetCommandStatusText( api2Data.hReader, api2Data.rfidStatus));
			fprintf( api3Data.fpCurLog,"<br>");
			logToResultFile(L"RFID2API-FUN");
		}
		
	}
}

//bool SetCap_RFID_INFCAP_SUPPORTEDCAPS(DWORD dwCapId[40])				                 //1
//{
//		
//	DWORD dwTemp[40];
//	fprintf(fpLog,"\n\n***InSide the Function SetCap_RFID_INFCAP_SUPPORTEDCAPS **** Value  : \n");
//	for (int i=0;i<40;i++)
//	fprintf(fpLog,"  %x ",dwCapId[i]);
//	dwNumItems = 40;					
//	dwSize = sizeof(dwCapId)/sizeof(dwCapId[0]);
//	dwStatus = RFID_SetCapCurrValue(hReader, RFID_INFCAP_SUPPORTEDCAPS, &dwNumItems, dwSize, dwCapId); 
//	fprintf(fpLog,"\nCalling the SetCapCurrVal function for RFID_INFCAP_SUPPORTEDCAPS  :%S",RFID_GetCommandStatusText(hReader, dwStatus));
//	
//	RFID_GetCapCurrValue(hReader, RFID_INFCAP_SUPPORTEDCAPS, &dwNumItems, dwSize, &dwTemp); 
//	fprintf(fpLog,"\nThe RFID_INFCAP_SUPPORTEDCAPS value is : \n");
//	for (int i=0;i<40;i++)
//		fprintf(fpLog,"  %x ",dwTemp[i]);
//	bool c;
//	for (int i=0;i<40;i++)
//		if(dwTemp[i] != dwCapId[i])
//		{
//			c = FALSE;
//			break;
//		}
//		else
//		c = TRUE;
//
//	return c;
//}
//
//bool SetCap_RFID_READCAP_G2_START_Q(unsigned char ucharQValue)					//20
//{
//		
//	unsigned char ucharTemp=0;
//	fprintf(fpLog,"\n\n***InSide the Function SetCap_RFID_READCAP_G2_START_Q **** Value  : %d",ucharQValue);
//	dwNumItems = 1;					
//	dwSize = sizeof(ucharQValue);
//	dwStatus = RFID_SetCapCurrValue(hReader, RFID_READCAP_G2_START_Q, &dwNumItems, dwSize, &ucharQValue); 
//	fprintf(fpLog,"\nCalling the SetCapCurrVal function for RFID_READCAP_G2_START_Q - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));
//	
//	RFID_GetCapCurrValue(hReader, RFID_READCAP_G2_START_Q, &dwNumItems, dwSize, &ucharTemp); 
//	fprintf(fpLog,"\nThe RFID_READCAP_G2_SEL value is : %d",ucharTemp);
//
//	if (ucharQValue==ucharTemp)
//		return TRUE;
//	else
//		return FALSE;
//}

void testRFID2_EVENTNAME( wchar_t* wchar_tEventName )					//3
{
	DWORD dwStatusSet,dwStatusGet;

	wchar_t wchar_tTemp[64];
	fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: testRFID2_EVENTNAME %S \n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,wchar_tEventName );
	fprintf( api3Data.fpCurLog,"<br>");
	
	DWORD dwNumItems = wcslen( wchar_tEventName ) + 1;	
	DWORD dwSize = dwNumItems*sizeof( wchar_tEventName[0] );
	dwStatusSet = RFID_SetCapCurrValue( api2Data.hReader, RFID_READCAP_EVENTNAME, &dwNumItems, dwSize, wchar_tEventName); 
	
	dwStatusGet = RFID_GetCapCurrValue( api2Data.hReader, RFID_READCAP_EVENTNAME, &dwNumItems, dwSize, &wchar_tTemp); 
	
	if( (dwStatusSet == RFID_SUCCESS) && (dwStatusGet == RFID_SUCCESS) )
	{
		SetLogResult( TRUE );
	}

	fprintf( api3Data.fpCurLog,"<br>");
	logToResultFile(L"RFID2API-FUN");
}

void testRFIDEventNames( )
{	
	testRFID2_EVENTNAME( L"myvalidEvent");
	testRFID2_EVENTNAME( L"");
	testRFID2_EVENTNAME( L"veeeeeeeeeeeeeeeeeeeeryLooooooooooooooooooooooongEeeeeeeeeeeeeeeeeeeeeeeeeeevent" );
}



void SetCap_RFID_DEVCAP_ANTENNA_SEQUENCE( )			//13
{
	UINT8 antennaSeq[] = { 1,2,3,4,5,6,7,8 };
	DWORD dwStatusSet,dwStatusGet;	
	UINT8 uchartemp[10];
	
	// antenna sequence range from 1..8 
	//
	for (int antIndex = 1; antIndex <= 8; antIndex++ )
	{	
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: SetCap_RFID_DEVCAP_ANTENNA_SEQUENCE antennaID %d \n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,antennaSeq[antIndex] );
		fprintf( api3Data.fpCurLog,"<br>");
		DWORD dwNumItems = antIndex;					
		DWORD dwSize =dwNumItems*sizeof( antennaSeq[ 0 ] ) ;
		
		dwStatusSet = RFID_SetCapCurrValue( api2Data.hReader, RFID_DEVCAP_ANTENNA_SEQUENCE, &dwNumItems, dwSize, antennaSeq) ; 
		dwStatusGet =RFID_GetCapCurrValue( api2Data.hReader, RFID_DEVCAP_ANTENNA_SEQUENCE, &dwNumItems, dwSize, &uchartemp); 
		
		if( (dwStatusSet == RFID_SUCCESS) && (dwStatusGet == RFID_SUCCESS) )
		{
			SetLogResult( TRUE );
		}

		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RFID2API-FUN");
	}

}

void testRFID2_Write_ATTENUATION()			//5
{
	
	UINT32 arrayList[] = { 255,0,127,1000,655535 };

	for( UINT8 i=0; i<5; i++ )
	{
		fprintf(api3Data.fpCurLog,"\n\n<a name=RFID2API-FUN-%d></a> RFID2API-FUN-%d:\n\n<br>Description: testRFID2_Write_ATTENUATION Value %d \n <br> Expected OutPut:API Suucess\r\n",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo,arrayList[ i ] );
		fprintf( api3Data.fpCurLog,"<br>");
		api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_READCAP_RF_ATTENUATION,arrayList[ i ] );
		api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_WRITECAP_RF_ATTENUATION,arrayList[ i ] );
		api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_MEM_BANK, RFID_GEN2_MEMORY_BANK_EPC );
		api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_READCAP_G2_MEM_BANK_WORD_LEN, 0 );
		api2Data.rfidStatus = SetCapValue( api2Data.hReader,RFID_TAGCAP_G2_WORD_POINTER, 2 );
		
		if( RFID2ReadTags( api2Data.hReader,4 ,1 ) )
		{
			SetLogResult( TRUE );
		}
		fprintf( api3Data.fpCurLog,"<br>");
		logToResultFile(L"RFID2API-FUN");		
	}
}
	
		
/*void CapTest_RFID_READCAP_OUTLOOP()					//11
{
		
	dwSubTestCaseNo = 1;
	fprintf(fpLog,"\n\n***Inside The Function CapTest_RFID_READCAP_OUTLOOP and CapTest_RFID_READCAP_INLOOP **** ");
	fprintf(fpLog,"\n***Functionality Testing**** ");

	//Checking with random numbers 

	for (i=0;i<=128;i+=64)
	{
		
		FormTestID(dwFunTestCaseNo++,dwSubTestCaseNo,"FUN");
		boolstatus=SetCap_RFID_READCAP_OUTLOOP(i);
		if (boolstatus)
		{
			//fprintf(fpResult,"\n%s:Test with valid outer Loop Value : PASS",charszTestID,i);
			boolstatus=SetCap_RFID_READCAP_INLOOP(i);
			if (boolstatus)
			{
				//fprintf(fpResult,"\n%s:Test with valid inner Loop Value : PASS",charszTestID,i);
				SetCap_RFID_READCAP_RF_ATTENUATION(0);
				ReadTags(i,i);
				if((dwStatus==RFID_SUCCESS) && (SiTagListEX.dwTotalReads>0))
				{
					fprintf(fpResult,"\n%s: Test with valid outer Loop value and valid inner Loop Value (value is %d):PASS",charszTestID,i);
				}
				else
				{
					fprintf(fpResult,"\n%s:Test with valid outer Loop value and valid inner Loop Value (value is %d): FAILED",charszTestID,i);
				}
			}
			else
			{
				fprintf(fpResult,"\n%s: Test with  valid inner Loop Value :FAILED",charszTestID);
			}
		}
		else
		{
			fprintf(fpResult,"\n%s: Test with  valid outer Loop Value :FAILED",charszTestID);
		}
	}
}*/

void testRFID2Capabilities( )
{
	testCapabilityDefaults( );	// not supported
	//test_WriteOptions( );		// not supported
	//testRFID2_Write_ATTENUATION();
	testRFIDEventNames();
	//testRFID2_Read_ATTENUATION();
	test_EnabledTagTypes();
	SetCap_RFID_DEVCAP_ANTENNA_SEQUENCE();
}

#endif
#endif