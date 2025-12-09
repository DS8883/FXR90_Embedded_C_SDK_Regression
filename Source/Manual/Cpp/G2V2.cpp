#include "ManualTests.h"

extern struct RFID3DATA api3Data;

//RFID_STATUS WINAPI RFID_Authenticate(
//	RFID_HANDLE32 readerHandle,
//	UINT8* pTagID,
//	UINT32 tagIDLength,
//	LPAUTHENTICATE_ACCESS_PARAMS lpAuthenticateAccessParams,
//	LPACCESS_FILTER lpAccessFilter,
//	LPANTENNA_INFO lpAntennaInfo,
//	LPTAG_DATA lpTagData,
//	LPVOID rsvd);
//
//RFID_STATUS WINAPI RFID_ReadBuffer(
//	RFID_HANDLE32 readerHandle,
//	UINT8* pTagID,
//	UINT32 tagIDLength,
//	LPREADBUFFER_ACCESS_PARAMS lpReadBufferAccessParams,
//	LPACCESS_FILTER lpAccessFilter,
//	LPANTENNA_INFO lpAntennaInfo,
//	LPTAG_DATA lpTagData,
//	LPVOID rsvd);
//
//RFID_STATUS WINAPI RFID_Untraceable(
//	RFID_HANDLE32 readerHandle,
//	UINT8* pTagID,
//	UINT32 tagIDLength,
//	LPUNTRACEABLE_ACCESS_PARAMS lpUntraceableAccessParams,
//	LPACCESS_FILTER lpAccessFilter,
//	LPANTENNA_INFO lpAntennaInfo,
//	LPTAG_DATA lpTagData,
//	LPVOID rsvd);
//TAM1:
//Key0:	3333ABCD3333ABCD3333ABCD3333ABCD
//	Message : 000096564402375796C69664(96 bits)
//	Response:	878CB76C5A59B622AEBD74AA291F0AE9  
//	Decrypt : 96C5A913BDB196564402375796C69664
//
//	Key1 : ABCD3333ABCD3333ABCD3333ABCD3333
//	Message : 000196564402375796C69664(96 bits)
//	Response:	6B59906FD5010EB3B0BE404A63D323A4
//	Decrypt : 96C5596E4B4B96564402375796C69664
//
//	TAM2 :
//
//	Key1:	ABCD3333ABCD3333ABCD3333ABCD3333
//		Message : 2001FD5D8048F48DD09AAD22000111(120 bits)
//		Response : 07976DED0EFCAF8EB5ADE539D81DAFBC88D9B562AC14C8ADB05F79B896C2C27D
//		Decrypt : 96C5A1945635 FD5D8048F48DD09AAD2207976DEF11F793ABB5ADE539D81DAFBC

void TestG2V2Authenticate(RFID_VERSION rfidVersion)
{
	wchar_t resultBuffer[4096] = { 0, };
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	UINT8 Pattern1[2] = { 0xDA,0xDA };
	UINT8 Pattern2[2] = { 0xFE,0xFE };
	UINT8 tagMask[] = { 0xFF,0xFF };
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{ 0 } };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{ 0 } };
	POST_FILTER postFilter = { {0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	static UINT8 tam1Msg[12] = { 0x00, 0x00, 0x96, 0x56, 0x44, 0x02, 0x37, 0x57, 0x96, 0xC6, 0x96, 0x64 };
	static UINT8 tam2Msg[15] = { 0x20, 0x01, 0xFD, 0x5D, 0x80, 0x48, 0xF4, 0x8D, 0xD0, 0x9A, 0xAD, 0x22, 0x00, 0x01, 0x11 };

	AUTHENTICATE_ACCESS_PARAMS authenticateParams = { TRUE,TRUE,0,96,tam1Msg,0 };

	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);

	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	// Authenticate with TAM1 Message Format.
	//
	if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(pTagData);
	}

	// Print the returned response from the API
	//
	api3Data.rfidStatus = RFID_Authenticate(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &authenticateParams, NULL, NULL, pTagData, NULL);
	convertTagDataToResults(pTagData, resultBuffer,FALSE);


	// Authenticate with TAM2 Message format.
	//
	authenticateParams.length = 120;
	authenticateParams.pMessage = tam2Msg;

	// Print the returned response from the API
	//
	api3Data.rfidStatus = RFID_Authenticate(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &authenticateParams, NULL, NULL, pTagData, NULL);
	convertTagDataToResults(pTagData, resultBuffer, FALSE);
	

	// do access of read buffer
	READBUFFER_ACCESS_PARAMS readBufferParams;
	readBufferParams.wordPtr = 0;
	readBufferParams.bitCount = 0x80;
	readBufferParams.accessPassword = 0;

	api3Data.rfidStatus = RFID_ReadBuffer(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &readBufferParams, NULL, NULL, pTagData, NULL);
	// Authenticate with TAM2 Message Format.
	//
	


}

