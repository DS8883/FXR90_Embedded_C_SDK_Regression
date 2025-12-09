// rfid2Helper.cpp : RFID2 Interface Helper Routines
#ifndef _WIN64 
#ifndef linux 
#include "rfidcapi.h"
#include "RFIDCapsEngine.h"
#include "rfiddefs.h"
#include "rfidapi.h"
#include "rfid2Helper.h"
#include "common.h"

RFID2DATA api2Data;					// API2 Data
extern struct RFID3DATA api3Data;	// API3 Data

TYPE_TAG SiTag;			// Tag Data
TYPE_TAG_EX SiTagEX;	// Extended TagData 

TAG_LIST SiTagList;		// Taglist
TAG_LIST_EX SiTagListEX;// Extended Tag List
	
TAG_MASK SiTagMask;		// Tag Mask
TAG_MASK_EX SiTagMaskEX;// Extended Tag Mask

// RFID2 Initialization
void IntialiseReader(void)
{
	DWORD dwStatus;
	DWORD dwNumItems;
	DWORD dwSize;
	HANDLE hFind;				
	RFID_FINDINFO SiFindInfo;	

	fprintf( api3Data.fpLog,"\n\n***InSide the Function IntialiseReader****");
	
	dwStatus = RFID_Open(&api2Data.hReader);
	fprintf(api3Data.fpLog,"\nCalling the RFID_Open function- Status :%S",RFID_GetCommandStatusText(api2Data.hReader, dwStatus));

	dwStatus = RFID_FindFirst( api2Data.hReader,&SiFindInfo,&hFind);
	fprintf( api3Data.fpLog,"\nCalling the RFID_FindFirst function- Status :%S",RFID_GetCommandStatusText( api2Data.hReader, dwStatus));
	fprintf( api3Data.fpLog,"\n Found the Device on PORT %d", SiFindInfo.nPortNumber );
	fprintf( api3Data.fpLog,"\n IP of the Device is %S", SiFindInfo.tszDeviceName );
	
	dwStatus = RFID_FindNext( api2Data.hReader,&SiFindInfo,&hFind);
	fprintf( api3Data.fpLog,"\nCalling the RFID_FindNext function- Status :%S",RFID_GetCommandStatusText( api2Data.hReader, dwStatus));
	fprintf( api3Data.fpLog,"\n Found the Device on PORT %d", SiFindInfo.nPortNumber );
	fprintf( api3Data.fpLog,"\n IP of the Device is %S", SiFindInfo.tszDeviceName );
	dwStatus = RFID_FindClose( api2Data.hReader, hFind );
	

	dwNumItems = 1;
	dwSize = sizeof(api2Data.dwPort);
	dwStatus = RFID_SetCapCurrValue( api2Data.hReader, RFID_DEVCAP_IP_PORT, &dwNumItems, dwSize, &api2Data.dwPort);
	fprintf(api3Data.fpLog,"\nCalling the SetCapCurr function for IP_PORT - Status :%S",RFID_GetCommandStatusText(api2Data.hReader, dwStatus));


	dwNumItems = wcslen(api2Data.szIPAddress) + 1; //+1 for the null character
	dwSize = dwNumItems * sizeof(api2Data.szIPAddress[0]);
	dwStatus = RFID_SetCapCurrValue( api2Data.hReader, RFID_DEVCAP_IP_NAME, &dwNumItems, dwSize, api2Data.szIPAddress );
	fprintf(api3Data.fpLog,"\nCalling the SetCapCurr function for IP_NAME - Status :%S",RFID_GetCommandStatusText(api2Data.hReader, dwStatus));
	
	dwStatus = RFID_OpenReader(api2Data.hReader,api2Data.dwPort );
	fprintf(api3Data.fpLog,"\nCalling the OPenReader function - Status :%S",RFID_GetCommandStatusText(api2Data.hReader, dwStatus));
}
// Set Capability Value( UINT )
BOOLEAN SetCapValue( HANDLE hReader,int CapID, unsigned char CapValue )
{
	DWORD dwStatus,dwNumItems,dwSize;	
	unsigned char ucharTemp=0;
	dwNumItems = 1;					
	dwSize = sizeof(CapValue);
	dwStatus = RFID_SetCapCurrValue( hReader, CapID, &dwNumItems, dwSize, &CapValue); 
	if( RFID_SUCCESS != dwStatus )
	{
		return dwStatus;
	}

	dwStatus = RFID_GetCapCurrValue( hReader, CapID, &dwNumItems, dwSize, &ucharTemp); 

	if( RFID_SUCCESS != dwStatus )
	{
		return dwStatus;
	}

	if ( CapValue == ucharTemp )
		return RFID_SUCCESS;
	else
	{
		fprintf( api3Data.fpLog,"\nThe RFID_READCAP_G2_SEL : %d Could not be Set ERRORCODE: %d", CapValue,dwStatus );
		return dwStatus;
	}
}
// Set Capability Value( DW )
BOOLEAN SetCapValueDWORD( HANDLE hReader,int CapValue, DWORD dwCap )
{
	DWORD dwStatus,dwNumItems,dwSize;	
	DWORD dwgetCap=0;
	dwNumItems = 1;					
	dwSize = sizeof( DWORD );
	dwStatus = RFID_SetCapCurrValue(api2Data.hReader, CapValue, &dwNumItems, dwSize, &dwCap); 
	
	dwStatus = RFID_GetCapCurrValue(hReader, CapValue, &dwNumItems, dwSize, &dwgetCap); 

	if (dwCap==dwgetCap)
		return TRUE;
	else
	{
		fprintf(api3Data.fpLog,"\nThe RFID_READCAP_G2_SEL : %d Could not be Set",dwCap);
		return FALSE;
	}
}
// Set Capability Value( W )
BOOLEAN SetCapValueWORD( HANDLE hReader,int CapValue, WORD wCap )
{
	DWORD dwStatus,dwNumItems,dwSize;	
	WORD wgetCap=0;
	dwNumItems = 1;					
	dwSize = sizeof( wCap );
	dwStatus = RFID_SetCapCurrValue(api2Data.hReader, CapValue, &dwNumItems, dwSize, &wCap); 
	
	dwStatus = RFID_GetCapCurrValue(hReader, CapValue, &dwNumItems, dwSize, &wgetCap); 

	if( wCap == wgetCap )
		return TRUE;
	else
	{
		fprintf(api3Data.fpLog,"\nThe RFID_READCAP_G2_SEL : %d Could not be Set",wCap);
		return FALSE;
	}
}

