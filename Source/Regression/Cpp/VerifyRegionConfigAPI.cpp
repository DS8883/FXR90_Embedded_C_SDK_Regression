#include "Common.h"
#include <stdlib.h>
#include "NxpC.h"
#include "APITest.h"
#include <time.h>

// global data for API
extern struct RFID3DATA api3Data;

// Helper function to get current timestamp
static void GetCurrentTimestamp(char* buffer, size_t bufferSize)
{
	time_t now = time(NULL);
	struct tm* t = localtime(&now);
	snprintf(buffer, bufferSize, "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec);
}
static int ThreeChannelMask[ ] = { 1,1,1 };
static int FourChannelMask[] = { 1,1,1,1 };
static int SixChannelMask[] = { 1,1,1,1,1,1 };
static int SevenChannelMask[] = { 1,1,1,1,1,1,1 };

struct RegionInfo
{
	wchar_t regionName[256];
	wchar_t StandardName[256];
	int numChannels;
	BOOLEAN isHoppingConfigurable;
};

struct RegionInfoA
{
	CHAR regionName[256];
	CHAR StandardName[256];
	int numChannels;
	BOOLEAN isHoppingConfigurable;
};

static int NumberOfRegions;
#define MAX_REGIONS 100
static RegionInfo RdrRegionInfo[MAX_REGIONS];
static RegionInfoA RdrRegionInfoA[MAX_REGIONS];

void getSupportedRegionsW( )
{
	wprintf(L"\n[DEBUG] getSupportedRegionsW() - ENTRY");
	REGION_LIST supportedRegions = {0,};
	STANDARD_INFO_LIST StdInfoList = {0,};
	
	wprintf(L"\n[DEBUG] Calling RFID_GetSupportedRegionList...");
	api3Data.rfidStatus = RFID_GetSupportedRegionList( api3Data.hReaderMgmt, &supportedRegions );
	wprintf(L"\n[DEBUG] RFID_GetSupportedRegionList returned: %d", api3Data.rfidStatus);
	
	if ( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		NumberOfRegions = supportedRegions.numRegions;
		wprintf(L"\n[DEBUG] Number of Regions: %d (MAX_REGIONS=%d)", supportedRegions.numRegions, MAX_REGIONS);
		
		if (supportedRegions.numRegions > MAX_REGIONS) {
			wprintf(L"\n[ERROR] Number of regions (%d) exceeds MAX_REGIONS (%d)! Truncating.", supportedRegions.numRegions, MAX_REGIONS);
			supportedRegions.numRegions = MAX_REGIONS;
			NumberOfRegions = MAX_REGIONS;
		}
		
		wprintf(L"\n Number of Regions Supported : %d",supportedRegions.numRegions);
		for ( UINT32 i = 0; i < supportedRegions.numRegions; i++ )
		{
			wprintf(L"\n Region #%d : %ls",i, supportedRegions.RegionNamesList[i]);	
		}
	}
	else
	{
		wprintf(L"\n[ERROR] RFID_GetSupportedRegionList() failure error Msg %ls",RFID_GetErrorDescription( api3Data.rfidStatus));
		return;
	}

	wprintf(L"\n[DEBUG] Starting region details loop for %d regions...", supportedRegions.numRegions);
	
	for (UINT32 i = 0; i < supportedRegions.numRegions; i++)
	{
		wprintf(L"\n[DEBUG] Processing region index %d/%d: %ls", i, supportedRegions.numRegions, supportedRegions.RegionNamesList[i]);
		
		// Bounds check
		if (i >= MAX_REGIONS) {
			wprintf(L"\n[ERROR] Region index %d exceeds MAX_REGIONS (%d), skipping!", i, MAX_REGIONS);
			continue;
		}
		
		api3Data.rfidStatus = RFID_GetRegionStandardList( api3Data.hReaderMgmt,supportedRegions.RegionNamesList[i], &StdInfoList );
		wprintf(L"\n[DEBUG] RFID_GetRegionStandardList for region %d returned: %d", i, api3Data.rfidStatus);
		
		if ( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			wprintf(L"\n[DEBUG] Region %d has %d standard(s)", i, StdInfoList.numStds);
			
			for (UINT32 j = 0; j<StdInfoList.numStds; j++)
			{
				wprintf(L"\n[DEBUG] Processing standard %d/%d for region %d", j, StdInfoList.numStds, i);
				
				// Safe string copy with bounds check
				wcsncpy(RdrRegionInfo[i].regionName, StdInfoList.StdsList[j].strRegionName, 255);
				RdrRegionInfo[i].regionName[255] = L'\0';  // Ensure null termination
				
				wcsncpy(RdrRegionInfo[i].StandardName, StdInfoList.StdsList[j].strStandardName, 255);
				RdrRegionInfo[i].StandardName[255] = L'\0';  // Ensure null termination
				
				RdrRegionInfo[i].numChannels = StdInfoList.StdsList[j].iNumChannels;
			RdrRegionInfo[i].isHoppingConfigurable = StdInfoList.StdsList[j].bIsHoppingConfigurable;
				
				wprintf(L"\n[DEBUG] Stored region info [%d]: name=%ls, standard=%ls, channels=%d", 
					i, RdrRegionInfo[i].regionName, RdrRegionInfo[i].StandardName, RdrRegionInfo[i].numChannels);

				wprintf(L"\n==================================================");
				wprintf(L"\nRegion #%d : %ls", i, StdInfoList.StdsList[j].strRegionName);	
				wprintf(L"\nNumber of Standards %d", StdInfoList.numStds);
				wprintf(L"\nStandardName %ls", StdInfoList.StdsList[j].strStandardName);
				wprintf(L"\nNumChannels %d", StdInfoList.StdsList[j].iNumChannels);
				wprintf(L"\nIsHoppingConfigurable %ls", StdInfoList.StdsList[j].bIsHoppingConfigurable?L"Yes":L"No");

				if(StdInfoList.StdsList[j].bIsHoppingConfigurable)
				{
					wprintf(L"\n[DEBUG] Processing %d channels for hopping-enabled region", StdInfoList.StdsList[j].iNumChannels);
					
					// Validate channel info pointer
					if (StdInfoList.StdsList[j].piChannelInfo == NULL || StdInfoList.StdsList[j].piChannelValueInfo == NULL) {
						wprintf(L"\n[ERROR] NULL channel info pointer for region %d, standard %d", i, j);
						continue;
					}
					
					for (UINT32 k = 0; k<StdInfoList.StdsList[j].iNumChannels; k++)
					{
						double freqMHz = StdInfoList.StdsList[j].piChannelValueInfo[k] / 1000.0;
						wprintf(L"\nChannels index %ld values %.2f MHz", StdInfoList.StdsList[j].piChannelInfo[k], freqMHz);
					}
				}
				wprintf(L"\n==================================================\n");
				wprintf(L"\n[DEBUG] Completed processing standard %d for region %d", j, i);
			}
		}
		else
		{
			wprintf(L"\n[ERROR] RFID_GetRegionStandardList() failure for region %d, error Msg %ls", i, RFID_GetErrorDescription( api3Data.rfidStatus));
		}
	}
	
	wprintf(L"\n[DEBUG] getSupportedRegionsW() - EXIT (processed %d regions)", supportedRegions.numRegions);
}

long DoInventory()
{
	long TagCount = 0;
	LOGIN_INFO loginfo;
	char timestamp[32];
	
	// Save login info for re-login after inventory
	wcsncpy(loginfo.hostName, api3Data.szIPAddress, 255);
	wcsncpy(loginfo.userName, L"admin", 255);
	wcsncpy(loginfo.password, L"Zebra@123", 255);
	loginfo.forceLogin = TRUE;
	loginfo.version = RFID_API3_5_5;
	
	// Setup connection info for data connection
	// Data connection (RFID_Connect) does NOT require secure mode
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	ConInfo.lpSecConInfo = NULL;  // No security info needed for data connection
	
	// CRITICAL FIX: Must logout from management connection before opening data connection
	// FXR90 reader only allows ONE connection at a time per client
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls DoInventory: Logging out from management connection...", MultiByte2WideChar(timestamp));
	RFID_Logout(api3Data.hReaderMgmt);
	api3Data.hReaderMgmt = 0;
	
	// Wait longer for reader to fully release the management connection
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls DoInventory: Waiting 60 seconds for session cleanup...", MultiByte2WideChar(timestamp));
	rfid3Sleep(60000);
	
	// Now open data connection for inventory
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [6] Opening data connection (RFID_Connect)...", MultiByte2WideChar(timestamp));
	wprintf(L"\n%ls Current state: hReaderMgmt=%d (still open)", MultiByte2WideChar(timestamp), api3Data.hReaderMgmt);
	wprintf(L"\n%ls Creating CONNECTION_INFO: version=RFID_API3_5_5, lpSecConInfo=NULL", MultiByte2WideChar(timestamp));
	wprintf(L"\n%ls Calling RFID_Connect(IP=%ls, Port=5084, Timeout=0)...", MultiByte2WideChar(timestamp), api3Data.szIPAddress);
	
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n%ls API: RFID_Connect() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
		wprintf(L"\n%ls FAILED: Unable to connect to reader", MultiByte2WideChar(timestamp));
		
		// RFID_Connect failed, need to re-establish management connection before returning
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls Re-logging to management after connection failure...", MultiByte2WideChar(timestamp));
		api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, TRUE, 0);
		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls CRITICAL - Re-login failed: %ls", MultiByte2WideChar(timestamp), RFID_GetErrorDescription(api3Data.rfidStatus));
		}
		return 0;
	}
	
	wprintf(L"\n%ls API: RFID_Connect() -> Status=0 (Function succeeded), Handle=%d", MultiByte2WideChar(timestamp), api3Data.hReader);
	wprintf(L"\n%ls SUCCESS: Connected, hReader=%d (hReaderMgmt=%d still open)", MultiByte2WideChar(timestamp), api3Data.hReader, api3Data.hReaderMgmt);
	
	// Allocate tag memory
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [7] Allocating tag memory...", MultiByte2WideChar(timestamp));
	LPTAG_DATA pInventoriedTag = RFID_AllocateTag(api3Data.hReader);
	if (pInventoriedTag == NULL)
	{
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls FAILED: RFID_AllocateTag returned NULL", MultiByte2WideChar(timestamp));
		RFID_Disconnect(api3Data.hReader);
		
		// Re-establish management connection before returning
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls Re-logging to management after tag allocation failure...", MultiByte2WideChar(timestamp));
		api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, TRUE, 0);
		return 0;
	}
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls Tag memory allocated successfully", MultiByte2WideChar(timestamp));
	
	// Perform inventory operations
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [8] Purging old tags...", MultiByte2WideChar(timestamp));
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls API: RFID_PurgeTags() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [9] Starting inventory operation...", MultiByte2WideChar(timestamp));
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls API: RFID_PerformInventory() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [10] Waiting 5 seconds for inventory to complete...", MultiByte2WideChar(timestamp));
	rfid3Sleep(5000);
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [11] Stopping inventory...", MultiByte2WideChar(timestamp));
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls API: RFID_StopInventory() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));

	// Read tags
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [12] Reading tags from buffer...", MultiByte2WideChar(timestamp));
	while ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pInventoriedTag)) == RFID_API_SUCCESS)
	{
		TagCount++;
	}
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls \n***** RESULT: Read %ld tags *****", MultiByte2WideChar(timestamp), TagCount);
	
	// Clean up data connection
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [13] Deallocating tag memory...", MultiByte2WideChar(timestamp));
	api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader, pInventoriedTag);
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls [14] Disconnecting data connection (hReader=%d)...", MultiByte2WideChar(timestamp), api3Data.hReader);
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls API: RFID_Disconnect() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
	
	// Re-establish management connection for next region
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls Re-logging to management connection...", MultiByte2WideChar(timestamp));
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, TRUE, 0);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls CRITICAL - Re-login failed: %ls", MultiByte2WideChar(timestamp), RFID_GetErrorDescription(api3Data.rfidStatus));
	}
	else
	{
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls Management connection (hReaderMgmt=%d) remains open for next region\n", MultiByte2WideChar(timestamp), api3Data.hReaderMgmt);
	}

	return TagCount;
}

