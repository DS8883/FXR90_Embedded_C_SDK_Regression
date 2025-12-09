#include "Common.h"
#include "APITest.h"
#include <stdio.h>

extern struct RFID3DATA api3Data;	// RFID3 Data

//RFID_STATUS WINAPI RFID_Authenticate(	RFID_HANDLE32 readerHandle,	UINT8* pTagID,	UINT32 tagIDLength,	LPAUTHENTICATE_ACCESS_PARAMS lpAuthenticateAccessParams,LPACCESS_FILTER lpAccessFilter,
//										LPANTENNA_INFO lpAntennaInfo,	LPTAG_DATA lpTagData,	LPVOID rsvd);
//
//RFID_STATUS WINAPI RFID_ReadBuffer(	RFID_HANDLE32 readerHandle,	UINT8* pTagID,	UINT32 tagIDLength,	LPREADBUFFER_ACCESS_PARAMS lpReadBufferAccessParams,	LPACCESS_FILTER lpAccessFilter,
//										LPANTENNA_INFO lpAntennaInfo,	LPTAG_DATA lpTagData,	LPVOID rsvd);
//
//RFID_STATUS WINAPI RFID_Untraceable( 	RFID_HANDLE32 readerHandle,	UINT8* pTagID,	UINT32 tagIDLength,	LPUNTRACEABLE_ACCESS_PARAMS lpUntraceableAccessParams,	LPACCESS_FILTER lpAccessFilter,
//										LPANTENNA_INFO lpAntennaInfo,	LPTAG_DATA lpTagData,	LPVOID rsvd);
//
//RFID_STATUS WINAPI RFID_Crypto(RFID_HANDLE32 readerHandle,UINT8* pTagID,UINT32 tagIDLength,LPCRYPTO_ACCESS_PARAMS lpCryptoAccessParams,LPACCESS_FILTER lpAccessFilter,LPANTENNA_INFO lpAntennaInfo,
//								LPTAG_DATA lpTagData,LPVOID rsvd);
//
//	TAM1 :
//	Key0	: 3333ABCD3333ABCD3333ABCD3333ABCD
//	Message : 000096564402375796C69664(96 bits)
//	Response: 878CB76C5A59B622AEBD74AA291F0AE9  
//	Decrypt : 96C5A913BDB196564402375796C69664
//
//	Key1	: ABCD3333ABCD3333ABCD3333ABCD3333
//	Message : 000196564402375796C69664(96 bits)
//	Response: 6B59906FD5010EB3B0BE404A63D323A4
//	Decrypt : 96C5596E4B4B96564402375796C69664
//
//	TAM2 :
//
//	Key1	: ABCD3333ABCD3333ABCD3333ABCD3333
//	Message : 2001FD5D8048F48DD09AAD22000111(120 bits)
//	Response: 07976DED0EFCAF8EB5ADE539D81DAFBC88D9B562AC14C8ADB05F79B896C2C27D
//	Decrypt : 96C5A1945635 FD5D8048F48DD09AAD2207976DEF11F793ABB5ADE539D81DAFBC
//
// DADACAFE TAG should be present in the Regression TagBoard for G2V2 to Work
//
void TestG2V2Authenticate(RFID_VERSION rfidVersion)
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));

	wchar_t resultBuffer[4096] = { 0, };				// Result Buffer for Buffer ReadBack from the API
	UINT8 Pattern1[2] = { 0xDA,0xDA };					// DADACAFE TAG
	UINT8 Pattern2[2] = { 0xFE,0xFE };					// MASK
	UINT8 tagMask[2] = { 0xFF,0xFF };					// MASK
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{ 0 } };	// TPA
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{ 0 } };	// TPB
	POST_FILTER postFilter = { {0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };		// POST FILTER.

	// TAM1 and TAM2 Msg
	//
	static UINT8 tam1Msg[12] = { 0x00, 0x00, 0x96, 0x56, 0x44, 0x02, 0x37, 0x57, 0x96, 0xC6, 0x96, 0x64 };
	static UINT8 tam2Msg[15] = { 0x20, 0x01, 0xFD, 0x5D, 0x80, 0x48, 0xF4, 0x8D, 0xD0, 0x9A, 0xAD, 0x22, 0x00, 0x01, 0x11 };

	AUTHENTICATE_ACCESS_PARAMS authenticateParams = { TRUE,TRUE,0,96,tam1Msg,0 };

	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, NULL);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	// Authenticate with TAM1 Message Format.
	//
	if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(api3Data.pG2V2Tag);
	}
	else
	{
		wprintf(L"\n No DADACAFE(G2V2)  Tag found in the FOV.. SKipping the test exectuion \n");
		return;
	}

	// Print the returned response from the API
	//
	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Authenticate with TAM1 Message \n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo );
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : \n ");
	api3Data.rfidStatus = RFID_Authenticate(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &authenticateParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
	convertTagDataToResults(api3Data.pG2V2Tag, resultBuffer, FALSE);

	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	logToResultFile(L"CAPI-G2V2");

	// Authenticate with TAM2 Message format.
	//
	authenticateParams.length = 120;
	authenticateParams.pMessage = tam2Msg;

	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Authenticate with TAM2 Message \n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo );
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : \n ");
	// Print the returned response from the API
	//
	api3Data.rfidStatus = RFID_Authenticate(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &authenticateParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
	convertTagDataToResults(api3Data.pG2V2Tag, resultBuffer, FALSE);
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		SetLogResult(TRUE);
	}
	else
	{
		SetLogResult(FALSE);
	}

	logToResultFile(L"CAPI-G2V2");

	// Authenticate with Opseq.
	// Print the returned response from the API
	//
	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Authenticate with TAM2 Message with OPSeq\n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : \n ");

	api3Data.rfidStatus = RFID_InitializeAccessSequence(api3Data.hReader);

	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_AUTHENTICATE,&authenticateParams,{0} };
	UINT32 opIndex;

	api3Data.rfidStatus = RFID_AddOperationToAccessSequence(api3Data.hReader, &opParams, &opIndex);
	api3Data.rfidStatus = RFID_PerformAccessSequence(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopAccessSequence(api3Data.hReader);
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence(api3Data.hReader);
	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag))
	{
		//printTagDataWithResults(api3Data.pG2V2Tag);
		if (api3Data.pG2V2Tag->opStatus == ACCESS_SUCCESS)
		{
			convertTagDataToResults(api3Data.pG2V2Tag, resultBuffer, FALSE);
			SetLogResult(TRUE);
			break;
		}
	}

	logToResultFile(L"CAPI-G2V2");

	// Authenticate with TAM2 Message Format.
	//
	wprintf(L"\n Executing %ls Complated ", MultiByte2WideChar(__FUNCTION__));
}

