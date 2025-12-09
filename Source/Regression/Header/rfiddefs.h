/* $Source: /home/cvs/cvs_root/RFIDAPI3/src/lib/win32/BackwardCompatibility/Include/Attic/rfiddefs.h,v $
 *
 * $Revision: 1.1.2.3 $
 * $Date: 2008/12/04 06:45:37 $
 * $Author: ggayathri $
 *
 * $Log: rfiddefs.h,v $
 * Revision 1.1.2.3  2008/12/04 06:45:37  ggayathri
 * Minor corrections
 *
 * Revision 1.1.2.2  2008/12/03 06:53:32  rsuresh
 * UNDER_CE directive changed to UNDER_MOBILE to avoid conflicts with CE build
 *
 * Revision 1.1.2.1  2008/11/18 02:51:03  ggayathri
 * DLL Headers for RFIDAPI2 Backward compatibility
 *
 * Revision 1.2  2008/08/08 19:11:11  ryicks
 * Adding RFID access option
 *
 * Revision 1.1.1.1  2008/07/21 11:40:48  rsuresh
 * no message
 *
// 
//    Rev 1.22   Jul 03 2008 05:53:30   gayathrig
// MC9090 R3.2 Support for En 302-208
// 
//    Rev 1.21   May 10 2008 06:00:22   gayathrig
// EU changes - Documentation for DutyCycle
// 
//    Rev 1.19   Dec 07 2007 00:21:04   guptaa
// Select record added independent of access commands
// 
//    Rev 1.18   Nov 06 2007 05:46:58   guptaa
// Post filtering removed. Only select record possible. Default select record deletion removed
// 
//    Rev 1.17   Oct 30 2007 01:58:08   guptaa
// RF On time setting hidden for US devices.
// Sequential launching of Symbol Roam and RFID demo app at startup
// 
//    Rev 1.16   Oct 14 2007 13:36:46   guptaa
// bug fixes for MC9090 RC3 release
// 
//    Rev 1.15   Oct 01 2007 09:56:10   guptaa
// MC9090 ER bug fixes
// Remove select command added for RD5000
// 
//    Rev 1.14   Jul 13 2007 03:16:20   guptaa
// Gen2 features added.
// Debug Logs Removed in Release build
// 
//    Rev 1.13   Apr 10 2007 16:03:08   ohaire
// Added defines for G2 features.  Needs review...
// 
//    Rev 1.12   Oct 12 2006 17:08:10   a00011001
// No change.
// 
//    Rev 1.11   Sep 26 2006 11:40:06   ohaire
// Version 3.01 checkin from CVS
 * Revision 1.19  2006/09/25 19:38:17  ohaire
 * 301 release.  changed taglist size to 200... KEEP Same with win32/ce
 *
 * Revision 1.18  2006/06/16 13:42:39  ohaire
 * Change RFID_ANTENNA_SEQUENCE_LENGTH to 64 again!!! This is in RFID_CAPS struct, and apps rely on the size!!!
 *
  * Revision 1.17  2006/06/15 03:39:44  yjiang
 * implement new tag read error handling for tag lists
 * change maximum number of tags in list for CE build to 100 for lack of memory
 * XR400 C-API code cut 5 (2.99r)
 *
 * Revision 1.16  2006/06/12 22:08:40  yjiang
 * Merge Apex code change with XR400 C-API code cut 4
 *
 * Revision 1.15  2006/05/31 19:22:13  ohaire
 * added back structure definitions...  make sure win32 apps point to SMDK include files...
 *
 * In TYPE_TAG_EX structure, remove several reserved DWORDs since they are now used.  Otherwise,
 * structure will grow, and it should not.
 *
  * Revision 1.14.2.1  2006/06/12 21:40:51  yjiang
 * XR400 C-API code cut 4
 *
 * Revision 1.14  2006/05/23 15:53:13  yjiang
 * use enum for the tag type index definition
 *
 * Revision 1.13  2006/05/11 21:26:13  yjiang
 * WIP for XR400 API
 * Auto detect XR400/XR480 by using active antenna list
 *
 * Revision 1.12  2006/05/01 21:44:09  yjiang
 * support multiple XR400 instance
 * add XR400_API macro definition
 *
 * Revision 1.11  2006/04/26 15:43:36  yjiang
 * WIP for support PC version, multiple reader instances XR400 C-API
 *
 * Revision 1.10  2006/04/06 13:44:29  ohaire
 * Extensive changes for 9090/apex option boards, autodetection of module, TCP/IP support.
 * Also added emulated RFID module support for testing DLL without real radio. Currently supports TAG reading, no writing...
 *
 * Revision 1.9  2006/03/27 19:13:08  yjiang
 * adjust the size of the reserved field in RFID_CAPS structure to maintain compatibility with existing C# API
 *
 * Revision 1.8  2006/02/15 23:15:26  yjiang
 * handle WJ error message
 * open lock mask and lock action caps
 * bump up version to 2.49zd
 *
 * Revision 1.7  2006/01/27 23:28:54  yjiang
 * test Gen 2 kill
 *
 * Revision 1.6  2006/01/25 14:22:53  yjiang
 * add Gen 2 parameter validation and clean up for T&V
 *
 * Revision 1.5  2006/01/20 16:14:13  yjiang
 * merge with Mike's change
 *
 * Revision 1.4  2006/01/19 21:43:02  software
 * First checkin of merged code from gen 2 and class0 user data update
 
 * Revision 1.3  2006/01/10 23:57:34  yjiang
 * WIP for WJ Gen 2 support
 *
 * Revision 1.2  2005/12/07 23:24:21  yjiang
 * add CVS header
 *
 *
 */


