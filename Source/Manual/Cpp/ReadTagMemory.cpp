#include "stdafx.h"

static RFID_HANDLE32 hReader = 0;
static FILE* fpLog = NULL;

void PrintTagData(LPTAG_DATA pTagData)
{
    if (!pTagData) return;

    // Print EPC
    wprintf(L"\n=== Tag Found ===\n");
    wprintf(L"EPC (ID): ");
    for (UINT32 i = 0; i < pTagData->tagIDLength; i++) {
        wprintf(L"%02X ", pTagData->pTagID[i]);
    }
    wprintf(L"\nPC: %04X, CRC: %04X\n", pTagData->PC, pTagData->CRC);
    wprintf(L"RSSI: %d dBm, Count: %u\n", pTagData->peakRSSI, pTagData->tagSeenCount);

    // Print memory bank data if available
    if (pTagData->memoryBankDataLength > 0) {
        if (pTagData->memoryBank == MEMORY_BANK_TID) {
            wprintf(L"TID: ");
        } else if (pTagData->memoryBank == MEMORY_BANK_USER) {
            wprintf(L"USER: ");
        } else {
            wprintf(L"Bank %d: ", pTagData->memoryBank);
        }
        
        for (UINT32 i = 0; i < pTagData->memoryBankDataLength; i++) {
            wprintf(L"%02X ", pTagData->pMemoryBankData[i]);
        }
        wprintf(L"\n");
    }
}

BOOLEAN ReadMemoryBank(LPTAG_DATA pTagData, MEMORY_BANK bank, UINT32 offset, UINT32 length)
{
    READ_ACCESS_PARAMS rParams;
    rParams.memoryBank = bank;
    rParams.byteOffset = offset;
    rParams.byteCount = length;
    rParams.accessPassword = 0;

    RFID_STATUS status = RFID_Read(hReader, pTagData->pTagID, pTagData->tagIDLength, 
                                     &rParams, NULL, NULL, pTagData, 0);
    
    if (status == RFID_API_SUCCESS) {
        return TRUE;
    } else {
        wprintf(L"  Error reading %ls: %ls\n", 
                bank == MEMORY_BANK_TID ? L"TID" : L"User",
                RFID_GetErrorDescription(status));
        return FALSE;
    }
}

int main(int argc, char* argv[])
{
    RFID_STATUS status;
    wchar_t readerName[256] = {0};
    
    // Open log file
    fpLog = fopen("ReadTagMemory.log", "w");
    if (!fpLog) {
        wprintf(L"Failed to open log file\n");
        return -1;
    }

    fprintf(fpLog, "=== Read Tag Memory Test ===\n");
    
    // Get reader name from command line or use default
    if (argc > 1) {
        mbstowcs(readerName, argv[1], 255);
    } else {
        wcscpy(readerName, L"169.254.10.1");
    }

    wprintf(L"\n=== Read Tag Memory Test ===\n");
    wprintf(L"Reader: %ls\n\n", readerName);

    // Connect to reader (port 5084 is default RFID reader port)
    wprintf(L"Connecting to reader...\n");
    status = RFID_Connect(&hReader, readerName, 5084, 0, NULL);
    if (status != RFID_API_SUCCESS) {
        wprintf(L"Failed to connect: %ls\n", RFID_GetErrorDescription(status));
        fprintf(fpLog, "Failed to connect: error code %d\n", status);
        fclose(fpLog);
        return -1;
    }
    wprintf(L"Connected successfully\n\n");
    fprintf(fpLog, "Connected successfully\n");

    // Allocate tag data structure
    LPTAG_DATA pTagData = RFID_AllocateTag(hReader);
    if (!pTagData) {
        wprintf(L"Failed to allocate tag structure\n");
        RFID_Disconnect(hReader);
        fclose(fpLog);
        return -1;
    }

    // Perform inventory
    wprintf(L"Starting inventory...\n");
    fprintf(fpLog, "Starting inventory...\n");
    
    status = RFID_PerformInventory(hReader, NULL, NULL, NULL, 0);
    if (status != RFID_API_SUCCESS) {
        wprintf(L"Inventory failed: %ls\n", RFID_GetErrorDescription(status));
        fprintf(fpLog, "Inventory failed: error %d\n", status);
    } else {
        wprintf(L"Inventory started\n\n");
        fprintf(fpLog, "Inventory started\n");
    }

    // Wait for tags
    sleep(3);

    // Stop inventory
    status = RFID_StopInventory(hReader);
    wprintf(L"Inventory stopped\n\n");

    // Read tags
    UINT32 tagCount = 0;
    wprintf(L"Reading tags...\n");
    fprintf(fpLog, "\nReading tags...\n");

    while (RFID_GetReadTag(hReader, pTagData) == RFID_API_SUCCESS) {
        tagCount++;
        
        // Print basic tag info
        PrintTagData(pTagData);
        
        fprintf(fpLog, "\n=== Tag %d ===\n", tagCount);
        fprintf(fpLog, "EPC: ");
        for (UINT32 i = 0; i < pTagData->tagIDLength; i++) {
            fprintf(fpLog, "%02X ", pTagData->pTagID[i]);
        }
        fprintf(fpLog, "\n");

        // Read TID
        wprintf(L"\nReading TID...\n");
        fprintf(fpLog, "Reading TID...\n");
        if (ReadMemoryBank(pTagData, MEMORY_BANK_TID, 0, 12)) {
            PrintTagData(pTagData);
            fprintf(fpLog, "TID: ");
            for (UINT32 i = 0; i < pTagData->memoryBankDataLength; i++) {
                fprintf(fpLog, "%02X ", pTagData->pMemoryBankData[i]);
            }
            fprintf(fpLog, "\n");
        }

        // Read User Memory
        wprintf(L"\nReading User Memory...\n");
        fprintf(fpLog, "Reading User Memory...\n");
        if (ReadMemoryBank(pTagData, MEMORY_BANK_USER, 0, 32)) {
            PrintTagData(pTagData);
            fprintf(fpLog, "User: ");
            for (UINT32 i = 0; i < pTagData->memoryBankDataLength; i++) {
                fprintf(fpLog, "%02X ", pTagData->pMemoryBankData[i]);
            }
            fprintf(fpLog, "\n");
        }

        wprintf(L"\n");
    }

    wprintf(L"\nTotal tags read: %d\n", tagCount);
    fprintf(fpLog, "\nTotal tags read: %d\n", tagCount);

    // Cleanup
    if (pTagData) {
        RFID_DeallocateTag(hReader, pTagData);
    }

    if (hReader) {
        RFID_Disconnect(hReader);
        wprintf(L"Disconnected from reader\n");
        fprintf(fpLog, "Disconnected from reader\n");
    }

    if (fpLog) {
        fclose(fpLog);
    }

    wprintf(L"\nPress Enter to exit...\n");
    getchar();

    return 0;
}
