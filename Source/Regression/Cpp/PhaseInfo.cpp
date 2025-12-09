// PhaseInfo.cpp : 
//	Verify Phase information in the TagInfo based on the tagfield settings
//	Antenna RFConfiguration and Periodic Reporting of Tags
//
#include "Common.h"

extern struct RFID3DATA api3Data;	// RFID3 Data


void Fx7500PhaseInfo( )
{
	TAG_STORAGE_SETTINGS tagSettings;
	api3Data.rfidStatus = RFID_GetTagStorageSettings( api3Data.hReader, &tagSettings);
	

}

void Fx7500AntennaRFConfig( )
{

}

void Fx7500PeriodicReportTrigger( )
{
	
}
