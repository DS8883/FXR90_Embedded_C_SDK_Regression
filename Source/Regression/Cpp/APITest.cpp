// Sample.cpp : Defines the entry point for the console application.
//

#include "Common.h"
#include "APITest.h"
#include "Config.h"
#include "testNegative.h"

#ifndef linux
#include <tchar.h>		// windows specific tchar
#endif

#define MAX_FEATURE_LIST 39	// max function entry list

extern struct RFID3DATA api3Data;
extern MYTAGDATA MyTags[ 2 ];
void (*RFID3API[MAX_FEATURE_LIST])(void);		// increment the array for every new function added to automation

#ifndef linux
void TestSprint3Additions( )
{
	SetEAS_Functional(api3Data.hReader);
	SetEAS_Negative( api3Data.hReader );
	SetQuiet( api3Data.hReader );
	SetQuiet_Negative( api3Data.hReader );
}
#endif // !linux

// Validates RFID_Connect()  and RFID_DisConnect() API calls with positive and negative values
// 
void TestRFID3ConnectAPI()
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	logCurrentExecution( "TestConnectAPI");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 7;
	api3Data.dwTestCaseNo = 01;
	TestRFID_ConnectAPI( );
	TestRFID_DisConnectAPI( api3Data.hReader );
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	fprintf( api3Data.fpCurLog,"</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}

// initialize function pointers before  calling in the main function
// add in the list if extra functions are added. List only to be modified 
// if additional features are added in to the automation in future.
// 
void initFPS()
{
	RFID3API[0] = TestG2Truncate;
	RFID3API[1] = TestRFIDStateUnAware;
	RFID3API[2] = StateunAwareWithTagevents;
	RFID3API[3] = TestRFIDStateAware;
	RFID3API[4] = Test_PostFiltersReadAuto;
	RFID3API[5] = Test_PostFiltersRead;
	RFID3API[6] = TestRSSIFilter;
	RFID3API[7] = TestSimpleReadWriteErase;
	RFID3API[8] = TestOpSequenceReadWriteErase;
	RFID3API[9] = TestOpSeqNeg;
	RFID3API[10] = TestWriteSpecificAPI;
	RFID3API[11] = TestReadAllMemoryBanks;
	RFID3API[12] = TestRFIndex;
	RFID3API[13] = TestingTriggers;
	RFID3API[14] = TestingEvents;
	RFID3API[15] = testRecommissionAPI;
	RFID3API[16] = VerifyDutyCycle;
	RFID3API[17] = VerifyLocationingSanity;
	RFID3API[18] = Fujitsu64KTest;
	RFID3API[19] = ImpinjTest;
	RFID3API[20] = nxpTagTest;
	RFID3API[21] = TestRFID3ConfigParameters;
	RFID3API[22] = ValidaterRFID3rmapi;
	RFID3API[23] = VerifyRMProfileTimeZoneAPI;
	RFID3API[24] = TestAccessFilterAPI;
	RFID3API[25] = Monza5TagFocus;
	RFID3API[26] = Monza5FastID;
	RFID3API[27] = Fx7500PhaseInfo;
	RFID3API[28] = Fx7500AntennaRFConfig;
	RFID3API[29] = Fx7500AbFlipSLAll;
	RFID3API[30] = Fx7500PeriodicReportTrigger;
	RFID3API[31] = Fx7500RMAPI;
#if !defined PIXIE
	RFID3API[32] = TestG2V2Features;
	RFID3API[33] = TestLICManager; // rfid3UnSupportedFeature;
#else
	RFID3API[32] = rfid3UnSupportedFeature;
	RFID3API[33] = rfid3UnSupportedFeature;
#endif
	RFID3API[34] = TestRFID3ConnectAPI;
	RFID3API[35] = VerifyRegionConfigAPI;
	RFID3API[36] = TestRFID3ANSIAPI;
	RFID3API[37] = TestAntennaConfig;
	RFID3API[38] = rfid3UnSupportedFeature;			// RFID2API Depreacted over a period of time,
}

int getOption()
{
	//Local Declarations
	  long option;	  
	//Statements
	  wprintf(L"\t******************************************************");
	  wprintf(L"\n\t*                  MENU               *");
	  wprintf(L"\n  0. ALL FUNCTIONS ");
	  wprintf(L"\n  1. TestG2Truncate ");
	  wprintf(L"\n  2. TestRFIDStateUnAware ");
	  wprintf(L"\n  3. StateunAwareWithTagevents ");
	  wprintf(L"\n  4. TestRFIDStateAware ");
	  wprintf(L"\n  5. Test_PostFiltersReadAuto");
	  wprintf(L"\n  6. Test_PostFiltersRead");
	  wprintf(L"\n  7. TestRSSIFilter");
	  wprintf(L"\n  8. TestSimpleReadWriteErase");
	  wprintf(L"\n  9. TestOpSequenceReadWriteErase");
	  wprintf(L"\n  10. TestOpSeqNeg");
	  wprintf(L"\n  11. TestWriteSpecificAPI");
	  wprintf(L"\n  12. TestReadAllMemoryBanks");
	  wprintf(L"\n  13. TestRFIndex");
	  wprintf(L"\n  14. TestingTriggers");
	  wprintf(L"\n  15. TestingEvents");
	  wprintf(L"\n  16. testRecommissionAPI");
	  wprintf(L"\n  17. VerifyDutyCycle");
	  wprintf(L"\n  18. VerifyLocationingSanity");
	  wprintf(L"\n  19. Fujitsu64KTest");
	  wprintf(L"\n  20. ImpinjTest");
	  wprintf(L"\n  21. nxpTagTest");
	  wprintf(L"\n  22. TestRFID3ConfigParameters");
	  wprintf(L"\n  23. ValidaterRFID3rmapi");
	  wprintf(L"\n  24. VerifyRMProfileTimeZoneAPI");	 
	  wprintf(L"\n  25. TestAccessFilterAPI");
	  wprintf(L"\n  26. Monza5TagFocus");
	  wprintf(L"\n  27. Monza5FastID");
	  wprintf(L"\n  28. Fx7500PhaseInfo - Sanity");
	  wprintf(L"\n  29. Fx7500AntennaRFConfig - Sanity");
	  wprintf(L"\n  30. Fx7500AbFlipSLAll");
	  wprintf(L"\n  31. Fx7500PeriodicReportTrigger");
	  wprintf(L"\n  32. Fx7500RMAPI( CableLoss/AppInstall/SecureConfig -Sanity");
	  wprintf(L"\n  33. G2V2 Features");
	  wprintf(L"\n  34. TestLICManager");
	  wprintf(L"\n  35. TestRFID3ConnectAPI");
	  wprintf(L"\n  36. TestRFIDRegionConfigAPI");
	  wprintf(L"\n  37. TestRFID3ANSIAPI");
	  wprintf(L"\n  38. TestAntennaRFCOnfig");
	  wprintf(L"\n  39. TestRFID2API - Deprecated - not Supported ");
	  wprintf(L"\n  100. Exit  Exit  Exit Exit*");
	  wprintf(L"\n\t******************************************************");
	  
	  wprintf(L"\n\nPlease type your choice ");
	  wprintf(L"and key return: ");
	  wscanf (L"%ld", &option);
	  return option;
} //getOption  


#ifdef linux
int main(int argc, char* argv[])
{
#else
int _tmain(int argc, wchar_t* argv[])
{
#endif
	int iteration=1;
	// These tags( A22F and B22F ) use the same tag board as the regression set up of LLRP
	//
	UINT8 EPC1[ ] = { 0xA2,0x2F };
	UINT8 EPC2[ ] = { 0xB2,0x2F };
	
	if( !getConnectConfig(L"config.txt") )
	{
		wprintf(L"\n Config file is missing..Place the config file in the Current Directory\n");
		return FALSE;
	}

	initFPS( );
	wprintf(L"\n Test Execution Started............\n");  

	//// Initialize API 
	wprintf(L"\n Initializing the Reader........%ls \n",api3Data.szIPAddress);  
	initRFID( api3Data.szIPAddress,api3Data.dwPort,api3Data.readerType );
	wprintf(L"\n Reader Initialization Succcess............\n");  
	api3Data.fpCurLog = api3Data.fpLog;
	fprintf(api3Data.fpCurLog,"API Version :%ls",api3Data.dllVersionInfo);
	
	// Verify for few iterations  before showing an error message for the Required tags for automation to Run
	//
RETRY:
	if( !getTwoTags( EPC1, EPC2,&api3Data.pInfo,iteration ) )
	{
		iteration++;
		if(iteration < 3 )
		{
			goto RETRY;
		}
		else
		{
			fprintf( api3Data.fpLog ,"\n\n Could not get the Specified Tags MEMORY , Tags May have LOCKED, Place FRESH Tags \n"); 
			wprintf(L"\n\n Could not get the Specified Tags MEMORY , Tags May have LOCKED, Place FRESH Tags \n"); 
			wprintf(L"\n Exiting the Program");
			exit(1);
		}
	}

	// check if the command line argument is for automation. Some functionality is not supported for FX Readers
	// Similarly RM commands are not supported in MC Reader. Only supported functionality is executed on respective Readers.
	//
#ifndef linux
	if(argc > 1 && ( wcscmp(argv[1],L"-a") == 0))
	{
#else
	if(argc > 1)
	{
#endif
		for(int index = 0; index < MAX_FEATURE_LIST; index++ )
		{
			(*RFID3API[index])( );
		}

		DeInitRFID( );		// DeInit After the exection of all the Features.
	}
	else
	{
		bool Exit = FALSE;
		do
		{
			int Choice = getOption( );
			if (Choice == 0)
			{
				for (int index = 0; index < MAX_FEATURE_LIST; index++)
				{
					(*RFID3API[index])();
				}

				DeInitRFID();
				Exit = TRUE;
			}
			else if (Choice > 0 && Choice < 100 )
			{
				(*RFID3API[Choice - 1])( );
			}
			else if (Choice == 100)
			{
				DeInitRFID();
				Exit = TRUE;
			}

			/*switch( Choice )
			{
				case 0:
					for (int index = 0; index < MAX_FEATURE_LIST; index++)
					{
						(*RFID3API[index])();
					}

					DeInitRFID( );
					Exit = TRUE;
				break;
				case 1 ... 38:
				/*case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:
				case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
				case 30:case 31:case 32:case 33:case 34:case 35: case 36: case 37: case 38:

				(*RFID3API[Choice - 1])();
				break;
				case 100:
					DeInitRFID();
					Exit = TRUE;
				default:
					break;
			}*/
		}while( Exit == FALSE );
	}

	fflush( api3Data.fpLog );
	fflush( api3Data.fpResult );
	exit(0);
}