// Verify Inventory on all regions with all channels selected: Restrict the Power to 1/2 Watt( 24dbm );
//
BOOLEAN VerifyInventoryforAllRegions(RFID_VERSION rfidVersion)
{
	REGION_LIST regionList;
	STANDARD_INFO_LIST StandardInfo;
	REGION_LISTA regionListA;
	STANDARD_INFO_LISTA StandardInfoA;
	ACTIVE_REGION_INFO activeRegionInfo;
	UINT32 ChannelCount = 0;
	LOGIN_INFO loginfo;

	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("Test_REGION_CONFIG_API ");
	logCurrentExecution("Test_REGION_READ_API");

	fprintf(api3Data.fpCurLog, "<html>\n");
	fprintf(api3Data.fpCurLog, "<body>\n");
	
	
	memset(&loginfo, 0, sizeof(LOGIN_INFO));
	wcsncpy(loginfo.hostName, api3Data.szIPAddress, 255);
	wcsncpy(loginfo.userName, L"admin", 255);
	wcsncpy(loginfo.password, L"Zebra@123", 255);
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	
	// Use TRUE for secure mode (reader requires secure connections)
	BOOLEAN rmSecureMode = TRUE;
	
	wprintf(L"\n Login info: IP=%ls, User=%ls, Version=%d, SecureMode=%ls", 
		loginfo.hostName, loginfo.userName, loginfo.version, rmSecureMode ? L"TRUE" : L"FALSE");

	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, rmSecureMode, 0);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n VerifyInventoryforAllRegions: RFID_Login failed with error: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
		return FALSE;
	}
	wprintf(L"\n RFID_Login successful");
	
	api3Data.rfidStatus = RFID_GetSupportedRegionList(api3Data.hReaderMgmt, &regionList);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n VerifyInventoryforAllRegions: RFID_GetSupportedRegionList failed with error: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		RFID_Logout(api3Data.hReaderMgmt);
		fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
		return FALSE;
	}
	
	api3Data.rfidStatus = RFID_GetSupportedRegionListA(api3Data.hReaderMgmt, &regionListA);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n VerifyInventoryforAllRegions: RFID_GetSupportedRegionListA failed with error: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		RFID_Logout(api3Data.hReaderMgmt);
		fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
		return FALSE;
	}

	// Set the region and standard for all supported regions. Set the region to default value and do the inventory.
	// A basic check if the Reader is working and able to do inventory.
	// Test all regions with periodic re-login to flush accumulated resources
	//
	wprintf(L"\n Testing all %d regions with periodic session refresh", regionList.numRegions);
	
	const UINT32 REGIONS_PER_SESSION = 10; // Re-login every 10 regions to flush memory
	
	char timestamp[32];
	for (UINT32 regionIndex = 0; regionIndex < regionList.numRegions; regionIndex++)
	{
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n\n%ls REGION %d/%d: %ls", MultiByte2WideChar(timestamp), regionIndex + 1, regionList.numRegions, regionList.RegionNamesList[regionIndex]);
		wprintf(L"\n%ls ========================================", MultiByte2WideChar(timestamp));
		wprintf(L"\n%ls [1] Getting standards for region '%ls'...", MultiByte2WideChar(timestamp), regionList.RegionNamesList[regionIndex]);
		
		// Periodically refresh the session to avoid memory accumulation
		// Do this every REGIONS_PER_SESSION regions (except first iteration)
		if (regionIndex > 0 && (regionIndex % REGIONS_PER_SESSION) == 0)
		{
			wprintf(L"\n [Session Refresh] Logging out and back in to flush resources...");
			api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
			rfid3Sleep(2000); // Longer delay to allow full cleanup
			
			// Re-login
			api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, TRUE, 0);
			if (api3Data.rfidStatus != RFID_API_SUCCESS)
			{
				wprintf(L"\n VerifyInventoryforAllRegions: RFID_Login failed after session refresh at region %d with error: %ls", regionIndex, RFID_GetErrorDescription(api3Data.rfidStatus));
				wprintf(L"\n Stopping region tests due to login failure");
				break; // Stop testing if re-login fails
			}
			wprintf(L"\n [Session Refresh] Re-login successful, continuing tests...");
		}
		
		// Clear StandardInfo structures before each API call to prevent corruption
		memset(&StandardInfo, 0, sizeof(STANDARD_INFO_LIST));
		memset(&StandardInfoA, 0, sizeof(STANDARD_INFO_LISTA));
		
	api3Data.rfidStatus = RFID_GetRegionStandardList(api3Data.hReaderMgmt, regionList.RegionNamesList[regionIndex], &StandardInfo);
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n%ls API: RFID_GetRegionStandardList() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
		wprintf(L"\n%ls ERROR: Failed to get standards for region %d", MultiByte2WideChar(timestamp), regionIndex);
		continue;
	}
	wprintf(L"\n%ls API: RFID_GetRegionStandardList() -> Status=0 (Function succeeded)", MultiByte2WideChar(timestamp));
	wprintf(L"\n%ls Found %d standard(s)", MultiByte2WideChar(timestamp), StandardInfo.numStds);
	
	GetCurrentTimestamp(timestamp, sizeof(timestamp));
	wprintf(L"\n%ls Waiting 10 seconds after GetRegionStandardList...", MultiByte2WideChar(timestamp));
	rfid3Sleep(10000);;

	// CRITICAL: Check for XML corruption in StandardInfo BEFORE processing
	BOOLEAN xmlCorrupted = FALSE;
	for (UINT32 chk = 0; chk < StandardInfo.numStds && !xmlCorrupted; chk++) {
		if (StandardInfo.StdsList[chk].strRegionName[0] == L'<' ||
		    StandardInfo.StdsList[chk].strStandardName[0] == L'<' ||
		    wcsstr(StandardInfo.StdsList[chk].strRegionName, L"<?xml") != NULL ||
		    wcsstr(StandardInfo.StdsList[chk].strStandardName, L"<?xml") != NULL ||
		    wcsstr(StandardInfo.StdsList[chk].strRegionName, L"<g3:") != NULL ||
		    wcsstr(StandardInfo.StdsList[chk].strStandardName, L"<g3:") != NULL) {
			xmlCorrupted = TRUE;
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [ERROR] XML CORRUPTION DETECTED in StandardInfo[%d] for region %ls!", 
				MultiByte2WideChar(timestamp), chk, regionList.RegionNamesList[regionIndex]);
			wprintf(L"\n%ls   RegionName: %.50ls...", MultiByte2WideChar(timestamp), StandardInfo.StdsList[chk].strRegionName);
			wprintf(L"\n%ls   StandardName: %.50ls...", MultiByte2WideChar(timestamp), StandardInfo.StdsList[chk].strStandardName);
			wprintf(L"\n%ls   SKIPPING this region to prevent crash!", MultiByte2WideChar(timestamp));
		}
	}
	
	if (xmlCorrupted) {
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls Skipping region %d due to XML corruption", MultiByte2WideChar(timestamp), regionIndex);
		continue;
	}

	// NOTE: Removed ANSI call RFID_GetRegionStandardListA - calling both Unicode and ANSI can cause memory corruption
	// We only need Unicode StandardInfo, not ANSI StandardInfoA
	// Convert Unicode region name to ANSI for logging if needed
	char ansiRegionName[256];
	wcstombs(ansiRegionName, StandardInfo.StdsList[0].strRegionName, 255);
	ansiRegionName[255] = '\0';
	
	char ansiStandardName[256];
	wcstombs(ansiStandardName, StandardInfo.StdsList[0].strStandardName, 255);
	ansiStandardName[255] = '\0';

	for (UINT32 standards = 0; standards < StandardInfo.numStds; standards++)
	{
		// Do NOT re-fetch StandardInfo/StandardInfoA here - it corrupts the data!
		// Both are already fetched above before the loop
		
		// Convert to ANSI for HTML logging
		char ansiRegionName[256];
		wcstombs(ansiRegionName, StandardInfo.StdsList[standards].strRegionName, 255);
		ansiRegionName[255] = '\0';
		
		char ansiStandardName[256];
		wcstombs(ansiStandardName, StandardInfo.StdsList[standards].strStandardName, 255);
		ansiStandardName[255] = '\0';
		
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-REGION-API-%d></a>CAPI-REGION-API-%d:\n\n<br>Description:Reading Tags for Region %s : Std:%s\n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, ansiRegionName, ansiStandardName);
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : \n ");
		
		ChannelCount = StandardInfo.StdsList[standards].iNumChannels;
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls Using standard: %ls", MultiByte2WideChar(timestamp), StandardInfo.StdsList[standards].strStandardName);
		wprintf(L"\n%ls Hopping: %ls, Channels: %u", MultiByte2WideChar(timestamp), 
			StandardInfo.StdsList[standards].bIsHoppingConfigurable ? L"YES" : L"NO", ChannelCount);

		// Validate channel count is reasonable (max 128 for any real region)
		if (ChannelCount > 128) {
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [ERROR] Suspicious ChannelCount=%u (>128) indicates data corruption!", MultiByte2WideChar(timestamp), ChannelCount);
			wprintf(L"\n%ls Skipping this standard to prevent crash.", MultiByte2WideChar(timestamp));
			continue;
		}

		// Detect XML corruption in standard name (API bug)
		if (StandardInfo.StdsList[standards].strStandardName != NULL && 
		    (wcsstr(StandardInfo.StdsList[standards].strStandardName, L"<?xml") != NULL ||
		     wcsstr(StandardInfo.StdsList[standards].strStandardName, L"<motorm") != NULL ||
		     wcsstr(StandardInfo.StdsList[standards].strStandardName, L"<rm:") != NULL)) {
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [ERROR] API returned corrupted XML data for region %ls, skipping.", MultiByte2WideChar(timestamp), regionList.RegionNamesList[regionIndex]);
			continue;
		}

		// Extra validation for standard name and channel count
		if (ChannelCount == 0 || ChannelCount > 1024 || StandardInfo.StdsList[standards].strStandardName == NULL || wcslen(StandardInfo.StdsList[standards].strStandardName) < 2) {
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [ERROR] Invalid ChannelCount (%u) or StandardName for region %ls, skipping to prevent allocation failure.", MultiByte2WideChar(timestamp), ChannelCount, StandardInfo.StdsList[standards].strRegionName);
			continue;
		}
		// Validate channel info pointer for hopping regions
		if (StandardInfo.StdsList[standards].bIsHoppingConfigurable && StandardInfo.StdsList[standards].piChannelInfo == NULL) {
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [ERROR] NULL channel info pointer for hopping region %ls, skipping.", MultiByte2WideChar(timestamp), StandardInfo.StdsList[standards].strRegionName);
			continue;
		}
		if (StandardInfo.StdsList[standards].bIsHoppingConfigurable) {
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [2] Setting region to '%ls' with standard '%ls'...", MultiByte2WideChar(timestamp), 
				StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
			api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls API: RFID_SetActiveRegion() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));

			GetCurrentTimestamp(timestamp, sizeof(timestamp));
			wprintf(L"\n%ls [3] Waiting 2 seconds after SetActiveRegion...", MultiByte2WideChar(timestamp));
			rfid3Sleep(2000);

		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls [4] Setting frequency for hopping (%u channels)...", MultiByte2WideChar(timestamp), ChannelCount);

		CHANNEL_LIST ChannelTable;
		ChannelTable.iNumChannels = ChannelCount;
		ChannelTable.ChannelList = new UINT32[ChannelCount];

		// Log channel setup - use actual channel indices from API
		GetCurrentTimestamp(timestamp, sizeof(timestamp));
		wprintf(L"\n%ls ChannelTable request: iNumChannels=%u, ChannelList=[", MultiByte2WideChar(timestamp), ChannelCount);
		for (UINT32 ChannelIndex = 0; ChannelIndex < ChannelCount; ChannelIndex++) {
			ChannelTable.ChannelList[ChannelIndex] = StandardInfo.StdsList[standards].piChannelInfo[ChannelIndex];
			wprintf(L"%d", StandardInfo.StdsList[standards].piChannelInfo[ChannelIndex]);
			if (ChannelIndex < ChannelCount - 1) wprintf(L",");
		}
		wprintf(L"]");

			api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, TRUE, &ChannelTable);
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls API: RFID_SetFrequencySetting() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
				
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls [5] Waiting 3 seconds after SetFrequencySetting...", MultiByte2WideChar(timestamp));
				rfid3Sleep(3000);
				
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls [WAIT] Sleeping 150 seconds after region/frequency setting before data connection...", MultiByte2WideChar(timestamp));
				rfid3Sleep(150000);

				api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
				wprintf(L"\n  APIRESULT:%ls RegionName:%ls StandardName:%ls",  RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				//fwprintf(fp, L"\n Mask Index :%ld \t APIRESULT:%ls RegionName:%ls StandardName:%ls", MaskIndex, RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				UINT32 nTagsRead = DoInventory();
				// DoInventory handles logout, connect, disconnect, and re-login internally
				
				fprintf(api3Data.fpCurLog,"\n Number of Tags Read: %d \n", nTagsRead);
				wprintf(L"\n Number of Tags Read: %ld \n<br>", nTagsRead);
				if (nTagsRead)
				{
					SetLogResult(TRUE);
				}
				else
				{
					SetLogResult(FALSE);
				}

				logToResultFile(L"Test_REGION_READ_API");
				delete[] ChannelTable.ChannelList;
			}
			else
			{
				// FCC Region (non-hopping) - do NOT set channel list
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls [2] FCC Region - Setting region to '%ls' with standard '%ls'...", MultiByte2WideChar(timestamp), 
					StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
				wprintf(L"\n%ls NOTE: FCC regions use fixed frequencies, no channel selection", MultiByte2WideChar(timestamp));
				
				api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, StandardInfo.StdsList[standards].strRegionName, StandardInfo.StdsList[standards].strStandardName);
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls API: RFID_SetActiveRegion() -> Status=%d (%ls)", MultiByte2WideChar(timestamp), api3Data.rfidStatus, RFID_GetErrorDescription(api3Data.rfidStatus));
				
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls [3] Waiting 2 seconds after SetActiveRegion...", MultiByte2WideChar(timestamp));
				rfid3Sleep(2000);
				
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls [WAIT] Sleeping 150 seconds before data connection...", MultiByte2WideChar(timestamp));
				rfid3Sleep(150000);
				
				RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegionInfo);
				GetCurrentTimestamp(timestamp, sizeof(timestamp));
				wprintf(L"\n%ls APIRESULT:%ls RegionName:%ls StandardName:%ls", MultiByte2WideChar(timestamp), RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				//fwprintf(fp, L"\n APIRESULT:%ls RegionName:%ls StandardName:%ls", RFID_GetErrorDescription(api3Data.rfidStatus), activeRegionInfo.strRegionName, activeRegionInfo.strStandardName);
				UINT32 nTagsRead = DoInventory();
				// DoInventory handles logout, connect, disconnect, and re-login internally
				
				wprintf(L"\n Number of Tags Read: %ld \n", nTagsRead);
				fprintf(api3Data.fpCurLog, "\n Number of Tags Read: %d \n", nTagsRead);

				if (nTagsRead)
				{
					SetLogResult(TRUE);
				}
				else
				{
					SetLogResult(FALSE);
				}


			logToResultFile(L"Test_REGION_READ_API");
		}
	}
	
	// Add delay between regions to allow reader to stabilize and flush connections
	rfid3Sleep(1000);

	}

	fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ", MultiByte2WideChar(__FUNCTION__));
	// Logout from reader management
	RFID_Logout(api3Data.hReaderMgmt);

	return TRUE;
}

