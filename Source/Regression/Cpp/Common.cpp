// Common.cpp : Defines Helper Routines.
//
#include "Common.h"
#include "rfid2Helper.h"
#include "APITest.h"

#define rfid_stprintf swprintf
 
RFID3DATA api3Data;					// RFID3 Data
extern struct RFID2DATA api2Data;	// RFID2 Data

HANDLE_LIST hList;					// Global List of Handles
MYTAGDATA MyTags[2];				// Tags used in this automation
ANTENNA_INFO antennaInfo;			// Global antenna info structure
static char max[ 1024 ];			
#define MAX_CERT_KEY_FILE_SIZE		10240
char ptagBuffer[4096];
wchar_t wtagBuffer[4096];

// event count
static UINT32 numEventAll = 0;
static UINT32 numEventNewTag = 0;
static UINT32 numEventVisible = 0;
static UINT32 numEventInvisible = 0;
static UINT32 numEventMoving = 0;
static UINT32 numEventStationary = 0;
static UINT32 numMatchedTag = 0;
static bool bDisplayTagData = true;
static wchar_t CountryName[128];

// The following definitions used interchangeably for RFID Constansts used in the Automation.
// Most of the defitions below are used for printing purpose only.
//
char G2MEMBANK[][32] = {"MEMORY_BANK_RESERVED", "MEMORY_BANK_EPC", "MEMORY_BANK_TID", "MEMORY_BANK_USER"};
char SESSIONS[][16] = {"SESSION_S0","SESSION_S1","SESSION_S2","SESSION_S3"};
char SLFLAGS[][32] = {"SL_FLAG_ASSERTED","SL_FLAG_DEASSERTED"};
char INVENTORYSTATES[][32] = {"INVENTORY_STATE_A","INVENTORY_STATE_B"};
char TAGEVENTS[][32] = { "UNKNOWN_STATE", "NEW_TAG_VISIBLE", "TAG_NOT_VISIBLE", "TAG_BACK_TO_VISIBILITY","TAG_MOVING","TAG_STATIONARY","NONE" };
char UNAWAREACTION[][64] = {"STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT","STATE_UNAWARE_ACTION_SELECT","STATE_UNAWARE_ACTION_NOT_UNSELECT","STATE_UNAWARE_ACTION_UNSELECT","STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT","STATE_UNAWARE_ACTION_NOT_SELECT"};
char ACCESSSTATUS[][64] = {"ACCESS_SUCCESS","ACCESS_TAG_NON_SPECIFIC_ERROR","ACCESS_READER_NON_SPECIFIC_ERROR","ACCESS_NO_RESPONSE_FROM_TAG","ACCESS_INSUFFIFICENT_POWER","ACCESS_INSUFFICENT_POWER","ACCESS_TAG_MEMORY_LOCKED_ERROR","ACCESS_TAG_MEMORY_OVERRUN_ERROR","ACCESS_ZERO_KILL_PASSWORD_ERROR"};
char RECOMOPCODES[][64] = {"RECOMMISSION_DISABLE_PERMALOCK","RECOMMISSION_DISABLE_USER_MEMORY","RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION","RECOMMISSION_DISABLE_PASSWORD","RECOMMISSION_DISABLE_PERMALOCK_PASSWORD","RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD","RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION"};	
char LOCKPRIVILEGE[][32] = {"LOCK_PRIVILEGE_NONE","LOCK_PRIVILEGE_READ_WRITE","LOCK_PRIVILEGE_READ_WRITE","LOCK_PRIVILEGE_PERMA_LOCK","LOCK_PRIVILEGE_PERMA_UNLOCK","LOCK_PRIVILEGE_UNLOCK"};
char LOCKDATAFIELD[][32] = {"LOCK_KILL_PASSWORD","LOCK_ACCESS_PASSWORD","LOCK_EPC_MEMORY","LOCK_TID_MEMORY","LOCK_USER_MEMORY"};
char COMMNSTANDARD[][64] = {"UNSPECIFIED","US_FCC_PART_15","ETSI_302_208","ETSI_300_220","AUSTRALIA_LIPD_1W","AUSTRALIA_LIPD_4W","JAPAN_ARIB_STD_T89","HONGKONG_OFTA_1049","TAIWAN_DGT_LP0002","KOREA_MIC_ARTICLE_5_2"};
char ERRORSTATUS[][64] = {
							"RFID_API_SUCCESS","RFID_API_COMMAND_TIMEOUT","RFID_API_PARAM_ERROR","RFID_API_PARAM_OUT_OF_RANGE","RFID_API_CANNOT_ALLOC_MEM","RFID_API_UNKNOWN_ERROR","RFID_API_INVALID_HANDLE","RFID_API_BUFFER_TOO_SMALL",
							"RFID_READER_FUNCTION_UNSUPPORTED","RFID_RECONNECT_FAILED","RFID_API_DATA_NOT_INITIALISED","RFID_API_ZONE_ID_ALREADY_EXITS","RFID_API_ZONE_ID_NOT_FOUND","RFID_COMM_OPEN_ERROR","RFID_COMM_CONNECTION_ALREADY_EXISTS","RFID_COMM_RESOLVE_ERROR","RFID_COMM_SEND_ERROR",
							"RFID_COMM_RECV_ERROR","RFID_COMM_NO_CONNECTION","RFID_INVALID_SOCKET","RFID_READER_REGION_NOT_CONFIGURED","RFID_READER_REINITIALIZING","RFID_SECURE_CONNECTION_ERROR","RFID_ROOT_SECURITY_CERTIFICATE_ERROR","RFID_HOST_SECURITY_CERTIFICATE_ERROR","RFID_HOST_SECURITY_KEY_ERROR","RFID_CONFIG_GET_FAILED",
							"RFID_CONFIG_SET_FAILED","RFID_CONFIG_NOT_SUPPORTED","RFID_CAP_NOT_SUPPORTED","RFID_CAP_GET_FAILED","RFID_FILTER_NO_FILTER","RFID_FILTER_INVALID_INDEX","RFID_FILTER_MAX_FILTERS_EXCEEDED","RFID_NO_READ_TAGS","RFID_NO_REPORTED_EVENTS",
							"RFID_INVENTORY_MAX_TAGS_EXCEEDED","RFID_INVENTORY_IN_PROGRESS","RFID_NO_INVENTORY_IN_PROGRESS","RFID_TAG_LOCATING_IN_PROGRESS","RFID_NO_TAG_LOCATING_IN_PROGRESS","RFID_NXP_EAS_SCAN_IN_PROGRESS",
							"RFID_NO_NXP_EAS_SCAN_IN_PROGRESS","RFID_ACCESS_IN_PROGRESS","RFID_NO_ACCESS_IN_PROGRESS",
							"RFID_ACCESS_TAG_READ_FAILED","RFID_ACCESS_TAG_WRITE_FAILED","RFID_ACCESS_TAG_LOCK_FAILED","RFID_ACCESS_TAG_KILL_FAILED","RFID_ACCESS_TAG_BLOCK_ERASE_FAILED","RFID_ACCESS_TAG_BLOCK_WRITE_FAILED",
							"RFID_ACCESS_TAG_NOT_FOUND","RFID_ACCESS_SEQUENCE_NOT_INITIALIZED","RFID_ACCESS_SEQUENCE_EMPTY","RFID_ACCESS_SEQUENCE_IN_USE","RFID_ACCESS_SEQUENCE_MAX_OP_EXCEEDED","RFID_ACCESS_TAG_RECOMMISSION_FAILED","RFID_ACCESS_TAG_BLOCK_PERMALOCK_FAILED","RFID_ACCESS_NXP_TAG_SET_EAS_FAILED","RFID_ACCESS_NXP_TAG_READ_PROTECT_FAILED","RFID_ACCESS_FUJITSU_CHANGE_WORDLOCK_FAILED",
							"RFID_ACCESS_FUJITSU_CHANGE_BLOCKLOCK_FAILED" ,"RFID_ACCESS_FUJITSU_READ_BLOCKLOCK_FAILED","RFID_ACCESS_FUJITSU_BURST_WRITE_FAILED","RFID_ACCESS_FUJITSU_BURST_ERASE_FAILED","RFID_ACCESS_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_FAILED","RFID_ACCESS_FUJITSU_AREA_READLOCK_FAILED","RFID_ACCESS_FUJITSU_AREA_WRITELOCK_FAILED",
							"RFID_ACCESS_FUJITSU_AREA_WRITELOCK_WOPASSWORD_FAILED","RFID_ACCESS_NXP_CHANGE_CONFIG_FAILED","RFID_ACCESS_IMPINJ_QT_READ_FAILED","RFID_ACCESS_IMPINJ_QT_WRITE_FAILED","RFID_ACCESS_G2V2_AUTHENTICATE_FAILED","RFID_ACCESS_G2V2_READBUFFER_FAILED","RFID_ACCESS_G2V2_UNTRACEABLE_FAILED","RFID_ACCESS_G2V2_CRYPTO_FAILED","RFID_RM_INVALID_USERNAME_PASSWORD","RFID_RM_NO_UPDATION_IN_PROGRESS","RFID_RM_UPDATION_IN_PROGRESS","RFID_RM_COMMAND_FAILED","RFID_NXP_BRANDID_CHECK_IN_PROGRESS","RFID_INVALID_ERROR_CODE",
							"RFID_NO_RF_SURVEY_OPERATION_IN_PROGRESS","RFID_RFSURVEY_IN_PROGRESS"
						};
char ACCESSOPERATIONCODES[][64] = { "ACCESS_OPERATION_READ","ACCESS_OPERATION_WRITE","ACCESS_OPERATION_LOCK","ACCESS_OPERATION_KILL","ACCESS_OPERATION_BLOCK_WRITE","ACCESS_OPERATION_BLOCK_ERASE",
									"ACCESS_OPERATION_RECOMMISSION","ACCESS_OPERATION_BLOCK_PERMALOCK","ACCESS_OPERATION_NXP_SET_EAS","ACCESS_OPERATION_NXP_READ_PROTECT","ACCESS_OPERATION_NXP_RESET_READ_PROTECT",
									"ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK","ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK","ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK","ACCESS_OPERATION_FUJITSU_BURST_WRITE",
									"ACCESS_OPERATION_FUJITSU_BURST_ERASE","ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD","ACCESS_OPERATION_FUJITSU_AREA_READLOCK","ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK",
									"ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD","ACCESS_OPERATION_IMPINJ_QT_WRITE","ACCESS_OPERATION_IMPINJ_QT_READ","ACCESS_OPERATION_NXP_CHANGE_CONFIG"
								  };
char TAGEVENTREPORTTRIGGER[][32] = { "NEVER","IMMEDIATE","MODERATED" };