// obselete for now.
void TestG2V2ReadBuffer(RFID_VERSION rfidVersion)
{

}

// Executes on DADACAFE tag in regression
//
void TestG2V2HideShowMemBank(RFID_VERSION rfidVersion)
{

	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	UNTRACEABLE_ACCESS_PARAMS untracebleParams = {TRUE, TRUE, 0, (TID_HIDE_STATE)0, TRUE, (TAG_OPERATING_RANGE)0, 0x00000000 };
	
	UINT8 Pattern1[2] = { 0xDA,0xDA };
	UINT8 Pattern2[2] = { 0xFE,0xFE };
	UINT8 tagMask[] = { 0xFF,0xFF };
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{ 0 } };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{ 0 } };
	POST_FILTER postFilter = { {0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,0,0x00000000,0 };

	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, 5084, 0, &ConInfo);
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	
	// Test Untraceble feature for EPC
	// MSB (show/hide): Always show or hide works on the MSB side only and not on the LSB side.
	// 0: show memory 	above EPC
	// 1111222233334444
	// 1 : hide memory above EPC
	// 5 LSBs(length) : New EPC length field(new L bits)
	for (int epclen = 0; epclen <= 6; epclen++)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(pTagData);
		}
		untracebleParams.hideEPC = TRUE;
		untracebleParams.assertU = FALSE;
		untracebleParams.EPCLength = epclen;
		api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &untracebleParams, NULL, NULL, pTagData, NULL);
		if ( (api3Data.rfidStatus == RFID_API_SUCCESS) && (pTagData->opCode = ACCESS_OPERATION_UNTRACEABLE) && (pTagData->opStatus == ACCESS_SUCCESS))
		{
			wprintf(L"\n RFID_Untraceable Success");
		}
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{
			rParams.memoryBank = MEMORY_BANK_EPC;
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, NULL);
			wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
			if (pTagData->tagIDLength == epclen * 2)
			{
				wprintf(L"Test untraceble command for epclen %l Success\n", epclen);
			}

			printTagDataWithResults(pTagData);
		}
	}

	for (int epclen = 0; epclen <= 6; epclen++)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(pTagData);
		}
		untracebleParams.EPCLength = epclen;
		untracebleParams.hideEPC = FALSE;
		untracebleParams.assertU = FALSE;
		api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &untracebleParams, NULL, NULL, pTagData, NULL);
		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (pTagData->opCode = ACCESS_OPERATION_UNTRACEABLE) && (pTagData->opStatus == ACCESS_SUCCESS))
		{
			wprintf(L"\n RFID_Untraceable Success");
		}
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{
			rParams.memoryBank = MEMORY_BANK_EPC;
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, NULL);
			wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
			if (pTagData->tagIDLength == epclen * 2)
			{
				wprintf(L"Test untraceble command for epclen %l Success\n", epclen);
			}

			printTagDataWithResults(pTagData);
		}
	}

	//00: hide none
	//01 : hide some
	//10 : hide all
	//11 : RFU
	for (int epclen = 0; epclen <4; epclen++)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(pTagData);
		}

		untracebleParams.hideEPC = FALSE;
		untracebleParams.EPCLength = 6;
		untracebleParams.hideTID = (TID_HIDE_STATE)epclen;
		untracebleParams.assertU = TRUE;
		rParams.memoryBank = MEMORY_BANK_TID;
		api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &untracebleParams, NULL, NULL, pTagData, NULL);
		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (pTagData->opCode = ACCESS_OPERATION_UNTRACEABLE) && (pTagData->opStatus == ACCESS_SUCCESS))
		{
			wprintf(L"\n RFID_Untraceable Success");
		}
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, NULL);
			wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
			printTagDataWithResults(pTagData);
		}
	}
	
	// USER
	// 0: view
	// 1 : hide
	untracebleParams.hideEPC = FALSE;
	untracebleParams.hideTID = (TID_HIDE_STATE)0;
	untracebleParams.hideUser = TRUE;
	untracebleParams.EPCLength = 6;

	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(pTagData);
	}
	
	rParams.memoryBank = MEMORY_BANK_USER;
	api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &untracebleParams, NULL, NULL, pTagData, NULL);
	if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (pTagData->opCode = ACCESS_OPERATION_UNTRACEABLE) && (pTagData->opStatus == ACCESS_SUCCESS))
	{
		wprintf(L"\n RFID_Untraceable Success");
	}
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
	{
		api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, NULL);
		wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
		printTagDataWithResults(pTagData);
	}
	
	api3Data.rfidStatus = RFID_Disconnect(api3Data.hReader);

}