void StringToByteArray(char *s, int len,unsigned char *pResult)
{
	for(int i=0;i<len;i++)
		pResult[i] = 0;
	byte val,temp;
	int j;
	for(int i=0;i<len;i++)
	{
		j=i*2;

		temp = (byte)s[j];
		if(temp >= 48 && temp <= 57 )
			temp = (byte)(temp - 48);
		else if(temp >= 65 && temp <= 70 )
			temp = (byte)(temp - 55);
		else if(temp >= 97 && temp <= 102 )
			temp = (byte)(temp - 87);
		
		val = temp;
		j++;
		temp = (byte)s[j];
		if(temp >= 48 && temp <= 57 )
			temp = (byte)(temp - 48);
		else if(temp >= 65 && temp <= 70 )
			temp = (byte)(temp - 55);
		else if(temp >= 97 && temp <= 102 )
			temp = (byte)(temp - 87);
		 pResult[i] = (byte)((val<<4) | temp);
	}

}

void SetTAG_MASK_EX( HANDLE hReader, unsigned int nBitLen,unsigned int nOffset,unsigned int nMemBank,unsigned int nBitEPCLen,UINT8 *mask)
{
		
	unsigned char cTagMask[RFID_MAX_TAG_ID_LENGTH_EX];	
	int len;
		
	len  = (nBitLen+7)/8 ; 
	TAG_MASK_EX1_INIT(SiTagMaskEX);
	SiTagMaskEX.nBitLen = nBitLen;
	SiTagMaskEX.nBitStartOffset = nOffset;
	SiTagMaskEX.nMemBank = nMemBank;
	SiTagMaskEX.nBitLenEPCID  = nBitEPCLen;

	if(len!=0)
	{
		memcpy(SiTagMaskEX.cTagMask,mask,len );
		/*StringToByteArray(mask,len, cTagMask);
		for (int i=0;i<len;i++)
			SiTagMaskEX.cTagMask[i] = cTagMask[i];*/

	}
	
	DWORD dwStatus = RFID_SetTagMaskEX(hReader, &SiTagMaskEX);
	fprintf(api3Data.fpLog,"\nCalling the RFID_SetTagMaskEX function - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));

}

DWORD SetG2MemBankOffsetAndLength(HANDLE hReader, BYTE bG2MemBank, WORD wOffset, WORD wLength)
{
	DWORD dwStatus;
	DWORD dwNumItems;

	BYTE blength;
	
	blength = (BYTE)wLength;
	dwNumItems = 1;
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_TAGCAP_G2_WORD_POINTER, &dwNumItems, sizeof(wOffset), &wOffset);
	
	//LogMessageWithTimeStamp("WJLog.txt", "Number of words to read: %d", blength);
	
	dwNumItems = 1;
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_READCAP_G2_MEM_BANK_WORD_LEN, &dwNumItems, sizeof(blength), &blength);
	dwStatus = RFID_GetCapCurrValue(hReader, RFID_READCAP_G2_MEM_BANK_WORD_LEN, &dwNumItems, sizeof(blength), &blength);
