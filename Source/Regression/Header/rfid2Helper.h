
#ifndef RFID2HELPER_H
#define RFID2HELPER_H
#endif

#ifndef linux
#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include "rfidcapi.h"
#include "RFIDCapsEngine.h"
#include "rfiddefs.h"
#include "rfidapi.h"

struct RFID2DATA
{
   	HANDLE hReader;			// Handle to the Reader
	DWORD dwPort;			// Port Nulmber to which reader connecting
	wchar_t szIPAddress[512];	// IP of the Reader to connect
	BOOLEAN bContinue;			// For thread Sync
	DWORD rfidStatus;		// return value of the routine
};

void IntialiseReader(void);																	//2
//void DisplayCapabilities( HANDLE hReader );																//3
void CloseConnection( HANDLE hReader );																	//4		
void ConfigureAntenna(HANDLE hReader,char cReadAttenuation, char cWriteAttenuation);						//5
void ConfigureTagSupport(HANDLE hReader,bool Class0,bool Class0Plus,bool Class1,bool G2C1);				//6
BOOLEAN ConfigureReadTagEvent(HANDLE hReader, wchar_t *szUserReadTagEventName);

DWORD RFID2ReadTags( HANDLE hReader, BYTE inloop,BYTE outloop);
BOOLEAN SetCapValue( HANDLE hReader,int CapID, unsigned char CapValue );
BOOLEAN SetCapValueWORD( HANDLE hReader,int CapID, WORD CapValue );
BOOLEAN SetCapValueDWORD( HANDLE hReader,int CapID, DWORD CapValue );
void SetTAG_MASK_EX( HANDLE hReader, unsigned int nBitLen,unsigned int nOffset,unsigned int nMemBank,unsigned int nBitEPCLen,UINT8 *mask);
#endif