// Wide char versions ..To support on Linux
//
wchar_t wG2MEMBANK[][32] = {L"MEMORY_BANK_RESERVED", L"MEMORY_BANK_EPC", L"MEMORY_BANK_TID", L"MEMORY_BANK_USER"};
wchar_t wSESSIONS[][16] = {L"SESSION_S0",L"SESSION_S1",L"SESSION_S2",L"SESSION_S3"};
wchar_t wSLFLAGS[][32] = {L"SL_FLAG_ASSERTED",L"SL_FLAG_DEASSERTED"};
wchar_t wINVENTORYSTATES[][32] = {L"INVENTORY_STATE_A",L"INVENTORY_STATE_B"};
wchar_t wTAGEVENTS[][32] = { L"UNKNOWN_STATE", L"NEW_TAG_VISIBLE", L"TAG_NOT_VISIBLE", L"TAG_BACK_TO_VISIBILITY",L"TAG_MOVING",L"TAG_STATIONARY",L"NONE" };
wchar_t wUNAWAREACTION[][64] = {L"STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT",L"STATE_UNAWARE_ACTION_SELECT",L"STATE_UNAWARE_ACTION_NOT_UNSELECT",L"STATE_UNAWARE_ACTION_UNSELECT",L"STATE_UNAWARE_ACTION_UNSELECT_NOT_SELECT",L"STATE_UNAWARE_ACTION_NOT_SELECT"};
wchar_t wACCESSSTATUS[][64] = {L"ACCESS_SUCCESS",L"ACCESS_TAG_NON_SPECIFIC_ERROR",L"ACCESS_READER_NON_SPECIFIC_ERROR",L"ACCESS_NO_RESPONSE_FROM_TAG",L"ACCESS_INSUFFIFICENT_POWER",L"ACCESS_INSUFFICENT_POWER",L"ACCESS_TAG_MEMORY_LOCKED_ERROR",L"ACCESS_TAG_MEMORY_OVERRUN_ERROR",L"ACCESS_ZERO_KILL_PASSWORD_ERROR"};
wchar_t wRECOMOPCODES[][64] = {L"RECOMMISSION_DISABLE_PERMALOCK",L"RECOMMISSION_DISABLE_USER_MEMORY",L"RECOMMISSION_DISABLE_USER_MEMORY_2ND_OPTION",L"RECOMMISSION_DISABLE_PASSWORD",L"RECOMMISSION_DISABLE_PERMALOCK_PASSWORD",L"RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD",L"RECOMMISSION_DISABLE_USER_MEMORY_PASSWORD_2ND_OPTION"};	
wchar_t wLOCKPRIVILEGE[][32] = {L"LOCK_PRIVILEGE_NONE",L"LOCK_PRIVILEGE_READ_WRITE",L"LOCK_PRIVILEGE_READ_WRITE",L"LOCK_PRIVILEGE_PERMA_LOCK",L"LOCK_PRIVILEGE_PERMA_UNLOCK",L"LOCK_PRIVILEGE_UNLOCK"};
wchar_t wLOCKDATAFIELD[][32] = {L"LOCK_KILL_PASSWORD",L"LOCK_ACCESS_PASSWORD",L"LOCK_EPC_MEMORY",L"LOCK_TID_MEMORY",L"LOCK_USER_MEMORY"};
wchar_t wCOMMNSTANDARD[][64] = {L"UNSPECIFIED",L"US_FCC_PART_15",L"ETSI_302_208",L"ETSI_300_220",L"AUSTRALIA_LIPD_1W",L"AUSTRALIA_LIPD_4W",L"JAPAN_ARIB_STD_T89",L"HONGKONG_OFTA_1049",L"TAIWAN_DGT_LP0002",L"KOREA_MIC_ARTICLE_5_2"};
wchar_t wERRORSTATUS[][64] = {
							L"RFID_API_SUCCESS",L"RFID_API_COMMAND_TIMEOUT",L"RFID_API_PARAM_ERROR",L"RFID_API_PARAM_OUT_OF_RANGE",L"RFID_API_CANNOT_ALLOC_MEM",L"RFID_API_UNKNOWN_ERROR",L"RFID_API_INVALID_HANDLE",L"RFID_API_BUFFER_TOO_SMALL",
							L"RFID_READER_FUNCTION_UNSUPPORTED",L"RFID_RECONNECT_FAILED",L"RFID_COMM_OPEN_ERROR",L"RFID_COMM_CONNECTION_ALREADY_EXISTS",L"RFID_COMM_RESOLVE_ERROR",L"RFID_COMM_SEND_ERROR",
							L"RFID_COMM_RECV_ERROR",L"RFID_COMM_NO_CONNECTION",L"RFID_INVALID_SOCKET",L"RFID_READER_REGION_NOT_CONFIGURED",L"RFID_READER_REINITIALIZING",L"RFID_CONFIG_GET_FAILED",
							L"RFID_CONFIG_SET_FAILED",L"RFID_CAP_NOT_SUPPORTED",L"RFID_CAP_GET_FAILED",L"RFID_FILTER_NO_FILTER",L"RFID_FILTER_INVALID_INDEX",L"RFID_FILTER_MAX_FILTERS_EXCEEDED",L"RFID_NO_READ_TAGS",L"RFID_NO_REPORTED_EVENTS",
							L"RFID_INVENTORY_MAX_TAGS_EXCEEDED",L"RFID_INVENTORY_IN_PROGRESS",L"RFID_NO_INVENTORY_IN_PROGRESS",L"RFID_TAG_LOCATING_IN_PROGRESS",L"RFID_NO_TAG_LOCATING_IN_PROGRESS",L"RFID_NXP_EAS_SCAN_IN_PROGRESS",
							L"RFID_NO_NXP_EAS_SCAN_IN_PROGRESS",L"RFID_PORTAL_DIRECTIONALITY_OPERTATION_IN_PROGRESS",L"RFID_NO_PORTAL_DIRECTIONALITY_OPERTATION_IN_PROGRESS",L"RFID_ACCESS_IN_PROGRESS",L"RFID_NO_ACCESS_IN_PROGRESS",
							L"RFID_ACCESS_TAG_READ_FAILED",L"RFID_ACCESS_TAG_WRITE_FAILED",L"RFID_ACCESS_TAG_LOCK_FAILED",L"RFID_ACCESS_TAG_KILL_FAILED",L"RFID_ACCESS_TAG_BLOCK_ERASE_FAILED",L"RFID_ACCESS_TAG_BLOCK_WRITE_FAILED",
							L"RFID_ACCESS_TAG_NOT_FOUND",L"RFID_ACCESS_SEQUENCE_NOT_INITIALIZED",L"RFID_ACCESS_SEQUENCE_EMPTY",L"RFID_ACCESS_SEQUENCE_IN_USE",L"RFID_ACCESS_SEQUENCE_MAX_OP_EXCEEDED",L"RFID_ACCESS_TAG_RECOMMISSION_FAILED",L"RFID_ACCESS_TAG_BLOCK_PERMALOCK_FAILED",L"RFID_ACCESS_NXP_TAG_SET_EAS_FAILED",L"RFID_ACCESS_NXP_TAG_READ_PROTECT_FAILED",L"RFID_ACCESS_FUJITSU_CHANGE_WORDLOCK_FAILED",
							L"RFID_ACCESS_FUJITSU_CHANGE_BLOCKLOCK_FAILED" ,L"RFID_ACCESS_FUJITSU_READ_BLOCKLOCK_FAILED",L"RFID_ACCESS_FUJITSU_BURST_WRITE_FAILED",L"RFID_ACCESS_FUJITSU_BURST_ERASE_FAILED",L"RFID_ACCESS_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD_FAILED",L"RFID_ACCESS_FUJITSU_AREA_READLOCK_FAILED",L"RFID_ACCESS_FUJITSU_AREA_WRITELOCK_FAILED",
							L"RFID_ACCESS_FUJITSU_AREA_WRITELOCK_WOPASSWORD_FAILED",L"RFID_ACCESS_NXP_CHANGE_CONFIG_FAILED",L"RFID_ACCESS_IMPINJ_QT_READ_FAILED",L"RFID_ACCESS_IMPINJ_QT_WRITE_FAILED",L"RFID_RM_INVALID_USERNAME_PASSWORD",L"RFID_RM_NO_UPDATION_IN_PROGRESS",L"RFID_RM_UPDATION_IN_PROGRESS",L"RFID_RM_COMMAND_FAILED",L"RFID_INVALID_ERROR_CODE"
						};
wchar_t wACCESSOPERATIONCODES[][64] = { L"ACCESS_OPERATION_READ",L"ACCESS_OPERATION_WRITE",L"ACCESS_OPERATION_LOCK",L"ACCESS_OPERATION_KILL",L"ACCESS_OPERATION_BLOCK_WRITE",L"ACCESS_OPERATION_BLOCK_ERASE",
									L"ACCESS_OPERATION_RECOMMISSION",L"ACCESS_OPERATION_BLOCK_PERMALOCK",L"ACCESS_OPERATION_NXP_SET_EAS",L"ACCESS_OPERATION_NXP_READ_PROTECT",L"ACCESS_OPERATION_NXP_RESET_READ_PROTECT",
									L"ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK",L"ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK",L"ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK",L"ACCESS_OPERATION_FUJITSU_BURST_WRITE",
									L"ACCESS_OPERATION_FUJITSU_BURST_ERASE",L"ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD",L"ACCESS_OPERATION_FUJITSU_AREA_READLOCK",L"ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK",
									L"ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD",L"ACCESS_OPERATION_IMPINJ_QT_WRITE",L"ACCESS_OPERATION_IMPINJ_QT_READ",L"ACCESS_OPERATION_NXP_CHANGE_CONFIG"
								  };
wchar_t wTAGEVENTREPORTTRIGGER[][32] = { L"NEVER",L"IMMEDIATE",L"MODERATED" };
wchar_t wRFIDEVENTS[][32] = { L"GPI_EVENT",L"TAG_DATA_EVENT",L"BUFFER_FULL_WARNING_EVENT",L"ANTENNA_EVENT",L"INVENTORY_START_EVENT",L"INVENTORY_STOP_EVENT",L"ACCESS_START_EVENT",L"ACCESS_STOP_EVENT",L"BUFFER_FULL_EVENT",L"NXP_EAS_ALARM_EVENT",L"READER_EXCEPTION_EVENT",L"HANDHELD_TRIGGER_EVENT"};
wchar_t wMODULATION[][16] = { L"MV_FM0",L"MV_2",L"MV_4",L"MV_8"};
wchar_t wFORWARD_LINK_MODULATION[][32] = {L"FORWARD_LINK_MODULATION_PR_ASK",L"FORWARD_LINK_MODULATION_SSB_ASK",L"FORWARD_LINK_MODULATION_DSB_ASK"};
wchar_t wSPECTRAL_MASK_INDICATOR[][16] = { L"SMI_UNKNOWN",L"SMI_SI",L"SMI_MI",L"SMI_DI"};

void CleanupTagQueue( )
{
	api3Data.rfidStatus = RFID_PurgeTags( api3Data.hReader, 0 );
	api3Data.rfidStatus = RFID_DeletePreFilter( api3Data.hReader,0,0);
}

// Common Wrapper definition for getting SystemTime
//
void rfid3SystemTime( SYSTEMTIME* rfid3TIME )
{
#ifdef linux
	GetLocalTime( rfid3TIME );
#else
	GetSystemTime( rfid3TIME );
#endif
}
// Wide Char Version Converter
//
wchar_t* MultiByte2WideChar( const char* mbs )
{
	mbstowcs( api3Data.pRfid3WideCharString,mbs,512);
	return api3Data.pRfid3WideCharString;
}
// Common Wrapper definition for Sleep
//
void rfid3Sleep( int milliSeconds )
{
#ifdef linux
	sleep( milliSeconds /1000);
#else
	Sleep( milliSeconds );
#endif
}
// Conversion routine to print Byte Data in Chars
//
char* Array2Char( UINT8* byte,DWORD offSet,DWORD len )
{
	char p[3];
	DWORD byteOffSet = offSet;
	ZeroMemory( max,1024);
	for(DWORD k=0;k<len;k++)
	{
		sprintf( p,"%02x",byte[byteOffSet]);
		byteOffSet++;
		strcat( max,p );
	}

	return max;
}

void DisplayLastErrorInfo( )
{
	ERROR_INFO errorInfo;
	api3Data.rfidStatus = RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
	wprintf( L"\n rfidStatusCode - %d <br>",errorInfo.rfidStatusCode);
	wprintf( L"\n statusDesc - %ls ",errorInfo.statusDesc);
	wprintf( L"\n vendorMessage - %ls", errorInfo.vendorMessage);
}

// Defualts to the list defined below of certain Config Parameters are not provided.
//
void Initialize2DefaultConfig( wchar_t* readerIP )
{
	api3Data.apiVersion = RFID_API3_5_1;
	api3Data.readerType = FX;
	api3Data.pInfo.pAntennaList = NULL;
	api3Data.pInfo.length = 0;
	api3Data.dwPort = 5084;
	wcscpy( api3Data.szIPAddress,readerIP );
}
// Read Connection Configuraion from Configuration file
// 
BOOLEAN getConnectConfig( wchar_t* configFile )
{
	wchar_t functionName[256];
	mbstowcs(functionName, __FUNCTION__, 256);
	wprintf(L"\n Executing %ls Started ", functionName);
	DWORD antTotal=0;
	UINT16 antSeq[8];
	char ansiConfig[128];
	int i=0;
	api3Data.readerType = FX;	//default config
	// get the connect config data ip,port and reader type from the test file
	//
	FILE* fConfig;
	wchar_t readerString[32],readerIP[32],readerType[32],apiVersion[32];
	int secureMode = 0;
	wcstombs(ansiConfig,configFile,128);
	if(  (fConfig = fopen( ansiConfig,"r") ) == NULL )
	{
		wprintf(L"\n error opening config.txt file\n");
		return FALSE;
	}

	fscanf( fConfig,"%S %S\n",readerString,apiVersion);
	fscanf( fConfig,"%S %S\n",readerString,readerIP);
	fscanf( fConfig,"%S %S\n",readerString,readerType);
	fscanf( fConfig,"%S %d\n",readerString,&antTotal );
	fscanf( fConfig,"%S %hd %hd %hd %hd %hd %hd %hd %hd\n", readerString, &(antSeq[0]),&(antSeq[1]),&antSeq[2],&antSeq[3],&antSeq[4],&antSeq[5],&antSeq[6],&antSeq[7]);
	
	// Get SecureMode parameter from Config file
	fscanf( fConfig,"%S %hd\n",readerString,&secureMode );
	wprintf(L"\n Config text: apiVersion: %ls\t readerIP/COM:%ls\treaderType:%ls\tAntennas:%ld", apiVersion, readerIP, readerType, antTotal);
	if( antTotal > 0 )
	{
		// do it on selected antennas
		//
		api3Data.pInfo.pAntennaList = new UINT16( antTotal );
		api3Data.pInfo.length = antTotal;
		api3Data.pInfo.pAntennaOpList = NULL;

		for( i=0; i<antTotal; i++ )
		{
			api3Data.pInfo.pAntennaList[i] = antSeq[ i ];
			
		}
	}
	else
	{
		api3Data.pInfo = { 0, };
		api3Data.pInfo.length = 0;
		api3Data.pInfo.pAntennaList = NULL;
		api3Data.pInfo.pAntennaOpList = NULL;
	}
	
	
	fclose( fConfig );	//close config file
	
	// Capture Reade rIp Address from the Config File
	wcscpy( api3Data.szIPAddress,readerIP );
	wcstombs(api3Data.mbcsIPAddress,readerIP,256);

	if( ( wcscmp( readerType,L"MC" ) == 0 ) )api3Data.readerType = MC;
	else if ( ( wcscmp( readerType,L"FX" ) == 0 ) )api3Data.readerType = FX;
	else if ( ( wcscmp( readerType,L"XR" ) == 0 ) )api3Data.readerType = XR;
#if defined PIXIE
	else if ((wcscmp(readerType, L"PX") == 0))api3Data.readerType = PX;
	if (api3Data.readerType == PX) api3Data.dwPort = 921600;
#endif
	wcscpy( api3Data.apiVersionString, apiVersion );

	
	if (api3Data.readerType == FX || api3Data.readerType == XR) api3Data.dwPort = 5084;

	// Capture API library Version info from the Config File
	if( ( wcscmp( apiVersion,L"RFID_API3_5_0" ) == 0 ) )
	{
		api3Data.apiVersion = RFID_API3_5_0;
	}
	else if( ( wcscmp( apiVersion,L"RFID_API3_5_1" ) == 0 ) )
	{
		api3Data.apiVersion = RFID_API3_5_1;
	}
	else if( ( wcscmp( apiVersion,L"RFID_API3_5_5" ) == 0 ) )
	{
		api3Data.apiVersion = RFID_API3_5_5;
	}
	
	api3Data.bSecureMode = (BOOLEAN)secureMode;
	
	wprintf(L"\n Executing %ls Completed ", functionName);
	return TRUE;
}