//	LogMessageWithTimeStamp("WJLog.txt", "Number of words to read: %d", blength);

	// setup the G2 mem bank to write
	dwNumItems = 1;
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_TAGCAP_G2_MEM_BANK, &dwNumItems, sizeof(bG2MemBank), &bG2MemBank);

	return(dwStatus);

};

BOOLEAN ConfigureReadTagEvent(HANDLE hReader, wchar_t *szUserReadTagEventName)
{
	DWORD dwNumItems;
	wchar_t szReadTagEventName[255]; 
	wcscpy(szReadTagEventName, szUserReadTagEventName);
	dwNumItems = wcslen(szUserReadTagEventName);
	if(dwNumItems)
	{
		dwNumItems++;										// include the null character
	};
	DWORD dwResult = RFID_SetCapCurrValue(hReader, RFID_READCAP_EVENTNAME, &dwNumItems, dwNumItems * sizeof(szUserReadTagEventName[0]), szUserReadTagEventName);
	return(0);
};

void ConfigureAntenna(HANDLE hReader, char cReadAttenuation, char cWriteAttenuation)
{
	char cAntennaSequence[2];
	DWORD dwStatus;
	DWORD dwNumItems;
	DWORD dwSize;

	fprintf(api3Data.fpLog,"\n\n***InSide the Function Configure Antenna****");

	cAntennaSequence[ 0 ] = 1;		// First antenna
	
	//cAntennaSequence[ 1 ] = 1;		// Second antenna
	//cAntennaSequence[2] = 1;		// Third antenna //Usefull for XR 
//	cAntennaSequence[3] = 1;		// Fourth antenna // Useful for XR

	
				// For MC9090 WJ module has only two antennas
	dwSize = sizeof(cAntennaSequence[0])*2;
	dwNumItems = dwSize/sizeof(cAntennaSequence[0]);
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_DEVCAP_ANTENNA_SEQUENCE, &dwNumItems, dwSize, cAntennaSequence); 
	fprintf(api3Data.fpLog,"\nCalling the SetCapCurrVal function for Antenna_Sequence - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));
	

					
	dwSize = sizeof(cReadAttenuation);
	dwNumItems = 1;	
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_READCAP_RF_ATTENUATION, &dwNumItems, dwSize, &cReadAttenuation); 
	fprintf(api3Data.fpLog,"\nCalling the SetCapCurrVal function for Read Attenuation - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));

				
	dwSize = sizeof(cWriteAttenuation);
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_WRITECAP_RF_ATTENUATION, &dwNumItems, dwSize, &cWriteAttenuation); 
	fprintf(api3Data.fpLog,"\nCalling the SetCapCurrVal function for Write Attenuation - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));
	
}

