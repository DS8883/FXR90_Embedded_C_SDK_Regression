/* $Source: /home/cvs/cvs_root/RFIDAPI3/src/lib/win32/BackwardCompatibility/Include/Attic/rfidcapi.h,v $
 *
 * $Revision: 1.1.2.1 $
 * $Date: 2008/11/18 02:51:03 $
 * $Author: ggayathri $
 *
 * $Log: rfidcapi.h,v $
 * Revision 1.1.2.1  2008/11/18 02:51:03  ggayathri
 * DLL Headers for RFIDAPI2 Backward compatibility
 *
 * Revision 1.1.1.1  2008/07/21 11:40:47  rsuresh
 * no message
 *
// 
//    Rev 1.16   Jul 03 2008 05:53:26   gayathrig
// MC9090 R3.2 Support for En 302-208
// 
//    Rev 1.13.1.0   May 06 2008 05:21:50   dhard
// for dutycycle maintainance
// 
//    Rev 1.13   Oct 01 2007 09:58:14   guptaa
// MC9090 ER bug fixes
// Remove select command added for RD5000
// 
//    Rev 1.11   Sep 26 2006 11:40:00   ohaire
// Version 3.01 checkin from CVS
 * Revision 1.4  2006/06/12 22:08:40  yjiang
 * Merge Apex code change with XR400 C-API code cut 4
 *
 * Revision 1.3.2.1  2006/06/12 21:40:51  yjiang
 * XR400 C-API code cut 4
 *
 * Revision 1.3  2006/05/17 22:26:41  yjiang
 * WIP for XR400 API
 * Wrap RFID transport into lock protection
 * Add GPIO support for XR400
 *
 * Revision 1.2  2005/12/07 23:24:21  yjiang
 * add CVS header
 *
 *
 */


/*****************************************************************************/
/*

File Name		:	rfidcapi.h

Copyright(c) Symbol Technologies Inc. 2004. All rights reserved.

Description		:	RFID C API function prototypes

Notes			:	These functions are equivalent to those offered via the CRFIDReader Class API with the following exceptions:

						1.	This API interface requires the application get a hReader handle via a call to RFID_Open().
						2.	The application should call RFID_Close(hReader) When the RFID reader or hReader handle is no longer needed.
						3.	Each of these functions require the hReader handle returned via the RCM_Open() function.

Author			:	Mike O'Haire

Creation Date	:	02JUN2004

Derived From :	new file

Edit History : 

$Log:

*/
/*****************************************************************************/
#ifndef RFIDCAPI_H
#define RFIDCAPI_H

// include file that contains a common set of defines used by the CRFIDReader Class, and the 'C' api application interface
#include "rfiddefs.h"
#include "rfidcapsengine.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RFIDDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RFIDDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifdef RFIDDLL_EXPORTS
#define RFIDDLL_API //__declspec(dllexport)
#define RFIDDLL_API_CD	__declspec(dllexport)
#else
#define RFIDDLL_API __declspec(dllimport)
#define RFIDDLL_API_CD
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

// Function declarations

DWORD WINAPI RFID_FindFirst(HANDLE hReader, RFID_FINDINFO *ptRFIDFindInfo, LPHANDLE phFind);
DWORD WINAPI RFID_FindNext(HANDLE hReader, RFID_FINDINFO *lptRFIDFindInfo, HANDLE hFind);
DWORD WINAPI RFID_FindClose(HANDLE hReader, HANDLE hFind);

DWORD WINAPI RFID_Open(HANDLE *phReader);
DWORD WINAPI RFID_Close(HANDLE *phReader);
const TCHAR *WINAPI RFID_GetCommandStatusText(HANDLE hRFIDReader, int stat);
DWORD WINAPI RFID_GetStats(HANDLE hReader, RFID_STATS *pStats);
DWORD WINAPI RFID_GetCaps(HANDLE hReader, RFID_CAPS *pCaps);
DWORD WINAPI RFID_RegisterMonitorEvent(HANDLE hReader, HANDLE *phMonitor);
DWORD WINAPI RFID_SetTXMessage(HANDLE hReader, HWND hWnd, int nMessage);
DWORD WINAPI RFID_SetRXMessage(HANDLE hReader, HWND hWnd, int nMessage);
DWORD WINAPI RFID_GetPowerState(HANDLE hReader, BOOL *bPowerState);
DWORD WINAPI RFID_SetPowerState(HANDLE hReader, BOOL bNewPowerState);
DWORD WINAPI RFID_SetRFPowerState(HANDLE hReader, BOOL bRFPowerState);
DWORD WINAPI RFID_GetRFPowerState(HANDLE hReader, BOOL *bRFPowerState);

