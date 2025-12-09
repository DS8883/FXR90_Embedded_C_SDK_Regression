#include "ManualTests.h"

extern struct RFID3DATA api3Data;
#define MAX_BUF 512

// test cable loss compensation
//
void TestCableLossCompensation( RFID_VERSION rfidVersion )
{
	char buf[MAX_BUF]={0,};
	CABLE_LOSS_COMPENSATION* pCableLoss[15];
	CABLE_LOSS_COMPENSATION getCableLoss;
	CONNECTION_INFO ConInfo = { rfidVersion, 0 };
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
	for( int antennaIndex = 0; antennaIndex < api3Data.rCaps.numAntennas;antennaIndex++)
	{
		pCableLoss[antennaIndex] = new CABLE_LOSS_COMPENSATION;
		pCableLoss[antennaIndex]->antennaID = antennaIndex+1;
		pCableLoss[antennaIndex]->cableLengthInFt = 15.0;
		pCableLoss[antennaIndex]->cableLossPer100Ft = (float)-0.4;
	}

	wprintf(L"Executing TestCableLossCompensation()\n");
	LOGIN_INFO loginfo;
	wcscpy( loginfo.hostName,api3Data.szIPAddress);
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = rfidVersion;
	
	api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
	
	// Get Max Power index value
	UINT16* pPowerValues = new UINT16[api3Data.rCaps.transmitPowerLevelTable.numValues];
	memcpy( pPowerValues,api3Data.rCaps.transmitPowerLevelTable.pPowerValueList,api3Data.rCaps.transmitPowerLevelTable.numValues);

	api3Data.rfidStatus = RFID_SetCableLossCompensation( api3Data.hReaderMgmt,api3Data.rCaps.numAntennas,pCableLoss);

	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,5084,0,&ConInfo );
	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader,&api3Data.rCaps);
	UINT16* pPowerValuesCableLoss = api3Data.rCaps.transmitPowerLevelTable.pPowerValueList;

	for( int index = 1;index<= api3Data.rCaps.numAntennas;index++)
	{
		getCableLoss.antennaID = index;
		api3Data.rfidStatus = RFID_GetCableLossCompensation( api3Data.hReaderMgmt, &getCableLoss);
		wprintf(L"\n antenna ID : %ld cableLenght:%lf CableLoss:%lf",getCableLoss.antennaID,getCableLoss.cableLengthInFt,getCableLoss.cableLossPer100Ft);
	}
	api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt);
	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );

}