// Executes on DADACAFE tag in regression
//
void TestG2V2HideShowMemBank(RFID_VERSION rfidVersion)
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	UNTRACEABLE_ACCESS_PARAMS untracebleParams = { 1,1,0,TID_HIDE_NONE,0,RANGE_NORMAL,0x00000001,0 };

	UINT8 Pattern1[2] = { 0xDA,0xDA };
	UINT8 Pattern2[2] = { 0xFE,0xFE };
	UINT8 tagMask[] = { 0xFF,0xFF };
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,Pattern1,16,tagMask,16,{ 0 } };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,Pattern2,16,tagMask,16,{ 0 } };
	POST_FILTER postFilter = { {0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL};
	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_TID,0,0,0x00000001,0 };

	// Test Untraceble feature for EPC
	// MSB (show/hide): Always show or hide works on the MSB side only and not on the LSB side.
	// 0: show memory 	above EPC
	// 1111222233334444
	// 1 : hide memory above EPC
	// 5 LSBs(length) : New EPC length field(new L bits)
	//
	for (int epclen = 2; epclen <= 6; epclen++)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(api3Data.pG2V2Tag);
		}

		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Untraceble Command with EPC MemBank: Hide Memory \n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : Validate the Result Manually \n ");
		untracebleParams.hideEPC= 1;
		untracebleParams.assertU = 0;
		untracebleParams.EPCLength = epclen;
		untracebleParams.accessPassword = 0x00000001;
		
		api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &untracebleParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (api3Data.pG2V2Tag->opCode == ACCESS_OPERATION_UNTRACEABLE) && (api3Data.pG2V2Tag->opStatus == ACCESS_SUCCESS))
		{
			wprintf(L"\n RFID_Untraceable Success");
			SetLogResult(TRUE);
		}
		else
		{
			SetLogResult(FALSE);
		}

		logToResultFile(L"CAPI-G2V2");
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, NULL);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
		{
			rParams.memoryBank = MEMORY_BANK_EPC;
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &rParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
			wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
			if (api3Data.pG2V2Tag->tagIDLength == epclen * 2)
			{
				wprintf(L"Test untraceble command for epclen %ld Success\n", epclen);
			}

			printTagDataWithResults(api3Data.pG2V2Tag);
		}
	}

	// Hide Memory above EPC
	//
	for (int epclen = 2; epclen <= 6; epclen++)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(api3Data.pG2V2Tag);
		}
		untracebleParams.EPCLength = epclen;
		untracebleParams.hideEPC= 0;
		untracebleParams.assertU = 0;
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Untraceble Command with EPC MemBank: Show Memory \n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo );
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : Validate the Result Manually \n ");

		api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &untracebleParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (api3Data.pG2V2Tag->opCode == ACCESS_OPERATION_UNTRACEABLE) && (api3Data.pG2V2Tag->opStatus == ACCESS_SUCCESS))
		{
			wprintf(L"\n RFID_Untraceable Success");
		}
		else
		{
			wprintf(L"\n RFID_Untraceable FAIL");
		}

		logToResultFile(L"CAPI-G2V2");

		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
		{
			rParams.memoryBank = MEMORY_BANK_EPC;
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, api3Data.pG2V2Tag->pTagID,12, &rParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
			wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
			if (api3Data.rfidStatus != RFID_API_SUCCESS ) // The read should fail, typically memory overrun
			{
				SetLogResult(TRUE);
			}
			else
			{
				SetLogResult(FALSE);
			}

			printTagDataWithResults(api3Data.pG2V2Tag);
		}
	}

	// 00 : hide none
	// 01 : hide some
	// 10 : hide all
	// 11 : RFU
	//
	untracebleParams.hideEPC= 0;
	untracebleParams.EPCLength = 6;
	untracebleParams.assertU = 0;
	rParams.memoryBank = MEMORY_BANK_TID;
	rParams.byteCount = 12;

	for (UINT8 hideOption  = 0; hideOption < 3; hideOption++)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(5000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
		{
			printTagDataWithResults(api3Data.pG2V2Tag);
		}
		untracebleParams.hideTID = (TID_HIDE_STATE)hideOption;
				
		fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Untraceble Command with TID MemBank: HIDE/SHOW TID Memory \n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo );
		fprintf(api3Data.fpCurLog, "<br>");
		fprintf(api3Data.fpCurLog, "\n Result is : Validate the Result Manually \n ");

		api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &untracebleParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
		if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (api3Data.pG2V2Tag->opCode == ACCESS_OPERATION_UNTRACEABLE) && (api3Data.pG2V2Tag->opStatus == ACCESS_SUCCESS))
		{
			wprintf(L"\n RFID_Untraceable Success");
		}
		else
		{
			wprintf(L"\n RFID_Untraceable Fail");
		}

		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
		rfid3Sleep(3000);
		api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
		if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
		{
			api3Data.rfidStatus = RFID_Read(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &rParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
			wprintf(L"Test Read command for epclen %ls Success\n", RFID_GetErrorDescription(api3Data.rfidStatus));
			printTagDataWithResults(api3Data.pG2V2Tag);
			// Check if memory over run error is coming for Reader
			//
			switch (untracebleParams.hideTID)
			{
			case 0: // View the MemBank.
				if (api3Data.pG2V2Tag->opStatus == ACCESS_SUCCESS) // The read should succeed
				{
					SetLogResult(TRUE);
				}
				else
				{
					SetLogResult(FALSE);
				}
				wprintf(L"Test Read command for epclen %ls \n", RFID_GetErrorDescription(api3Data.rfidStatus));
				printTagDataWithResults(api3Data.pG2V2Tag);
				break;
			case 1:
				if (api3Data.pG2V2Tag->opStatus != ACCESS_SUCCESS) // Hide Some, the first two words are hidden
				{
					SetLogResult(TRUE);
				}
				else
				{
					SetLogResult(FALSE);
				}
				wprintf(L"Test Read command for epclen %ls \n", RFID_GetErrorDescription(api3Data.rfidStatus));
				printTagDataWithResults(api3Data.pG2V2Tag);
				break;
			case 2:
				if (api3Data.pG2V2Tag->opStatus != ACCESS_SUCCESS) // Hide All, Entire memoryBank is hidden
				{
					SetLogResult(TRUE);
				}
				else
				{
					SetLogResult(FALSE);
				}
				wprintf(L"Test Read command for epclen %ls \n", RFID_GetErrorDescription(api3Data.rfidStatus));
				printTagDataWithResults(api3Data.pG2V2Tag);
				break;
			default:
				break;
			}

			logToResultFile(L"CAPI-G2V2");
		}
	}

	// USER MEMORY VIEW or HIDE featture.
	// 0 : VIEW
	// 1 : HIDE 
	untracebleParams.hideEPC= 0;
	untracebleParams.hideTID = TID_HIDE_NONE;
	untracebleParams.hideUser = 1;
	untracebleParams.EPCLength = 6;

	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
	{
		printTagDataWithResults(api3Data.pG2V2Tag);
	}

	rParams.memoryBank = MEMORY_BANK_USER;
	rParams.byteCount = 12;
	fprintf(api3Data.fpCurLog, "\n\n<a name=CAPI-G2V2-API-%d></a>CAPI-G2V2-API-%d:\n\n<br>Description: Untraceble Command with USER MemBank: HIDE/SHOW TID Memory \n <br> Expected OutPut : API SUCCESS \r\n", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo );
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result is : Validate the Result Manually \n ");
	api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &untracebleParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
	if ((api3Data.rfidStatus == RFID_API_SUCCESS) && (api3Data.pG2V2Tag->opCode == ACCESS_OPERATION_UNTRACEABLE) && (api3Data.pG2V2Tag->opStatus == ACCESS_SUCCESS))
	{
		wprintf(L"\n RFID_Untraceable Success");
	}
	
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, &postFilter, NULL, NULL, NULL);
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	if ((api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, api3Data.pG2V2Tag)) == RFID_API_SUCCESS)
	{
		api3Data.rfidStatus = RFID_Read(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &rParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
		switch (untracebleParams.hideUser)
		{
		case 0: // View the MemBank.
			if (api3Data.rfidStatus == RFID_API_SUCCESS) // The read should fail, typically memory overrun
			{
				SetLogResult(TRUE);
			}
			else
			{
				SetLogResult(FALSE);
			}
			wprintf(L"Test Read command for epclen %ls \n", RFID_GetErrorDescription(api3Data.rfidStatus));
			printTagDataWithResults(api3Data.pG2V2Tag);
			break;
		case 1:
			if (api3Data.rfidStatus != RFID_API_SUCCESS) // The read should fail, typically memory overrun
			{
				SetLogResult(TRUE);
			}
			else
			{
				SetLogResult(FALSE);
			}
			wprintf(L"Test Read command for epclen %ls \n", RFID_GetErrorDescription(api3Data.rfidStatus));
			printTagDataWithResults(api3Data.pG2V2Tag);
			break;
		default:
			break;
		}
		
	}
	
	logToResultFile(L"CAPI-G2V2");
	
	wprintf(L"\n Executing %ls Complated ", MultiByte2WideChar(__FUNCTION__));

}