#ifndef RFIDDEFS_H
#define RFIDDEFS_H

//#include "strucinf.h"
#include "wtypes.h"
#include "stdio.h"

// Command Codes utilized by lower level command interface and stored in the RFID_STATS structure as dwLastReaderCmd.
#define RFID_GET_FW_VERSION					0x00
#define RFID_SET_READER_NUMBER				0x01
#define RFID_GET_READER_NUMBER				0x02
#define RFID_SET_BAUD_RATE					0x03
#define RFID_REBOOT							0X04
#define RFID_SET_IO_PORT_VALUE				0x05
#define RFID_GET_IO_PORT_VALUE				0x06
#define RFID_SET_ACTIVE_ANTENNA				0x07
#define RFID_GET_ACTIVE_ANTENNA				0x08
#define RFID_SET_RF_POWER					0x09
#define RFID_GET_RF_POWER					0x0A
#define RFID_SUSPEND_READER					0x0E
#define RFID_RESTORE_FACTORY_DEFAULTS		0x10
#define RFID_GET_READER_HW_INFO				0x11
#define RFID_GET_READER_MFNG_INFO			0x12
#define RFID_MANAGE_RESERVED_PARAMETERS		0x13
#define RFID_SET_TAG_MASK					0x14
#define RFID_GET_TAG_MASK					0x15
#define RFID_SET_RF_STATE					0x1A
#define RFID_GET_RF_STATE					0x1B
#define RFID_SET_ANTENNA_SEQUENCE			0x1C
#define RFID_GET_ANTENNA_SEQUENCE			0x1D
#define RFID_SLEEP							0x20
#define RFID_WAKE							0x21
#define RFID_PROGRAM_ROW					0x23
#define RFID_GET_TAG_ID						0x24
#define RFID_GET_RAW_TAG_ID_VERIFY			0x3D
#define RFID_GET_RAW_TAG_ID_GL_SCROLL		0x3E
#define RFID_INVENTORY						0x40
#define RFID_WAKE_N							0x41
#define RFID_DIRECTED_LIST_CONTROL			0x42
#define RFID_INVENTORY_DIAGNOSTIC			0x4E
#define RFID_PROGRAM_TAG					0x50
#define RFID_ERASE_TAG						0x51
#define RFID_KILL_TAG						0x52
#define RFID_LOCK_TAG						0x53
#define RFID_ENTER_BOOTLOADER				0xD1
#define RFID_FIRMWARE_UPLOAD_LINE			0xD2


// Response Codes from all CReader class functions that return int.
// Response Codes utilized by lower level command interface and stored in the RFID_STATS structure as dwLastReaderRsp

#define	RFID_SUCCESS				0
#define RFID_PORT_OPEN_ERROR		1
#define RFID_PARAM_ERROR			2
#define RFID_CRC_ERROR				3
#define RFID_CANNOT_ALLOC_MEM		4
#define RFID_PORT_WRITE_ERROR		5
#define RFID_PORT_READ_ERROR		6
#define RFID_UNKNOWN_ERROR			7
#define RFID_MAX_TAGS_EXCEEDED		8
#define RFID_COMMAND_TIMEOUT		9
#define RFID_UPLOADOK				10
#define RFID_UPLOADEND				11
#define RFID_ENGINE_BUSY			12
#define RFID_PORT_NOT_OPEN			13
#define RFID_UNKNOWN_COMM_TYPE		14
#define RFID_BUFFER_TO_SMALL		15
#define RFID_FIND_COMPLETE			16
#define RFID_INVALID_HANDLE			17
#define RFID_NO_DEVICE_POWER		18
#define RFID_INVALID_FLASHFILE		19
#define RFID_CAPNOTSUPPORTED		20
#define RFID_CAPREADONLY			21
#define RFID_TAG_DETECTED			22
#define RFID_TAGTYPE_DISABLED		23
#define RFID_TAGTYPE_NOSUPPORT		24
#define RFID_FILTER_EXISTS			25
#define RFID_FILTER_DOES_NOT_EXIST	26

#define RFID_CMD_UNKLEN				0x81
#define RFID_CMD_UNKVAL				0x82
#define RFID_CMD_UNKCMD				0x83
#define RFID_CMD_UNKTAGCMD			0x84
#define RFID_CMD_OVERERR			0x85
#define RFID_CMD_NOTAG				0x86
#define RFID_CMD_ERASEFAIL			0x87
#define RFID_CMD_PROGFAIL			0x88
#define RFID_CMD_TAGLOCK			0x89
#define RFID_CMD_KILLFAIL			0x8A
#define RFID_CMD_LOCKFAIL			0x8B
#define RFID_CMD_DATASIZE			0x8C
#define RFID_CMD_HWERR				0x8D
#define RFID_CMD_LISTFULL			0x8E
#define RFID_CMD_UPLOADERR			0x8F
#define RFID_CMD_UPLOADINVALID		0x90
#define RFID_CMD_UPLOADCRC			0x91