// Get Tag EPC starting with BEDD for Write Operations.
//
BOOLEAN getBEDDTag( )
{
	UINT8 beddTag[] = { 0xBE,0xDD };
	UINT8 ffTag[] = { 0x11,0x11 };
	UINT8 tagMask[] = { 0xFF,0xFF};
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,beddTag,16,tagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,ffTag,16,tagMask,16,{0} };
	POST_FILTER postFilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	if( wcscmp(api3Data.rCaps.modelName,L"75004") == 0 )
	{
		postFilter.lpRSSIRangeFilter = NULL;
	}

	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
	//addanWareSelectRecord( api3Data.hReader,beddTag,MEMORY_BANK_EPC,32,32,FILTER_ACTION_STATE_UNAWARE,STATE_UNAWARE_ACTION_SELECT_NOT_UNSELECT );
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,(LPPOST_FILTER)&postFilter,NULL,NULL,NULL);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	if( (api3Data.rfidStatus = RFID_GetReadTag( api3Data.hReader,pTagData ) ) == RFID_API_SUCCESS )
	{
		printTagDataWithResults( pTagData );
		memcpy(api3Data.pbeddTag->pTagID,pTagData->pTagID,pTagData->tagIDLength);
		api3Data.pbeddTag->tagIDLength = pTagData->tagIDLength;
	}
	else
	{
		wprintf(L"\n No Tags are Read, Verify Read Criteria\n");
	}
	
	RFID_DeallocateTag( api3Data.hReader,pTagData );

	if(api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		wprintf(L"\n Successfully Read BEDD Tag\n");
		return TRUE;
	}
	else
	{
		api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader,0);
		wprintf(L"\n Cannot Read BEDD Tag\n");
		return FALSE;
	}
	return FALSE;
}


// Initialize SIngulation parameters
//
void InitSingulationControl( SINGULATION_CONTROL* pSingControl,SESSION session,UINT16 tagPopulation,UINT16 tagTransitTimeMilliseconds,INVENTORY_STATE inventoryState,SL_FLAG slFlag)
{
	pSingControl->session = session;
	pSingControl->tagPopulation  = tagPopulation;
	pSingControl->stateAwareSingulationAction.inventoryState = inventoryState;
	pSingControl->stateAwareSingulationAction.perform = TRUE;
	pSingControl->stateAwareSingulationAction.slFlag = slFlag;
}

