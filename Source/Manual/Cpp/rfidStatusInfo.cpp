



#include "ManualTests.h"
extern struct RFID3DATA api3Data;

void TestErrorInfo( )
{
	UINT16 errorArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 200, 201,202, 300, 301, 400, 401, 402, 403, 404, 405,
		406, 407, 420, 421, 422, 423, 500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523,
		524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 601, 602, 603, 604, 605, 700, 701, 702 };


	for (int i = 0; i < (int)(sizeof(errorArray) / sizeof(errorArray[0])); i++)
	{
		wprintf(L"\n\n Error Information for ErrorCode %ld", errorArray[i]);
		api3Data.rfidStatus = (RFID_STATUS)errorArray[i];
		if (RFID_GetErrorDescription((RFID_STATUS)errorArray[i]) != NULL)
		{
			wprintf(L"\n Valid Error Descripton");
		}

		wprintf(L"\n Error Description  for ERRORCODE : %d \t %ls \n ", errorArray[i], RFID_GetErrorDescription(api3Data.rfidStatus));
	}
}

void TestErrorInfoA( )
{
	UINT16 errorArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 200, 201, 300, 301, 400, 401, 402, 403, 404, 405,
		406, 407, 420, 421, 422, 423, 500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523,
		524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 601, 602, 603, 604, 605, 700, 701, 702 };

	ACCESS_OPERATION_STATUS accessOperationStatus[] = { (ACCESS_OPERATION_STATUS)0, (ACCESS_OPERATION_STATUS)1, (ACCESS_OPERATION_STATUS)2, (ACCESS_OPERATION_STATUS)3, (ACCESS_OPERATION_STATUS)4, (ACCESS_OPERATION_STATUS)5, (ACCESS_OPERATION_STATUS)6, (ACCESS_OPERATION_STATUS)7 };



	for (int i = 0; i < (int)(sizeof(errorArray) / sizeof(errorArray[0])); i++)
	{
		api3Data.rfidStatus = (RFID_STATUS)errorArray[i];
		if (RFID_GetErrorDescriptionA((RFID_STATUS)errorArray[i]) != NULL)
		{
			wprintf(L"\n Valid Error Descripton");
		}

		printf("\n Error Description  for ERRORCODE : %d \t %s \n ", errorArray[i], RFID_GetErrorDescriptionA(api3Data.rfidStatus));
	}
}

void TestRFID4ErrorInformationText(void)
{
	CONNECTION_INFO ConInfo = { RFID_API3_5_5, 0 };
	api3Data.rfidStatus = RFID_Connect(&api3Data.hReader, api3Data.szIPAddress, api3Data.dwPort, 0, &ConInfo);
	
	// WideChar Version
	TestErrorInfo( );

	// ANSI Version
	TestErrorInfoA( );
}