#define RFID_CMD_GEN2_ACCESS_TAG_LOST			0x92
#define RFID_CMD_GEN2_ACCESS_RESP_CRC			0x93

#define RFID_CMD_GEN2_WRITE_NOT_VERIFIED		0x94
#define RFID_CMD_GEN2_WRITE_ERASE_ERROR			0x95
#define RFID_CMD_GEN2_WRITE_LOCK_ERROR			0x96

#define RFID_CMD_GEN2_TAG_ERROR_OTHER			0x97
#define RFID_CMD_GEN2_TAG_ERROR_MEM_OVERRUN		0x98
#define RFID_CMD_GEN2_TAG_ERROR_MEM_LOCKED		0x99
#define RFID_CMD_GEN2_TAG_ERROR_LACK_POWER		0x9A
#define RFID_CMD_GEN2_TAG_ERROR_NON_SPECIFIC	0x9B

#define RFID_CMD_GEN2_SELECT_RECORDS_FULL		0x9C
#define RFID_CMD_GEN2_NO_SELECT_RECORD			0xA2
#define RFID_CMD_GEN2_REMOVE_SELECT_RECORD_FAIL	0xA3
#define RFID_CMD_DUTY_CYCLE_LIMIT_REACHED		0xA4
#define RFID_CMD_SYNCHRONISATION_ERROR			0xA5



// Memory bank options for Gen 2 write tags' memory bank command (21h)
enum RFID_GEN2_MEMORY_BANKS {
	RFID_GEN2_MEMORY_BANK_RESERVED	= 0,
	RFID_GEN2_MEMORY_BANK_EPC		= 0x01,
	RFID_GEN2_MEMORY_BANK_TID		= 0x02,
	RFID_GEN2_MEMORY_BANK_USER		= 0x03,
};

// options for for RFID_READCAP_G2_SEL
enum RFID_GEN2_SELS {
	RFID_GEN2_SEL_IGNORE_SL = 0,
	RFID_GEN2_SEL_SL_NOT_SET= 2,
	RFID_GEN2_SEL_SL_SET	= 3,
};

// options for RFID_READCAP_G2_SESSION
enum RFID_GEN2_SESSIONS {
	RFID_GEN2_SESSION_S0 = 0,
	RFID_GEN2_SESSION_S1 = 1,
	RFID_GEN2_SESSION_S2 = 2,
	RFID_GEN2_SESSION_S3 = 3,
};

// options for RFID_READCAP_G2_TARGET
enum RFID_GEN2_INVENTORIED_FLAGS {
	RFID_GEN2_INVENTORIED_FLAG_A = 0,
	RFID_GEN2_INVENTORIED_FLAG_B = 1,
};


// writing options for Gen 2 write tags' memory bank
enum RFID_GEN2_WRITE_OPTIONS {
	RFID_GEN2_WRITE_OPTION_NONE					= 0,
	RFID_GEN2_WRITE_OPTION_BLOCK_WRITE			= 0x01,
	RFID_GEN2_WRITE_OPTION_BLOCK_ERASE_FIRST	= (0x01<<1),
	RFID_GEN2_WRITE_OPTION_PROGRAM_EPC			= (0x01<<2),
};

// Lock options for Gen 2 write tags' memory bank
enum RFID_GEN2_LOCK_OPTIONS {
	RFID_GEN2_LOCK_OPTION_NONE							= 0,
	RFID_GEN2_LOCK_OPTION_PERMA_LOCK_AFTER_WRITE		= 0x01,
	RFID_GEN2_LOCK_OPTION_USE_PERMA_LOCK_BIT			= (0x01<<1),
	RFID_GEN2_LOCK_OPTION_PWD_WRITE_AFTER_WRITE			= (0x01<<2),
	RFID_GEN2_LOCK_OPTION_USE_PWD_WRITE_BIT				= (0x01<<3),
	RFID_GEN2_LOCK_OPTION_PERMA_LOCK_BANK2_AFTER_WRITE	= (0x01<<4),
	RFID_GEN2_LOCK_OPTION_USE_PERMA_LOCK_BIT2			= (0x01<<5),
	RFID_GEN2_LOCK_OPTION_PWD_WRITE_BANK2_AFTER_WRITE	= (0x01<<6),
	RFID_GEN2_LOCK_OPTION_USE_PWD_WRITE_BIT2			= (0x01<<7),
};


//#### Lock action settings
enum RFID_GEN2_LOCK_ACTION_FIELD_FNC
{
 
 RFID_GEN2_RDWRT_OPEN_OR_SECURED								= 0x0000,	// ####
 RFID_GEN2_RDWRT_ALWAYS_OPEN_OR_SECURE_NVRLOCK	= 0x0100,	// See page 62 of the EPCglobal RFID Protocols
 RFID_GEN2_RDWRT_SECURE_ONLY										= 0x0200,	// Class-1 Generation-2 UHF RFID Protocol for
 RFID_GEN2_READ_MEM_NO_RDWRT_PSWD								= 0x0300	// Communications at 860 MHz-960 MHz ver 1.0.9

};