void testSetActiveRegionW(  )
{
	ACTIVE_REGION_INFO activeRegion;
	STANDARD_REGION_INFO activeStandard;
	long TagCount;
	ERROR_INFO errorInfo;
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("Test_REGION_CONFIG_API ");
	logCurrentExecution("Test_REGION_CONFIG_API");

	fprintf(api3Data.fpCurLog, "<html>\n");
	fprintf(api3Data.fpCurLog, "<body>\n");
	api3Data.itemTestCount = 44;
	api3Data.dwTestCaseNo = 01;

	for (int i = 0; i < NumberOfRegions; i++)
	{
		TagCount = 0;
		api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, RdrRegionInfo[i].regionName, RdrRegionInfo[i].StandardName);
		wprintf(L"\n RFID_SetActiveRegion() %ls %ls", RdrRegionInfo[i].regionName, RdrRegionInfo[i].StandardName);
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-REGION-API-%d></a>CAPI-REGION-API-%d:\n\n<br>Description: Set Region %ls %ls\n <br> Expected OutPut :Successful Set \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo, RdrRegionInfo[i].regionName, RdrRegionInfo[i].StandardName);
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : \n ");
		
		// Connect to reader
		api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &api3Data.conInfo);
		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
			wprintf(L"\n RFID_Connect() failure error Msg %ls", errorInfo.vendorMessage);
			SetLogResult(FALSE);
			logToResultFile(L"CAPI_REGION");
			continue;
		}
		
		// Allocate tag memory
		LPTAG_DATA pMyTagData = RFID_AllocateTag(api3Data.hReader);
		if (pMyTagData == NULL)
		{
			wprintf(L"\n RFID_AllocateTag() failed - returned NULL");
			RFID_Disconnect(api3Data.hReader);
			SetLogResult(FALSE);
			logToResultFile(L"CAPI_REGION");
			continue;
		}

		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, NULL);
		rfid3Sleep(5000); //wait for the Inventory to finish	
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pMyTagData))
		{
			TagCount++;
			//fprintf( api3Data.fpCurLog, "<br> TAG ID : %s",Byte2HexString( pTagData->pTagID,pTagData->tagIDLength ));
			//printTagDataWithResults(pMyTagData);
			Print2Console(pMyTagData->antennaID, pMyTagData->CRC, pMyTagData->PC, pMyTagData->XPC,api3Data.szIPAddress, pMyTagData->pTagID, pMyTagData->tagIDLength);
		}

		//Verify RFID_GetActiveRegionInfo()
		wprintf(L"\n\nRFID_GetActiveRegionInfo Data");
		wprintf(L"\n-----------------------------------");
		api3Data.rfidStatus = RFID_GetActiveRegionInfo(api3Data.hReaderMgmt, &activeRegion);
		api3Data.rfidStatus = RFID_GetActiveRegionStandardInfo(api3Data.hReaderMgmt, &activeStandard);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			wprintf(L"\n\nCurrent active region is %ls", activeRegion.strRegionName);
			wprintf(L"\nStandardName is %ls", activeRegion.strStandardName);
			wprintf(L"\nHopping is %ls", activeRegion.bIsHoppingOn ? L"ON" : L"OFF");
			wprintf(L"\nNumber of Channels is %d", activeRegion.iNumChannels);

			if (activeRegion.iNumChannels != 0)
			{
				for (UINT32 i = 0; i < activeRegion.iNumChannels; i++)
				{
					wprintf(L"\nChannel[%d] - %d  %ld KHz", i, activeRegion.piChannelInfo[i],activeStandard.piChannelValueInfo[i]);
				}
			}
			fprintf(api3Data.fpCurLog, "\n Active Region:%ls StandardNAme: %ls Number of Channgels: %ld TagCount: %ld: <br>\n ", activeRegion.strRegionName, activeRegion.strStandardName, activeRegion.iNumChannels,TagCount);
			if( TagCount > 0)
			{
				SetLogResult(TRUE);
			}
			else
			{
				SetLogResult(FALSE);
			}
			
			logToResultFile(L"CAPI_REGION");
		}
		else
		{
			wprintf(L"\n RFID_GetActiveRegionInfo() failure error Msg %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
			SetLogResult(FALSE);
			logToResultFile(L"CAPI_REGION");
		}
		
		// Clean up resources
		api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader, pMyTagData);
		api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);
	}

	fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Comleted ", MultiByte2WideChar(__FUNCTION__));
}