void ConfigureTagSupport(HANDLE hReader, bool Class0,bool Class0Plus,bool Class1,bool G2C1)
{
	BYTE bEnabledTagTypesParam[5];
	BYTE bSupportedTagTypesParam[5];
	DWORD dwStatus;
	DWORD dwNumItems;
	DWORD dwSize;

	fprintf(api3Data.fpLog,"\n\n***InSide the Function ConfigureTagSupport ****");
	
	ZeroMemory(bEnabledTagTypesParam, sizeof(bEnabledTagTypesParam));
	ZeroMemory(bSupportedTagTypesParam, sizeof(bSupportedTagTypesParam));

	dwNumItems= sizeof(bSupportedTagTypesParam) / sizeof(bSupportedTagTypesParam[0]);
	// Buffer size is in bytes
	dwSize= sizeof(bSupportedTagTypesParam);
		
	dwStatus = RFID_GetCapCurrValue(hReader, RFID_TAGCAP_SUPPORTED_TYPES, &dwNumItems, dwSize, bSupportedTagTypesParam);
	fprintf(api3Data.fpLog,"\nCalling the GetCapCurrVal fn For RFID_TAGCAP_SUPPORTED_TYPES - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));
	dwStatus = RFID_GetCapCurrValue(hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize, bEnabledTagTypesParam);
	fprintf(api3Data.fpLog,"\nCalling the GetCapCurrVal fn For RFID_TAGCAP_ENABLED_TYPES - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));
	
	fprintf(api3Data.fpLog,"\nSupported Tag Types....\n");
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0 : %d",bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0PLUS : %d",bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0PLUS]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS1 : %d",bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASS1]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASSG2 : %d",bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASSG2]);

	fprintf(api3Data.fpLog,"\nCurrently Enabled Tag Types....\n");
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0PLUS : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0PLUS]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS1 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS1]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASSG2 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASSG2]);
	
	ZeroMemory(bEnabledTagTypesParam, sizeof(bEnabledTagTypesParam));

	if(Class0==true)//if(Class0==true && bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0])
		bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0] = 1;
	if(Class0Plus==true)//if(Class0Plus==true && bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0PLUS])
		bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0PLUS] = 1;
	if(Class1==true)//if(Class1==true && bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASS1])
		bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS1] = 1;
	if(G2C1==true)//if(G2C1==true && bSupportedTagTypesParam[RFID_TAG_TYPE_EPC_CLASSG2])
		bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASSG2] = 1;

	fprintf(api3Data.fpLog,"\nNew Enabled Tag Types....\n");
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0PLUS : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0PLUS]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS1 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS1]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASSG2 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASSG2]);
	dwSize = dwNumItems *sizeof(bEnabledTagTypesParam[0]);
	
	/*dwSize = sizeof(bEnabledTagTypesParam);
	dwNumItems = dwSize/sizeof(bEnabledTagTypesParam[0]);*/
	
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize, bEnabledTagTypesParam);
	fprintf(api3Data.fpLog,"\nCalling the SetCapCurrVal fn For RFID_TAGCAP_SUPPORTED_TYPES - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));

	dwStatus = RFID_GetCapCurrValue(hReader, RFID_TAGCAP_ENABLED_TYPES, &dwNumItems, dwSize, bEnabledTagTypesParam);
	fprintf(api3Data.fpLog,"\nCalling the GetCapCurrVal fn For RFID_TAGCAP_ENABLED_TYPES - Status :%S",RFID_GetCommandStatusText(hReader, dwStatus));
	
	fprintf(api3Data.fpLog,"\nNew Enabled Tag Types....\n");
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS0PLUS : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS0PLUS]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASS1 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASS1]);
	fprintf(api3Data.fpLog,"\nRFID_TAG_TYPE_EPC_CLASSG2 : %d",bEnabledTagTypesParam[RFID_TAG_TYPE_EPC_CLASSG2]);
}

void HexArrToString(unsigned char *pBytes, int nBytes, char *pOutBuf)
{
	int i;
	char *psz = pOutBuf;

	for(i = 0; i < nBytes; i++)
	{
		psz += sprintf(psz, "%2.2X", (int)*pBytes); // convert the hex values of the tag id into a string value.
		pBytes++;
	};
}