// Lock mask
enum RFID_GEN2_LOCK_MASK {
	RFID_GEN2_LOCK_MASK_NONE							= 0,
	RFID_GEN2_LOCK_MASK_USER_PERM						= (0x01<<0),
	RFID_GEN2_LOCK_MASK_USER_PWD						= (0x01<<1),
	RFID_GEN2_LOCK_MASK_TID_PERM						= (0x01<<2),
	RFID_GEN2_LOCK_MASK_TID_PWD							= (0x01<<3),
	RFID_GEN2_LOCK_MASK_EPC_PERM						= (0x01<<4),
	RFID_GEN2_LOCK_MASK_EPC_PWD							= (0x01<<5),
	RFID_GEN2_LOCK_MASK_ACCESSPWD_PERM					= (0x01<<6),
	RFID_GEN2_LOCK_MASK_ACCESSPWD_PWD					= (0x01<<7),
	RFID_GEN2_LOCK_MASK_KILLPWD_PERM					= (0x01<<8),
	RFID_GEN2_LOCK_MASK_KILLPWD_PWD						= (0x01<<9),
};

// Lock mask
enum RFID_GEN2_LOCK_ACTION {
	RFID_GEN2_LOCK_ACTION_NONE							= 0,
	RFID_GEN2_LOCK_ACTION_USER_PERM						= (0x01<<0),
	RFID_GEN2_LOCK_ACTION_USER_PWD						= (0x01<<1),
	RFID_GEN2_LOCK_ACTION_TID_PERM						= (0x01<<2),
	RFID_GEN2_LOCK_ACTION_TID_PWD						= (0x01<<3),
	RFID_GEN2_LOCK_ACTION_EPC_PERM						= (0x01<<4),
	RFID_GEN2_LOCK_ACTION_EPC_PWD						= (0x01<<5),
	RFID_GEN2_LOCK_ACTION_ACCESSPWD_PERM				= (0x01<<6),
	RFID_GEN2_LOCK_ACTION_ACCESSPWD_PWD					= (0x01<<7),
	RFID_GEN2_LOCK_ACTION_KILLPWD_PERM					= (0x01<<8),
	RFID_GEN2_LOCK_ACTION_KILLPWD_PWD					= (0x01<<9),
};


enum RFID_GEN2_SELECT_TARGET {
	RFID_GEN2_SELECT_MOD_S0_INV					= 0x00,
	RFID_GEN2_SELECT_MOD_S1_INV					= 0x01,
	RFID_GEN2_SELECT_MOD_S2_INV					= 0x02,
	RFID_GEN2_SELECT_MOD_S3_INV					= 0x03,
	RFID_GEN2_SELECT_MOD_SL_FLAG				= 0x04
};


// When RFID_GEN2_SELECT_TARGET is RFID_GEN2_SELECT_MOD_S0_INV, MOD INV A or B, otherwise mod SL
enum RFID_GEN2_SELECT_ACTION {
	RFID_GEN2_SELECT_INV_A_NOT_INV_B			= 0x00,
	RFID_GEN2_SELECT_ASRT_SL_NOT_DSRT_SL		= 0x00,

	RFID_GEN2_SELECT_INV_A						= 0x01,
	RFID_GEN2_SELECT_ASRT_SL					= 0x01,

	RFID_GEN2_SELECT_NOT_INV_B					= 0x02,
	RFID_GEN2_SELECT_NOT_DSRT_SL				= 0x02,

	RFID_GEN2_SELECT_INV_A2BB2A_NOT_INV_A		= 0x03,
	RFID_GEN2_SELECT_NEG_SL_NOT_ASRT_SL			= 0x03,

	RFID_GEN2_SELECT_INV_B_NOT_INV_A			= 0x04,
	RFID_GEN2_SELECT_DSRT_SL_NOT_ASRT_SL		= 0x04,

	RFID_GEN2_SELECT_INV_B						= 0x05,
	RFID_GEN2_SELECT_DSRT_SL					= 0x05,

	RFID_GEN2_SELECT_NOT_INV_A					= 0x06,
	RFID_GEN2_SELECT_NOT_ASRT_SL				= 0x06,

	RFID_GEN2_SELECT_NOT_INV_A2BB2A				= 0x07,
	RFID_GEN2_SELECT_NOT_NEG_SL					= 0x07
};

#ifndef STRUCT_INFO
// Structure information structure

typedef struct tagSTRUCT_INFO
{
	DWORD	dwAllocated;				// Size of allocated structure in bytes
	DWORD	dwUsed;						// Amount of structure actually used

} STRUCT_INFO;

typedef STRUCT_INFO * LPSTRUCT_INFO;

#define MAX_CAP_NAME					64