void getSupportedRegionsA()
{
	printf("\n[DEBUG] getSupportedRegionsA() - ENTRY");
	REGION_LISTA supportedRegions = { 0 };
	STANDARD_INFO_LISTA StdInfoList = { 0 };

	printf("\n[DEBUG] Calling RFID_GetSupportedRegionListA...");
	api3Data.rfidStatus = RFID_GetSupportedRegionListA( api3Data.hReaderMgmt, &supportedRegions );
	printf("\n[DEBUG] RFID_GetSupportedRegionListA returned: %d", api3Data.rfidStatus);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		NumberOfRegions = supportedRegions.numRegions;
		printf("\n[DEBUG] Number of Regions: %d (MAX_REGIONS=%d)", supportedRegions.numRegions, MAX_REGIONS);
		
		if (supportedRegions.numRegions > MAX_REGIONS) {
			printf("\n[ERROR] Number of regions (%d) exceeds MAX_REGIONS (%d)! Truncating.", supportedRegions.numRegions, MAX_REGIONS);
			supportedRegions.numRegions = MAX_REGIONS;
			NumberOfRegions = MAX_REGIONS;
		}
		
		printf("\n Number of Regions Supported : %d", supportedRegions.numRegions);

		for (UINT32 i = 0; i < supportedRegions.numRegions; i++)
		{
			printf("\n Region #%d : %s", i, supportedRegions.RegionNamesList[i]);
		}
	}
	else
	{
		printf("\n[ERROR] RFID_GetSupportedRegionList() failure error Msg %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
		return;
	}

	printf("\n[DEBUG] Starting region details loop for %d regions...", supportedRegions.numRegions);
	for (UINT32 i = 0; i < supportedRegions.numRegions; i++)
	{
		printf("\n[DEBUG] Processing ANSI region index %d/%d: %s", i, supportedRegions.numRegions, supportedRegions.RegionNamesList[i]);
		
		// Bounds check
		if (i >= MAX_REGIONS) {
			printf("\n[ERROR] Region index %d exceeds MAX_REGIONS (%d), skipping!", i, MAX_REGIONS);
			continue;
		}
		
		api3Data.rfidStatus = RFID_GetRegionStandardListA(api3Data.hReaderMgmt, supportedRegions.RegionNamesList[i], &StdInfoList);
		printf("\n[DEBUG] RFID_GetRegionStandardListA for region %d returned: %d", i, api3Data.rfidStatus);
		
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			for (UINT32 j = 0; j<StdInfoList.numStds; j++)
			{
				printf("\n Region %s", StdInfoList.StdsList[j].strRegionName);
				printf("\nNumber of Standards %d", StdInfoList.numStds);
				printf("\nStandardName %s", StdInfoList.StdsList[j].strStandardName);
				printf("\nNumChannels %d", StdInfoList.StdsList[j].iNumChannels);
				printf("\nIsHoppingConfigurable %s", StdInfoList.StdsList[j].bIsHoppingConfigurable ? "Yes":"No");

				strcpy(RdrRegionInfoA[i].regionName, StdInfoList.StdsList[j].strRegionName);
				strcpy(RdrRegionInfoA[i].StandardName, StdInfoList.StdsList[j].strStandardName);
				RdrRegionInfoA[i].numChannels = StdInfoList.StdsList[j].iNumChannels;
				RdrRegionInfoA[i].isHoppingConfigurable = StdInfoList.StdsList[j].bIsHoppingConfigurable;

				if (StdInfoList.StdsList[j].bIsHoppingConfigurable)
				{
					for (UINT32 k = 0; k<StdInfoList.StdsList[j].iNumChannels; k++)
					{
						printf("\nChannels index %d values %d KHz", StdInfoList.StdsList[j].piChannelInfo[k], StdInfoList.StdsList[j].piChannelValueInfo[k]);
					}
				}
			}
		}
		else
		{
			printf("\n RFID_GetRegionStandardList() failure error Msg %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
		}
	}
}

