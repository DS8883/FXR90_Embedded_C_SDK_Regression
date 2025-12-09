// ManualTests.cpp : Defines the entry point for the console application.
//
#include "ManualTests.h"

RFID3DATA api3Data;
void initAPI3Data()
{
	api3Data.apiVersion = RFID_API3_5_5;
	api3Data.bContinue = api3Data.bTagReadEvent = api3Data.bWait=FALSE;
	api3Data.bStartEvent = FALSE;
	api3Data.bStopEvent = FALSE;
	api3Data.fpLog = api3Data.fpOut = api3Data.fpResult = api3Data.fpStressLog = NULL;
	api3Data.pRfid3WideCharString = NULL;
	api3Data.readerType = FX;
	api3Data.pRfid3WideCharString = new wchar_t[512];	// allocate memory
}
int getOption()
{
	//Local Declarations
	long option;	  
	//Statements
	wprintf(L"\n\t***************************************");
	wprintf(L"\n\t*                  MENU               *");
	wprintf(L"\n  1. StressInventory-- Connect --InV--DisConnect. ");
	wprintf(L"\n  2. StressResetConfig-- Radio ON--OFF--ResetConfig. ");
	wprintf(L"\n  3. TagWrites - Place a Single Higgs3tag in FOV. ");
	wprintf(L"\n  4. Only Radio On OFF ");
	wprintf(L"\n  5. BlockPermaLock - Place Single Higgs3 in FOV ");
	wprintf(L"\n  6. StressBlockErase - Place Single Higgs3 in FOV ");
	wprintf(L"\n  7. StressBlockWrite - Place Single Higgs3 in FOV ");
	wprintf(L"\n  8. UpdateFWonFXDevices - Update FW");
	wprintf(L"\n  9. AccessSuccessFailCount- Measure Access Performance");
	wprintf(L"\n  10. TestRFID3AcceptConnection - LLRP as Client");
	wprintf(L"\n  11. ReadPerformance - in a Duration");
	wprintf(L"\n  12. RFIDEvents - GPI,RDRDISCONNECT,HANDHELDTRIGGER,ANTDISCONNECT ");
	wprintf(L"\n  13. Test for TagFocus" );
	wprintf(L"\n  14. Test for Fast ID " );
	wprintf(L"\n  15. Test for ABFILIP and SLALL" );
	wprintf(L"\n  16. Stress RMAPI" );
	wprintf(L"\n  17. StressConfigAPI" );
	wprintf(L"\n  18. Connect2MultipleHosts" );
	wprintf(L"\n  19. PeriodicReport" );
	wprintf(L"\n  20. Impinj Testing - QT" );
	wprintf(L"\n  21. TagobservationTrigger" );
	wprintf(L"\n  22. RSSI FILTER" );	
	wprintf(L"\n  23. Cable Loss Compensation" );
	wprintf(L"\n  24. Idle Mode Timeout" );
	wprintf(L"\n  25. Secure Connection" );
	wprintf(L"\n  26. Test Install User APP" );
	wprintf(L"\n  27. GetSetRadioPowerState" );
	wprintf(L"\n  28. AntennaRFConfig" );
	wprintf(L"\n  29. Secure Connection Config" );
	wprintf(L"\n  30. Truncate" );
	wprintf(L"\n  31. TestSecureNonSecure" );
	wprintf(L"\n  32. Read All MemoryBanks." );
	wprintf(L"\n  33. Test--rfid4--events" );
	wprintf(L"\n  34. nxp EAS Scan - with EAS and C1G2" );
	wprintf(L"\n  35. access in ALL { Regions, Modes } - Longer Time to Run" );
	wprintf(L"\n  36. Call Clean Up Functions" );
	wprintf(L"\n  37. StressZoneInfo - RM" );
	wprintf(L"\n  38. TestPowerNegotion - RM");
	wprintf(L"\n  39. TestNXPBrandID");
	wprintf(L"\n  40. RFID4ErrorInformation"); 
	wprintf(L"\n  41. TestTransmitDelay");
	wprintf(L"\n  42. TestNewAPIadditions");
	wprintf(L"\n  43. TestRFIDFreeFilters");
	wprintf(L"\n  100. Exit  Exit  Exit Exit*");
	wprintf(L"\n ************************************* *");

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
	long Choice = 0;
	long Count;
	int version = 1;
	int bMode;
	RFID_VERSION apiVersion;
	VERSION_INFO versionInfo;
	UINT8 EPC1[] = {0x2F,0x22};
	UINT8 EPC2[] = {0xCC,0x00};
	UINT8 TagMask[] = {0xFF,0xFF};

		
	initAPI3Data();
#if defined(__linux__)
	if (argc > 1)
	{
		wcscpy(api3Data.szIPAddress, MultiByte2WideChar(argv[1]));
	}
#elif defined(_WIN64)
	if (argc > 1)
	{
		wcscpy(api3Data.szIPAddress, argv[1]);
	}
#endif
	
	wprintf(L"\n RFID_API3_5_0 -- 0");
	wprintf(L"\n RFID_API3_5_1 -- 1");
	wprintf(L"\n RFID_API3_5_5 -- 2");
	wprintf(L"\n RFID_API3_5_6 -- 3");
	
	wprintf(L"\n Select Version: ");
	wscanf(L"%d", &version);
	
	if( version == 0 )
	{
		apiVersion = RFID_API3_5_0;
	}
	else if(version == 1)
	{
		apiVersion = RFID_API3_5_1;
	}
	else if(version == 2)
	{
		apiVersion = RFID_API3_5_5;
	}
	else if (version == 3)
	{
		apiVersion = RFID_API3_5_6;
	}
#if defined(__linux__) && defined(PIXIE)
	wcscpy(api3Data.szIPAddress, L"/dev/ttyUSB0");
	api3Data.dwPort = 921600;
	/*wcscpy(api3Data.szIPAddress, L"10.17.130.229");
	api3Data.dwPort = 9180;*/
#elif defined(_WIN64) && defined(PIXIE)
	wcscpy(api3Data.szIPAddress, L"COM42");
	api3Data.dwPort = 921600;
	/*wcscpy(api3Data.szIPAddress, L"10.17.130.229");
	api3Data.dwPort = 9180;*/
#endif
	memset(&api3Data.postFilter,0,sizeof(POST_FILTER));
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,EPC1,16,TagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,EPC2,16,TagMask,16,{0} };
	api3Data.postFilter.lpTagPatternA = &tpA;api3Data.postFilter.lpTagPatternB = &tpB;
	api3Data.postFilter.matchPattern = A_AND_NOTB;

	RFID_GetDllVersionInfo(&versionInfo);
	printf("\n\n Dll Verison: %S\n\n", versionInfo.dllVersion);
	
	do
	{
		Choice = getOption();
		switch( Choice )
		{
			case 1:
				wprintf(L"\n Input the number of times you want to Run the Test:" );
				wscanf (L"%ld", &Count);
				StressInventory( Count,apiVersion);
				break;
			case 2:
				wprintf(L"\n Input the number of times you want to Run the Test:" );
				wscanf (L"%ld", &Count);
				TestResetConfig( Count,apiVersion);
				break;
			case 3:
				wprintf(L"\n Input the number of writes to the Tag - USERMEM:" );
				wscanf (L"%ld", &Count);
				SingleTagWrites( Count,apiVersion);
				break;
			case 4:
				wprintf(L"\nInput the number of times you want to Run the Test:" );
				wscanf (L"%ld", &Count);
				StressRadioONOFF( Count,apiVersion);
				break;
			case 5:
				{
					wprintf(L"\n Place a fresh Higgs3 tag in FOV.. accesspassword as 0x00000001..press any key" );
					int c = getchar();
					TestBlockPermaLock(apiVersion );
				}
				break;
			case 6:
				wprintf(L"\n Input the number of Erases to the Tag - USERMEM:" );
				wscanf (L"%ld", &Count);
				StressBlockErase( Count,apiVersion);
				break;
			case 7:
				wprintf(L"\n Input the number of BlockWrites to the Tag - USERMEM:" );
				wscanf( L"%ld", &Count );
				StressBlockWrite( Count,apiVersion );
				break;
			case 8:
				//UpdateFWonFXDevices(apiVersion );
				UpdateFWonPXDevices(apiVersion);
				break;
			case 9:
				{
					wprintf(L"\n Test Access Success or Failure Rate.. Input Number of Writes you want" );
					wscanf (L"%ld", &Count);
					wprintf(L"\n Place a single Higgs3 Tag in FOV ..press any key");
					int x = getchar();
					AccessSuccessFailCount( Count,apiVersion );
				}
				break;
			case 10:
				{
					wprintf(L"\n Test Accept Connection.. LLRP in Client Mode." );
					wprintf(L"\n Input Port Number and Initiate Connection from the Reader on this Port with m/c IP:" );
					wscanf (L"%ld", &Count);
					AcceptConnectionInRegularMode( Count );
					//AcceptConnectioninSecureMode( Count );

				}
				break;
			case 11:
				{
					SESSION Sessions[] = { SESSION_S0,SESSION_S1,SESSION_S2,SESSION_S3 };
					long Iterations,Duration,Session,antenna;
					wprintf(L"\n Read performance -[IN] Iterations,Duration,Session" );
					wprintf(L"\n Iterations[ 1..n ]: ");
					wscanf (L"%ld", &Iterations);
					wprintf(L"\n Duration( ms ): ");
					wscanf (L"%ld", &Duration);
					wprintf(L"\n Session[ 0 1 2 3 ]: ");
					wscanf (L"%ld", &Session);
					wprintf(L"\n Antenna[ 0 - ALL ] ");
					wscanf (L"%ld", &antenna);
					ReadPerformance(antenna, Iterations, Duration, Sessions[Session],apiVersion  );
				}
				break;
			case 12:
				{
					wprintf(L"\n Test Events : GPI READERDISCONNECT HHTRIGGER" );
					TestRFIDEvents(  );
				}
				break;
			case 13:
				{
					wprintf(L"\n Test for  TagFocus" );
					
					TestTagFocus(apiVersion);
				}
				break;
			case 14:
				{
					wprintf(L"\n Test for Fast ID " );
					FastID(apiVersion);
				}
				break;
			case 15:
				{
					wprintf(L"\n Input the number of times you want to Run the Test:");
					wscanf(L"%ld", &Count);
					for (int i = 0; i < Count; i++)
					{
						wprintf(L"\n Test StateAwareFilters");
						TRIGGER_INFO tInfo;
						memset(&tInfo, 0, sizeof(tInfo));
						tInfo.startTrigger.type = START_TRIGGER_TYPE_IMMEDIATE;
						tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_N_ATTEMPTS_WITH_TIMEOUT;
						tInfo.stopTrigger.value.numAttempts.n = 4; //Test with Two Tags in the field
						tInfo.stopTrigger.value.numAttempts.timeoutMilliseconds = 5000;
						tInfo.tagReportTrigger = 1;
						TAG_EVENT_REPORT_INFO tagEventInfo = { IMMEDIATE,1000,IMMEDIATE,1000,IMMEDIATE,1000 };
						tInfo.lpTagEventReportInfo = &tagEventInfo;

						TestNewStateAwareFilters(apiVersion, &tInfo);

						//TestNewStateAwareFilters(apiVersion, NULL);

						tInfo.lpTagEventReportInfo = NULL;
						tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_TAG_OBSERVATION_WITH_TIMEOUT;
						tInfo.stopTrigger.value.tagObservation.n = 5;
						tInfo.stopTrigger.value.tagObservation.timeoutMilliseconds = 5000;

						TestNewStateAwareFilters(apiVersion, &tInfo);

						tInfo.stopTrigger.type = STOP_TRIGGER_TYPE_DURATION;
						tInfo.stopTrigger.value.duration = 4000;

						TestNewStateAwareFilters(apiVersion, &tInfo);
					}
				}
				break;
			case 16:
				{
					int Iterations;
					wprintf(L"\n Stress RMAPI ..in put iterations " );
					wscanf (L"%ld", &Iterations);
					StressRMAPI(Iterations,apiVersion);
				}
				break;
			case 17:
				{
					int Iterations;
					wprintf(L"\n Stress Config API .in put iterations" );
					wscanf (L"%ld", &Iterations);
					StressConfigAPI(Iterations,apiVersion);
				}
				break;
			case 18:
				{
					int Iterations;
					wprintf(L"\n Connect 2 multipls hosts and inventory" );
					
					//Connect2multipleHosts(L"10.17.130.229", 9180);
					
					Connect2multipleHosts(L"COM12", 921600);
					rfid3Sleep(5000);
					Connect2multipleHosts(L"COM38", 921600);
					
					/*Connect2multipleHosts(L"10.17.129.42", 5084);
					rfid3Sleep(5000);
					Connect2multipleHosts(L"10.17.130.229", 5084);
					rfid3Sleep(5000);
					Connect2multipleHosts(L"10.17.130.74", 5084);
					rfid3Sleep(5000);
					Connect2multipleHosts(L"10.17.130.131", 5084);*/
					rfid3Sleep(5000000);
				}
				break;

			case 19:
				{
					wprintf(L"\n periodic report info" );
					TestPeriodicReport(apiVersion);
				}
				break;

			case 20:
				{
					wprintf(L"\n Impinj Testing" );
					ImpinjTest( apiVersion );
				}
				break;

			case 21:
				{
					wprintf(L"\n TagObservation trigger" );
					TagObservationwithTagEvents(apiVersion);
				}
				break;
			case 22:
				{
					wprintf(L"\n RSSI FIlter Testing" );
					TestRSSI(apiVersion );
				}
				break;
			case 23:
				{
					wprintf(L"\n Cable Loss Compensation - FX7500" );
					TestCableLossCompensation( apiVersion );
				}
				break;
			case 24:
				{
					wprintf(L"\n idle Mode" );
					TestRadioIdleTimeout( apiVersion );
				}
				break;
			case 25:
				{
					wprintf(L"\n Stress Test Secure Connection\n" );
					wprintf(L"\n Windows: Place All .Pem files in C:\\FX7500\\CertDir \n");
					wprintf(L"\n Linux : Place All .Pem files in Current Dir\n");
					wprintf(L"\n Input Number of iterations you want to run the Test:" );
					wscanf (L"%ld", &Count);
					StressSecureConnection( apiVersion,Count );
				}
				break;
			case 26:
				{
					wprintf(L"\n Install User Applications - Configure Reader in desired Mode" );
					wprintf(L"\n Input the Mode you want to run the Test 0 - http 1-https:" );
					wscanf (L"%ld", &bMode);
					wprintf(L"\n Input Number of iterations you want to run the Test:" );
					wscanf (L"%ld", &Count);
					InstallUSerAppTest( apiVersion,(BOOLEAN)bMode,Count );
				}
				break;
			case 27:
				{
					wprintf(L"\n GetSetRadio" );
					GetSetRadioPowerState( apiVersion );
				}
				break;
			case 28:
				{
					wprintf(L"\n Antenna RFConfig" );
					AntennaRFConfig( apiVersion );
				}
				break;
			case 29:
				{
					wprintf(L"\n Secure LLRP Conn Config" );
					SecureLLRPConnectionConfig( apiVersion);
				}
				break;
			case 30:
				{
					wprintf(L"\n Truncate" );
					Truncate( apiVersion);
				}
				break;
			case 31:
				{
					wprintf(L"\n Test NonSecure \n" );
					TestSecureNonSecure( apiVersion );
				}
				break;
			case 32:
				{
					wprintf(L"\n Real All Memory Banks with different Offsets and Lengths." );
					ReadAllMemoryBanks( apiVersion );
				}
				break;
			case 33:
				{
					wprintf(L"\n Test--rfid4--events" );
					rfid4events( );
				}
				break;
			case 34:
				{
#ifndef linux
					wprintf(L"\n nxp EAS Scan with NXP,C1G2\n" );
					DoNxpScan( apiVersion );
#endif
				}
				break;
			case 35:
				{
					wprintf(L"\n  access in ALL { Regions, Modes } \n" );
					if (api3Data.dwPort != 921600)
					{
						accessWithRFModesFX(apiVersion);
					}
					else
					{
#ifdef PIXIE
						accessWithRFModesPXI(apiVersion);
						//RegulatoryTesting(apiVersion);

#endif // PIXIE

					}

				}
				break;
				case 36:
				{
					wprintf(L"\n CallAllCleanUPFunctions \n" );
					CallAllCleanUPFunctions( apiVersion );
				}
				break;
				case 37:
				{
					wprintf(L"\n Stress TimeZone Info\n" );
					wprintf(L"\n Input Number of iterations you want to run the Test:" );
					wscanf (L"%ld", &Count);
					StressZoneInfo( Count,apiVersion );
				}
				break;
				case 38:
				{
					wprintf(L"\n Stress Power NEg Info\n");
					wprintf(L"\n Input Number of iterations you want to run the Test:");
					wscanf(L"%ld", &Count);
					TestPoweNegAPI(RFID_API3_5_5,0, 1000);
				}
				break;
				case 39:
				{
					wprintf(L"\n NXP Brand ID - Check\n");
					//TestNXPBrandID( apiVersion );
					//0x01,0x01, 0x10, 0x10,0x40,0x40, 0x200,0x200, 0x400,0x400, 0x600,0x600
					//nxpOpSeqResetConfigG2IL(0xFFFF);
				}
				break;
				case 40:
				{
					wprintf(L"\n Test RFID4 Error Information\n");
					TestRFID4ErrorInformationText();
				}
				break;
				case 41:
				{
					wprintf(L"\n Transmit Delay Tests\n");
					TestTransmitDelay(apiVersion);
				}
				break;
				case 42:
				{
#if !defined PIXIE
					wprintf(L"\nTestNewAPIadditions Tests\n");
					TestNewAPIadditions( );
					//TestG2V2HideShowMemBank(apiVersion);
					//TestPostFilters();
					//SwitchWithSecureModes();
					
#endif
				}
				break;
				case 43:
				{
					wprintf(L"\ TestFreeFilters Tests\n");
					TestRFID4PREFILTERS_STATE_AWARE( );
					TestRFID4PREFILTERS_STATE_UNAWARE();
				}
				break;
			default:
				break;
		}
	}while(Choice != 100 );

	return 0;
}