// Capability information structure
typedef struct tagCAPINFO {
	STRUCT_INFO	StructInfo;

	DWORD		dwCapId;			
	BOOLEAN		bSupported;			
	DWORD		dwDataTypeId;			
	DWORD		dwDataSize;			
	DWORD		dwContainerId;	
	DWORD		dwContainerSize;	
	DWORD		dwNumItems;			 
	TCHAR		tszName[MAX_CAP_NAME];
} CAPINFO, *LPCAPINFO;
#endif //STRUCT_INFO


typedef struct
{
	int nPortIndex;
	int nPortNumber;
	TCHAR tszDeviceName[8];
	DWORD dwMagic;
	DWORD dwReaderType;
	DWORD dwReserved[4];				// Reserve 4 DWORDS for future
}RFID_FINDINFO;




// Used by Firmware download thread to report progress of firmware update.
// Application will get these messages if they register for them via SetStatusMessage().
#define RFIDAPI_FIRMWARE_DOWNLOAD_PERCENT		1
#define RFIDAPI_FIRMWARE_DOWNLOAD_COMPLETE		2
#define RFIDAPI_FIRMWARE_DOWNLOAD_FAILED		3
#define RFIDAPI_FIRMWARE_DOWNLOAD_DONE			4

// GetTag returns these values in the pTag->status field of the TYPE_TAG structure
#define RFID_GETTAG_OK			0
#define RFID_GETTAG_NOTAG		1
#define RFID_GETTAG_COLLISION	2
#define RFID_GETTAG_CRCERROR	3
#define RFID_GETTAG_ERASED		4
#define RFID_GETTAG_ERROR		0xFF	//dwOperationStatus / wStatusDetail contain extended information

#ifdef UNDER_MOBILE
#define RFID_MAX_TAGS			200		//Maximum number of tags to store in memory (DO NOT CHANGE for WINCE)
#else
#define RFID_MAX_TAGS			200		//Maximum number of tags to store in memory (DO NOT CHANGE for WIN32 desktop)
#endif

#define RFID_MAX_TAG_ID_LENGTH	17		//Max Byte size of tag ID 17 bytes  (was 16 but struct alignemnt allows us to put extra byte with no impact)

#if 0
#define RFID_TAG_TYPE_OTHER				0			// 
#define RFID_TAG_TYPE_EPC_CLASS0		1			// 
#define RFID_TAG_TYPE_EPC_CLASS0PLUS	2			// 
#define RFID_TAG_TYPE_EPC_CLASS1		3			// 
#define RFID_TAG_TYPE_EPC_CLASSG2		4			// 
#define RFID_TAG_TYPE_EPC_CLASS0ZUMA	5			// 
#define RFID_TAG_TYPE_MAXINDEX			5			// Max used
#define RFID_TAG_TYPE_ERROR				0xFFFFFFFF	// Unassigned type
#endif //0

enum RFID_TAG_TYPE_INDEX {
	RFID_TAG_TYPE_OTHER				= 0,			// 
	RFID_TAG_TYPE_EPC_CLASS0		= 1,			// 
	RFID_TAG_TYPE_EPC_CLASS0PLUS	= 2,			// 
	RFID_TAG_TYPE_EPC_CLASS1		= 3,			// 
	RFID_TAG_TYPE_EPC_CLASSG2		= 4,			// 
	RFID_TAG_TYPE_EPC_CLASS0ZUMA	= 5,			// 
	RFID_TAG_TYPE_ISO18000_6B	    = 6,            //
	RFID_TAG_TYPE_MAXINDEX			= 6,			// Max used
	RFID_TAG_TYPE_ERROR				= 0xFFFFFFFF,	// Unassigned type
};

#define RFID_TAG_TYPE_MAX				20			// Reserve for 20 types of tag in stats block

/*********** 		type storing tag data for a Tag List		**************/
typedef struct
{
	unsigned char status;							//Status of entry - discovered, ...			1
	unsigned char antennaNum;						//Last antenna this tag has been read		2
	unsigned char dataLength;						//Tag length in bytes						3
	unsigned char tagID[RFID_MAX_TAG_ID_LENGTH];	//ID code of tag							4-20
	SYSTEMTIME lastSeen;							//Timestamp of last time tag was seen		21-36
	unsigned long readCount;						//Number of reads of this tag				37-40
	DWORD dwType;									//Type of Tag								41-44
	DWORD dwFormat;									//Tag Data Format							45-48							
	DWORD dwOperationStatus;						//RFIDAPI status code						49-52
	WORD wStatusDetail;								//extended status information				53-54
	WORD wReserved1;								//											55-56	
	DWORD dwReserved4;								//											57-60
	DWORD dwReserved5;								// Reserve 5 DWORDS for future				61-64
}TYPE_TAG;


#define RFID_STRUCTURE_HEADER_INIT_VALUE		0x417

typedef struct
{
	int				nInit;								// This will be initialized by macro
	int				nTypeTagStructureFormat;
	int				nTypeTagStructureSize;
} TYPE_TAG_STRUCTURE_HEADER;

#define RFID_MAX_TAG_ID_LENGTH_EX		64			// max tag id length

