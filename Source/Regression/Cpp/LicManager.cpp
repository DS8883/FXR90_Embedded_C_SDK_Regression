#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;	// RFID3 Data

static wchar_t LicManagerURLW[ ] = L"https://zebra-licensing-uat.flexnetoperations.com/flexnet/deviceservices";
static char LicManagerURLA[ ] = "https://zebra-licensing-uat.flexnetoperations.com/flexnet/deviceservices";
static wchar_t activationIDW[ ] = L"8c88-d0e7-9f3c-435b-968b-69a8-7f8e-a302" ;
static char activationIDA[ ] = "8c88-d0e7-9f3c-435b-968b-69a8-7f8e-a302";
static wchar_t filePathW[ ] = L"C:\\Junk\\FX9600_84_24_8D_EF_94_17.bin";
static char filePathA[ ] = "C:\\Junk\\FX9600_84_24_8D_EF_94_17.bin";

// Test LIC Mangager Features.
////#ifdef UNICODE
//#define RFID_AcquireLicenseOnLine RFID_AcquireLicenseOnLineW
//#define RFID_AcquireLicenseOffLine RFID_AcquireLicenseOffLineW
//#define RFID_ReturnLicense	RFID_ReturnLicenseW
//#define RFID_GetAvailableLicenses RFID_GetAvailableLicensesW
//#else
//#define RFID_AcquireLicenseOnLine RFID_AcquireLicenseOnLineA
//#define RFID_AcquireLicenseOffLine RFID_AcquireLicenseOffLineA
//#define RFID_ReturnLicense RFID_ReturnLicenseA
//#define RFID_GetAvailableLicenses RFID_GetAvailableLicensesA

void TestLICManager( )
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("Test_LicManager");
	logCurrentExecution("Test_LicManager");

	fprintf(api3Data.fpCurLog, "<html>\n");
	fprintf(api3Data.fpCurLog, "<body>\n");

	LICENSE_LISTW licenseListW = {0,};
	LICENSE_LISTA licenseListA = { 0, };
	if (api3Data.readerType == FX)
	{
		if (!api3Data.hReaderMgmt )
		{
			Login();
		}
		
		api3Data.rfidStatus = RFID_AcquireLicenseOffLineW(api3Data.hReaderMgmt, filePathW, FALSE, NULL);
		api3Data.rfidStatus = RFID_AcquireLicenseOffLineA(api3Data.hReaderMgmt, filePathA,  TRUE, NULL);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{

		}
		else
		{

		}

		api3Data.rfidStatus = RFID_AcquireLicenseOnLineW(api3Data.hReaderMgmt, LicManagerURLW, activationIDW, FALSE, NULL);
		wprintf(L"\n Api Result : %ls", RFID_GetErrorDescriptionW(api3Data.rfidStatus));
		api3Data.rfidStatus = RFID_AcquireLicenseOnLineA(api3Data.hReaderMgmt, LicManagerURLA, activationIDA, TRUE, NULL);
		printf("\n Api Result : %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
		
		api3Data.rfidStatus = RFID_GetAvailableLicensesA(api3Data.hReaderMgmt, &licenseListA, NULL);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			for (int liclist = 0; liclist < licenseListW.numLicenses; liclist++)
			{
				printf("\nLiC Info : Licence : %d count : %d index : %d szExpiryDate : %s szHostID : %s Name: %s Version : %s \n ", licenseListA.numLicenses, licenseListA.pLicenseList->count, licenseListA.pLicenseList->index, licenseListA.pLicenseList->szExpiryDate, licenseListA.pLicenseList->szHostID, licenseListA.pLicenseList->szName, licenseListA.pLicenseList->szVersion);
			}

			{
				wprintf(L"NumLicenses %ld", licenseListW.numLicenses);

			}
		}

		api3Data.rfidStatus = RFID_GetAvailableLicensesW(api3Data.hReaderMgmt, &licenseListW, NULL);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			for (int liclist = 0; liclist < licenseListW.numLicenses; liclist++)
			{
				wprintf(L"\n LiC Info : Licence : %ld count : %ld index : %ld szExpiryDate : %ls szHostID : %ls Name: %ls Version : %ls \n ", licenseListW.numLicenses, licenseListW.pLicenseList->count, licenseListW.pLicenseList->index, licenseListW.pLicenseList->szExpiryDate, licenseListW.pLicenseList->szHostID, licenseListW.pLicenseList->szName, licenseListW.pLicenseList->szVersion);
			}
		}


		
			

		api3Data.rfidStatus = RFID_ReturnLicenseW(api3Data.hReaderMgmt, LicManagerURLW, activationIDW, NULL);
		api3Data.rfidStatus = RFID_ReturnLicenseA(api3Data.hReaderMgmt, LicManagerURLA, activationIDA, NULL);
		
	}
	else
	{
		wprintf(L"\n Functionality Not Supported");
	}
	

}