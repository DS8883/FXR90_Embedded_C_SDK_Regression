/* $Source: /home/cvs/cvs_root/RFIDAPI3/src/lib/win32/BackwardCompatibility/Include/Attic/rfidcapsengine.h,v $
 *
 * $Revision: 1.1.2.1 $
 * $Date: 2008/11/18 02:51:03 $
 * $Author: ggayathri $
 *
 * $Log: rfidcapsengine.h,v $
 * Revision 1.1.2.1  2008/11/18 02:51:03  ggayathri
 * DLL Headers for RFIDAPI2 Backward compatibility
 *
 * Revision 1.1.1.1  2008/07/21 11:40:48  rsuresh
 * no message
 *
// 
//    Rev 1.13   Jul 03 2008 05:53:28   gayathrig
// MC9090 R3.2 Support for En 302-208
// 
//    Rev 1.10.1.1   May 09 2008 11:03:26   gayathrig
// EU Dutycycle changes
// 
//    Rev 1.10   Oct 01 2007 09:56:10   guptaa
// MC9090 ER bug fixes
// Remove select command added for RD5000
// 
//    Rev 1.9   Sep 05 2007 03:48:24   guptaa
// Configuration of RF On time
// 
//    Rev 1.8   Aug 24 2007 02:00:54   guptaa
// Bug fixes for RC2 Release
// 
//    Rev 1.7   Apr 10 2007 16:02:40   ohaire
// Added defines for new G2 caps
// 
//    Rev 1.6   Oct 12 2006 17:08:10   a00011001
// No change.
// 
//    Rev 1.5   Sep 26 2006 11:40:00   ohaire
// Version 3.01 checkin from CVS
 * Revision 1.13  2006/06/16 15:19:40  ohaire
 * Always include imgdef.h  for win32 make sure project has SMDK installed.  Have project point to this include file
 *
 * Revision 1.12  2006/06/12 22:08:40  yjiang
 * Merge Apex code change with XR400 C-API code cut 4
 *
 * Revision 1.10.2.1  2006/06/12 21:40:51  yjiang
 * XR400 C-API code cut 4
 *
 * Revision 1.10  2006/05/11 21:26:13  yjiang
 * WIP for XR400 API
 * Auto detect XR400/XR480 by using active antenna list
 *
 * Revision 1.9  2006/05/01 21:44:09  yjiang
 * support multiple XR400 instance
 * add XR400_API macro definition
 *
 * Revision 1.8  2006/04/06 13:44:29  ohaire
 * Extensive changes for 9090/apex option boards, autodetection of module, TCP/IP support.
 * Also added emulated RFID module support for testing DLL without real radio. Currently supports TAG reading, no writing...
 *
 * Revision 1.7  2006/02/02 22:42:49  yjiang
 * add Gen 2 select mem bank
 *
 * Revision 1.6  2006/01/25 14:22:53  yjiang
 * add Gen 2 parameter validation and clean up for T&V
 *
 * Revision 1.5  2006/01/04 23:12:41  yjiang
 * WIP for WJ Gen 2 support
 *
 * Revision 1.4  2005/12/21 18:31:30  yjiang
 * Work in progress for Gen 2 support for WJ module
 *
 * Revision 1.3  2005/12/13 21:11:12  yjiang
 * add G2 session as capacity
 *
 * Revision 1.2  2005/12/07 23:24:21  yjiang
 * add CVS header
 *
 *
 */


#ifndef RFIDCAPSENGINE_H
#define RFIDCAPSENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rfiddefs.h"

//#include "Imgdef.h"

//--------------------------------------------------------------------
// Nested Includes
//--------------------------------------------------------------------

// Defines for capability data types

#define TWTY_INT8        0x0000    /* Means Item is a TW_INT8   */
#define TWTY_INT16       0x0001    /* Means Item is a TW_INT16  */
#define TWTY_INT32       0x0002    /* Means Item is a TW_INT32  */

#define TWTY_UINT8       0x0003    /* Means Item is a TW_UINT8  */
#define TWTY_UINT16      0x0004    /* Means Item is a TW_UINT16 */
#define TWTY_UINT32      0x0005    /* Means Item is a TW_UINT32 */

#define TWTY_DWORD		TWTY_UINT32


#if 0
#define	TWTY_BOOL						0x0006
#endif //0

// Defines for capability container types
#define TWON_UNKNOWN					0
#define TWON_ARRAY						3
#define TWON_ENUMERATION				4
#define	TWON_ONEVALUE					5
#define TWON_RANGE						6

/****************************************************************************
 * Capability Constants                                                     *
 ****************************************************************************/


//-----------------------------------------------------------------------------
// Capability management fucntions
//-----------------------------------------------------------------------------