// this structure should not change it's size.
// 
// in the future, new structures can be created.  If new structures are created,
// if they are, they will be called TYPE_TAG_STRUCTURE_FORMAT_EX2, TYPE_TAG_STRUCTURE_FORMAT_EX3, etc...
// also the api must know how to handle these structures...
typedef struct
{
	TYPE_TAG_STRUCTURE_HEADER Header;
	unsigned char	status;							//Status of entry - discovered, ...			
	unsigned char	antennaNum;						//Last antenna this tag has been read		
	DWORD			dataLength;						//number if bytes in the tagID field.  Includes the EPC number, and the memory page data length.  Memory page length is stored in dwMemoryPageLength
	SYSTEMTIME		firstSeen;						//Timestamp of first time tag was seen
	SYSTEMTIME		lastSeen;						//Timestamp of last time tag was seen
	unsigned long	readCount;						//Number of reads of this tag
	DWORD			dwType;							//Type of Tag
	DWORD			dwFormat;						//Tag Data Format

	int				nMemoryPageNumber;				//For class 0 tags, memory page 2 is EPC. Memory page 2 information is not stored in this and the following memory page variables
	DWORD			dwMemoryPageOffset;				//starting bit Offset into tag's memory page where data was read.
	DWORD			dwMemoryPageLength;				//length of memory page in bytes.

	DWORD			dwOperationStatus;				//
	WORD			wStatusDetail;					//
	WORD			wCRC;							//
	WORD			wPC;							//
	WORD			wRSSI;							//
	DWORD			dwMillisecond;					//

	DWORD			dwReserved5;					// Reserve DWORDS
	unsigned char tagID[RFID_MAX_TAG_ID_LENGTH_EX];	//ID code of tag
}TYPE_TAG_EX;


#define TYPE_TAG_STRUCTURE_FORMAT_EX1	0

//#define TYPE_TAG_STRUCTURE_FORMAT_EX2	1				// TBD...  future...
//			. . .
//			. . .
//#define TYPE_TAG_STRUCTURE_FORMAT_EXN	2				// TBD...  future...

#define TYPE_TAG_EX1_INIT(TypeTag)	\
{\
	ZeroMemory(&TypeTag, sizeof(TypeTag));\
	TypeTag.Header.nInit = RFID_STRUCTURE_HEADER_INIT_VALUE; \
	TypeTag.Header.nTypeTagStructureSize = sizeof(TypeTag); \
	TypeTag.Header.nTypeTagStructureFormat = TYPE_TAG_STRUCTURE_FORMAT_EX1;\
}

#define RFID_FILTER_OPTION_PASS				0				// pass tags that match the supplied filter
#define RFID_FILTER_OPTION_DROP				1				// drop tags that match the supplied filter
#define RFID_FILTER_OPTION_NOMATCH_PASS		2				// pass tags that do not match the supplied filter
#define RFID_FILTER_OPTION_NOMATCH_DROP		3				// drop tags that do not match the supplied filter


typedef struct
{
	unsigned char cBitLen;
	unsigned char cBitStartOffset;
	unsigned char cTagMask[RFID_MAX_TAG_ID_LENGTH];
	DWORD dwReserved[20];				// Reserve 20 DWORDS for future
}TAG_MASK;

typedef struct
{
	int				nInit;								// This will be initialized by macro
	int				nTagMaskStructureFormat;
	int				nTagMaskStructureSize;
} TAG_MASK_STRUCTURE_HEADER;

typedef struct
{
	TAG_MASK_STRUCTURE_HEADER Header;
	unsigned int nBitLen;
	unsigned int nBitStartOffset;
	unsigned char cTagMask[RFID_MAX_TAG_ID_LENGTH_EX];
	DWORD dwReserved[18];				// Reserve 18 DWORDS for future
	unsigned int nMemBank;	//new field added
	unsigned int nBitLenEPCID;	//new field added

}TAG_MASK_EX;

#define TAG_MASK_STRUCTURE_FORMAT_EX1	0

#define TAG_MASK_EX1_INIT(TagMask)	\
{\
	ZeroMemory(&TagMask, sizeof(TagMask));\
	TagMask.nMemBank=1;\
	TagMask.nBitStartOffset = 16;\
	TagMask.Header.nInit = RFID_STRUCTURE_HEADER_INIT_VALUE;\
	TagMask.Header.nTagMaskStructureSize = sizeof(TagMask);\
	TagMask.Header.nTagMaskStructureFormat = TAG_MASK_STRUCTURE_FORMAT_EX1;\
}

#define RFID_STATS_VERSION	2

