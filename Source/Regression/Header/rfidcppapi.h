/* $Source: /home/cvs/cvs_root/RFIDAPI3/src/lib/win32/BackwardCompatibility/Include/Attic/rfidcppapi.h,v $
 *
 * $Revision: 1.1.2.1 $
 * $Date: 2008/11/18 02:51:03 $
 * $Author: ggayathri $
 *
 * $Log: rfidcppapi.h,v $
 * Revision 1.1.2.1  2008/11/18 02:51:03  ggayathri
 * DLL Headers for RFIDAPI2 Backward compatibility
 *
// 
//    Rev 1.13   Jan 16 2008 14:45:10   wrighte
// Commented out OQueue::SWTools.cpp::line:800
// Modified RFIDDLL::RFIDPrivate.h::line:177 to 3.15B Version
// 
//    Rev 1.12   Oct 12 2006 17:08:10   a00011001
// No change.
// 
//    Rev 1.11   Sep 26 2006 11:40:06   ohaire
// Version 3.01 checkin from CVS
 * Revision 1.2  2005/12/07 23:24:21  yjiang
 * add CVS header
 *
 *
 */


/*****************************************************************************/
/*

File Name		:	rfidcppapi.h

Copyright(c) Symbol Technologies Inc. 2004. All rights reserved.

Description		:	RFID CPP API function prototypes

Notes			:	These functions are equivalent to those offered via the C API with the following exceptions:

						1.	This C Class opens it's own handle on class construction. and releases it on destruction.
						2.	This C Class owns the handle, therefore the class functions do not require the hReader handle as a parameter,

Author			:	Mike O'Haire

Creation Date	:	02JUN2004

Derived From :	new file

Edit History : 

$Log:

*/
/*****************************************************************************/
#ifndef RFIDCPPAPI_H
#define RFIDCPPAPI_H

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

class RFIDDLL_API CRFIDReader
{
private:
	// Handle used internally by 'CPP' class.  (Same as handle used in 'C' callable interface
	HANDLE hReader;
public:
	RFIDDLL_API_CD CRFIDReader(void);
	RFIDDLL_API_CD virtual ~CRFIDReader(void);

	DWORD WINAPI FindFirst(RFID_FINDINFO *lptRFIDFindInfo, LPHANDLE phFind);
	DWORD WINAPI FindNext(RFID_FINDINFO *lptRFIDFindInfo, HANDLE hFind);
	DWORD WINAPI FindClose(HANDLE hFind);

	// returns function return status as human readable english string
	const TCHAR * WINAPI GetCommandStatusText(int stat);

	// Opens/Closes port used by RFID module
	DWORD WINAPI OpenReader(int port);
	DWORD WINAPI CloseReader();

	// turns card power on or off.  (Used to increase performance)
	DWORD WINAPI SetPowerState(BOOL bPowerState);
	DWORD WINAPI GetPowerState(BOOL *pbPowerState);

	// Each reader can have a number.  Allows user to get/set reader number.
	DWORD WINAPI SetReaderNumber(unsigned char ReaderNumber);
	DWORD WINAPI GetReaderNumber(unsigned char *ReaderNumber);

	// RF transmission functions
	DWORD WINAPI GetRFPowerState(BOOL *bRFPowerState);
	DWORD WINAPI SetRFPowerState(BOOL bRFPowerState);

	DWORD WINAPI GetRFAttenuation(unsigned char *cRFAttenuationDB);
	DWORD WINAPI SetRFAttenuation(unsigned char cRFAttenuationDB);

	// Tag Functions
	DWORD WINAPI SetTagMask(TAG_MASK *pTagMask);
	DWORD WINAPI GetTagMask(TAG_MASK *pTagMask);
	DWORD WINAPI GetTagID(TYPE_TAG *pTag);
	DWORD WINAPI ReadTagInventory(TAG_LIST *pTagList, BOOL bClearInventory);
	DWORD WINAPI ReadTagInventoryScroll(TAG_LIST *pTagList, BOOL bClearScrollTagInventory);
	DWORD WINAPI ReadTagInventoryArray(TAG_LIST *pTagList, BOOL bClearInventory, unsigned char inCnt, unsigned char *inArray);
	DWORD WINAPI ReadTagInventoryParamList(TAG_LIST *pTagList, BOOL bClearInventory, unsigned char cInventoryType, unsigned char cOuterLoop, unsigned char cInnerLoop, unsigned char cOuterWakes, unsigned char cInnerOuterWakes, unsigned char cHoppingFlag, unsigned char cNumSleeps);
	DWORD WINAPI ProgramTags(unsigned char *pTagID, unsigned char cTagLength, unsigned char cTagVerifyCount, unsigned char cEraseAttempts, unsigned char cProgramAttempts);
	DWORD WINAPI EraseTag(unsigned char cTagVerifyCount, unsigned char cEraseAttempts);
	DWORD WINAPI LockTag(unsigned char cTagLength, unsigned char cTagVerifyCount, unsigned char cLockAttempts, unsigned char cTagKillCode);
	DWORD WINAPI KillTag(unsigned char *pTagID, unsigned char cTagLength, unsigned char cTagVerifyCount, unsigned char cKillAttempts, unsigned char cTagKillCode);
	DWORD WINAPI GetTagList(TAG_LIST *pTagList);