void PrintTagsToFile(SYSTEMTIME sTimeStart,SYSTEMTIME sTimeEnd,BYTE inloop,BYTE outloop)
{
	DWORD dwI=0;
	DWORD totalReads =0;
	char *szProtocol;
	char szTagID[256];
	char szPayload[256];
	DWORD hh,mm,ss;
	float TagsPerSec = 0;

	fprintf(api3Data.fpCurLog,"\n *********************Tag Reads Summary*****************\n");
	fprintf(api3Data.fpCurLog,"\n Number of Tags Read		:	%d",SiTagListEX.dwTotalTags);
	fprintf(api3Data.fpCurLog,"\n Number of Read Cycles	:	%d",(inloop*outloop));
	fprintf(api3Data.fpCurLog,"\n Start Time				:	%02d:%02d:%02d",sTimeStart.wHour,sTimeStart.wMinute,sTimeStart.wSecond);
	fprintf(api3Data.fpCurLog,"\n End Time					:	%02d:%02d:%02d",sTimeEnd.wHour,sTimeEnd.wMinute,sTimeEnd.wSecond);
	
	if (sTimeStart.wSecond>sTimeEnd.wSecond)
	{
		ss = (sTimeEnd.wSecond + 60) -  sTimeStart.wSecond;
		sTimeEnd.wMinute = sTimeEnd.wMinute - 1;
	}
	else
		ss = sTimeEnd.wSecond -  sTimeStart.wSecond;
	
	if (sTimeStart.wMinute>sTimeEnd.wMinute)
	{
		mm = (sTimeEnd.wMinute + 60) -  sTimeStart.wMinute;
		sTimeEnd.wHour = sTimeEnd.wHour - 1;
	}
	else
		mm = sTimeEnd.wMinute -  sTimeStart.wMinute;

	if (sTimeStart.wHour>sTimeEnd.wHour)
	{
		hh = (sTimeEnd.wHour + 12) -  sTimeStart.wHour;
		
	}
	else
		hh = sTimeEnd.wHour -  sTimeStart.wHour;

	fprintf(api3Data.fpCurLog,"\n Total Time Taken			:%02d:%02d:%02d",hh,mm,ss);

	if (SiTagListEX.dwTotalTags>0)
	{
		fprintf(api3Data.fpCurLog,"\nS.No\tTagType\t\tAntenna\t\tReads\t\tPC\t\tTagID");
		fprintf(api3Data.fpCurLog,"\t\t\t\tOperation Status\t\tStatus\t\tStatusDetail\t\tRSSIAVG\t\tRSSI");// Comment For Performance
		for(dwI = 0; dwI < SiTagListEX.dwTotalTags; dwI++)
		{
			if(SiTagListEX.Tags[dwI].dataLength > 64 )//  max length
			{
				fprintf(api3Data.fpCurLog,"\n@@@Invalid Data Length %d",SiTagListEX.Tags[dwI].dataLength);
			
			}
			else
			{
				HexArrToString(SiTagListEX.Tags[dwI].tagID, SiTagListEX.Tags[dwI].dataLength, szTagID);
				
				if(SiTagListEX.Tags[dwI].dwMemoryPageLength > 0 )
				{
					strcat(szTagID,"::");
					HexArrToString(&SiTagListEX.Tags[dwI].tagID[SiTagListEX.Tags[dwI].dataLength],SiTagListEX.Tags[dwI].dwMemoryPageLength,szPayload);
					strcat(szTagID,szPayload);
				}

				
				if(SiTagListEX.Tags[dwI].dwType == RFID_TAG_TYPE_EPC_CLASS0)
				{
					szProtocol = "C0";
				}
				else if(SiTagListEX.Tags[dwI].dwType == RFID_TAG_TYPE_EPC_CLASS1)
				{
					szProtocol = "C1";
				}
				else if(SiTagListEX.Tags[dwI].dwType == RFID_TAG_TYPE_EPC_CLASSG2)
				{
					szProtocol = "G2";
				}
				else
				{
					szProtocol = "  ";
				}

				fprintf(api3Data.fpCurLog,"\n%3.3u\t%s\t\t%2.2u\t\t%3.3u\t\t%4.4x\t\t%s", dwI+1,szProtocol, SiTagListEX.Tags[dwI].antennaNum, SiTagListEX.Tags[dwI].readCount, SiTagListEX.Tags[dwI].wPC, szTagID);
				fprintf(api3Data.fpCurLog,"\t%d\t\t%d\t\t%d",SiTagListEX.Tags[dwI].dwOperationStatus,SiTagListEX.Tags[dwI].status, SiTagListEX.Tags[dwI].wStatusDetail);//Comment for performance

				//fprintf(fpLog,"\t\t%04X\t\t%04X",SiTagListEX.Tags[dwI].RSSIavg,SiTagListEX.Tags[dwI].RSSIval);
				totalReads = totalReads + SiTagListEX.Tags[dwI].readCount;
			}
		}
		fprintf(api3Data.fpCurLog,"\n Total Number Of Tag reads	: %d",totalReads);
		TagsPerSec = ((float)(totalReads))/((float)((hh*3600)+(mm*60)+ss));
		fprintf(api3Data.fpCurLog,"\n Number Of Tags Per Second	: %2.2f",TagsPerSec);
		fprintf(api3Data.fpCurLog,"<br>");
	}

}

// Read Tags routine - RFID2 interface
DWORD RFID2ReadTags( HANDLE hReader, BYTE inloop,BYTE outloop )
{
	UINT32 tagCount=0;
	SYSTEMTIME sTimeStart,sTimeEnd;
	DWORD dwStatus;
	DWORD dwNumItems;
	DWORD dwSize;
	
	TAG_LIST_EX1_INIT(SiTagListEX,100);
	SetCapValueDWORD( hReader,RFID_READCAP_READMODE_ONDEMAND,RFID_READCAP_READMODE );
	dwNumItems = 1;
	dwSize = sizeof(BYTE);
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_READCAP_OUTLOOP, &dwNumItems, dwSize, &outloop);
		
	dwStatus = RFID_SetCapCurrValue(hReader, RFID_READCAP_INLOOP, &dwNumItems, dwSize, &inloop);
	
	rfid3SystemTime(&sTimeStart);
	api2Data.rfidStatus = RFID_ReadTagInventoryEX(hReader,&SiTagListEX,false);
	rfid3SystemTime(&sTimeEnd);

	PrintTagsToFile(sTimeStart,sTimeEnd,inloop,outloop);

	return SiTagListEX.dwTotalTags;
}

#endif // end Linux
#endif // end _WIN64