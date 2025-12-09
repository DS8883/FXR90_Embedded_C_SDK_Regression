#include <stdio.h>
#include <wchar.h>
#include <unistd.h>
#define UNICODE
#define _UNICODE
#include "rfidapi.h"

// Test if management and data connections can coexist
int main(int argc, char* argv[])
{
    RFID_HANDLE32 hReaderMgmt = 0;
    RFID_HANDLE32 hReader = 0;
    RFID_STATUS status;
    
    const wchar_t* readerIP = L"169.254.10.1";
    const wchar_t* username = L"admin";
    const wchar_t* password = L"Zebra@123";
    
    wprintf(L"\n=== Test Dual Connection ===\n");
    
    // Step 1: Login (management connection)
    wprintf(L"\n[1] Opening management connection via RFID_Login...\n");
    LOGIN_INFO loginfo = {0};
    wcscpy(loginfo.hostName, readerIP);
    wcscpy(loginfo.userName, username);
    wcscpy(loginfo.password, password);
    loginfo.forceLogin = TRUE;
    loginfo.version = RFID_API3_5_5;
    
    status = RFID_Login(&hReaderMgmt, &loginfo, FX, TRUE, 0);
    if (status != RFID_API_SUCCESS)
    {
        wprintf(L"ERROR: RFID_Login failed: %ls\n", RFID_GetErrorDescription(status));
        return 1;
    }
    wprintf(L"SUCCESS: Management connection open, hReaderMgmt=%d\n", hReaderMgmt);
    
    // Step 2: Try to open data connection while management is still open
    wprintf(L"\n[2] Opening data connection via RFID_Connect (while management is still open)...\n");
    CONNECTION_INFO conInfo = { RFID_API3_5_5, 0 };
    
    status = RFID_Connect(&hReader, (WCHAR*)readerIP, 5084, 0, &conInfo);
    wprintf(L"DEBUG: RFID_Connect returned status=%d (%ls), hReader=%d\n", 
            status, RFID_GetErrorDescription(status), hReader);
    
    if (status != RFID_API_SUCCESS)
    {
        wprintf(L"RESULT: Cannot open data connection while management connection is open!\n");
        
        // Try getting error details
        ERROR_INFO errorInfo = {0};
        RFID_STATUS errStatus = RFID_GetLastErrorInfo(hReaderMgmt, &errorInfo);
        if (errStatus == RFID_API_SUCCESS && wcslen(errorInfo.vendorMessage) > 0)
        {
            wprintf(L"Error details: %ls\n", errorInfo.vendorMessage);
        }
        
        RFID_Logout(hReaderMgmt);
        return 1;
    }
    
    wprintf(L"SUCCESS: Data connection open, hReader=%d\n", hReader);
    wprintf(L"RESULT: Both connections can coexist!\n");
    
    // Cleanup
    wprintf(L"\n[3] Closing both connections...\n");
    RFID_Disconnect(hReader);
    RFID_Logout(hReaderMgmt);
    
    wprintf(L"\n=== Test Complete ===\n");
    return 0;
}