DWORD WINAPI RFID_GetTagID(HANDLE hReader, TYPE_TAG *pTag);
DWORD WINAPI RFID_GetTagIDEX(HANDLE hReader, TYPE_TAG_EX *pTag);

DWORD WINAPI RFID_OpenReader(HANDLE hReader, int port);
DWORD WINAPI RFID_CloseReader(HANDLE hReader);
DWORD WINAPI RFID_GetReaderNumber(HANDLE hReader, unsigned char *pReaderNumber);
DWORD WINAPI RFID_SetReaderNumber(HANDLE hReader, unsigned char ReaderNumberSet);
DWORD WINAPI RFID_Reboot(HANDLE hReader);
DWORD WINAPI RFID_GetRFAttenuation(HANDLE hReader, unsigned char *cRFAttenuationDB);
DWORD WINAPI RFID_SetRFAttenuation(HANDLE hReader, unsigned char cRFAttenuationDB);
DWORD WINAPI RFID_ProgramTags(HANDLE hReader, unsigned char *pTagID, unsigned char cTagLength, unsigned char cTagVerifyCount, unsigned char cEraseAttempts, unsigned char cProgramAttempts);
DWORD WINAPI RFID_EraseTag(HANDLE hReader, unsigned char cTagVerifyCount, unsigned char cEraseAttempts);
DWORD WINAPI RFID_LockTag(HANDLE hReader, unsigned char cTagLength, unsigned char cTagVerifyCount, unsigned char cLockAttempts, unsigned char cTagKillCode);
DWORD WINAPI RFID_KillTag(HANDLE hReader, unsigned char *pTagID, unsigned char cTagLength, unsigned char cTagVerifyCount, unsigned char cKillAttempts, unsigned char cTagKillCode);

DWORD WINAPI RFID_SetTagMask(HANDLE hReader, TAG_MASK *pTagMask);
DWORD WINAPI RFID_SetTagMaskEX(HANDLE hReader, TAG_MASK_EX *pTagMask);

DWORD WINAPI RFID_RemoveTagMaskEX(HANDLE hReader);
DWORD WINAPI RFID_GetTagMask(HANDLE hReader, TAG_MASK *pTagMask);
DWORD WINAPI RFID_GetTagMaskEX(HANDLE hReader, TAG_MASK_EX *pTagMask);

DWORD WINAPI RFID_ReadTagInventoryArray(HANDLE hReader, TAG_LIST *pTagList, BOOL bClearInventory, unsigned char inCnt, unsigned char *inArray);
DWORD WINAPI RFID_ReadTagInventoryScroll(HANDLE hReader, TAG_LIST *pTagList, BOOL bClearScrollTagInventory);
DWORD WINAPI RFID_ReadTagInventoryParamList(HANDLE hReader, TAG_LIST *pTagList, BOOL bClearInventory, unsigned char cInventoryType, unsigned char cOuterLoop, unsigned char cInnerLoop, unsigned char cOuterWakes, unsigned char cInnerOuterWakes, unsigned char cHoppingFlag, unsigned char cNumSleeps);

DWORD WINAPI RFID_ReadTagInventory(HANDLE hReader, TAG_LIST *pTagList, BOOL bClearInventory);
DWORD WINAPI RFID_ReadTagInventoryEX(HANDLE hReader, void *pTagList, BOOL bClearInventory);

