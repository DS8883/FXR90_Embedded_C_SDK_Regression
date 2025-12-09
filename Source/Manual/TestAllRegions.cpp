#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#define UNICODE
#define _UNICODE
#include "rfidapi.h"

// Helper to convert wide string to regular string for printing
void wstr_to_str(const wchar_t* wstr, char* str, size_t max_len) {
    wcstombs(str, wstr, max_len);
    str[max_len-1] = '\0';
}

// Helper function to print timestamp
void printTimestamp() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    printf("[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
    fflush(stdout);
}

// Helper function to log API calls with result
void logAPICall(const wchar_t* apiName, RFID_STATUS status, RFID_HANDLE32 handle = 0) {
    char apiNameStr[256];
    char errDescStr[512];
    wstr_to_str(apiName, apiNameStr, 256);
    wstr_to_str(RFID_GetErrorDescription(status), errDescStr, 512);
    
    printTimestamp();
    if (handle != 0) {
        printf("API: %s() -> Status=%d (%s), Handle=%d\n", 
                apiNameStr, status, errDescStr, handle);
    } else {
        printf("API: %s() -> Status=%d (%s)\n", 
                apiNameStr, status, errDescStr);
    }
    fflush(stdout);
}

// Test all 76 regions with inventory - isolated clean test with detailed logging
int main(int argc, char* argv[])
{
    // Set locale for wide character output
    setlocale(LC_ALL, "en_US.UTF-8");
    
    RFID_HANDLE32 hReaderMgmt = 0;
    RFID_HANDLE32 hReader = 0;
    RFID_STATUS status;
    
    const wchar_t* readerIP = L"169.254.10.1";
    const wchar_t* username = L"admin";
    const wchar_t* password = L"Zebra@123";
    
    int successCount = 0;
    int failCount = 0;
    
    printTimestamp();
    printf("\n=== Test All 76 Regions with Inventory (VERBOSE MODE) ===\n");
    printTimestamp();
    printf("Reader: 169.254.10.1\n");
    printTimestamp();
    printf("Username: admin\n");
    printTimestamp();
    printf("API Version: RFID_API3_5_5\n");
    printTimestamp();
    printf("Secure Mode: TRUE\n\n");
    fflush(stdout);
    
    // Step 1: Initial login to get region list
    printTimestamp();
    printf("\n[STEP 1] Logging in to get region list...\n");
    printTimestamp();
    printf("Creating LOGIN_INFO structure...\n");
    fflush(stdout);
    LOGIN_INFO loginfo = {0};
    wcscpy(loginfo.hostName, readerIP);
    wcscpy(loginfo.userName, username);
    wcscpy(loginfo.password, password);
    loginfo.forceLogin = TRUE;
    loginfo.version = RFID_API3_5_5;
    
    printTimestamp();
    printf("Calling RFID_Login(hostName=169.254.10.1, userName=admin, readerType=FX, secureMode=TRUE)...\n");
    fflush(stdout);
    
    status = RFID_Login(&hReaderMgmt, &loginfo, FX, TRUE, 0);
    logAPICall(L"RFID_Login", status, hReaderMgmt);
    
    if (status != RFID_API_SUCCESS)
    {
        char errMsg[512];
        wstr_to_str(RFID_GetErrorDescription(status), errMsg, 512);
        printTimestamp();
        printf("ERROR: Initial login failed: %s\n", errMsg);
        return 1;
    }
    printTimestamp();
    printf("SUCCESS: Logged in, hReaderMgmt=%d\n\n", hReaderMgmt);
    fflush(stdout);
    
    // Step 2: Get supported regions
    printTimestamp();
    printf("\n[STEP 2] Getting supported regions...\n");
    fflush(stdout);
    REGION_LIST regionList = {0};
    
    printTimestamp();
    printf("Calling RFID_GetSupportedRegionList(hReaderMgmt=%d)...\n", hReaderMgmt);
    fflush(stdout);
    status = RFID_GetSupportedRegionList(hReaderMgmt, &regionList);
    logAPICall(L"RFID_GetSupportedRegionList", status);
    
    if (status != RFID_API_SUCCESS)
    {
        char errMsg[512];
        wstr_to_str(RFID_GetErrorDescription(status), errMsg, 512);
        printTimestamp();
        printf("ERROR: GetSupportedRegionList failed: %s\n", errMsg);
        RFID_Logout(hReaderMgmt);
        return 1;
    }
    printTimestamp();
    printf("SUCCESS: Found %d regions\n\n", regionList.numRegions);
    fflush(stdout);
    
    // Step 3: Test each region
    for (UINT32 regionIdx = 0; regionIdx < regionList.numRegions; regionIdx++)
    {
        char regionName[256];
        wstr_to_str(regionList.RegionNamesList[regionIdx], regionName, 256);
        
        printTimestamp();
        printf("\n\n========================================\n");
        printTimestamp();
        printf("REGION %d/%d: %s\n", regionIdx+1, regionList.numRegions, regionName);
        printTimestamp();
        printf("========================================\n");
        fflush(stdout);
        
        // Get standards for this region
        printTimestamp();
        printf("[1] Getting standards for region '%s'...\n", regionName);
        fflush(stdout);
        STANDARD_INFO_LIST stdList = {0};
        status = RFID_GetRegionStandardList(hReaderMgmt, regionList.RegionNamesList[regionIdx], &stdList);
        logAPICall(L"RFID_GetRegionStandardList", status);
        
        if (status != RFID_API_SUCCESS || stdList.numStds == 0)
        {
            printTimestamp();
            printf("ERROR: Could not get standards for region\n");
            fflush(stdout);
            failCount++;
            continue;
        }
        
        char stdName[256];
        wstr_to_str(stdList.StdsList[0].strStandardName, stdName, 256);
        printTimestamp();
        printf("Found %d standard(s), using: %s\n", stdList.numStds, stdName);
        printTimestamp();
        printf("Hopping: %s, Channels: %d\n", 
                stdList.StdsList[0].bIsHoppingConfigurable ? "YES" : "NO",
                stdList.StdsList[0].iNumChannels);
        fflush(stdout);
        
        // Set the region (use first standard)
        char regionNameStd[256];
        wstr_to_str(stdList.StdsList[0].strRegionName, regionNameStd, 256);
        printTimestamp();
        printf("[2] Setting region to '%s' with standard '%s'...\n", regionNameStd, stdName);
        fflush(stdout);
        
        status = RFID_SetActiveRegion(hReaderMgmt, stdList.StdsList[0].strRegionName, stdList.StdsList[0].strStandardName);
        logAPICall(L"RFID_SetActiveRegion", status);
        
        if (status != RFID_API_SUCCESS)
        {
            char errMsg[512];
            wstr_to_str(RFID_GetErrorDescription(status), errMsg, 512);
            printTimestamp();
            printf("ERROR: SetActiveRegion failed: %s\n", errMsg);
            fflush(stdout);
            failCount++;
            continue;
        }
        
        // Wait after setting region (matching old code timing)
        printTimestamp();
        printf("[3] Waiting 2 seconds after SetActiveRegion...\n");
        fflush(stdout);
        sleep(2);
        
        // Set frequency if hopping enabled (like old code)
        if (stdList.StdsList[0].bIsHoppingConfigurable && stdList.StdsList[0].iNumChannels > 0)
        {
            printTimestamp();
            printf("[4] Setting frequency for hopping (%d channels)...\n", stdList.StdsList[0].iNumChannels);
            fflush(stdout);
            CHANNEL_LIST channelTable = {0};
            UINT32* channels = new UINT32[stdList.StdsList[0].iNumChannels];
            printTimestamp();
            printf("ChannelTable request: iNumChannels=%d, ChannelList=[", stdList.StdsList[0].iNumChannels);
            for (int ch = 0; ch < stdList.StdsList[0].iNumChannels; ch++) {
                channels[ch] = ch + 1; // 1-based index for channel
                printf("%d%s", channels[ch], (ch < stdList.StdsList[0].iNumChannels-1) ? "," : "]\n");
            }
            channelTable.iNumChannels = stdList.StdsList[0].iNumChannels;
            channelTable.ChannelList = channels;
            fflush(stdout);

            status = RFID_SetFrequencySetting(hReaderMgmt, TRUE, &channelTable);
            logAPICall(L"RFID_SetFrequencySetting", status);

            if (status != RFID_API_SUCCESS) {
                printTimestamp();
                printf("[ERROR] RFID_SetFrequencySetting request payload: iNumChannels=%d, ChannelList=[", channelTable.iNumChannels);
                for (int ch = 0; ch < channelTable.iNumChannels; ch++) {
                    printf("%d%s", channelTable.ChannelList[ch], (ch < channelTable.iNumChannels-1) ? "," : "]\n");
                }
                fflush(stdout);
                ERROR_INFO errorInfo = {0};
                RFID_STATUS errorStatus = RFID_GetLastErrorInfo(hReaderMgmt, &errorInfo);
                if (errorStatus == RFID_API_SUCCESS && errorInfo.vendorMessage[0] != 0) {
                    char vendorMsg[1024];
                    wstr_to_str(errorInfo.vendorMessage, vendorMsg, 1024);
                    printTimestamp();
                    printf("[ERROR] RFID_SetFrequencySetting response payload: %s\n", vendorMsg);
                    fflush(stdout);
                }
            }

            delete[] channels;
            printTimestamp();
            printf("[5] Waiting 3 seconds after SetFrequencySetting...\n");
            fflush(stdout);
            sleep(3);
        }
        else
        {
            printTimestamp();
            printf("[4] Region not hopping-configurable, waiting 3 seconds...\n");
            fflush(stdout);
            sleep(3);
        }
        
        printTimestamp();
        printf("[WAIT] Sleeping 60 seconds after region/frequency setting before data connection...\n");
        fflush(stdout);
        sleep(60);
        // Try WITHOUT logging out (exactly like old code does)
        printTimestamp();
        printf("[6] Opening data connection while management still open (RFID_Connect)...\n");
        printTimestamp();
        printf("Current state: hReaderMgmt=%d (still open)\n", hReaderMgmt);
        printTimestamp();
        printf("Creating CONNECTION_INFO: version=RFID_API3_5_5, lpSecConInfo=NULL\n");
        fflush(stdout);
        CONNECTION_INFO conInfo = { RFID_API3_5_5, 0 };
        conInfo.lpSecConInfo = NULL;
        
        printTimestamp();
        printf("Calling RFID_Connect(IP=169.254.10.1, Port=5084, Timeout=0)...\n");
        fflush(stdout);
        status = RFID_Connect(&hReader, (WCHAR*)readerIP, 5084, 0, &conInfo);
        logAPICall(L"RFID_Connect", status, hReader);
        
        if (status != RFID_API_SUCCESS)
        {
            char errMsg[512];
            wstr_to_str(RFID_GetErrorDescription(status), errMsg, 512);
            printTimestamp();
            printf("ERROR: RFID_Connect failed: %s\n", errMsg);
            printTimestamp();
            printf("NOTE: Connection failed while management connection (hReaderMgmt=%d) is still open!\n", hReaderMgmt);
            fflush(stdout);
            
            // Get detailed error info
            ERROR_INFO errorInfo = {0};
            RFID_STATUS errorStatus = RFID_GetLastErrorInfo(hReaderMgmt, &errorInfo);
            if (errorStatus == RFID_API_SUCCESS && errorInfo.vendorMessage[0] != 0)
            {
                char vendorMsg[1024];
                wstr_to_str(errorInfo.vendorMessage, vendorMsg, 1024);
                printTimestamp();
                printf("Error Details - Vendor Message: %s\n", vendorMsg);
                fflush(stdout);
            }
            
            failCount++;
            continue;
        }
        
        printTimestamp();
        printf("SUCCESS: Connected, hReader=%d (hReaderMgmt=%d still open)\n", hReader, hReaderMgmt);
        fflush(stdout);
        
        // Allocate tag memory
        printTimestamp();
        printf("[7] Allocating tag memory...\n");
        fflush(stdout);
        LPTAG_DATA pTag = RFID_AllocateTag(hReader);
        if (pTag == NULL)
        {
            printTimestamp();
            printf("ERROR: AllocateTag returned NULL\n");
            printTimestamp();
            printf("Calling RFID_Disconnect(hReader=%d)...\n", hReader);
            fflush(stdout);
            RFID_Disconnect(hReader);
            failCount++;
            continue;
        }
        printTimestamp();
        printf("Tag memory allocated successfully\n");
        fflush(stdout);
        
        // Perform inventory
        printTimestamp();
        printf("[8] Purging old tags...\n");
        fflush(stdout);
        status = RFID_PurgeTags(hReader, 0);
        logAPICall(L"RFID_PurgeTags", status);
        
        printTimestamp();
        printf("[9] Starting inventory operation...\n");
        fflush(stdout);
        status = RFID_PerformInventory(hReader, NULL, NULL, NULL, NULL);
        logAPICall(L"RFID_PerformInventory", status);
        
        if (status != RFID_API_SUCCESS)
        {
            char errMsg[512];
            wstr_to_str(RFID_GetErrorDescription(status), errMsg, 512);
            printTimestamp();
            printf("ERROR: PerformInventory failed: %s\n", errMsg);
            fflush(stdout);
        }
        
        printTimestamp();
        printf("[10] Waiting 5 seconds for inventory to complete...\n");
        fflush(stdout);
        sleep(5);
        
        printTimestamp();
        printf("[11] Stopping inventory...\n");
        fflush(stdout);
        status = RFID_StopInventory(hReader);
        logAPICall(L"RFID_StopInventory", status);
        
        // Read tags
        printTimestamp();
        printf("[12] Reading tags from buffer...\n");
        fflush(stdout);
        int tagCount = 0;
        while ((status = RFID_GetReadTag(hReader, pTag)) == RFID_API_SUCCESS)
        {
            tagCount++;
            if (tagCount == 1) {
                printTimestamp();
                printf("First tag EPC: ");
                for (UINT32 i = 0; i < pTag->tagIDLength && i < 12; i++) {
                    printf("%02X", pTag->pTagID[i]);
                }
                printf("\n");
                fflush(stdout);
            }
        }
        
        printTimestamp();
        printf("\n*** RESULT: Read %d tags from region '%s' ***\n", tagCount, regionName);
        fflush(stdout);
        
        // Cleanup data connection (management connection stays open)
        printTimestamp();
        printf("[13] Deallocating tag memory...\n");
        fflush(stdout);
        RFID_DeallocateTag(hReader, pTag);
        
        printTimestamp();
        printf("[14] Disconnecting data connection (hReader=%d)...\n", hReader);
        fflush(stdout);
        status = RFID_Disconnect(hReader);
        logAPICall(L"RFID_Disconnect", status);
        hReader = 0;
        
        printTimestamp();
        printf("Management connection (hReaderMgmt=%d) remains open for next region\n", hReaderMgmt);
        fflush(stdout);
        
        if (tagCount > 0)
            successCount++;
        else
            failCount++;
    }
    
    // Final cleanup
    printTimestamp();
    printf("\n\n[CLEANUP] Logging out from management connection...\n");
    fflush(stdout);
    if (hReaderMgmt != 0)
    {
        status = RFID_Logout(hReaderMgmt);
        logAPICall(L"RFID_Logout", status);
    }
    
    printTimestamp();
    printf("\n\n========================================\n");
    printTimestamp();
    printf("=== TEST COMPLETE ===\n");
    printTimestamp();
    printf("Tested: %d regions\n", regionList.numRegions);
    printTimestamp();
    printf("Success (tags read): %d\n", successCount);
    printTimestamp();
    printf("Failed: %d\n", failCount);
    printTimestamp();
    printf("========================================\n");
    fflush(stdout);
    
    return (failCount == 0) ? 0 : 1;
}
