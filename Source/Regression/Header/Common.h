#ifndef COMMON_H
#define COMMON_H

#include "stdafx.h"
//#define WIDEN2(x) L ## x
//#define WIDEN(x) WIDEN2(x)
//#define __WFILE__ WIDEN( __FILE__ )
//#define __WLINE__ WIDEN( __LINE__ )


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

struct MYTAGDATA
{
	UINT16 CRC;
	UINT16 PC;
	UINT8 Res[512];
	UINT16 reslen;
	UINT8 epc[512];
	UINT16 epclen;
	UINT8 tid[512];
	UINT16 tidlen;
	UINT8 user[512];
	UINT16 userlen;
};

struct RFID3DATA
{
    FILE *fpLog;			//File Pointer for logs
	FILE *fpResult;			//File Pointer for results
	FILE *fpStressLog;		// File pointer for stress test results.
	FILE *fpOut;			// TestSummary file 
	FILE *fpCurResult;
	FILE *fpCurLog;

	DWORD dwTestCaseNo;		//Test Case number tracking purpose
	DWORD dwPassCount;		// PAss count to keep track pass cases in current execution
	DWORD dwFailCount;		// Fail Count to keep track fail cases in current execution
	DWORD dwSkipCount;		// Skip Count to keep track fail cases in current execution
	DWORD dwFeatureCount;	// Feature wise out put to the File
	DWORD itemTestCount;	// this variable holds the total test cases for each feature under test, 
							// the number is manually counted for each and intitialized before the start of each test
	wchar_t dllVersionInfo[512];
	RFID_VERSION apiVersion;
	wchar_t apiVersionString[512];
	char logText[ 512 ];	// Buffer to hold the Current logging message
	UINT8 logResult;				// Current Test Case Result Which indicates TRUE or FALSE
	char CurrExecution[512];	// to store current executing function

	ANTENNA_INFO pInfo;
	RFID_STATUS rfidStatus;		//To get the Status while using the CAPI calls
	READER_CAPS rCaps;
	char CurrentTime[128];

	RFID_HANDLE32 hReader;		// Reader Handle
	CONNECTION_INFO conInfo;	// Connection info
	LPTRIGGER_INFO pTriggerInfo;
	wchar_t* pRfid3WideCharString;
	wchar_t tcProfileName[512];
	READER_TYPE readerType;
	wchar_t szIPAddress[512];		// IP of the Reader to connect
	char mbcsIPAddress[256];
	DWORD dwPort;				// Port Nulmber to which reader connecting
	
	LPTAG_DATA pbeddTag;
	LPTAG_DATA pfujitsu64Ktag;
	LPTAG_DATA pfujitsu8Ktag;
	LPTAG_DATA pImpinjTag;
	LPTAG_DATA pnxpTag;
	LPTAG_DATA pG2V2Tag;

	BOOLEAN bContinue;
	BOOLEAN bWait;	//Thread siggalling variables

	//Secure Mode Parameters
	BOOLEAN bSecureMode;
	SEC_CONNECTION_INFO SecureConnectionInfo;
	RFID_HANDLE32 hReaderMgmt;
	int hh,mm,ss;
	BOOLEAN bStopEvent;
	BOOLEAN bStartEvent;
	BOOLEAN bTagReadEvent;
	BOOLEAN api2Logging; // to check if the error code is coming from API2 logging or API3 logging.
};



typedef enum _TAGS2READ
{
    PATTERN_ONE_TAGS = 0,	
    PATTERN_TWO_TAGS,	
	TAGS_OTHERTHAN_PATTERN_ONE,
	TAGS_OTHERTHAN_PATTERN_TWO,
	ALL_TAGS,
}TAGS2READ;

extern char G2MEMBANK[][32];
extern char SESSIONS[][16];
extern char SLFLAGS[][32];
extern char INVENTORYSTATES[][32];
extern char TAGEVENTS[][32];
extern char UNAWAREACTION[][64];
extern char ACCESSSTATUS[][64];
extern char RECOMOPCODES[][64];
extern char LOCKPRIVILEGE[][32];
extern char LOCKDATAFIELD[][32];
extern char ERRORSTATUS[][64];
extern char ACCESSOPERATIONCODES[][64];
extern char TAGEVENTREPORTTRIGGER[][32];

extern wchar_t wG2MEMBANK[][32];
extern wchar_t wSESSIONS[][16];
extern wchar_t wSLFLAGS[][32];
extern wchar_t wINVENTORYSTATES[][32];
extern wchar_t wTAGEVENTS[][32];
extern wchar_t wUNAWAREACTION[][64];
extern wchar_t wACCESSSTATUS[][64];
extern wchar_t wRECOMOPCODES[][64];
extern wchar_t wLOCKPRIVILEGE[][32];
extern wchar_t wLOCKDATAFIELD[][32];
extern wchar_t wERRORSTATUS[][64];
extern wchar_t wACCESSOPERATIONCODES[][64];
extern wchar_t wTAGEVENTREPORTTRIGGER[][32];
extern wchar_t wRFIDEVENTS[][32];
extern wchar_t wMODULATION[][16];
extern wchar_t wFORWARD_LINK_MODULATION[][32];
extern wchar_t wSPECTRAL_MASK_INDICATOR[][16];