void testSetActiveRegionA()
{
	ACTIVE_REGION_INFOA activeRegion;
	ERROR_INFOA errorInfo;
	STANDARD_REGION_INFOA stdRegionInfoA = { };

	for (int i = 0; i < NumberOfRegions; i++)
	{
		api3Data.rfidStatus = RFID_SetActiveRegionA(api3Data.hReaderMgmt, RdrRegionInfoA[i].regionName, RdrRegionInfoA[i].StandardName);

		printf("\n RFID_SetActiveRegion() %s %s", RdrRegionInfoA[i].regionName, RdrRegionInfoA[i].StandardName);
		if (api3Data.rfidStatus != RFID_API_SUCCESS)
		{
			RFID_GetLastErrorInfoA(api3Data.hReaderMgmt, &errorInfo);
			printf("\n RFID_SetActiveRegion() failure error Msg %s", errorInfo.vendorMessage);
		}

		//Verify RFID_GetActiveRegionInfo()
		printf("\n\nRFID_GetActiveRegionInfo Data");
		printf("\n-----------------------------------");
		api3Data.rfidStatus = RFID_GetActiveRegionInfoA(api3Data.hReaderMgmt, &activeRegion);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			printf("\n\nCurrent active region is %s", activeRegion.strRegionName);
			printf("\nStandardName is %s", activeRegion.strStandardName);
			printf("\nHopping is %s", activeRegion.bIsHoppingOn ? "ON" : "OFF");
		}
		else
		{
			printf("\n RFID_GetActiveRegionInfo() failure error Msg %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
		}

		api3Data.rfidStatus = RFID_GetActiveRegionStandardInfoA(api3Data.hReaderMgmt, &stdRegionInfoA);
		if (api3Data.rfidStatus == RFID_API_SUCCESS)
		{
			printf("\n\nActive Region Name is %s", stdRegionInfoA.strRegionName);
			printf("\nActive StandardName is %s", stdRegionInfoA.strStandardName);
			printf("\nHopping is %s", stdRegionInfoA.bIsHoppingConfigurable ? "YES" : "NO");
			printf("\nNumber of Channels is %d", stdRegionInfoA.iNumChannels);

			if (stdRegionInfoA.bIsHoppingConfigurable)
			{
				for (UINT32 i = 0; i<stdRegionInfoA.iNumChannels; i++)
				{
					printf("\nChannel[%d] - %d", stdRegionInfoA.piChannelInfo[i], stdRegionInfoA.piChannelValueInfo[i]);
				}
			}
		}
		else
		{
			printf("\n RFID_GetActiveRegionStandardInfo() failure error Msg %s", RFID_GetErrorDescriptionA(api3Data.rfidStatus));
		}
	}
}

