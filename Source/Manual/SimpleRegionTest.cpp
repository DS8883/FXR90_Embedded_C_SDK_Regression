#include <stdio.h>
#include <wchar.h>
#include <unistd.h>
#define UNICODE
#define _UNICODE
#include "rfidapi.h"

// Simple test to set region and run inventory
int main(int argc, char* argv[])
{
    RFID_HANDLE32 hReaderMgmt = 0;
    RFID_HANDLE32 hReader = 0;
    RFID_STATUS status;
    
    // Configuration
    const wchar_t* readerIP = L"169.254.10.1";
    const wchar_t* username = L"admin";
    const wchar_t* password = L"Zebra@123";
    
    wprintf(L"\n=== Simple Region Test ===\n");
    wprintf(L"Reader IP: %ls\n", readerIP);
    
    // Step 1: Login to reader management
    wprintf(L"\n[1] Logging in to reader...\n");
    LOGIN_INFO loginfo = {0};
    wcscpy(loginfo.hostName, readerIP);
    wcscpy(loginfo.userName, username);
    wcscpy(loginfo.password, password);
    loginfo.forceLogin = TRUE;
    loginfo.version = RFID_API3_5_5;
    
    status = RFID_Login(&hReaderMgmt, &loginfo, FX, TRUE, 0);
    if (status != RFID_API_SUCCESS)
    {
        wprintf(L"ERROR: Login failed: %ls\n", RFID_GetErrorDescription(status));
        return 1;
    }
    wprintf(L"SUCCESS: Logged in\n");
    
    // Step 2: Get supported regions
    wprintf(L"\n[2] Getting supported regions...\n");
    REGION_LIST regionList = {0};
    status = RFID_GetSupportedRegionList(hReaderMgmt, &regionList);
    if (status != RFID_API_SUCCESS)
    {
        wprintf(L"ERROR: GetSupportedRegionList failed: %ls\n", RFID_GetErrorDescription(status));
        RFID_Logout(hReaderMgmt);
        return 1;
    }
    wprintf(L"SUCCESS: Found %d regions\n", regionList.numRegions);
    
    // Print ALL supported regions
    wprintf(L"\n=== ALL SUPPORTED REGIONS (%d total) ===\n", regionList.numRegions);
    for (UINT32 i = 0; i < regionList.numRegions; i++)
    {
        wprintf(L"%3d. %ls\n", i+1, regionList.RegionNamesList[i]);
    }
    wprintf(L"=====================================\n");
    
    // Test ALL regions
    int totalSuccess = 0;
    int totalFailed = 0;
    
    for (UINT32 regionIdx = 0; regionIdx < regionList.numRegions; regionIdx++)
    {
        wprintf(L"\n\n========== TESTING REGION %d/%d: %ls ==========\n", 
                regionIdx+1, regionList.numRegions, regionList.RegionNamesList[regionIdx]);
        
        // Get standards for this region
        STANDARD_INFO_LIST stdList = {0};
        status = RFID_GetRegionStandardList(hReaderMgmt, regionList.RegionNamesList[regionIdx], &stdList);
        if (status != RFID_API_SUCCESS || stdList.numStds == 0)
        {
            wprintf(L"ERROR: Could not get standards for region '%ls'\n", regionList.RegionNamesList[regionIdx]);
            totalFailed++;
            continue;
        }
        
        wprintf(L"Region has %d standard(s)\n", stdList.numStds);
        
        // Set the region with first standard
        wprintf(L"[3] Setting region to '%ls' with standard '%ls'...\n", 
                stdList.StdsList[0].strRegionName, stdList.StdsList[0].strStandardName);
        
        status = RFID_SetActiveRegion(hReaderMgmt, stdList.StdsList[0].strRegionName, stdList.StdsList[0].strStandardName);
        if (status != RFID_API_SUCCESS)
        {
            wprintf(L"ERROR: SetActiveRegion failed: %ls\n", RFID_GetErrorDescription(status));
            totalFailed++;
            continue;
        }
        wprintf(L"SUCCESS: Region set\n");
        
        // Wait for reader to stabilize
        wprintf(L"[4] Waiting 5 seconds for reader to stabilize...\n");
        sleep(5);
        
        // Verify active region
        ACTIVE_REGION_INFO activeRegionInfo = {0};
        status = RFID_GetActiveRegionInfo(hReaderMgmt, &activeRegionInfo);
        if (status == RFID_API_SUCCESS)
        {
            wprintf(L"Active Region: %ls, Standard: %ls\n", 
                    activeRegionInfo.strRegionName, 
                    activeRegionInfo.strStandardName);
        }
        
        // KEEP management connection open, create separate data connection for inventory
        // This is how the old working code does it!
        wprintf(L"[5] Connecting to reader for inventory (keeping management connection open)...\n");
        wprintf(L"DEBUG: hReaderMgmt=%d, attempting RFID_Connect on port 5084...\n", hReaderMgmt);
        
        CONNECTION_INFO conInfo = { RFID_API3_5_5, 0 };
        
        status = RFID_Connect(&hReader, (WCHAR*)readerIP, 5084, 0, &conInfo);
        wprintf(L"DEBUG: RFID_Connect returned status=%d, hReader=%d\n", status, hReader);
        
        if (status != RFID_API_SUCCESS)
        {
            wprintf(L"ERROR: RFID_Connect failed: %ls\n", RFID_GetErrorDescription(status));
            
            // Try getting more error details
            ERROR_INFO errorInfo = {0};
            if (hReaderMgmt != 0)
            {
                RFID_STATUS errStatus = RFID_GetLastErrorInfo(hReaderMgmt, &errorInfo);
                if (errStatus == RFID_API_SUCCESS && wcslen(errorInfo.vendorMessage) > 0)
                {
                    wprintf(L"DEBUG: Vendor error message: %ls\n", errorInfo.vendorMessage);
                }
            }
            
            totalFailed++;
            continue;
        }
        wprintf(L"SUCCESS: Connected to reader\n");
        
        // Allocate tag memory
        LPTAG_DATA pTag = RFID_AllocateTag(hReader);
        if (pTag == NULL)
        {
            wprintf(L"ERROR: RFID_AllocateTag returned NULL\n");
            RFID_Disconnect(hReader);
            totalFailed++;
            continue;
        }
        
        // Perform inventory
        wprintf(L"[6] Performing inventory...\n");
        status = RFID_PurgeTags(hReader, 0);
        status = RFID_PerformInventory(hReader, NULL, NULL, NULL, NULL);
        if (status != RFID_API_SUCCESS)
        {
            wprintf(L"ERROR: PerformInventory failed: %ls\n", RFID_GetErrorDescription(status));
            RFID_DeallocateTag(hReader, pTag);
            RFID_Disconnect(hReader);
            totalFailed++;
            continue;
        }
        
        wprintf(L"Inventory started, waiting 5 seconds...\n");
        sleep(5);
        
        status = RFID_StopInventory(hReader);
        
        // Read tags
        int tagCount = 0;
        while ((status = RFID_GetReadTag(hReader, pTag)) == RFID_API_SUCCESS)
        {
            tagCount++;
        }
        
        wprintf(L"\n*** REGION %d RESULT: Read %d tags ***\n", regionIdx+1, tagCount);
        
        // Cleanup data connection (keep management connection open)
        RFID_DeallocateTag(hReader, pTag);
        RFID_Disconnect(hReader);
        hReader = 0;
        
        totalSuccess++;
        
        // Extra cleanup every 10 regions
        if ((regionIdx + 1) % 10 == 0)
        {
            wprintf(L"\n--- Checkpoint after %d regions ---\n", regionIdx+1);
            sleep(2);
        }
    }
    
    // Final cleanup
    wprintf(L"\n[9] Final cleanup...\n");
    if (hReaderMgmt != 0)
    {
        RFID_Logout(hReaderMgmt);
    }
    
    wprintf(L"\n=== ALL REGIONS TEST COMPLETE ===\n");
    wprintf(L"Successfully tested: %d/%d regions\n", totalSuccess, regionList.numRegions);
    wprintf(L"Failed: %d regions\n", totalFailed);
    wprintf(L"==================================\n");
    
    return (totalFailed == 0) ? 0 : 1;
}