	// Issues hardware reset command to radio module
	DWORD WINAPI Reboot(void);

	// Antenna Functions  Not user accessable.
	DWORD WINAPI GetActiveAntenna(unsigned char *pActiveAntenna);
	DWORD WINAPI SetActiveAntenna(unsigned char ActiveAntenna);
	DWORD WINAPI GetAntennaSequence(unsigned char *pAntennaSequence, int *nLength);
	DWORD WINAPI SetAntennaSequence(unsigned char *pAntennaSequence, int nLength);

	// Flash Firmware functions
	DWORD WINAPI FlashFirmware(TCHAR *szFlashFileNameParam);
	DWORD WINAPI FlashFirmwareStop(void);
	DWORD WINAPI SetStatusMessage(HWND hWnd, int nMessage);

	// Debug functions
	DWORD WINAPI SetTXMessage(HWND hWnd, int nMessage);
	DWORD WINAPI SetRXMessage(HWND hWnd, int nMessage);

	// Monitor / agent functions
	DWORD WINAPI GetStats(RFID_STATS *pStats);
	DWORD WINAPI GetCaps(RFID_CAPS *pCaps);
	DWORD WINAPI ClearStats(void);

	DWORD WINAPI EnableTriggerMessage(HANDLE *phTrigger, BOOL bFlag, HWND hDlg, unsigned int nTriggerMessage, DWORD dwTriggerMask);
	DWORD WINAPI EnableTriggerCallback(HANDLE *phTrigger, BOOL bFlag, void (*pTriggerCallbackParam)(DWORD dwTriggerStatus), DWORD dwTriggerMask);

	// Retrieves DWORD list of supported caps 
	DWORD WINAPI GetCapList(DWORD *pdwCapId, DWORD *pNumCaps);

	// Retrieves information on the specified capability
	DWORD WINAPI GetCapInfo(DWORD dwCapId, LPCAPINFO lpCapInfo);

	// Retrieves the current value of a capability
	DWORD WINAPI GetCapCurrValue(DWORD dwCapId, DWORD *pdwNumItems, DWORD dwValueBufSize, LPVOID pvValueBuf);

	// Retrieves the default value of a capability
	DWORD WINAPI GetCapDfltValue(DWORD dwCapId, DWORD *pdwNumItems, DWORD dwValueBufSize, LPVOID pvValueBuf);

	// Sets the current value of a capability
	DWORD WINAPI SetCapCurrValue(DWORD dwCapId, DWORD *pdwNumItems, DWORD dwValueBufSize, LPVOID pvValueBuf);

	// Sets the default values for all capabilities
	DWORD WINAPI SetCapDflts(void);

	// Sets the default value of a capability
	DWORD WINAPI SetCapDfltValue(DWORD dwCapId);

	// tag filtering extensions

	// add a tag filter.  limits tag reads to specified filters added.
	DWORD WINAPI TagFilterAdd(TCHAR *pFilterName, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, unsigned char *pMatchBits, DWORD dwOptionMask, void (*pMatchCallBack)(TYPE_TAG *pTag, TCHAR *pFilterName, DWORD dwBitsToMatch, DWORD dwBitMatchOffset, const unsigned char *pMatchBits, DWORD dwOptionMask));

	// remove a specific filter.  User must specify the same filter as one previously added
	DWORD WINAPI TagFilterRemove(DWORD dwBitsToMatch, DWORD dwBitMatchOffset, unsigned char *pMatchBits, DWORD dwOptionMask);

	// remove a filter with the specified name
	DWORD WINAPI TagFilterRemoveByName(TCHAR *pFilterName);

	// remove all current tag filters 
	DWORD WINAPI TagFilterListFlush(void);
};

#endif //	#ifndef RFIDCPPAPI_H