void ShowtheMemBankG2V2Tag()
{
	// 0 to Show the memory
	// 1 to hide the memory
	// Initialize op sequence
	//
	LPTAG_DATA pg2v2Tag = RFID_AllocateTag(api3Data.hReader);
	api3Data.rfidStatus = RFID_InitializeAccessSequence(api3Data.hReader);
	UNTRACEABLE_ACCESS_PARAMS untraceParams = { 0,0,0,TID_HIDE_NONE,0,RANGE_NORMAL,0x00000001,0 };

	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_UNTRACEABLE,&untraceParams,{0} };
	UINT32 opIndex;

	api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, NULL, 0, &untraceParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence(api3Data.hReader, &opParams, &opIndex);
	api3Data.rfidStatus = RFID_PerformAccessSequence(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopAccessSequence(api3Data.hReader);
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence(api3Data.hReader);
	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pg2v2Tag))
	{
		printTagDataWithResults(pg2v2Tag);
	}

	if (pg2v2Tag != NULL) RFID_DeallocateTag(api3Data.hReader, pg2v2Tag);
}

// NOTE: The functionalities from “RFID_Authenticate()” and “RFID_Crypto()” are totally same.
// “RFID_Crypto()” with “CustomData = FALSE” is equivalent to  “RFID_Authenticate()” in TAM1 mode.
// “RFID_Crypto()” with “CustomData = TRUE” is equivalent to  “RFID_Authenticate()” in TAM2 mode.
//
void TestGen2V2Crypto(RFID_VERSION rfidVersion)
{
	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	wchar_t resultBuffer[4096] = { 0, };				// Result Buffer for Buffer ReadBack from the API
	CRYPTO_ACCESS_PARAMS CryptoWithCustomData = { 1,{ 0x96564402, 0x2375796C, 0x96640000 },TRUE,1,0,1,1,0x00000001,NULL };
	CRYPTO_ACCESS_PARAMS CryptoWithNoCustomData{ 0,{ 0x96564402, 0x2375796C, 0x96640000 } ,FALSE,0,0,0,0,0x00000001,NULL };
	
	// Test with Custom Data
	api3Data.rfidStatus = RFID_Crypto( api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &CryptoWithCustomData, NULL, NULL, api3Data.pG2V2Tag, NULL);
	convertTagDataToResults(api3Data.pG2V2Tag, resultBuffer, FALSE);

	UINT32 accessSuccessCount;
	UINT32 accessFailureCount;
	api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader, &accessSuccessCount, &accessFailureCount);

	// print tag with result
	printTagDataWithResults( api3Data.pG2V2Tag);

	// Test with without Custom Data
	api3Data.rfidStatus = RFID_Crypto(api3Data.hReader, api3Data.pG2V2Tag->pTagID, api3Data.pG2V2Tag->tagIDLength, &CryptoWithNoCustomData, NULL, NULL, api3Data.pG2V2Tag, NULL);

	convertTagDataToResults(api3Data.pG2V2Tag, resultBuffer, FALSE);
	accessSuccessCount = accessFailureCount = 0;
	api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader, &accessSuccessCount, &accessFailureCount);

	// print tag with result
	printTagDataWithResults(api3Data.pG2V2Tag);

	// Test Crypto with AccessSequences
	/*api3Data.rfidStatus = RFID_InitializeAccessSequence(api3Data.hReader);
	UNTRACEABLE_ACCESS_PARAMS untraceParams = { 0,0,0,0,0,0x00000001,0 };

	OP_CODE_PARAMS opParams = { ACCESS_OPERATION_UNTRACEABLE,&untraceParams,{0} };
	UINT32 opIndex;

	api3Data.rfidStatus = RFID_Untraceable(api3Data.hReader, NULL, 0, &untraceParams, NULL, NULL, api3Data.pG2V2Tag, NULL);
	rfid3Sleep(3000);
	api3Data.rfidStatus = RFID_AddOperationToAccessSequence(api3Data.hReader, &opParams, &opIndex);
	api3Data.rfidStatus = RFID_PerformAccessSequence(api3Data.hReader, NULL, NULL, NULL, 0);
	rfid3Sleep(5000);
	api3Data.rfidStatus = RFID_StopAccessSequence(api3Data.hReader);
	api3Data.rfidStatus = RFID_DeleteOperationFromAccessSequence(api3Data.hReader, 0);
	api3Data.rfidStatus = RFID_DeinitializeAccessSequence(api3Data.hReader);
	while (RFID_API_SUCCESS == RFID_GetReadTag(api3Data.hReader, pg2v2Tag))
	{
		printTagDataWithResults(pg2v2Tag);
	}*/

	fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
	wprintf(L"\n Executing %ls Completed", MultiByte2WideChar(__FUNCTION__));
}