// cap bit/byte size for ONEVALUE
#define CAPINT8			(UINT32)(TWTY_INT8<<28)
#define CAPINT16		(UINT32)(TWTY_INT16<<28)
#define CAPINT32		(UINT32)(TWTY_DWORD<<28)
#define CAPUINT8		(UINT32)(TWTY_UINT8<<28)
#define CAPUINT16		(UINT32)(TWTY_UINT16<<28)
#define CAPUINT32		(UINT32)(TWTY_DWORD<<28)

// data container configuration
#define CONT_ONEVALUE	(UINT32)(TWON_ONEVALUE<<24)
#define CONT_ARRAY		(UINT32)(TWON_ARRAY<<24)

//-------------------------------------------------
// Defined capabilities
//-------------------------------------------------
#define RFID_INFCAP_SUPPORTEDCAPS			0x0000
											
#define RFID_READCAP_RF_ATTENUATION			0x0001
#define RFID_READCAP_EVENTNAME				0x0002
#define RFID_READCAP_EVENTTAGPTR			0x0003
											
#define RFID_WRITECAP_RF_ATTENUATION		0x0004
#define RFID_WRITECAP_TAGTYPE				0x0005
											
#define RFID_TAGCAP_LOCKCODE				0x0006
#define RFID_TAGCAP_SUPPORTED_TYPES			0x0007
#define RFID_TAGCAP_ENABLED_TYPES			0x0008

#define RFID_DEVCAP_IP_NAME					0x0009
#define RFID_DEVCAP_IP_PORT					0x000a
#define RFID_TAGCAP_C0_SINGULATION_FIELD	0x000b

#define RFID_READCAP_EVENT_ALLTAGS			0x000c

#define RFID_READCAP_METHOD					0x000d
#define RFID_READCAP_OUTLOOP				0x000e
#define RFID_READCAP_INLOOP					0x000f

#define RFID_DEVCAP_ANTENNA_SEQUENCE		0x0010

#define RFID_READCAP_READMODE				0x0011

#define RFID_WRITECAP_ANTENNA				0x0012

#define RFID_WRITECAP_C0_PAGE				0x0013
#define RFID_READCAP_C0_PAGE				0x0014

#define RFID_READCAP_EVENTTAGPTR_EX			0x0015
#define RFID_READCAP_DUTYCYCLE_ONTIME		0x0016
#define RFID_READCAP_DUTYCYCLE_OFFTIME_PCNT	0x0017

// Gen 2 inventory
#define RFID_READCAP_G2_SEL					0x0018
#define RFID_READCAP_G2_SESSION				0x0019
#define RFID_READCAP_G2_TARGET				0x001A
#define RFID_READCAP_G2_START_Q				0x001B

// Gen 2 read tag's memory bank (20h)
#define RFID_TAGCAP_G2_SELECT_MEM_BANK		0x001C
#define RFID_TAGCAP_G2_MEM_BANK				0x001D
#define RFID_TAGCAP_G2_WORD_POINTER			0x001E

#define RFID_WRITECAP_G2_WRITE_OPTIONS		0x001F
#define RFID_WRITECAP_G2_LOCK_OPTIONS		0x0020


// Gen 2 lock tags' memory (23h)
#define RFID_WRITECAP_G2_LOCK_MASK			0x0021
#define RFID_WRITECAP_G2_LOCK_ACTION		0x0022

// Gen 2 kill tag (24h)
#define RFID_TAGCAP_G2_ACCESS_PASSWORD		0x0023
#define RFID_TAGCAP_G2_KILL_PASSWORD		0x0024

#define RFID_DEVCAP_VALID_ANTENNA_LIST		0x0025

#define RFID_DEVCAP_ANTENNA_GROUP			0x0026

#define RFID_WRITECAP_G2_SELECT_ACTION		0x0027
#define RFID_WRITECAP_G2_SELECT_TARGET		0x0028
//#define RFID_WRITECAP_G2_SELECT_MEM_BANK	0x0029
#define RFID_TAGCAP_G2_REMOVE_SELECT_RECORD_INDEX	0x0029

#define RFID_READCAP_G2_MEM_BANK_WORD_LEN	0x002A

#define RFID_READCAP_DUTY_CYCLE_INTERVAL	0x02B

#define RFID_TAGCAP_G2_RF_ON_TIME			0x02C

#define RFID_TOTAL_CAPS						0x02D


#define RFID_READCAP_READMODE_ONDEMAND		0x00
#define RFID_READCAP_READMODE_AUTONOMOUS	0x01

#define RFID_TAGCAP_C0_SINGULATION_ID0		0x00
#define RFID_TAGCAP_C0_SINGULATION_ID1		0x01
#define RFID_TAGCAP_C0_SINGULATION_ID2		0x02

#define RFID_READCAP_C1_READMETHOD_TWALK	0x00
#define RFID_READCAP_C1_READMETHOD_MSCROLL	0x01


#ifdef __cplusplus
};
#endif

#endif