struct HANDLE_LIST
{
#ifdef linux
	sem_t handles[24];
#else
	HANDLE handles[24];
#endif 
	DWORD nTotalHandles;
};

BOOLEAN getBEDDTag( );
BOOLEAN getConnectConfig( wchar_t* configFile );
void Initialize2DefaultConfig( wchar_t* );
void SetLogText( char* logText );
void SetLogResult( UINT8 value );
int logToResultFile( wchar_t*);
void logToFile( char* pText );
void logCurrentExecution( char* pText );
void LogPassFailCount( );
void apiResultPassLog();
void apiResultFailLog();
void DisplayLastErrorInfo( );

void InitSecurityParameters();
int initRFID( wchar_t* IPAddress, DWORD nPortNo,READER_TYPE r );
void InitSingulationControl( SINGULATION_CONTROL*,SESSION session,UINT16 tagPopulation,UINT16 tagTransitTimeMilliseconds,INVENTORY_STATE inventoryState,SL_FLAG slFlag);
int DeInitRFID( void );
BOOLEAN Login( void );
BOOLEAN LoginA(void);
int IsAntennaConnected( RFID_HANDLE32 hReader,int antID );

void DisplayCapabilities( RFID_HANDLE32 hReader);
void CleanupTagQueue( );
DWORD ReadTags( RFID_HANDLE32 hReader,MEMORY_BANK mBank,UINT16 offSet,ANTENNA_INFO* pInfo,TRIGGER_INFO* tInfo,TAGS2READ tags2Read );
void addSelectRecord( RFID_HANDLE32 hReader, UINT8* pattern,MEMORY_BANK mBank,UINT8 bitoffset,UINT8 patternbitCount,STATE_AWARE_ACTION_PARAMS sParams );


void TestRFID3ANSIAPI();
///RM API
void TestRFID_GetReadPointStatus( RFID_HANDLE32 hReader );

void printTagDataWithResults(TAG_DATA *pTagData);
wchar_t* Byte2HexWString(UINT8* pTagIDData,UINT32 tagIDLength );
char* Byte2HexString(UINT8* pTagIDData,UINT32 tagIDLength );
char* Array2Char( UINT8* byte,DWORD offSet,DWORD len );
BOOLEAN GetMemoryBanks( RFID_HANDLE32 hReader, LPTAG_DATA pTagData,UINT32 axesPwd );
wchar_t* GetCountryNamebyCode(int Code);
BOOLEAN getTwoTags(  UINT8* tagPC1, UINT8* tagPC2,ANTENNA_INFO* pInfo,int attempt  );

// New helper functions for inventory and memory reading
BOOLEAN ReadMemoryBank(LPTAG_DATA pTagData, MEMORY_BANK bank, UINT32 offset, UINT32 length);
BOOLEAN PerformInventoryAndReadMemory(UINT32 inventoryDurationMs, BOOLEAN bReadTID, BOOLEAN bReadUser);
void PrintTagMemoryData(LPTAG_DATA pTagData);

void Fresult(char *TestCaseID, char* APIName, char* TestTitle, char* TestType);
void Print2Console(UINT16 antID,UINT16 crc,UINT16 PC,UINT32 XPC, wchar_t* szIP, UINT8*pTagID, UINT32 tagIDLength);
void Print2Log( wchar_t* mylogData,UINT16 antID,UINT16 crc,UINT16 PC,UINT32 XPC, wchar_t* szIP, UINT8*pTagID, UINT32 tagIDLength );
void convertTagDataToResults(TAG_DATA* pTagData, wchar_t* resultBuffer, bool bPhaseEnabled);
// RFID Management API related functions.
//
void rfid3Sleep( int );

#ifdef linux
#define rfid_swprintf swprintf
#else
#define rfid_swprintf(x, y, z, ...) swprintf(x, z, __VA_ARGS__)
#endif

#ifdef linux
tm SYSTEMTIME2tm(SYSTEMTIME *s);
void GetLocalTime(SYSTEMTIME * pSystemTime);
#endif
void rfid3SystemTime( SYSTEMTIME* rfid3TIME );
wchar_t* MultiByte2WideChar( const char* mbs);
#ifndef linux
// API2 definitions not valid and supported on Threadbo Platform
BOOLEAN ConfigureTCPIP(HANDLE hReader, wchar_t *pszIPAddress, DWORD dwPort);
#endif
char* PrintTime( void );
void PrintLine( FILE* );
#endif