int GetFileContent(char* filePath, BYTE *pBuffer, int *pLen)
{
	FILE * pFile;
	size_t result;

	pFile = fopen ( filePath , "r" );
	if (pFile==NULL) return 0;

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	*pLen = ftell (pFile);
	//rewind(pFile);

	// copy the file into the buffer:
	result = fread (pBuffer, 1, *pLen, pFile);
	*pLen = result;

	/* the whole file is now loaded in the memory buffer. */
	fclose (pFile);
	return *pLen;
}
#ifndef linux
BOOLEAN GetFileContentWinAPI(wchar_t *pfilename,BYTE *pBuffer,int *pLen)
{
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[MAX_CERT_KEY_FILE_SIZE] = { 0 };
	OVERLAPPED ol = { 0 };

	HANDLE hFile = CreateFile(
								pfilename,
								GENERIC_READ,          // open for reading
								FILE_SHARE_READ,       // share for reading
								NULL,                  // default security
								OPEN_EXISTING,         // existing file only
								FILE_ATTRIBUTE_NORMAL, // normal file
								NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		wprintf(L"\n Could not Open the File");
		return FALSE;
	}
	if (FALSE == ReadFile(hFile, pBuffer, MAX_CERT_KEY_FILE_SIZE - 1, &dwBytesRead, &ol))
	{
		wprintf(L"\n Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}
	SleepEx(5000, TRUE);

	// This is the section of code that assumes the file is ANSI text. 
	// Modify this block for other data types if needed.

	if (dwBytesRead > 0)
	{
		pBuffer[dwBytesRead] = '\0'; // NULL character
		printf("%s\n", ReadBuffer);
	}
	else if (dwBytesRead == 0)
	{
		wprintf(L"\n No data read from file \n");
	}
	else
	{
		wprintf(L"\n ** Unexpected value for dwBytesRead ** \n");
	}

	*pLen = dwBytesRead;
	return TRUE;
}
#endif
// The securify .pem files are placed in the Directory where the application is running.
// Note that the path is picked automatically and the files are named as below.
// Root certificate		: cacert.pem
// Client Certificate	: client_crt.pem
// Client Key			: client_key.pem
//
void InitSecurityParameters( )
{
	BYTE g_ClientCertContent[MAX_CERT_KEY_FILE_SIZE];
	int	 g_ClientCertSize;
	BYTE g_ClientKeyContent[MAX_CERT_KEY_FILE_SIZE];
	int	g_ClientKeySize;
	BYTE g_RootCertContent[MAX_CERT_KEY_FILE_SIZE];
	int	 g_RootCertSize;
	char tempStr[2048];
	wchar_t wtempStr[2048];
	wchar_t keyContentData[MAX_CERT_KEY_FILE_SIZE];

	// Get the Current Directory 
	wchar_t pCurrentDirectory[1024];
	char* pANSICurrentDirectory;

	memset(&api3Data.SecureConnectionInfo,0,sizeof(SEC_CONNECTION_INFO));
#ifdef linux
	pANSICurrentDirectory = GetCurrentDir( NULL,0 );
	if( pANSICurrentDirectory == NULL )
	{
		wprintf(L"\n Could not get Current Directory ");
		
	}
	else
	{
		mbstowcs( pCurrentDirectory, pANSICurrentDirectory,1024);
		printf("\n ANSI CurrentDirectory %s",pANSICurrentDirectory);
		wprintf(L"\n WIDECHAR Current Directory is %ls",pCurrentDirectory);
	}
#endif

#if !WINCE && !linux
	GetCurrentDirectory(1024,pCurrentDirectory);
#endif
	if (wcslen(pCurrentDirectory) > 0)
	{
#ifndef linux
		rfid_swprintf(wtempStr,2048,L"%ls\\%ls",pCurrentDirectory,L"client_crt.pem");
#else
		rfid_swprintf(wtempStr,2048,L"%ls/%ls",pCurrentDirectory,L"client_crt.pem");
#endif
		wcstombs(tempStr, wtempStr, 2048);
		wprintf(L"\n Path :%ls",wtempStr);
#ifdef linux
		if(GetFileContent(tempStr, g_ClientCertContent, &g_ClientCertSize) > 0)
#else
		if (GetFileContentWinAPI(wtempStr,g_ClientCertContent, &g_ClientCertSize) == TRUE)
#endif
		{
			api3Data.SecureConnectionInfo.clientCertBuff = new BYTE[g_ClientCertSize];
			memcpy(api3Data.SecureConnectionInfo.clientCertBuff,g_ClientCertContent,g_ClientCertSize);;
			api3Data.SecureConnectionInfo.sizeCertBuff = g_ClientCertSize;
			mbstowcs( keyContentData, (char*)api3Data.SecureConnectionInfo.clientCertBuff,g_ClientCertSize);
			wprintf(L"\n clientCertBuff : %ls",keyContentData);
		}
		
		if (wcslen(pCurrentDirectory) > 0)
		{
#ifndef linux
			rfid_swprintf(wtempStr,2048,L"%ls\\%ls",pCurrentDirectory,L"client_key.pem");
#else
			rfid_swprintf(wtempStr,2048,L"%ls/%ls",pCurrentDirectory,L"client_key.pem");
#endif
			wprintf(L"\n Path :%ls",wtempStr);
			wcstombs(tempStr, wtempStr, 2048);
#ifdef linux
			if (GetFileContent(tempStr, g_ClientKeyContent, &g_ClientKeySize) > 0)
#else
			if (GetFileContentWinAPI(wtempStr, g_ClientKeyContent, &g_ClientKeySize) == TRUE)
#endif
			{
				api3Data.SecureConnectionInfo.clientKeyBuff = new BYTE[g_ClientKeySize];
				memcpy(api3Data.SecureConnectionInfo.clientKeyBuff,g_ClientKeyContent,g_ClientKeySize);;
				api3Data.SecureConnectionInfo.sizeKeyBuff = g_ClientKeySize;
				mbstowcs( keyContentData, (char*)api3Data.SecureConnectionInfo.clientKeyBuff,g_ClientKeySize);
				wprintf(L"\n clientKeyBuff : %ls",keyContentData);
			}
		}
		
		api3Data.SecureConnectionInfo.phraseBuff = (BYTE*)"abcd12345";
		api3Data.SecureConnectionInfo.sizePhraseBuff = strlen((char *)"abcd12345");
#ifndef linux
		rfid_swprintf(wtempStr,2048,L"%ls\\%ls",pCurrentDirectory,L"cacert.pem");
#else
		rfid_swprintf(wtempStr,2048,L"%ls/%ls",pCurrentDirectory,L"cacert.pem");
#endif
		wprintf(L"\n Path :%ls",wtempStr);
		wcstombs(tempStr, wtempStr, 2048);
#ifdef linux
		if (GetFileContent(tempStr, g_RootCertContent, &g_RootCertSize) > 0)
#else
		if (GetFileContentWinAPI(wtempStr, g_RootCertContent, &g_RootCertSize) == TRUE)
#endif
		{
			api3Data.SecureConnectionInfo.rootCertBuff = new BYTE[g_RootCertSize];
			memcpy(api3Data.SecureConnectionInfo.rootCertBuff,g_RootCertContent,g_RootCertSize);
			api3Data.SecureConnectionInfo.sizeRootCertBuff = g_RootCertSize;
			mbstowcs( keyContentData, (char*)api3Data.SecureConnectionInfo.rootCertBuff,g_RootCertSize);
			wprintf(L"\n sizeRootCertBuff : %ls",keyContentData);
		}

		api3Data.SecureConnectionInfo.secureMode = TRUE;

		// enable Validate peer certificate if only the option is enabled in the Reader WebConsole.
		//
		api3Data.SecureConnectionInfo.validatePeerCert = TRUE;
		wprintf(L"\n Secure Mode :%ls \t ValidatePeer:%ls",api3Data.SecureConnectionInfo.secureMode?L"ENABLED":L"NOT-ENABLED",api3Data.SecureConnectionInfo.validatePeerCert?L"ENABLED":L"NOT-ENABLED");
	}
}

// Initialize variables, Open File Handles, Connect to Reader using IP and Port
//
int initRFID( wchar_t* IPAddress, DWORD nPortNo ,READER_TYPE r )
{
	api3Data.pRfid3WideCharString = new wchar_t[512]; // To avoid NULL Data for this String.

	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	TAG_STORAGE_SETTINGS tgSettings;
	
	memset(&api3Data.conInfo,0,sizeof(CONNECTION_INFO));
	api3Data.dwTestCaseNo = 1;
	api3Data.dwPassCount = 0;
	api3Data.dwFailCount = 0;
	api3Data.dwSkipCount = 0;
	api3Data.dwFeatureCount = 1;

	api3Data.bContinue = TRUE;	// thread handler

	wcscpy( api3Data.szIPAddress, IPAddress );
	api3Data.rfidStatus = RFID_API_SUCCESS ; 
	//&&api3Data.pInfo = &antennaInfo;
	char fname[128];
	sprintf( fname,"%s--%s",PrintTime(),"API3Log.txt");
	api3Data.fpLog = fopen(fname,"w");

	api3Data.fpOut = fopen( "TestSummary.txt","w");
	
	wprintf(L"\n RFID_Connect called \n");

	// If Secure Mode is enabled, initialize security Parameters. This Call fills the Security Parameters of 
	// api3Data.SecureConnectionInfo
	if( api3Data.bSecureMode == TRUE && api3Data.apiVersion == RFID_API3_5_5)
	{
		InitSecurityParameters();
		api3Data.conInfo.lpSecConInfo = &api3Data.SecureConnectionInfo;
		api3Data.conInfo.version = api3Data.apiVersion;
	}
	else
	{
		api3Data.conInfo.version = api3Data.apiVersion;
	}
	
	api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort, 0, &api3Data.conInfo );
		
	// set tagStorage settings
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{		
		fprintf( api3Data.fpLog, "\n Failed to Connect to the Reader: ErrorText: %S", RFID_GetErrorDescription( api3Data.rfidStatus));
		wprintf(L"\n Failed to Connect to the Reader: ErrorText: %ls", RFID_GetErrorDescription( api3Data.rfidStatus));
		exit(0);
	}

	// tag storage settings
	//
	api3Data.rfidStatus = RFID_GetTagStorageSettings(api3Data.hReader,&tgSettings);
	tgSettings.maxMemoryBankByteCount = 512;
	tgSettings.maxTagCount = 30000;
	tgSettings.maxTagIDByteCount = 512;
	tgSettings.tagFields = ALL_TAG_FIELDS;
	api3Data.rfidStatus = RFID_SetTagStorageSettings( api3Data.hReader,&tgSettings );
	
	VERSION_INFO verInfo;
	VERSION_INFOA verInfoA;

	api3Data.rfidStatus = RFID_GetDllVersionInfo( &verInfo );
	wcscpy( api3Data.dllVersionInfo,verInfo.dllVersion);
	
	api3Data.rfidStatus = RFID_GetDllVersionInfoA(&verInfoA);
	wprintf(L"\n Dll Version InfoA: %ls\n", verInfo.dllVersion);

	api3Data.rfidStatus = RFID_GetReaderCaps( api3Data.hReader, &api3Data.rCaps );
	wprintf(L"\n NGE fwVersion Info %ls\n", api3Data.rCaps.firmWareVersion);
	api3Data.rfidStatus = RFID_SetTraceLevel(api3Data.hReader,TRACE_LEVEL_OFF);

	// Allocate memory and free in Deinit RFID
	api3Data.pbeddTag = RFID_AllocateTag(api3Data.hReader);
	api3Data.pfujitsu64Ktag = RFID_AllocateTag(api3Data.hReader);
	api3Data.pfujitsu8Ktag = RFID_AllocateTag(api3Data.hReader);
	api3Data.pImpinjTag = RFID_AllocateTag(api3Data.hReader);
	api3Data.pnxpTag = RFID_AllocateTag(api3Data.hReader);
	api3Data.pG2V2Tag = RFID_AllocateTag(api3Data.hReader);
	
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	return 1;
}


// De- Initialize variables, Close File Handles, Dis-Connect to Reader using IP and Port
//
int DeInitRFID( void )
{
	if( api3Data.pRfid3WideCharString != NULL ) free(api3Data.pRfid3WideCharString);
	// free before losing the handle with disconnect
	if( api3Data.pbeddTag != NULL )api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,api3Data.pbeddTag );
	if( api3Data.pfujitsu64Ktag != NULL ) api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,api3Data.pfujitsu64Ktag );
	if( api3Data.pfujitsu8Ktag != NULL ) api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,api3Data.pfujitsu8Ktag );
	if( api3Data.pImpinjTag != NULL ) api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,api3Data.pImpinjTag );
	if( api3Data.pnxpTag != NULL ) api3Data.rfidStatus = RFID_DeallocateTag( api3Data.hReader,api3Data.pnxpTag );
	if( api3Data.pG2V2Tag != NULL) api3Data.rfidStatus = RFID_DeallocateTag(api3Data.hReader, api3Data.pG2V2Tag);

	api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	api3Data.hReader = (RFID_HANDLE32)0;
	
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		api3Data.rfidStatus = RFID_Connect( &api3Data.hReader,api3Data.szIPAddress,api3Data.dwPort, 0,0 );
		// Set radio mode to true if it was disabled
		//
		api3Data.rfidStatus = RFID_SetRadioPowerState( api3Data.hReader, TRUE );
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
		rfid3Sleep( 2000 );
		fprintf( api3Data.fpLog, "\n Failed close Reader: ErrorCode: %d", api3Data.rfidStatus);
	}	

    return TRUE;
}
void rfid3UnSupportedFeature( )
{
	wprintf(L"\n Work in Progress or functionality Not Supported\n");
}
// Convertion routine from Byte to Hexa String
//
char* Byte2HexString(UINT8* pTagIDData,UINT32 tagIDLength )
{
	memset(ptagBuffer,0,4096);
    char* pTagReportData = NULL;
	pTagReportData = ptagBuffer;
    for(UINT32 index = 0; index < tagIDLength; index++)
    {
        if(0 < index && index%2 == 0)
        {
                *pTagReportData++ = '-';
        }

        sprintf(pTagReportData,"%02X", pTagIDData[index]);
        while(*pTagReportData) pTagReportData++;
    }
	
	return ptagBuffer;
}
// Byte to Hex Wide Char String Conversion 
//
wchar_t* Byte2HexWString(UINT8* pTagIDData,UINT32 tagIDLength )
{
	memset(wtagBuffer,0,4096);
	char ptagBuffer[4096]={0,};
    char* pTagReportData = NULL;
 
	pTagReportData = ptagBuffer;
    for(UINT32 index = 0; index < tagIDLength; index++)
    {
        if(0 < index && index%2 == 0)
        {
                *pTagReportData++ = '-';
        }

        sprintf(pTagReportData,"%02X", pTagIDData[index]);
        while(*pTagReportData) pTagReportData++;
    }
	
	mbstowcs(wtagBuffer,ptagBuffer,4096);
	return wtagBuffer;
}
#if !defined PIXIE
void convertTagDataToResults(TAG_DATA* pTagData, wchar_t* resultBuffer, bool bPhaseEnabled)
{
	UINT8* pTagID = pTagData->pTagID;
	UINT8* pTagMBData = pTagData->pMemoryBankData;
	UINT32 epcLength = pTagData->tagIDLength;
	SYSTEMTIME localTime;

	char ansiBuffer[4096] = { 0, };
	wchar_t  memoryBankBuffer[128] = { 0, };
	wchar_t  accessMBBuffer[128] = { 0, };
	wchar_t  AccessResultBuffer[128] = { 0, };
	wchar_t  tagEventBuffer[128] = { 0, };
	wchar_t  tagLocationBuffer[128] = { 0, };
	wchar_t  AccessOpBuffer[128] = { 0, };
	wchar_t tagBuffer[MAX_PATH] = { 0, };
	wchar_t* pTagReportData = tagBuffer;
	int   index = 0;
#ifndef linux
	TIME_ZONE_INFORMATION tzInfo;
	GetTimeZoneInformation(&tzInfo);
#endif

	for (index = 0; index < epcLength; index++)
	{
		if (0 < index && index % 2 == 0)
		{
			*pTagReportData++ = L'-';
		}
		rfid_stprintf(pTagReportData, MAX_PATH, L"%02X", pTagID[index]);
		while (*pTagReportData) pTagReportData++;
	}

	if (pTagData->lpLocation)
	{
		rfid_stprintf(tagLocationBuffer, 128, L"Relative-Distance = %d", pTagData->lpLocation->relativeDistance);
	}

	if (pTagData->tagEvent == NONE)
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->seenTime.utcTime.firstSeenTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->seenTime.utcTime.firstSeenTimeStamp, sizeof(SYSTEMTIME));
#endif
	}
	else
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->tagEventTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->tagEventTimeStamp, sizeof(SYSTEMTIME));
#endif
	}

	numEventAll++;
	switch (pTagData->tagEvent)
	{
	case NONE:
		break;
	case UNKNOWN_STATE:
		break;
	case NEW_TAG_VISIBLE:
		numEventNewTag++;
		rfid_stprintf(tagEventBuffer, 128, L"NEW\t");
		break;
	case TAG_BACK_TO_VISIBILITY:
		numEventVisible++;
		rfid_stprintf(tagEventBuffer, 128, L"BACK\t");
		break;
	case TAG_NOT_VISIBLE:
		numEventInvisible++;
		rfid_stprintf(tagEventBuffer, 128, L"GONE\t");
		break;
	case TAG_MOVING:
		numEventMoving++;
		rfid_stprintf(tagEventBuffer, 128, L"MOVING\t");
		break;
	case TAG_STATIONARY:
		numEventStationary++;
		rfid_stprintf(tagEventBuffer, 128, L"STATION\t");
		break;
	default:
		rfid_stprintf(tagEventBuffer, 128, L"ERROR!!!!!!!!!");
		break;
	}

	if (pTagData->opCode != ACCESS_OPERATION_NONE)
		switch (pTagData->opCode)
		{
		case  ACCESS_OPERATION_READ:
			rfid_stprintf(AccessOpBuffer, 128, L"Read");
			break;
		case ACCESS_OPERATION_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_LOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Lock");
			break;
		case ACCESS_OPERATION_KILL:
			rfid_stprintf(AccessOpBuffer, 128, L"Kill");
			break;
		case ACCESS_OPERATION_BLOCK_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_BLOCK_ERASE:
			rfid_stprintf(AccessOpBuffer, 128, L"Erase");
			break;
		case ACCESS_OPERATION_RECOMMISSION:
			rfid_stprintf(AccessOpBuffer, 128, L"Recommision");
			break;
		case ACCESS_OPERATION_BLOCK_PERMALOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_NONE:
			rfid_stprintf(AccessOpBuffer, 128, L"None");
			break;
		case 	ACCESS_OPERATION_FUJITSU_CHANGE_WORDLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu:Change Word Lock");
			break;
		case ACCESS_OPERATION_FUJITSU_CHANGE_BLOCKLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Change Block lock");
			break;
		case ACCESS_OPERATION_FUJITSU_READ_BLOCKLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu:Read Block Lock");
			break;
		case ACCESS_OPERATION_FUJITSU_BURST_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Burst Write");
			break;
		case ACCESS_OPERATION_FUJITSU_BURST_ERASE:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Burst Erase");
			break;
		case ACCESS_OPERATION_FUJITSU_CHANGE_BLOCK_OR_AREA_GROUPPASSWORD:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Block/Area Grp Password");
			break;
		case ACCESS_OPERATION_FUJITSU_AREA_READLOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: Area Write Lock");
			break;
		case ACCESS_OPERATION_FUJITSU_AREA_WRITELOCK_WOPASSWORD:
			rfid_stprintf(AccessOpBuffer, 128, L"Fujitsu: WriteLock WO Password");
			break;
		case ACCESS_OPERATION_IMPINJ_QT_WRITE:
			rfid_stprintf(AccessOpBuffer, 128, L"Impinj: QT Write");
			break;
		case ACCESS_OPERATION_IMPINJ_QT_READ:
			rfid_stprintf(AccessOpBuffer, 128, L"Impinj: QT Read");
			break;
		case ACCESS_OPERATION_NXP_CHANGE_CONFIG:
			rfid_stprintf(AccessOpBuffer, 128, L"NXP: Chane Config");
			break;
		case ACCESS_OPERATION_AUTHENTICATE:
			rfid_stprintf(AccessOpBuffer, 128, L"G2V2: Authenticate");
			break;
		case ACCESS_OPERATION_READBUFFER:
			rfid_stprintf(AccessOpBuffer, 128, L"G2V2: ReadBuffer");
			break;
		case ACCESS_OPERATION_UNTRACEABLE:
			rfid_stprintf(AccessOpBuffer, 128, L"G2V2: Untraceable");
			break;
		default:
			rfid_stprintf(AccessOpBuffer, 128, L"ERROR!!!!!!!!!");
			break;
		}

	if (pTagData->opCode != ACCESS_OPERATION_NONE)
		switch (pTagData->opStatus)
		{
		case ACCESS_SUCCESS:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success", AccessOpBuffer);
			break;
		case ACCESS_TAG_NON_SPECIFIC_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Tag non-specific error", AccessOpBuffer);
			break;
		case ACCESS_READER_NON_SPECIFIC_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Reader non-specific error", AccessOpBuffer);
			break;
		case ACCESS_NO_RESPONSE_FROM_TAG:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: No response from Tag", AccessOpBuffer);
			break;
		case ACCESS_INSUFFICENT_POWER:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Insufficient power", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_LOCKED_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Tag memory locked", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_OVERRUN_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Tag memory overrun", AccessOpBuffer);
			break;
		case ACCESS_ZERO_KILL_PASSWORD_ERROR:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Zero kill password error", AccessOpBuffer);
			break;
		case ACCESS_TAG_IN_PROCESS_STILL_WORKING:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: In Process - Still working", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_STORED_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Stored response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_STORED_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Stored response with length", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_SEND_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Send response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_SUCCESS_SEND_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Success - Send response with length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_STORED_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Stored response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_STORED_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Stored response with length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_SEND_RESPONSE_WITHOUT_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Send response without length", AccessOpBuffer);
			break;
		case ACCESS_TAG_ERROR_SEND_RESPONSE_WITH_LENGTH:
			rfid_stprintf(AccessResultBuffer, 128, L"%ls: Error - Send response with length", AccessOpBuffer);
			break;
		default:
			rfid_stprintf(AccessResultBuffer, 128, L"ERROR!!!!!!!!!");
			break;
		}

	if ((pTagData->opCode == ACCESS_OPERATION_AUTHENTICATE) || (pTagData->opCode == ACCESS_OPERATION_READBUFFER) || (pTagData->opCode == ACCESS_OPERATION_CRYPTO))
	{
		if (pTagData->lpAccessOperationResult != 0)
		{
			wchar_t* pTagReportData = accessMBBuffer;
			UINT16 receivedBitCount = pTagData->lpAccessOperationResult->AuthenticateReadBufferResult.receivedBitCount;
			UINT8* pReceivedBitData = pTagData->lpAccessOperationResult->AuthenticateReadBufferResult.pReceivedBitData;

			rfid_stprintf(memoryBankBuffer, 128, L":BitLength(%d):", receivedBitCount);
			for (index = 0; index < (receivedBitCount + 7) / 8; index++)
			{
				if (0 < index && index % 2 == 0)
				{
					*pTagReportData++ = L'-';
				}
				rfid_stprintf(pTagReportData, 128, L"%02X", pReceivedBitData[index]);
				while (*pTagReportData) pTagReportData++;
			}
		}
	}

	if (pTagData->opCode != ACCESS_OPERATION_NONE && ACCESS_SUCCESS == pTagData->opStatus)
	{

		switch (pTagData->memoryBank)
		{
		case MEMORY_BANK_RESERVED:
			rfid_stprintf(memoryBankBuffer, 128, L"-RSD-");
			break;
		case MEMORY_BANK_EPC:
			rfid_stprintf(memoryBankBuffer, 128, L"-EPC-");
			break;
		case MEMORY_BANK_TID:
			rfid_stprintf(memoryBankBuffer, 128, L"-TID-");
			break;
		case MEMORY_BANK_USER:
			rfid_stprintf(memoryBankBuffer, 128, L"-USR-");
			break;
		}

		wchar_t* pTagReportData = accessMBBuffer;
		for (index = 0; index < pTagData->memoryBankDataLength; index++)
		{
			if (0 < index && index % 2 == 0)
			{
				*pTagReportData++ = L'-';
			}
			rfid_stprintf(pTagReportData, 128, L"%02X", pTagMBData[index]);
			while (*pTagReportData) pTagReportData++;
		}
	}
	if (bPhaseEnabled) {
		rfid_stprintf(resultBuffer, MAX_PATH, L"%2d/%2d/%2d %02d:%02d:%2d:%03d\t%ls\tAntenna:%2d\tPC:%5x\tXPC:%5x\tRSSI:%04d\t%ls %ls %ls %ls %ls\tPhase:%3.2f",
			localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
			tagEventBuffer,
			pTagData->antennaID, pTagData->PC,
			pTagData->XPC, pTagData->peakRSSI, tagBuffer, AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer, (180.0 / 32767)* pTagData->phaseInfo);
	}
	else {
		rfid_stprintf(resultBuffer, MAX_PATH, L"%2d/%2d/%2d %02d:%02d:%2d:%03d\t%ls\tAntenna:%2d\tPC:%5x\tXPC:%5x\tRSSI:%04d\t%ls %ls %ls %ls %ls",
			localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
			tagEventBuffer,
			pTagData->antennaID, pTagData->PC,
			pTagData->XPC, pTagData->peakRSSI, tagBuffer, AccessResultBuffer, memoryBankBuffer, accessMBBuffer, tagLocationBuffer);
		wprintf(L"%ls", resultBuffer);
	}

	wcstombs(ansiBuffer, resultBuffer, 4096);
	if (api3Data.fpCurLog != NULL)
	{
		fprintf(api3Data.fpCurLog, "%s%s", ansiBuffer, "<br>");
	}

}
#endif
// Print Routine for tag data results.
//
void printTagDataWithResults(TAG_DATA *pTagData)
{
	wchar_t resultBuffer[4096] = {0,};
	char ansiBuffer[4096] = {0,};
	UINT8*pTagID = pTagData->pTagID;
	UINT8*pTagMBData = pTagData->pMemoryBankData;
	UINT32 epcLength =  pTagData->tagIDLength;
#ifdef WIN32
	TIME_ZONE_INFORMATION tzInfo;
#endif
	SYSTEMTIME localTime;

	wchar_t  memoryBankBuffer[128] = {0,};
	wchar_t  accessMBBuffer[2048] = {0,};
	wchar_t  AccessResultBuffer[128] = {0,};
	wchar_t  tagEventBuffer[128] = {0,};
	wchar_t  tagLocationBuffer[128] = {0,};
	wchar_t  AccessOpBuffer[128] = {0,};
	wchar_t tagBuffer[260] = {0,};
	wchar_t* pTagReportData = tagBuffer;
	unsigned int   index = 0;
#ifndef linux
	GetTimeZoneInformation(&tzInfo);
#endif
	for(index = 0; index < epcLength; index++)
	{
		if(0 < index && index%2 == 0)
		{
			//*pTagReportData++ = L'-';
		}
		rfid_swprintf(pTagReportData, 2048, L"%02X", pTagID[index]);
		while(*pTagReportData) pTagReportData++;
	}

	if(pTagData->lpLocation)
	{
		rfid_swprintf(tagLocationBuffer, 128, L"Relative-Distance = %d", pTagData->lpLocation->relativeDistance);
	}

	if(pTagData->tagEvent == NONE)
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->seenTime.utcTime.firstSeenTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->seenTime.utcTime.firstSeenTimeStamp, sizeof(SYSTEMTIME));
#endif
	}
	else
	{
#if !WINCE && !linux
		SystemTimeToTzSpecificLocalTime(&tzInfo, &pTagData->tagEventTimeStamp, &localTime);
#else
		memcpy(&localTime, &pTagData->tagEventTimeStamp, sizeof(SYSTEMTIME));
#endif
	}

	switch(pTagData->tagEvent)
	{
	case NONE:
		break;
	case UNKNOWN_STATE:
		break;
	case NEW_TAG_VISIBLE:
		rfid_swprintf(tagEventBuffer, 128, L"NEW\t");
		break;
	case TAG_BACK_TO_VISIBILITY:
		rfid_swprintf(tagEventBuffer, 128, L"BACK\t");
		break;
	case TAG_NOT_VISIBLE:
		rfid_swprintf(tagEventBuffer, 128, L"GONE\t");
		break;
	default:
		rfid_swprintf(tagEventBuffer, 128, L"ERROR!!!!!!!!!");
		break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE)
		switch(pTagData->opCode)
	{
		case  ACCESS_OPERATION_READ:
			rfid_swprintf(AccessOpBuffer, 128, L"Read");
			break;
		case ACCESS_OPERATION_WRITE:
			rfid_swprintf(AccessOpBuffer, 128, L"Write");
			break;
		case ACCESS_OPERATION_LOCK:
			rfid_swprintf(AccessOpBuffer, 128, L"Lock");
			break;
		case ACCESS_OPERATION_KILL:
			rfid_swprintf(AccessOpBuffer, 128, L"Kill");
			break;
		case ACCESS_OPERATION_BLOCK_WRITE:
			rfid_swprintf(AccessOpBuffer, 128, L"BlkWrite");
			break;
		case ACCESS_OPERATION_BLOCK_ERASE:
			rfid_swprintf(AccessOpBuffer, 128, L"BlkErase");
			break;
		case ACCESS_OPERATION_RECOMMISSION:
			rfid_swprintf(AccessOpBuffer, 128, L"Recommision");
			break;
		case ACCESS_OPERATION_BLOCK_PERMALOCK:
			rfid_swprintf(AccessOpBuffer, 128, L"Blk-Permalock");
			break;
		case ACCESS_OPERATION_NONE:
			rfid_swprintf(AccessOpBuffer, 128, L"None");
			break;
		default:
			rfid_swprintf(AccessOpBuffer, 128, L"ERROR!!!!!!!!!");
			break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE)
	switch(pTagData->opStatus)
	{
		case ACCESS_SUCCESS:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Success", AccessOpBuffer);
			break;
		case ACCESS_TAG_NON_SPECIFIC_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag non-specific error", AccessOpBuffer);
			break;
		case ACCESS_READER_NON_SPECIFIC_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Reader non-specific error", AccessOpBuffer);
			break;
		case ACCESS_NO_RESPONSE_FROM_TAG:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: No response from Tag", AccessOpBuffer);
			break;
		case ACCESS_INSUFFICENT_POWER:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Insufficient power", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_LOCKED_ERROR	:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag memory locked", AccessOpBuffer);
			break;
		case ACCESS_TAG_MEMORY_OVERRUN_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Tag memory overrun", AccessOpBuffer);
			break;
		case ACCESS_ZERO_KILL_PASSWORD_ERROR:
			rfid_swprintf(AccessResultBuffer, 128, L"%ls: Zero kill password error", AccessOpBuffer);
			break;
		default:
			rfid_swprintf(AccessResultBuffer, 128, L"ERROR!!!!!!!!!");
			break;
	}

	if(pTagData->opCode != ACCESS_OPERATION_NONE && ACCESS_SUCCESS == pTagData->opStatus)
	{

		switch(pTagData->memoryBank)
		{
		case MEMORY_BANK_RESERVED:
			rfid_swprintf(memoryBankBuffer, 128, L"-RSD-");
			break;
		case MEMORY_BANK_EPC:
			rfid_swprintf(memoryBankBuffer, 128, L"-EPC-");
			break;
		case MEMORY_BANK_TID:
			rfid_swprintf(memoryBankBuffer, 128, L"-TID-");
			break;
		case MEMORY_BANK_USER:
			rfid_swprintf(memoryBankBuffer, 128, L"-USR-");
			break;
		}

		wchar_t* pTagReportData = accessMBBuffer;
		for(index = 0; index < pTagData->memoryBankDataLength; index++)
		{
			if(0 < index && index%2 == 0)
			{
				//*pTagReportData++ = L'-';
			}
			rfid_swprintf(pTagReportData, 2048, L"%02X", pTagMBData[index]);
			while(*pTagReportData) pTagReportData++;
		}
	}
	

	rfid_swprintf(resultBuffer, 4096,
		L"\n================ TAG READ =================\n"
		L"[Timestamp] %02d/%02d/%02d %02d:%02d:%02d.%03d\n"
		L"[Event]     %ls\n"
		L"[Antenna]   %2d\n"
		L"[CRC]       0x%05x\n"
		L"[PC]        0x%05x\n"
		L"[XPC]       0x%05x\n"
		L"[RSSI]      %02d\n"
		L"[Phase]     %3.2f\n"
		L"[Channel]   %02d\n"
		L"[Tag]       %ls\n"
		L"[Access]    %ls\n"
		L"[Bank]      %ls\n"
		L"[MB Data]   %ls\n"
		L"[Location]  %ls\n"
		L"[SeenCount] %d\n"
		L"==========================================\n\n",
		localTime.wDay, localTime.wMonth, localTime.wYear, localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds,
		tagEventBuffer,
		pTagData->antennaID,
		pTagData->CRC,
		pTagData->PC,
		pTagData->XPC,
		pTagData->peakRSSI,
		(180.0 / 32767) * pTagData->phaseInfo,
		pTagData->channelIndex,
		tagBuffer,
		AccessResultBuffer,
		memoryBankBuffer,
		accessMBBuffer,
		tagLocationBuffer,
		pTagData->tagSeenCount
	);

	wprintf(L"%ls", resultBuffer);
	wcstombs(ansiBuffer, resultBuffer, 4096);
	if (api3Data.fpCurLog != NULL)
	{
		fprintf(api3Data.fpCurLog, "%s%s", ansiBuffer, "<br>");
	}
}
// Prints Tag Data to the Console Out Put
//
void Print2Console(UINT16 antID, UINT16 crc,UINT16 PC,UINT32 XPC, wchar_t* szIP, UINT8*pTagID, UINT32 tagIDLength)
{
	wchar_t tagData[512];
    char  tagBuffer[512] = {0,};
    char* pTagReportData = tagBuffer;
    UINT16   index = 0;
    for( index = 0; index < tagIDLength; index++ )
    {
        if(0 < index && index%2 == 0)
        {
                *pTagReportData++ = '-';
        }
        sprintf(pTagReportData,"%02X", pTagID[index]);
        while(*pTagReportData) pTagReportData++;
    }
	
	mbstowcs(tagData,tagBuffer,512);
	
	wprintf(L"\n Tags From %ls antennaID:%d CRC %X PC %X XPC %X Tag ID: %ls",szIP, antID,crc,PC,XPC,tagData);
}