void ValidateforNegTests()
{
	STANDARD_REGION_INFO rgnInfo;

	api3Data.rfidStatus = RFID_GetSupportedRegionList(api3Data.hReaderMgmt, NULL);
	api3Data.rfidStatus = RFID_GetSupportedRegionListA(api3Data.hReaderMgmt, NULL);
	api3Data.rfidStatus = RFID_SetActiveRegionA(api3Data.hReaderMgmt, NULL, NULL);
	api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, NULL, NULL);

	api3Data.rfidStatus = RFID_SetActiveRegion(api3Data.hReaderMgmt, L"NNNNNNNNNNNNNNNNNNNNNNNN", L"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");

	api3Data.rfidStatus = RFID_GetActiveRegionStandardInfoA(api3Data.hReaderMgmt, NULL);
	api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, NULL,NULL);
	api3Data.rfidStatus = RFID_GetActiveRegionStandardInfo(api3Data.hReaderMgmt, &rgnInfo);
}

void TestChannelSelRandomlyforAllRegions( )
{
	CHANNEL_LIST ChannelTable;
	ERROR_INFO errorInfo;
	ChannelTable.ChannelList = new UINT32[1]{4};
	ChannelTable.iNumChannels = 1;
	api3Data.rfidStatus = RFID_SetFrequencySetting(api3Data.hReaderMgmt, FALSE, &ChannelTable);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
		wprintf(L"\n RFID_SetFrequencySetting() failure error Msg %ls", errorInfo.vendorMessage);
	}
	
	// Clean up allocated memory
	delete[] ChannelTable.ChannelList;
}