typedef struct
{
	DWORD dwVersion;				// 1
	DWORD dwTotalTX;				// 2
	DWORD dwTotalRX;				// 3
	DWORD dwPacketsTX;				// 4
	DWORD dwPacketsRX;				// 5
	DWORD dwIncompleteTX;			// 6
	DWORD dwPacketsFragmented;		// 7
	DWORD dwPacketsCRCError;		// 8
	DWORD dwSessionIDError;			// 9
	DWORD dwTotalReads;				// 10
	DWORD dwTotalTagCRCError;		// 11
	DWORD dwTotalTagCollisions;		// 12
	DWORD dwTimeouts;				// 13
	DWORD dwTotal_10MSOnTime;		// 14
	DWORD dwTotalMSRFOnTime;		// 15
	DWORD dwNoTagErr;				// 16
	DWORD dwEraseFailErr;			// 17
	DWORD dwProgFailErr;			// 18
	DWORD dwTagLockErr;				// 19
	DWORD dwKillFailErr;			// 20
	DWORD dwHardwareErr;			// 21
	DWORD dwDataSizeErr;			// 22
	DWORD dwReadTime_10MS;			// 23
	DWORD dwReadAttempts;			// 24
	DWORD dwReadSuccess;			// 25
	DWORD dwProgramAttempts;		// 26
	DWORD dwProgramSuccess;			// 27
	DWORD dwEraseAttempts;			// 28
	DWORD dwEraseSuccess;			// 29
	DWORD dwLockAttempts;			// 30
	DWORD dwLockSuccess;			// 31
	DWORD dwKillAttempts;			// 32
	DWORD dwKillSuccess;			// 33
	DWORD dwLockFailErr;			// 34
	DWORD dwTagTypeReadTotal[RFID_TAG_TYPE_MAX];		// 35-54 (35+(RFID_TAG_TYPE_MAX - 1)
	DWORD dwTagUnderrunErrors;		// 55
	DWORD dwDroppedTagEvents;		//56
	DWORD dwReserved[256-56];		// Total of 256 DWORDS for stats	(Fixed size total of 1K)
} RFID_STATS, LPRFID_STATS;
// RFID_CAPS defines
#define RFID_ANTENNA_SEQUENCE_LENGTH		64

#define MAX_NUMBER_ANTENNA					8


#define RFID_CAPS_VERSION	2
#define RFID_READER_TYPE_NONE			0			// RFID reader type (nReaderType)... Unique for each new reader hardware
#define RFID_READER_TYPE_A_CLASS1_1		1	//	C1		/ 9060
#define RFID_READER_TYPE_CLASS0_1		2	//	MR100 C0	/ 9060
#define RFID_READER_TYPE_CLASS01_1		3	// 	MPR7000 C0, C21, G2	/ 9060


#define RFID_READER_TYPE_XR400			4	// 	XR400 C0, C1, G2	/ XR400
#define RFID_READER_TYPE_MR400_9060		5	// 	MR400 C0, C1, G2	/ 9060


#define RFID_READER_TYPE_MPR_9090		6	// 	MPR7000 C0, C1, C2	/ 9090
#define RFID_READER_TYPE_MR400_9090		7	// 	MR400 C0, C1, G2	/ 9090


#define RFID_READER_TYPE_MPR_RD5000		8	// 	MPR7000 C0, C1, C2	/ Apex 
#define RFID_READER_TYPE_MR400_RD5000	9	// 	MR400 C0, C1, C2	/ Apex 


#define RFID_TOTAL_READER_TYPES			10	// total of 10 defined reader configs

#define RFIDMODULE_UNKNOWN			0
#define RFIDMODULE_ALIEN			1
#define RFIDMODULE_WJ_UNKNOWN		2
#define RFIDMODULE_WJ_MPR_7100		3
#define RFIDMODULE_WJ_WJR_7100		8
#define RFIDMODULE_WJ_WJR_7180		9
#define RFIDMODULE_WJ_WJR_7181		10

#define RFIDMODULE_EU_300_220		1
#define RFIDMODULE_EU_302_208		2

#define RFIDMODULE_MATRICS_UNKNOWN	4
#define RFIDMODULE_MATRICS_MR100	5
#define RFIDMODULE_MATRICS_MR400	6
#define RFIDMODULE_MATRICS_XR400	7

#define RFID_API_VERSION_STRING_LENGTH		20
#define RFID_FIRMWARE_VERSION_STRING_LENGTH	20
#define RFID_FIRMWARE_MFGDATECODE_LENGTH	20
#define RFID_FIRMWARE_SERIALINFO_LENGTH		20

/**********Predefined values for the two capabilities.***********/
#define RFID_RF_ON_TIME						80
#define RFID_DUTY_CYCLE_INTERVAL			60

/****************** EU 302-208 Tx-Channel Frequencies ***************************/
#define RFID_302_208_ALL_CHANNELS		0x0F //Default - Frequency Hopping Mode
#define RFID_302_208_CHANNEL_0			0x01 //Channel 0: 865.6 MHz - Bit - 0000 0001
#define RFID_302_208_CHANNEL_1			0x02 //Channel 1: 866.3 MHz - Bit - 0000 0010
#define RFID_302_208_CHANNEL_2			0x04 //Channel 2: 866.9 MHz - Bit - 0000 0100 
#define RFID_302_208_CHANNEL_3			0x08 //Channel 3: 867.5 MHz - Bit - 0000 1000 

/****************** C1G2 Specs Options ***************************/
#define RFID_SPECOPTION_NONE					0
#define RFID_SPECOPTION_C1G2WRITE				1
#define RFID_SPECOPTION_C1G2LOCK				2
#define RFID_SPECOPTION_C1G2KILL				4