// Prints Tag Data to the Log
//
void Print2Log( wchar_t* mylogData,UINT16 antID,UINT16 crc,UINT16 PC,UINT32 XPC, wchar_t* szIP, UINT8*pTagID, UINT32 tagIDLength )
{
        char  tagBuffer[512] = {0,};
        char* pTagReportData = tagBuffer;
        UINT16   index = 0;
        for( index = 0; index < tagIDLength; index++ )
        {
            if(0 < index && index%2 == 0)
            {
                    *pTagReportData++ = '-';
            }
            sprintf(pTagReportData,"%02X", pTagID[index]);
            while(*pTagReportData) pTagReportData++;
        }

		fprintf(api3Data.fpLog,"\n %S %S antennaID:%d CRC %X PC %X XPC %X Tag ID: %s Result: %S",mylogData,szIP, antID,crc,PC,XPC,tagBuffer,RFID_GetErrorDescription( api3Data.rfidStatus ));
}
// Antenna Check
//
int IsAntennaConnected( RFID_HANDLE32 hReader,int antID )
{	
	BOOLEAN pStatus;
	UINT32 pGain;

	api3Data.rfidStatus = RFID_GetPhysicalAntennaProperties( hReader, antID,&pStatus, &pGain);
	
	if( api3Data.rfidStatus == RFID_API_SUCCESS && pStatus == TRUE )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

void SetLogText( char* logText )
{
	strcpy( api3Data.logText, logText );
}
void SetLogResult( UINT8 value )
{
	api3Data.logResult = value;	
}

void apiResultPassLog()
{
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		ERROR_INFO errorInfo;
	
		RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
		fprintf( api3Data.fpCurLog,"rfidStatusCode - %d <br>",errorInfo.rfidStatusCode);
		fprintf( api3Data.fpCurLog,"statusDesc - %S <br>",errorInfo.statusDesc);
		fprintf( api3Data.fpCurLog,"vendorMessage - %S \n", errorInfo.vendorMessage);
		fprintf( api3Data.fpCurLog,"<br><br>");
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
}

void apiResultFailLog()
{
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		if(api3Data.rfidStatus == RFID_API_INVALID_HANDLE)
		{
			fprintf( api3Data.fpCurLog,"%S",RFID_GetErrorDescription(api3Data.rfidStatus));
			fprintf( api3Data.fpCurLog,"<br><br>");
		}
		else
		{
			fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S<br>",RFID_GetErrorDescription(api3Data.rfidStatus));
			ERROR_INFO errorInfo;
		
			RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
			fprintf( api3Data.fpCurLog,"rfidStatusCode - %d<br>",errorInfo.rfidStatusCode);
			fprintf( api3Data.fpCurLog,"statusDesc - %S\n<br>",errorInfo.statusDesc);
			fprintf( api3Data.fpCurLog,"vendorMessage - %S\n", errorInfo.vendorMessage);
			fprintf( api3Data.fpCurLog,"<br><br>");
			fprintf( api3Data.fpCurLog,"<br><br>");
			DisplayLastErrorInfo();
		}
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"FAIL:%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
}
int logToResultFile( wchar_t* szTestNo )
{
	char szMsg[256];
	if( api3Data.logResult == TRUE )
	{
		strcpy(szMsg,"PASS");
		api3Data.dwPassCount++;
	}
	else if( api3Data.logResult == FALSE )
	{
		strcpy(szMsg,"FAIL");
		api3Data.dwFailCount++;
	}
	else
	{
		strcpy(szMsg,"ANALYZE THE LOG FILE FOR RESULT");
		api3Data.dwPassCount++;
	}
	api3Data.logResult = FALSE;
	wchar_t logText[512];
	wchar_t resultMsg[256];
	mbstowcs(logText,api3Data.logText,512);
	mbstowcs(resultMsg,szMsg,256);
	wprintf(L"\n%ls-%d:%ls:%ls\n",szTestNo,api3Data.dwTestCaseNo,logText,resultMsg); 
	fprintf( api3Data.fpCurResult,"%ls-%d:%s:%s\n",szTestNo,api3Data.dwTestCaseNo++,api3Data.logText,szMsg); 

	fflush(api3Data.fpCurLog );
	fflush( api3Data.fpCurResult );
	return TRUE;
}

void logToFile( char* pText )
{
	char szTemp[512];
	
	sprintf( szTemp,"TestCaseNo-%d:%s,%s ",api3Data.dwTestCaseNo,api3Data.logText,pText );
	fprintf( api3Data.fpLog,"\n %s \n",szTemp);	
}
void logCurrentExecution( char* pText )
{
	strcpy( api3Data.CurrExecution, pText );
	char text[512];
	sprintf( text,"%s_Result.txt",pText );
	api3Data.fpCurResult = fopen( text,"w");
	rfid3Sleep(2000);
	sprintf( text,"%s_Log.html",pText );
	api3Data.fpCurLog = fopen( text,"w");
	rfid3Sleep(2000);
}	

void LogPassFailCount( )		
{	
	fprintf( api3Data.fpOut,"\n %ls%ls: %-40s: %d: %d: %d",api3Data.apiVersionString,api3Data.dllVersionInfo, api3Data.CurrExecution,api3Data.dwPassCount,api3Data.dwFailCount,api3Data.dwSkipCount );
	api3Data.dwFeatureCount++;
	api3Data.dwPassCount = api3Data.dwFailCount = api3Data.dwSkipCount=0;
	fflush( api3Data.fpOut );
	if( api3Data.fpCurLog ) fclose( api3Data.fpCurLog );
	if( api3Data.fpCurResult) fclose( api3Data.fpCurResult );
}

wchar_t* GetCountryNamebyCode(int Code)
{
	switch (Code)
	{
		case 4:		wcscpy(CountryName, L"Afghanistan"); break;
		case 8:		wcscpy(CountryName, L"Albania"); break;
		case 10:	wcscpy(CountryName, L"	Antarctica"); break;
		case 12:	wcscpy(CountryName, L"	Algeria"); break;
		case 16:	wcscpy(CountryName, L"	American Samoa"); break;
		case 20:	wcscpy(CountryName, L"	Andorra"); break;
		case 24:	wcscpy(CountryName, L"	Angola"); break;
		case 28:	wcscpy(CountryName, L"	Antigua and Barbuda"); break;
		case 31:	wcscpy(CountryName, L"	Azerbaijan"); break;
		case 32:	wcscpy(CountryName, L"	Argentina"); break;
		case 36:	wcscpy(CountryName, L"	Australia"); break;
		case 40:	wcscpy(CountryName, L"	Austria"); break;
		case 44:	wcscpy(CountryName, L"	Bahamas (the)"); break;
		case 48:	wcscpy(CountryName, L"	Bahrain"); break;
		case 50:	wcscpy(CountryName, L"	Bangladesh"); break;
		case 51:	wcscpy(CountryName, L"	Armenia"); break;
		case 52:	wcscpy(CountryName, L"	Barbados"); break;
		case 56:	wcscpy(CountryName, L"	Belgium"); break;
		case 60:	wcscpy(CountryName, L"	Bermuda"); break;
		case 64:	wcscpy(CountryName, L"	Bhutan"); break;
		case 68:	wcscpy(CountryName, L"	Bolivia (Plurinational State of)"); break;
		case 70:	wcscpy(CountryName, L"	Bosnia and Herzegovina"); break;
		case 72:	wcscpy(CountryName, L"	Botswana"); break;
		case 74:	wcscpy(CountryName, L"	Bouvet Island"); break;
		case 76:	wcscpy(CountryName, L"	Brazil"); break;
		case 84:	wcscpy(CountryName, L"	Belize"); break;
		case 86:	wcscpy(CountryName, L"	British Indian Ocean Territory (the)"); break;
		case 90:	wcscpy(CountryName, L"	Solomon Islands"); break;
		case 92:	wcscpy(CountryName, L"	Virgin Islands (British)"); break;
		case 96:	wcscpy(CountryName, L"	Brunei Darussalam"); break;
		case 100:	wcscpy(CountryName, L"Bulgaria"); break;
		case 104:	wcscpy(CountryName, L"Myanmar"); break;
		case 108:	wcscpy(CountryName, L"Burundi"); break;
		case 112:	wcscpy(CountryName, L"Belarus"); break;
		case 116:	wcscpy(CountryName, L"Cambodia"); break;
		case 120:	wcscpy(CountryName, L"Cameroon"); break;
		case 124:	wcscpy(CountryName, L"Canada"); break;
		case 132:	wcscpy(CountryName, L"Cabo Verde"); break;
		case 136:	wcscpy(CountryName, L"Cayman Islands (the)"); break;
		case 140:	wcscpy(CountryName, L"Central African Republic (the)"); break;
		case 144:	wcscpy(CountryName, L"Sri Lanka"); break;
		case 148:	wcscpy(CountryName, L"Chad"); break;
		case 152:	wcscpy(CountryName, L"Chile"); break;
		case 156:	wcscpy(CountryName, L"China"); break;
		case 158:	wcscpy(CountryName, L"Taiwan (Province of China)"); break;
		case 162:	wcscpy(CountryName, L"Christmas Island"); break;
		case 166:	wcscpy(CountryName, L"Cocos (Keeling) Islands (the)"); break;
		case 170:	wcscpy(CountryName, L"Colombia"); break;
		case 174:	wcscpy(CountryName, L"Comoros (the)"); break;
		case 175:	wcscpy(CountryName, L"Mayotte"); break;
		case 178:	wcscpy(CountryName, L"Congo (the)"); break;
		case 180:	wcscpy(CountryName, L"Congo (the Democratic Republic of the)"); break;
		case 184:	wcscpy(CountryName, L"Cook Islands (the)"); break;
		case 188:	wcscpy(CountryName, L"Costa Rica"); break;
		case 191:	wcscpy(CountryName, L"Croatia"); break;
		case 192:	wcscpy(CountryName, L"Cuba"); break;
		case 196:	wcscpy(CountryName, L"Cyprus"); break;
		case 203:	wcscpy(CountryName, L"Czechia"); break;
		case 204:	wcscpy(CountryName, L"Benin"); break;
		case 208:	wcscpy(CountryName, L"Denmark"); break;
		case 212:	wcscpy(CountryName, L"Dominica"); break;
		case 214:	wcscpy(CountryName, L"Dominican Republic (the)"); break;
		case 218:	wcscpy(CountryName, L"Ecuador"); break;
		case 222:	wcscpy(CountryName, L"El Salvador"); break;
		case 226:	wcscpy(CountryName, L"Equatorial Guinea"); break;
		case 231:	wcscpy(CountryName, L"Ethiopia"); break;
		case 232:	wcscpy(CountryName, L"Eritrea"); break;
		case 233:	wcscpy(CountryName, L"Estonia"); break;
		case 234:	wcscpy(CountryName, L"Faroe Islands (the)"); break;
		case 238:	wcscpy(CountryName, L"Falkland Islands (the) [Malvinas]"); break;
		case 239:	wcscpy(CountryName, L"South Georgia and the South Sandwich Islands"); break;
		case 242:	wcscpy(CountryName, L"Fiji"); break;
		case 246:	wcscpy(CountryName, L"Finland"); break;
		case 248:	wcscpy(CountryName, L"Aland Islands"); break;
		case 250:	wcscpy(CountryName, L"France"); break;
		case 254:	wcscpy(CountryName, L"French Guiana"); break;
		case 258:	wcscpy(CountryName, L"French Polynesia"); break;
		case 260:	wcscpy(CountryName, L"French Southern Territories (the)"); break;
		case 262:	wcscpy(CountryName, L"Djibouti"); break;
		case 266:	wcscpy(CountryName, L"Gabon"); break;
		case 268:	wcscpy(CountryName, L"Georgia"); break;
		case 270:	wcscpy(CountryName, L"Gambia (the)"); break;
		case 275:	wcscpy(CountryName, L"Palestine, State of"); break;
		case 276:	wcscpy(CountryName, L"Germany"); break;
		case 288:	wcscpy(CountryName, L"Ghana"); break;
		case 292:	wcscpy(CountryName, L"Gibraltar"); break;
		case 296:	wcscpy(CountryName, L"Kiribati"); break;
		case 300:	wcscpy(CountryName, L"Greece"); break;
		case 304:	wcscpy(CountryName, L"Greenland"); break;
		case 308:	wcscpy(CountryName, L"Grenada"); break;
		case 312:	wcscpy(CountryName, L"Guadeloupe"); break;
		case 316:	wcscpy(CountryName, L"Guam"); break;
		case 320:	wcscpy(CountryName, L"Guatemala"); break;
		case 324:	wcscpy(CountryName, L"Guinea"); break;
		case 328:	wcscpy(CountryName, L"Guyana"); break;
		case 332:	wcscpy(CountryName, L"Haiti"); break;
		case 334:	wcscpy(CountryName, L"Heard Island and McDonald Islands"); break;
		case 336:	wcscpy(CountryName, L"Holy See (the)"); break;
		case 340:	wcscpy(CountryName, L"Honduras"); break;
		case 344:	wcscpy(CountryName, L"Hong Kong"); break;
		case 348:	wcscpy(CountryName, L"Hungary"); break;
		case 352:	wcscpy(CountryName, L"Iceland"); break;
		case 356:	wcscpy(CountryName, L"India"); break;
		case 360:	wcscpy(CountryName, L"Indonesia"); break;
		case 364:	wcscpy(CountryName, L"Iran (Islamic Republic of)"); break;
		case 368:	wcscpy(CountryName, L"Iraq"); break;
		case 372:	wcscpy(CountryName, L"Ireland"); break;
		case 376:	wcscpy(CountryName, L"Israel"); break;
		case 380:	wcscpy(CountryName, L"Italy"); break;
		case 384:	wcscpy(CountryName, L"Ivory Coast"); break;
		case 388:	wcscpy(CountryName, L"Jamaica"); break;
		case 392:	wcscpy(CountryName, L"Japan"); break;
		case 398:	wcscpy(CountryName, L"Kazakhstan"); break;
		case 400:	wcscpy(CountryName, L"Jordan"); break;
		case 404:	wcscpy(CountryName, L"Kenya"); break;
		case 408:	wcscpy(CountryName, L"Korea (the Democratic People's Republic of)"); break;
		case 410:	wcscpy(CountryName, L"Korea (the Republic of)"); break;
		case 414:	wcscpy(CountryName, L"Kuwait"); break;
		case 417:	wcscpy(CountryName, L"Kyrgyzstan"); break;
		case 418:	wcscpy(CountryName, L"Lao People's Democratic Republic (the)"); break;
		case 422:	wcscpy(CountryName, L"Lebanon"); break;
		case 426:	wcscpy(CountryName, L"Lesotho"); break;
		case 428:	wcscpy(CountryName, L"Latvia"); break;
		case 430:	wcscpy(CountryName, L"Liberia"); break;
		case 434:	wcscpy(CountryName, L"Libya"); break;
		case 438:	wcscpy(CountryName, L"Liechtenstein"); break;
		case 440:	wcscpy(CountryName, L"Lithuania"); break;
		case 442:	wcscpy(CountryName, L"Luxembourg"); break;
		case 446:	wcscpy(CountryName, L"Macao"); break;
		case 450:	wcscpy(CountryName, L"Madagascar"); break;
		case 454:	wcscpy(CountryName, L"Malawi"); break;
		case 458:	wcscpy(CountryName, L"Malaysia"); break;
		case 462:	wcscpy(CountryName, L"Maldives"); break;
		case 466:	wcscpy(CountryName, L"Mali"); break;
		case 470:	wcscpy(CountryName, L"Malta"); break;
		case 474:	wcscpy(CountryName, L"Martinique"); break;
		case 478:	wcscpy(CountryName, L"Mauritania"); break;
		case 480:	wcscpy(CountryName, L"Mauritius"); break;
		case 484:	wcscpy(CountryName, L"Mexico"); break;
		case 492:	wcscpy(CountryName, L"Monaco"); break;
		case 496:	wcscpy(CountryName, L"Mongolia"); break;
		case 498:	wcscpy(CountryName, L"Moldova (the Republic of)"); break;
		case 499:	wcscpy(CountryName, L"Montenegro"); break;
		case 500:	wcscpy(CountryName, L"Montserrat"); break;
		case 504:	wcscpy(CountryName, L"Morocco"); break;
		case 508:	wcscpy(CountryName, L"Mozambique"); break;
		case 512:	wcscpy(CountryName, L"Oman"); break;
		case 516:	wcscpy(CountryName, L"Namibia"); break;
		case 520:	wcscpy(CountryName, L"Nauru"); break;
		case 524:	wcscpy(CountryName, L"Nepal"); break;
		case 528:	wcscpy(CountryName, L"Netherlands (the)"); break;
		case 531:	wcscpy(CountryName, L"Curacao"); break;
		case 533:	wcscpy(CountryName, L"Aruba"); break;
		case 534:	wcscpy(CountryName, L"Sint Maarten (Dutch part)"); break;
		case 535:	wcscpy(CountryName, L"Bonaire, Sint Eustatius and Saba"); break;
		case 540:	wcscpy(CountryName, L"New Caledonia"); break;
		case 548:	wcscpy(CountryName, L"Vanuatu"); break;
		case 554:	wcscpy(CountryName, L"New Zealand"); break;
		case 558:	wcscpy(CountryName, L"Nicaragua"); break;
		case 562:	wcscpy(CountryName, L"Niger (the)"); break;
		case 566:	wcscpy(CountryName, L"Nigeria"); break;
		case 570:	wcscpy(CountryName, L"Niue"); break;
		case 574:	wcscpy(CountryName, L"Norfolk Island"); break;
		case 578:	wcscpy(CountryName, L"Norway"); break;
		case 580:	wcscpy(CountryName, L"Northern Mariana Islands (the)"); break;
		case 581:	wcscpy(CountryName, L"United States Minor Outlying Islands (the)"); break;
		case 583:	wcscpy(CountryName, L"Micronesia (Federated States of)"); break;
		case 584:	wcscpy(CountryName, L"Marshall Islands (the)"); break;
		case 585:	wcscpy(CountryName, L"Palau"); break;
		case 586:	wcscpy(CountryName, L"Pakistan"); break;
		case 591:	wcscpy(CountryName, L"Panama"); break;
		case 598:	wcscpy(CountryName, L"Papua New Guinea"); break;
		case 600:	wcscpy(CountryName, L"Paraguay"); break;
		case 604:	wcscpy(CountryName, L"Peru"); break;
		case 608:	wcscpy(CountryName, L"Philippines (the)"); break;
		case 612:	wcscpy(CountryName, L"Pitcairn"); break;
		case 616:	wcscpy(CountryName, L"Poland"); break;
		case 620:	wcscpy(CountryName, L"Portugal"); break;
		case 624:	wcscpy(CountryName, L"Guinea-Bissau"); break;
		case 626:	wcscpy(CountryName, L"Timor-Leste"); break;
		case 630:	wcscpy(CountryName, L"Puerto Rico"); break;
		case 634:	wcscpy(CountryName, L"Qatar"); break;
		case 638:	wcscpy(CountryName, L"Reunion"); break;
		case 642:	wcscpy(CountryName, L"Romania"); break;
		case 643:	wcscpy(CountryName, L"Russian Federation (the)"); break;
		case 646:	wcscpy(CountryName, L"Rwanda"); break;
		case 652:	wcscpy(CountryName, L"Saint Barthelemy"); break;
		case 654:	wcscpy(CountryName, L"Saint Helena, Ascension and Tristan da Cunha"); break;
		case 659:	wcscpy(CountryName, L"Saint Kitts and Nevis"); break;
		case 660:	wcscpy(CountryName, L"Anguilla"); break;
		case 662:	wcscpy(CountryName, L"Saint Lucia"); break;
		case 663:	wcscpy(CountryName, L"Saint Martin (French part)"); break;
		case 666:	wcscpy(CountryName, L"Saint Pierre and Miquelon"); break;
		case 670:	wcscpy(CountryName, L"Saint Vincent and the Grenadines"); break;
		case 674:	wcscpy(CountryName, L"San Marino"); break;
		case 678:	wcscpy(CountryName, L"Sao Tome and Principe"); break;
		case 682:	wcscpy(CountryName, L"Saudi Arabia"); break;
		case 686:	wcscpy(CountryName, L"Senegal"); break;
		case 688:	wcscpy(CountryName, L"Serbia"); break;
		case 690:	wcscpy(CountryName, L"Seychelles"); break;
		case 694:	wcscpy(CountryName, L"Sierra Leone"); break;
		case 702:	wcscpy(CountryName, L"Singapore"); break;
		case 703:	wcscpy(CountryName, L"Slovakia"); break;
		case 704:	wcscpy(CountryName, L"VietNam"); break;
		case 705:	wcscpy(CountryName, L"Slovenia"); break;
		case 706:	wcscpy(CountryName, L"Somalia"); break;
		case 710:	wcscpy(CountryName, L"South Africa"); break;
		case 716:	wcscpy(CountryName, L"Zimbabwe"); break;
		case 724:	wcscpy(CountryName, L"Spain"); break;
		case 728:	wcscpy(CountryName, L"South Sudan"); break;
		case 729:	wcscpy(CountryName, L"Sudan (the)"); break;
		case 732:	wcscpy(CountryName, L"Western Sahara"); break;
		case 740:	wcscpy(CountryName, L"Suriname"); break;
		case 744:	wcscpy(CountryName, L"Svalbard and Jan Mayen"); break;
		case 748:	wcscpy(CountryName, L"Eswatini"); break;
		case 752:	wcscpy(CountryName, L"Sweden"); break;
		case 756:	wcscpy(CountryName, L"Switzerland"); break;
		case 760:	wcscpy(CountryName, L"Syrian Arab Republic"); break;
		case 762:	wcscpy(CountryName, L"Tajikistan"); break;
		case 764:	wcscpy(CountryName, L"Thailand"); break;
		case 768:	wcscpy(CountryName, L"Togo"); break;
		case 772:	wcscpy(CountryName, L"Tokelau"); break;
		case 776:	wcscpy(CountryName, L"Tonga"); break;
		case 780:	wcscpy(CountryName, L"Trinidad and Tobago"); break;
		case 784:	wcscpy(CountryName, L"United Arab Emirates (the)"); break;
		case 788:	wcscpy(CountryName, L"Tunisia"); break;
		case 792:	wcscpy(CountryName, L"Turkey"); break;
		case 795:	wcscpy(CountryName, L"Turkmenistan"); break;
		case 796:	wcscpy(CountryName, L"Turks and Caicos Islands (the)"); break;
		case 798:	wcscpy(CountryName, L"Tuvalu"); break;
		case 800:	wcscpy(CountryName, L"Uganda"); break;
		case 804:	wcscpy(CountryName, L"Ukraine"); break;
		case 807:	wcscpy(CountryName, L"Republic of North Macedonia"); break;
		case 818:	wcscpy(CountryName, L"Egypt"); break;
		case 826:	wcscpy(CountryName, L"United Kingdom of Great Britain and Northern Ireland (the)"); break;
		case 831:	wcscpy(CountryName, L"Guernsey"); break;
		case 832:	wcscpy(CountryName, L"Jersey"); break;
		case 833:	wcscpy(CountryName, L"Isle of Man"); break;
		case 834:	wcscpy(CountryName, L"Tanzania, United Republic of"); break;
		case 840:	wcscpy(CountryName, L"United States of America (the)"); break;
		case 850:	wcscpy(CountryName, L"Virgin Islands (U.S.)"); break;
		case 854:	wcscpy(CountryName, L"Burkina Faso"); break;
		case 858:	wcscpy(CountryName, L"Uruguay"); break;
		case 860:	wcscpy(CountryName, L"Uzbekistan"); break;
		case 862:	wcscpy(CountryName, L"Venezuela (Bolivarian Republic of)"); break;
		case 876:	wcscpy(CountryName, L"Wallis and Futuna"); break;
		case 882:	wcscpy(CountryName, L"Samoa"); break;
		case 887:	wcscpy(CountryName, L"Yemen"); break;
		case 894:	wcscpy(CountryName, L"Zambia"); break;
		default:
			wcscpy(CountryName, L"UnKnown Country Code"); break;
			break;
	}
	return CountryName;
}

// Get the required tags for regression
//
BOOLEAN getTwoTags( UINT8* EPC1, UINT8* EPC2,ANTENNA_INFO* pInfo,int attempt )
{
	fprintf( api3Data.fpLog,"\n\n **************Calling routine getTwoTags()*************");
	BOOLEAN bTag1,bTag2,bTag3;
	bTag1 = bTag2 = bTag3 = FALSE;

	UINT8 TagMask[] = { 0xFF,0xFF };
	
	LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );

	READ_ACCESS_PARAMS rParams = { MEMORY_BANK_EPC,4,12,0,{0} }; 
	TAG_PATTERN tpA = { MEMORY_BANK_EPC,32,EPC1,16,TagMask,16,{0} };
	TAG_PATTERN tpB = { MEMORY_BANK_EPC,32,EPC2,16,TagMask,16,{0} };
	POST_FILTER postfilter = {{0,0},&tpA,&tpB,A_AND_NOTB,NULL,NULL };

	if( api3Data.apiVersion ==  RFID_API3_5_1 || api3Data.apiVersion ==  RFID_API3_5_5  )
	{
		postfilter.lpRSSIRangeFilter = NULL;
	}
	
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,0);
	rfid3Sleep( 5000 );
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);

	// Read A22F TAG and B22F TAG

	fprintf( api3Data.fpLog, "\n------------------ TAG1---------------------\n");
	{
		BOOLEAN buser = FALSE;BOOLEAN tid = FALSE; 
		while( RFID_API_SUCCESS == RFID_GetReadTag( api3Data.hReader,pTagData ) )
		{
			// Check if the Tag is desired Tag for Regression
			if (pTagData->pTagID[0] == 0xA2 && !bTag1  )
			{
				memcpy(MyTags[0].epc, pTagData->pTagID, pTagData->tagIDLength);
				MyTags[0].PC = pTagData->PC;
				MyTags[0].CRC = pTagData->CRC;

				MyTags[0].epclen = pTagData->tagIDLength;
				//print EPC
				printTagDataWithResults(pTagData);
				//get TID memory bank
				rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
				rParams.accessPassword = 0;
				wprintf(L"\n Attempt %d, Reading TID memory bank\n", attempt);
				for (int iterCnt = 0; iterCnt < 10; iterCnt++)
				{
					RFID_STATUS rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0);
					if (RFID_API_SUCCESS == rfidStatus)
					{
						tid = TRUE;
						break;
					}
					else
					{
						wprintf(L"\n Error Reading memory Bank: %ls\n", RFID_GetErrorDescription(rfidStatus));
					}
				}
				
				if (pTagData->memoryBank != MEMORY_BANK_TID)
				{
					fprintf(api3Data.fpLog, "\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}
				memcpy(MyTags[0].tid, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				MyTags[0].tidlen = pTagData->memoryBankDataLength;
				printTagDataWithResults(pTagData);
				// User memory bank
				rParams.memoryBank = MEMORY_BANK_USER; rParams.byteOffset = 0; rParams.byteCount = 32;

				wprintf(L"\n attempt %d Reading User memory bank\n", attempt);
				for (int iterCnt = 0; iterCnt < 5; iterCnt++)
				{
					if (RFID_API_SUCCESS == RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0))
					{
						buser = TRUE;
						break;
					}
				}

				
				if (pTagData->memoryBank != MEMORY_BANK_USER)
				{
					fprintf(api3Data.fpLog, "\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}
				memcpy(MyTags[0].user, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				MyTags[0].userlen = pTagData->memoryBankDataLength;
				printTagDataWithResults(pTagData);

				if (buser && tid)
				{
					bTag1 = TRUE;
				}
				else
				{
					bTag1 = FALSE;
				}
				// Move to next tag after processing Tag 1
				continue;
			}

			// Check if the Tag is desired Tag for Regression
			if (pTagData->pTagID[0] == 0xB2 && !bTag2 )
			{
				memcpy(MyTags[1].epc, pTagData->pTagID, pTagData->tagIDLength);
				MyTags[1].PC = pTagData->PC;
				MyTags[1].CRC = pTagData->CRC;
				MyTags[1].epclen = pTagData->tagIDLength;
				printTagDataWithResults(pTagData);
				// get TID memory bank			
				rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
				rParams.accessPassword = 0;
				wprintf(L"\n attempt %d Reading TID memory bank\n", attempt);
				for (int iterCnt = 0; iterCnt < 10; iterCnt++)
				{
					if ((api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0)) == RFID_API_SUCCESS)
					{
						tid = TRUE;
						break;
					}
					wprintf(L"\n rfidStatus: %ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));
				}

				if (pTagData->memoryBank != MEMORY_BANK_TID)
				{
					fprintf(api3Data.fpLog, "\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}

				memcpy(MyTags[1].tid, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				MyTags[1].tidlen = pTagData->memoryBankDataLength;
				printTagDataWithResults(pTagData);
				// User memory bank
				rParams.memoryBank = MEMORY_BANK_USER; rParams.byteOffset = 0; rParams.byteCount = 32;
				wprintf(L"\n attempt : %d Reading User memory bank\n", attempt);
				for (int iterCnt = 0; iterCnt < 10; iterCnt++)
				{
					if (( api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0) ) == RFID_API_SUCCESS)
					{
						buser = TRUE;
						break;
					}

					wprintf(L"\n rfidStatus: %ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));
				}

				if (pTagData->memoryBank != MEMORY_BANK_USER)
				{
					fprintf(api3Data.fpLog, "\n This is not correct memory bank we are looking for, exiting\n");
					return FALSE;
				}
				memcpy(MyTags[1].user, pTagData->pMemoryBankData, pTagData->memoryBankDataLength);
				MyTags[1].userlen = pTagData->memoryBankDataLength;
				printTagDataWithResults(pTagData);
				if (buser && tid)
				{
					bTag2 = TRUE;
				}
				else bTag2 = FALSE;
			}
			// Check if the Tag is desired Tag for Regression
			//
		if (pTagData->pTagID[0] == 0xDA && !bTag3 )
		{
			printTagDataWithResults(pTagData);
			memcpy(api3Data.pG2V2Tag->pTagID, pTagData->pTagID, pTagData->tagIDLength);
			api3Data.pG2V2Tag->tagIDLength = pTagData->tagIDLength;

			// get TID memory bank
			rParams.memoryBank = MEMORY_BANK_TID; rParams.byteOffset = 0; rParams.byteCount = 12;
			//rParams.accessPassword = 0x00000001;
			wprintf(L"\n attempt %d Reading TID memory bank\n", attempt);
			for (int iterCnt = 0; iterCnt < 10; iterCnt++)
			{
				api3Data.rfidStatus = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, &rParams, NULL, NULL, pTagData, 0);

				wprintf(L"\n rfidStatus: %ls\n", RFID_GetErrorDescription(api3Data.rfidStatus));

				if (api3Data.rfidStatus == RFID_API_SUCCESS)
				{
					printTagDataWithResults(pTagData);
					bTag3 = TRUE;
					fprintf(api3Data.fpLog, "\n Successfully read TID for DADACAFE tag\n");
					break;
				}
			}
		}
		
		if (bTag1 && bTag2 &&  bTag3 ) break;
	}
		
	}

	if( pTagData != NULL ) RFID_DeallocateTag( api3Data.hReader,pTagData);
	fflush(api3Data.fpLog);

	if( bTag1 && bTag2 )
	{
		wprintf(L"\nSuccessfully Read the TAGS for Regression\n");
		return TRUE;
	}
	else
	{
		wprintf(L"\n Could not Read Tags A22F and B22F for Regression...Verify Tags in the FOV of the Reader\n");
		return FALSE;
	}
}

// Read Memory Bank - Helper function for reading TID and User memory
//
BOOLEAN ReadMemoryBank(LPTAG_DATA pTagData, MEMORY_BANK bank, UINT32 offset, UINT32 length)
{
	READ_ACCESS_PARAMS rParams;
	rParams.memoryBank = bank;
	rParams.byteOffset = offset;
	rParams.byteCount = length;
	rParams.accessPassword = 0;

	RFID_STATUS status = RFID_Read(api3Data.hReader, pTagData->pTagID, pTagData->tagIDLength, 
									&rParams, NULL, NULL, pTagData, 0);
	
	if (status == RFID_API_SUCCESS)
	{
		wprintf(L"  Successfully read %ls memory\n", 
				bank == MEMORY_BANK_TID ? L"TID" : 
				bank == MEMORY_BANK_USER ? L"User" : L"Memory");
		return TRUE;
	}
	else
	{
		wprintf(L"  Error reading %ls: %ls\n", 
				bank == MEMORY_BANK_TID ? L"TID" : 
				bank == MEMORY_BANK_USER ? L"User" : L"Memory",
				RFID_GetErrorDescription(status));
		return FALSE;
	}
}

// Perform Inventory and Read Tag Memory - Helper function for regression tests
// This function performs inventory, reads TID and User memory for all tags found
//
BOOLEAN PerformInventoryAndReadMemory(UINT32 inventoryDurationMs, BOOLEAN bReadTID, BOOLEAN bReadUser)
{
	wchar_t functionName[256];
	mbstowcs(functionName, __FUNCTION__, 256);
	wprintf(L"\n Executing %ls Started ", functionName);
	
	LPTAG_DATA pTagData = RFID_AllocateTag(api3Data.hReader);
	if (!pTagData)
	{
		wprintf(L"\n Failed to allocate tag structure");
		return FALSE;
	}

	// Purge old tags
	api3Data.rfidStatus = RFID_PurgeTags(api3Data.hReader, 0);
	
	// Start inventory
	wprintf(L"\n Starting inventory...");
	api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader, NULL, NULL, NULL, 0);
	if (api3Data.rfidStatus != RFID_API_SUCCESS)
	{
		wprintf(L"\n Inventory failed: %ls", RFID_GetErrorDescription(api3Data.rfidStatus));
		RFID_DeallocateTag(api3Data.hReader, pTagData);
		return FALSE;
	}
	
	wprintf(L"\n Inventory started, waiting %d ms...", inventoryDurationMs);
	rfid3Sleep(inventoryDurationMs);
	
	// Stop inventory
	api3Data.rfidStatus = RFID_StopInventory(api3Data.hReader);
	wprintf(L"\n Inventory stopped");
	
	// Read tags and their memory
	UINT32 tagCount = 0;
	wprintf(L"\n Reading tags...");
	
	while (RFID_GetReadTag(api3Data.hReader, pTagData) == RFID_API_SUCCESS)
	{
		tagCount++;
		
		// Print basic tag info
		wprintf(L"\n === Tag %d ===", tagCount);
		wprintf(L"\n EPC: %ls", Byte2HexWString(pTagData->pTagID, pTagData->tagIDLength));
		wprintf(L"\n PC: %04X, CRC: %04X", pTagData->PC, pTagData->CRC);
		wprintf(L"\n RSSI: %d dBm, Count: %u", pTagData->peakRSSI, pTagData->tagSeenCount);
		
		// Read TID if requested
		if (bReadTID)
		{
			wprintf(L"\n Reading TID...");
			if (ReadMemoryBank(pTagData, MEMORY_BANK_TID, 0, 12))
			{
				wprintf(L" TID: %ls", Byte2HexWString(pTagData->pMemoryBankData, pTagData->memoryBankDataLength));
			}
		}
		
		// Read User Memory if requested
		if (bReadUser)
		{
			wprintf(L"\n Reading User Memory...");
			if (ReadMemoryBank(pTagData, MEMORY_BANK_USER, 0, 32))
			{
				wprintf(L" User: %ls", Byte2HexWString(pTagData->pMemoryBankData, pTagData->memoryBankDataLength));
			}
		}
		
		wprintf(L"\n");
	}
	
	wprintf(L"\n Total tags read: %d", tagCount);
	wprintf(L"\n Executing %ls Completed ", functionName);
	
	// Cleanup
	RFID_DeallocateTag(api3Data.hReader, pTagData);
	
	return (tagCount > 0);
}

// Print Tag Memory Data - Helper to display tag memory information
//
void PrintTagMemoryData(LPTAG_DATA pTagData)
{
	if (!pTagData) return;

	// Print EPC
	wprintf(L"\n=== Tag Data ===");
	wprintf(L"\n EPC (ID): %ls", Byte2HexWString(pTagData->pTagID, pTagData->tagIDLength));
	wprintf(L"\n PC: %04X, CRC: %04X", pTagData->PC, pTagData->CRC);
	wprintf(L"\n RSSI: %d dBm, Count: %u", pTagData->peakRSSI, pTagData->tagSeenCount);

	// Print memory bank data if available
	if (pTagData->memoryBankDataLength > 0)
	{
		if (pTagData->memoryBank == MEMORY_BANK_TID)
		{
			wprintf(L"\n TID: ");
		}
		else if (pTagData->memoryBank == MEMORY_BANK_USER)
		{
			wprintf(L"\n USER: ");
		}
		else if (pTagData->memoryBank == MEMORY_BANK_EPC)
		{
			wprintf(L"\n EPC: ");
		}
		else if (pTagData->memoryBank == MEMORY_BANK_RESERVED)
		{
			wprintf(L"\n RESERVED: ");
		}
		else
		{
			wprintf(L"\n Bank %d: ", pTagData->memoryBank);
		}
		
		wprintf(L"%ls", Byte2HexWString(pTagData->pMemoryBankData, pTagData->memoryBankDataLength));
	}
	wprintf(L"\n");
}

void DisplayCapabilities( RFID_HANDLE32 hReader)
{
	READER_CAPS readerCaps;
	memset(&readerCaps,0,sizeof(READER_CAPS));
	fprintf(api3Data.fpLog,"\n\n***InSide the Function Display Capabilities****");
	wprintf(L"\n-----> Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));

    // Get the Reader Capability
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &readerCaps); 
	api3Data.rfidStatus = RFID_GetReaderCaps(api3Data.hReader, &api3Data.rCaps ); // duplicate call to avoid struct copy

	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		fprintf(api3Data.fpLog,"\n RFID_GetReaderCaps() FAIL - Status :%S",RFID_GetErrorDescription( api3Data.rfidStatus ));
	}
	
	fprintf(api3Data.fpLog,"\n...RFID Reader Capabilities...");
	fprintf(api3Data.fpLog,"\n readerCaps.blockEraseSupported: %d",readerCaps.blockEraseSupported);
	fprintf(api3Data.fpLog,"\n readerCaps.blockWriteSupported  : %d",readerCaps.blockWriteSupported );
	fprintf(api3Data.fpLog,"\n readerCaps.canDoTagInventoryStateAwareSingulation  : %d",readerCaps.stateAwareSingulationSupported);
	fprintf(api3Data.fpLog,"\n readerCaps.communicationStandard	: %s",COMMNSTANDARD[readerCaps.communicationStandard] );
	fprintf(api3Data.fpLog,"\n readerCaps.countryCode	: %d\t CountryName: %ls",readerCaps.countryCode,GetCountryNamebyCode(readerCaps.countryCode) );
	fprintf(api3Data.fpLog,"\n readerCaps.hasUTCClockCapability : %d",readerCaps.utcClockSupported );
	fprintf(api3Data.fpLog,"\n readerCaps.hoppingEnabled	: %d",readerCaps.hoppingEnabled);
	fprintf(api3Data.fpLog,"\n readerCaps.maxNumOperationsInAccessSequence : %d",readerCaps.maxNumOperationsInAccessSequence);
	fprintf(api3Data.fpLog,"\n readerCaps.maxNumPreFilters : %d",readerCaps.maxNumPreFilters);
	fprintf(api3Data.fpLog,"\n readerCaps.modelName : %S ",readerCaps.modelName);
	fprintf(api3Data.fpLog,"\n readerCaps.numAntennaSupported : %d ",readerCaps.numAntennas );
	fprintf(api3Data.fpLog,"\n readerCaps.numGPI : %d ",readerCaps.numGPIs);
	fprintf(api3Data.fpLog,"\n readerCaps.numGPO : %d ",readerCaps.numGPOs );
	fprintf(api3Data.fpLog,"\n readerCaps.pFirmwareVersion : %S ",readerCaps.firmWareVersion );
	fprintf(api3Data.fpLog,"\n readerCaps.readerID.type: %d ",readerCaps.readerID.type );
	fprintf(api3Data.fpLog,"\n readerCaps.readerID.value: %S ",readerCaps.readerID.value );
	fprintf(api3Data.fpLog,"\n readerCaps.writeUMISupported: %d ",readerCaps.writeUMISupported );
	
	// printing s Console
	wprintf(L"\n...RFID Reader Capabilities...");
	wprintf(L"\n readerCaps.blockEraseSupported: %d",readerCaps.blockEraseSupported);
	wprintf(L"\n readerCaps.blockWriteSupported  : %d",readerCaps.blockWriteSupported );
	wprintf(L"\n readerCaps.canDoTagInventoryStateAwareSingulation  : %d",readerCaps.stateAwareSingulationSupported);
	wprintf(L"\n readerCaps.communicationStandard	: %ls",wCOMMNSTANDARD[readerCaps.communicationStandard] );
	wprintf(L"\n readerCaps.countryCode	:%ld\t  CountryName: %ls", readerCaps.countryCode, GetCountryNamebyCode(readerCaps.countryCode));
	wprintf(L"\n readerCaps.hasUTCClockCapability : %d",readerCaps.utcClockSupported );
	wprintf(L"\n readerCaps.hoppingEnabled	: %d",readerCaps.hoppingEnabled);
	wprintf(L"\n readerCaps.maxNumOperationsInAccessSequence : %d",readerCaps.maxNumOperationsInAccessSequence);
	wprintf(L"\n readerCaps.maxNumPreFilters : %d",readerCaps.maxNumPreFilters);
	wprintf(L"\n readerCaps.modelName : %ls ",readerCaps.modelName);
	wprintf(L"\n readerCaps.numAntennaSupported : %d ",readerCaps.numAntennas );
	wprintf(L"\n readerCaps.numGPI : %d ",readerCaps.numGPIs);
	wprintf(L"\n readerCaps.numGPO : %d ",readerCaps.numGPOs );
	wprintf(L"\n readerCaps.pFirmwareVersion : %ls ",readerCaps.firmWareVersion );
	wprintf(L"\n readerCaps.readerID.type: %d ",readerCaps.readerID.type );
	wprintf(L"\n readerCaps.readerID.value: %ls ",readerCaps.readerID.value );
	wprintf(L"\n readerCaps.writeUMISupported: %d ",readerCaps.writeUMISupported );
	wprintf(L"\n readerCaps.writeUMISupported: %ld ",readerCaps.writeUMISupported );

	wprintf(L"\n readerCaps.AntennaRFConfigSupported: %ld ",readerCaps.AntennaRFConfigSupported );
	wprintf(L"\n readerCaps.FujitsuCommandsSupported: %ld ",readerCaps.FujitsuCommandsSupported );
	wprintf(L"\n readerCaps.PeriodicTagReportsSupported: %ld ",readerCaps.PeriodicTagReportsSupported );
	wprintf(L"\n readerCaps.radioPowerControlSupported: %ld ",readerCaps.radioPowerControlSupported );
	wprintf(L"\n readerCaps.TagPhaseReportingSupported: %ld ",readerCaps.TagPhaseReportingSupported );
	wprintf(L"\n readerCaps.ZoneSuppported: %ld ",readerCaps.ZoneSuppported );

	if( readerCaps.lpVersions != NULL )
	{
		for( int nVersions =0;nVersions < readerCaps.lpVersions->numVersions;nVersions++)
		{
			wprintf(L"\n readerCaps.lpVersion: %d ", readerCaps.lpVersions->numVersions );
			wprintf(L"\n readerCaps.ModuleName and version: %ls : %ls ",readerCaps.lpVersions->pVersionList->moduleVersion,readerCaps.lpVersions->pVersionList->moduleName );
		}
	}
	wprintf(L"\n-----> Executing %ls End ",MultiByte2WideChar( __FUNCTION__));
}
void PrintLine( FILE* fp )
{
	fprintf( fp, "\n*******************************************************************************************************************\n");
}

// Ansi Print Time Routine
char* PrintTime( void )
{
	SYSTEMTIME systime;
	rfid3SystemTime(&systime);
	char day[16];
	// Determine day of the week.

	switch (systime.wDayOfWeek)
	{
	case 0:
	   strcpy( day,"Sunday");
	   break;
	case 1:
	   strcpy( day,"Monday");
	   break;
	case 2:
	   strcpy( day,"Tuesday");
	   break;
	case 3:
	   strcpy( day,"Wednesday");
	   break;
	case 4:
	   strcpy( day,"Thursday");
	   break;
	case 5:
	   strcpy( day,"Friday");
	   break;
	case 6:
	   strcpy( day,"Saturday");
	   break;
	}

	// Show the time in a message box.
	sprintf(api3Data.CurrentTime, "%s-%u-%u-%u--%u-%u-%u-%u", day,systime.wYear, systime.wMonth, systime.wDay,systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	return api3Data.CurrentTime;
}
#ifdef linux
tm SYSTEMTIME2tm(SYSTEMTIME *s)
{
	tm t;

	t.tm_year  = s->wYear - 1900;
	t.tm_mon   = s->wMonth- 1;
	t.tm_wday  = s->wDayOfWeek;
	t.tm_mday  = s->wDay;
	t.tm_yday  = 0;
	t.tm_hour  = s->wHour;
	t.tm_min   = s->wMinute;
	t.tm_sec   = s->wSecond;
	t.tm_isdst = 0;

	return t;
}


static SYSTEMTIME tm2SYSTEMTIME(tm *t,int milliSecs)
{
	SYSTEMTIME s;

	s.wYear      = t->tm_year + 1900;
	s.wMonth     = t->tm_mon  + 1;
	s.wDayOfWeek = t->tm_wday;
	s.wDay       = t->tm_mday;
	s.wHour      = t->tm_hour;
	s.wMinute    = t->tm_min;
	s.wSecond    = t->tm_sec;
	s.wMilliseconds = milliSecs;

	return s;
}


void GetLocalTime(SYSTEMTIME * pSystemTime)
{
	timeval tv;
	struct timezone tz;
	tm *ptm;
	gettimeofday(&tv,&tz);
	ptm = localtime(&(tv.tv_sec));
	*pSystemTime = tm2SYSTEMTIME(ptm,tv.tv_usec/1000);
}
#endif
