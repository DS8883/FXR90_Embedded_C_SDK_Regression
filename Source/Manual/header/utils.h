#ifndef UTILS_H
#define UTILS_H

#include <stdafx.h>
#include "rfidapi.h"

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

#define MAX_COUNTRY_LIST			24	 // number of countries supported by the reader
#define MAX_LENGTH					256	 // length of the buffer
#define MAX_AUSTRALIA_FREQUENCIES	10	 // number of channels available for Australia region
#define MAX_FCC_FREQUENCIES			50	 // number of channels available for FCC
#define MAX_EU_FREQUENCIES			4	 // number of channels available for EU

// Structure that holds the region information 
struct RegionInfo
{
	wchar_t RegionName[64];					// Name of the Country Region
	wchar_t StandardName[64];					// Standard followed for the Region
	BOOLEAN bEURegion;							// flag that indicates if it is US/EU region
};

struct ChannelSelectInfo
{
	int ChannelInfo[MAX_EU_FREQUENCIES];		// Shows the pattern of channels selected.( only for EU)
	int freqCount;								// The number of frequencies selected.
};

wchar_t* PrintTime( void );
void Print2Console(UINT16 crc,UINT16 PC,UINT32 XPC, UINT8*pTagID, UINT32 tagIDLength);
void InitSingulationControl( SINGULATION_CONTROL* pSingControl,SESSION session,UINT16 tagPopulation,UINT16 tagTransitTimeMilliseconds,INVENTORY_STATE inventoryState,SL_FLAG slFlag);
void addSelectRecord( RFID_HANDLE32 hReader, UINT8* pattern,MEMORY_BANK mBank,UINT8 bitoffset,UINT8 patternbitCount,STATE_AWARE_ACTION_PARAMS sParams );
void addanWareSelectRecord(RFID_HANDLE32 hReader, UINT8* pattern, MEMORY_BANK mBank, UINT16 bitoffset, UINT8 patternbitCount, FILTER_ACTION fAction, STATE_UNAWARE_ACTION selectParam);
wchar_t* MultiByte2WideChar( const char* mbs );
void rfid3Sleep( int milliSeconds );
void printTagDataWithResults(TAG_DATA *pTagData);
wchar_t* Byte2HexWString(UINT8* pTagIDData,UINT32 tagIDLength );
void DisplayLastErrorInfo( );
int GetFileContent(char* filePath, BYTE *pBuffer, int *pLen);
int getPowerIndex( int PowerInDbm,READER_CAPS rCaps );
BOOLEAN initRegionList (RegionInfo *pInfo );
BOOLEAN ChangeRegion( RegionInfo *pInfo,ChannelSelectInfo* pChannelsInfo,BOOLEAN Hopping );
BOOLEAN RmLogin( wchar_t* IpAddress );
void convertTagDataToResults(TAG_DATA* pTagData, wchar_t* resultBuffer, bool bPhaseEnabled);
#ifdef linux
#define rfid_swprintf swprintf
#else
#define rfid_swprintf(x, y, z, ...) swprintf(x, z, __VA_ARGS__)

#endif

#endif //UTILS_H