typedef struct
{
	TCHAR szAPIVersionString[RFID_API_VERSION_STRING_LENGTH];
	TCHAR szFirmwareVersion[RFID_FIRMWARE_VERSION_STRING_LENGTH];
	TCHAR szMfgDateCode[RFID_FIRMWARE_MFGDATECODE_LENGTH];
	TCHAR szSerialInfo[RFID_FIRMWARE_SERIALINFO_LENGTH];
	unsigned char ReaderNumber;
	unsigned char Antenna;
	unsigned char AntennaSequence[RFID_ANTENNA_SEQUENCE_LENGTH];
	BOOL bPowerState;
	BOOL bPortOpen;
	DWORD dwReaderType;
	DWORD dwCurrentPort;
	DWORD dwCurrentBaud;
	DWORD dwRFIDDeviceTable[10];
	DWORD dwRFIDDeviceCount;
	DWORD dwRFAttenuation;							// 1
	DWORD dwRFChannel;								// 2
	DWORD dwLastReaderCmdSeqNum;					// 3
	DWORD dwLastReaderCmd;							// 4
	DWORD dwLastReaderRsp;							// 5
	DWORD dwTagListSeqNum;							// 6
	BYTE  bSupportedTagTypes[RFID_TAG_TYPE_MAX];	// 7  - 11
	BYTE  bEnabledTagTypes[RFID_TAG_TYPE_MAX];		// 12 - 16
	DWORD dwMonitorStatus;							// 17
	BYTE bRFIDModuleType;							// 18
	BYTE bRFIDEUMode;								// 19
	BYTE bEU302208TxChannel;						//20
	DWORD dwReserved[100-18];		// Reserve 100 DWORDS for future
}RFID_CAPS;

typedef struct
{
	DWORD dwTotalTags;					// 1
	DWORD dwTotalReads;					// 2
	DWORD dwNewTags;					// 3
	DWORD dwReadTimeMS;					// 4
	DWORD dwSeqNum;						// 5
	DWORD dwMaxTags;					// 6 Max tags supported.  0 means 200
	DWORD dwErrorTags;					// 7 total of error tag packets appended to end of valid tags..
	DWORD dwReserved[25-7];				// Use a total of 25 DWORDS
	TYPE_TAG Tags[RFID_MAX_TAGS];
} TAG_LIST;

/*	DUTY_CYCLE_PARAMETER is valid only for EU 300-220 Specification
	This Data Structure hold the data pertaining to the current duty-cycle 
*/
typedef struct
{
	DOUBLE TxOnTimeUsed;				// RF-on-time used, in seconds
	DOUBLE TxOnTimeAvailable;			// Remaining RF-on-time, in seconds
	DOUBLE TimeUntilAvailable;			// If RF-on-time used has reached the maximum of 10%, this value will be non-zero, indicating when the RF will be available next, in seconds
}
DUTY_CYCLE_PARAMETER;

typedef struct
{
	int				nInit;								// This will be initialized by macro
	int				nTagListStructureFormat;
	int				nTagListStructureSize;
} TAG_LIST_STRUCTURE_HEADER;

typedef struct
{
	TAG_LIST_STRUCTURE_HEADER Header;
	DWORD dwTotalTags;					// 
	DWORD dwTotalReads;					// 
	DWORD dwNewTags;					// 
	DWORD dwReadTimeMS;					// 
	DWORD dwSeqNum;						// 
	DWORD dwMaxTags;					// Max tags supported.  0 means 200
	DWORD dwErrorTags;					// total of error tag packets appended to end of valid tags..
	DWORD dwReserved[19];				// Resrve a total of 19 DWORDS
	TYPE_TAG_EX Tags[RFID_MAX_TAGS];
} TAG_LIST_EX;


#define TAG_LIST_STRUCTURE_FORMAT_EX1	0

#define TAG_LIST_OVERHEAD_EX1			(sizeof(TAG_LIST_EX) - (sizeof(TYPE_TAG_EX) * RFID_MAX_TAGS))

#define TAG_LIST_EX1_INIT(TagList, MaxTags)	\
{\
	ZeroMemory(&TagList, TAG_LIST_OVERHEAD_EX1 + sizeof(TYPE_TAG_EX) * MaxTags);\
	TagList.Header.nInit					= RFID_STRUCTURE_HEADER_INIT_VALUE;							\
	TagList.Header.nTagListStructureSize	= TAG_LIST_OVERHEAD_EX1 + sizeof(TYPE_TAG_EX) * MaxTags;	\
	TagList.Header.nTagListStructureFormat	= TAG_LIST_STRUCTURE_FORMAT_EX1;									\
	TagList.dwMaxTags						= MaxTags;													\
	TagList.dwTotalTags						= 0;														\
	TagList.dwTotalReads					= 0;														\
	TagList.dwNewTags						= 0;														\
	TagList.dwReadTimeMS					= 0;														\
}

#define TAG_LIST_EX1_ALLOC(dwNumTags)	(TAG_LIST_EX *)malloc(TAG_LIST_OVERHEAD_EX1 + (sizeof(TYPE_TAG_EX) * dwNumTags))
#define TAG_LIST_EX_FREE(pTagList)		free((void *)pTagList)

#endif //	#ifndef RFIDDEFS_H