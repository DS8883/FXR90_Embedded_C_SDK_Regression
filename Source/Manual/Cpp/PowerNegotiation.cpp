#include "ManualTests.h"

extern struct RFID3DATA api3Data;


void TestSetPowerNegotiation( )
{
	BOOLEAN bPowerNegSetFlag, bPowerNegGetFlag;
	
	POWER_NEGOTIATION_STATUS PowerNegMode;
	bPowerNegSetFlag = TRUE;

	api3Data.rfidStatus = RFID_SetPowerNegotiationState(api3Data.hReaderMgmt, bPowerNegSetFlag);

	api3Data.rfidStatus = RFID_GetPowerNegotiationStatus(api3Data.hReaderMgmt, &PowerNegMode);
	api3Data.rfidStatus = RFID_GetPowerNegotiationState(api3Data.hReaderMgmt, &bPowerNegGetFlag);
	switch (PowerNegMode)
	{
	case DISABLED_STATE:
		wprintf(L" \n POwer Negotiatioon State is in DISABLED_STATE");
		break;
	case ONGOING:
		wprintf(L" \n POwer Negotiatioon State is in ONGOING");
		break;
	case SUCCEEDED:
		wprintf(L" \n POwer Negotiatioon State is in SUCCEEDED");
		break;
	case FAILURE:
		wprintf(L" \n POwer Negotiatioon State is in FAILURE");
		break;
	default:
		break;
	}

}


void TestPoweNegAPI(RFID_VERSION rfidVersion, BOOLEAN bMode, int iterCount)
{
	LOGIN_INFO loginfo;

	wcscpy(loginfo.hostName, api3Data.szIPAddress);
	wcscpy(loginfo.userName, L"admin");
	wcscpy(loginfo.password, L"change");
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;

	api3Data.rfidStatus = RFID_Login(&api3Data.hReaderMgmt, &loginfo, FX, bMode, 0);
	for (int iter = 0; iter < iterCount; iter++)
	{
		TestSetPowerNegotiation( );
	}
	
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

}