DWORD WINAPI RFID_SetStatusMessage(HANDLE hReader, HWND hWnd, int nMessage);
DWORD WINAPI RFID_FlashFirmware(HANDLE hReader, TCHAR *szFlashFileNameParam);
DWORD WINAPI RFID_FlashFirmwareStop(HANDLE hReader);
DWORD WINAPI RFID_EnableTriggerCallback(HANDLE *phTrigger, HANDLE hReader, BOOL bFlag, void (*pTriggerCallbackParam)(DWORD dwTriggerStatus), DWORD dwTriggerMask);
DWORD WINAPI RFID_EnableTriggerMessage(HANDLE *phTrigger, HANDLE hReader, BOOL bFlag, HWND hDlg, unsigned int nTriggerMessage, DWORD dwTriggerMask);

DWORD WINAPI RFID_GetDutyCycleStats(HANDLE hReader,DUTY_CYCLE_PARAMETER	*);
DWORD WINAPI RFID_SelectTxChannel(HANDLE hReader,unsigned char cChannelMask);

// Extensions
// Retrieves DWORD list of supported caps 

DWORD WINAPI RFID_GetCapList(HANDLE	hReader, DWORD *pdwCapId, DWORD *pNumCaps);

// Retrieves information on the specified capability
DWORD WINAPI RFID_GetCapInfo(HANDLE	hReader, DWORD dwCapId, LPCAPINFO lpCapInfo);

// Retrieves the current value of a capability
DWORD WINAPI RFID_GetCapCurrValue(HANDLE hReader, DWORD dwCapId, DWORD *pdwNumItems, DWORD dwValueBufSize, LPVOID pvValueBuf);

// Retrieves the default value of a capability
DWORD WINAPI RFID_GetCapDfltValue(HANDLE hReader, DWORD dwCapId, DWORD *pdwNumItems, DWORD dwValueBufSize, LPVOID pvValueBuf);

// Sets the current value of a capability
DWORD WINAPI RFID_SetCapCurrValue(HANDLE hReader, DWORD dwCapId, DWORD *pdwNumItems, DWORD dwValueBufSize, LPVOID pvValueBuf);

// Sets the default values for all capabilities
DWORD WINAPI RFID_SetCapDflts(HANDLE hReader);

// Sets the default value of a capability
DWORD WINAPI RFID_SetCapDfltValue(HANDLE hReader, DWORD dwCapId);


// Tag Filter extensions

// add a tag filter.  limits tag reads to specified filters added.
DWORD WINAPI RFID_TagFilterAdd(HANDLE hReader, TCHAR *pFilterName, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, unsigned char *pMatchBits, DWORD dwOptionMask, void (*pMatchCallBack)(TYPE_TAG *pTag, TCHAR *pFilterName, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, const unsigned char *pMatchBits, DWORD dwOptionMask));

DWORD WINAPI RFID_TagFilterAddEX(HANDLE hReader, TCHAR *pFilterName, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, unsigned char *pMatchBits, DWORD dwOptionMask, void (*pMatchCallBack)(TYPE_TAG_EX *pTag, TCHAR *pFilterName, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, const unsigned char *pMatchBits, DWORD dwOptionMask));


DWORD WINAPI RFID_TagFilterRemove(HANDLE hReader, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, unsigned char *pMatchBits, DWORD dwOptionMask);

DWORD WINAPI RFID_TagFilterRemoveByName(HANDLE hReader, TCHAR *pFilterName);

DWORD WINAPI RFID_TagFilterListFlush(HANDLE hReader);


DWORD WINAPI RFID_GetGPIOValue(HANDLE hReader, BYTE *pbInputValue, BYTE *pbOutputValue);
DWORD WINAPI RFID_SetGPOValue(HANDLE hReader, BYTE GPOMask, BYTE bValue);
DWORD WINAPI RFID_SetGPIDetection(HANDLE hReader, HANDLE hNotifyEvent, BYTE bMask, BYTE bInterval);
DWORD WINAPI RFID_GetGPIEventValue(HANDLE hReader, BYTE *pbChangeMask, BYTE *pbCurrentValue, bool resetMask);

#if defined(__cplusplus)
}



#endif 

#endif //	#ifndef RFIDCAPI_H