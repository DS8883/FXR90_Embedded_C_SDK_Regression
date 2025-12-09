#ifndef MANUALTESTS_H
#define MANUALTESTS_H

#include <stdafx.h>
#include <stdio.h>
#include "rfidapi.h"
#include "utils.h"
#ifndef linux
#include <tchar.h>		// windows specific tchar
#endif

#ifdef WIN32
/* windows stuff */
//#define __WFUNC__ WIDEN( __FUNCTION __)

#else
#define LOBYTE(W)       (unsigned char)((W) & 0xFF)
#define HIBYTE(W)       (unsigned char)(((W) >> 8) & 0xFF)
#define MAKEWORD(H, L)  (unsigned short)((L) | ((H) << 8))
#define LOWORD(W)       (unsigned short)((W) & 0xFFFF)
#define HIWORD(W)       (unsigned short)(((W) >> 16) & 0xFFFF)
#define MAKEDWORD(H, L) (unsigned int)((L) | ((H) << 16))
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned int UNINT32;
#define ZeroMemory(p, sz) memset((p), 0, (sz))
#define GetCurrentDir getcwd
#define INVALID_HANDLE_VALUE -1
#define __WFUNC__ WIDEN( __func__ )
#endif

struct RFID3DATA
{
    FILE* fpLog;			// File Pointer for logs
	FILE* fpResult;			// File Pointer for results
	FILE* fpStressLog;		// File pointer for stress test results.
	FILE* fpOut;			// TestSummary file 
	DWORD dwTestCaseNo;		// Test Case number tracking purpose
	DWORD dwPassCount;		// PAss count to keep track pass cases in current execution
	DWORD dwFailCount;		// Fail Count to keep track fail cases in current execution
	DWORD dwSkipCount;		// Skip Count to keep track fail cases in current execution
	DWORD dwFeatureCount;	// Feature wise out put to the File
	RFID_VERSION apiVersion; // API Version to be used.
	UINT32 TagCount;

	char logText[ 512 ];	// Buffer to hold the Current logging message
	UINT8 logResult;				// Current Test Case Result Which indicates TRUE or FALSE
	char CurrExecution[512];	// to store current executing function
	wchar_t* pRfid3WideCharString;

	ANTENNA_INFO pInfo;
	RFID_STATUS rfidStatus;		//To get the Status while using the CAPI calls
	READER_CAPS rCaps;
	char CurrentTime[128];

	RFID_HANDLE32 hReader;		// Reader Handle

	wchar_t tcProfileName[512];
	READER_TYPE readerType;
	wchar_t szIPAddress[512];		// IP of the Reader to connect
	DWORD dwPort;				// Port Nulmber to which reader connecting

	BOOLEAN bContinue;
	BOOLEAN bWait;	//Thread siggalling variables

	RFID_HANDLE32 hReaderMgmt;
	int hh,mm,ss;
	BOOLEAN bStopEvent;
	BOOLEAN bStartEvent;
	BOOLEAN bTagReadEvent;
	POST_FILTER postFilter;
} ;
struct HANDLE_LIST
{
#ifdef linux
	sem_t handles[24];
#else
	HANDLE handles[24];
#endif 
	DWORD nTotalHandles;
};

void StressInventory( int Count,RFID_VERSION );
void TestResetConfig( int Count,RFID_VERSION );
void SingleTagWrites( int Count,RFID_VERSION );
void StressRadioONOFF(int Count,RFID_VERSION );
void TestBlockPermaLock( RFID_VERSION);
void StressBlockErase( int Count,RFID_VERSION );
void StressBlockWrite( int Count,RFID_VERSION );
void UpdateFWonFXDevices( RFID_VERSION);
void AccessSuccessFailCount( int NoOfTagWrites,RFID_VERSION);
void TestBlockPermaLock( RFID_VERSION);
int AcceptConnectionInRegularMode(UINT32 Port);
int AcceptConnectioninSecureMode( UINT32 PortNumber );
void ReadPerformance(int antenna,int iterations,int DurationInMs,SESSION session,RFID_VERSION );
void TestRFIDEvents( );
BOOLEAN TestRFID3EventsManual( RFID_HANDLE32 hReader );
void TestNXPFunctionality( RFID_HANDLE32 hReader );
void TestTagFocus(RFID_VERSION);
void FastID(RFID_VERSION);
void TestABFlip(RFID_VERSION);
void TestNewStateAwareFilters(RFID_VERSION,TRIGGER_INFO*);
void StressRMAPI( DWORD CycleCount,RFID_VERSION );
void StressConfigAPI( DWORD CycleCount,RFID_VERSION );
void Connect2multipleHosts(wchar_t* ip, UINT32 Port);
void TestBlockPermaLock( );
void TestPeriodicReport( RFID_VERSION version);
void ImpinjTest(RFID_VERSION version );
void TagObservationwithTagEvents( RFID_VERSION version );
void TestRSSI( RFID_VERSION version);
void nxpOpSeqResetConfigG2IL(UINT16 ConfigWord);
// FX 7500 related Tests 
void TestCableLossCompensation( RFID_VERSION rfidVersion );
void TestRadioIdleTimeout( RFID_VERSION rfidVersion );
void StressSecureConnection( RFID_VERSION rfidVersion , int Iterations);
void InstallUSerAppTest( RFID_VERSION rfidVersion,BOOLEAN bMode,int Iterations );
void GetSetRadioPowerState( RFID_VERSION rfidVersion );
void AntennaRFConfigWithRFModes( RFID_VERSION rfidVersion );
void AntennaRFConfig( RFID_VERSION rfidVersion);
void SecureLLRPConnectionConfig( RFID_VERSION rfidVersion);
void UpdateFWonPXDevices(RFID_VERSION version);
void TestRFID4PREFILTERS_STATE_AWARE();
void TestRFID4PREFILTERS_STATE_UNAWARE();
void TestRFID4ErrorInformationText(void);


void Truncate( RFID_VERSION rfidversion);
void TestSecureNonSecure( RFID_VERSION rfidVersion);
void ReadAllMemoryBanks(RFID_VERSION rfidVersion);
void DoNxpScan( RFID_VERSION rfidVersion );
void rfid4events();
BOOLEAN accessWithRFModesFX( RFID_VERSION rfidVersion  );
BOOLEAN accessWithRFModesPXI(RFID_VERSION rfidVersion);
BOOLEAN accessWithRFModesPX(RFID_VERSION rfidVersion);
BOOLEAN RegulatoryTesting(RFID_VERSION rfidVersion);
wchar_t* GetCountryNamebyCode(int Code);
void CallAllCleanUPFunctions( RFID_VERSION rfidVersion );
void StressZoneInfo( DWORD CycleCount ,RFID_VERSION version );

void TestPoweNegAPI(RFID_VERSION rfidVersion, BOOLEAN bMode, int iterCount);
void TestNXPBrandID(RFID_VERSION rfidVersion);
void TestTransmitDelay(RFID_VERSION rfidVersion);
void TestPostFilters();
void TestNewAPIadditions( );
void SwitchWithSecureModes();
#ifndef PIXIE
void TestG2V2HideShowMemBank(RFID_VERSION rfidVersion);
#endif // !PIXIE


#endif	//MANUALTESTS_H