// G2V2 Features Testing: Authenticate and Read Buffer, Crypto.
//
void TestG2V2Features( )
{
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	// Check if G2V2 is supported in the Current Release, else simply return.
	//
	if (!api3Data.rCaps.G2V2CommandSupported)
	{
		wprintf(L"\n G2V2 Not Supported");
		return;
	}

	wprintf(L"\n Executing %ls Started ", MultiByte2WideChar(__FUNCTION__));
	SetLogText("TestG2V2Features ");
	logCurrentExecution("TestG2V2Features");

	fprintf(api3Data.fpCurLog, "<html>\n");
	fprintf(api3Data.fpCurLog, "<body>\n");

	
	api3Data.rfidStatus = RFID_ResetConfigToFactoryDefaults(api3Data.hReader);

	// RFID_COMM_NO_CONNECTION due to RM API execution
	//
	if (api3Data.rfidStatus == RFID_COMM_NO_CONNECTION)
	{
		api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0,&ConInfo);
		api3Data.pG2V2Tag = RFID_AllocateTag(api3Data.hReader);
	}

	// Execute unhide the memory bank, Execute opseq
	//
	ShowtheMemBankG2V2Tag();

	TestG2V2Authenticate(api3Data.apiVersion);
	TestG2V2HideShowMemBank(api3Data.apiVersion);
	TestGen2V2Crypto(api3Data.apiVersion);

	api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader,api3Data.pG2V2Tag);

	fprintf(api3Data.fpCurLog, "</html>\n<br></body>\n<br>");
	LogPassFailCount();
	wprintf(L"\n Executing %ls Completed", MultiByte2WideChar(__FUNCTION__));
}