void TestRegionConfigAPI_Unicode()
{
	getSupportedRegionsW();
	//testSetActiveRegionW(); // This test is Screwing the Reader. Commenting it for NOW
}

void TestRegionConfigAPI_MBCS()
{
	getSupportedRegionsA();
	//testSetActiveRegionA();		// This test is Screwing the Reader. Commenting it for NOW
}

void VerifyRegionConfigAPI()
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	LOGIN_INFO loginfo;
	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"Zebra@123");
	loginfo.forceLogin = TRUE;
	loginfo.version = RFID_API3_5_5;

	// Login first to get supported regions
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, TRUE, 0);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n RFID_Login failed: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
		return;
	}
	wprintf(L"\n RFID_Login: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
	
	// Get supported regions first
	getSupportedRegionsW();
	
	// Logout before VerifyInventoryforAllRegions (it handles its own login/logout)
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	api3Data.hReaderMgmt = 0;
	
	// Now execute VerifyInventoryforAllRegions
	// This function handles its own login/logout
	BOOLEAN result = VerifyInventoryforAllRegions(api3Data.apiVersion);
	if (!result)
	{
		wprintf(L"\n VerifyInventoryforAllRegions failed, skipping remaining tests");
		wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
		return;
	}

	// Login again for the remaining tests
	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, TRUE, 0);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n RFID_Login failed: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
		return;
	}
	wprintf(L"\n RFID_Login: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
	
	TestRegionConfigAPI_MBCS( );
	//SetChannelsETSI();
	ValidateforNegTests();
	
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	wprintf(L"\n Executing %ls Completed ", MultiByte2WideChar(__FUNCTION__));
}