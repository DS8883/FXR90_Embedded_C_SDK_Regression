#ifndef linux
#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;

void SetEAS_Functional(RFID_HANDLE32 hreader)
{	
	Login();
	api3Data.rfidStatus = RFID_ClearReaderStats(api3Data.hReaderMgmt);
	fprintf( api3Data.fpLog,"**********Reader Statistics cleared*********\n");
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	// Code Snippets for setting tag's EAS bits and Running EAS on the supported reader
	UINT8 Pattern1[4] = { 0xAA,0xBB,0xCC,0xDD };
	UINT8 Pattern2[4] = { 0x30,0x05,0xfb,0x63 };
	UINT8 TagMask[4] = { 0xFF,0xFF,0xFF,0xFF };
	UINT8 NXPtagID[12] = {0x30,0x05,0xfb,0x63,0xac,0x1f,0x70,0x06,0xec,0x88,0x04,0x68};
	UINT8 NXPPasswordData[4] = {0x12,0x34,0x56,0x78};
	UINT32 nSuccess = 0, nFailure = 0;
	HANDLE hInventoryDone = CreateEvent(NULL, FALSE, FALSE, NULL);
	//UINT8 NXPPasswordData[4] = {0x00,0x00,0x00,0x00};
	// Write Access-password into NXP tag
	WRITE_ACCESS_PARAMS writeAccessParams;
	writeAccessParams.accessPassword = 0x00000000;
	//writeAccessParams.accessPassword = 0x12345678;
	writeAccessParams.pWriteData = NXPPasswordData;
	writeAccessParams.writeDataLength  = 4;
	writeAccessParams.memoryBank = MEMORY_BANK_RESERVED;
	writeAccessParams.byteOffset = 4;

	NXP_SET_EAS_PARAMS setEASParams;
	setEASParams.accessPassword = 0x12345678;
	//setEASParams.accessPassword = 0x00000000;
	setEASParams.EASState = true;
	//// Set EAS bit on the NXP tag providing AntennaInfo & Access filter as NULL
	api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NXPtagID,12,&setEASParams,NULL,NULL,NULL);

	TAG_PATTERN tpA = {MEMORY_BANK_EPC,32,Pattern1,32,TagMask,32,0};
	TAG_PATTERN tpB = {MEMORY_BANK_EPC,32,Pattern2,32,TagMask,32,0};
	ACCESS_FILTER aFilter ;aFilter.lpTagPatternA = &tpA;aFilter.lpTagPatternB = &tpB;
	aFilter.matchPattern = NOTA_AND_B;
	
	api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);	   		
	api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NULL,0,&setEASParams,&aFilter,0,NULL);
	WaitForSingleObject(hInventoryDone, 10000);

	ANTENNA_INFO antennaInfo;
	UINT16 antennaIDList[2] = {1,3};
	OPERATION_QUALIFIER opq[2] = {C1G2_OPERATION,NXP_EAS_SCAN};
	antennaInfo.pAntennaList = antennaIDList;
	antennaInfo.length = 2;
	antennaInfo.pAntennaOpList = opq;

	api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);	   		
	api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NULL,0,&setEASParams,NULL,&antennaInfo,NULL);
	WaitForSingleObject(hInventoryDone, 10000);
	
	/*READER_CAPS readerCaps;
	RFID_GetReaderCaps(api3Data.hReader, &readerCaps);*/


	//api3Data.rfidStatus = RFID_Write(api3Data.hReader,NXPtagID,12,&writeAccessParams,NULL,NULL,NULL);
	//if(api3Data.rfidStatus == RFID_API_SUCCESS)
	//{
	//   NXP_SET_EAS_PARAMS setEASParams;
	//   setEASParams.accessPassword = 0x12345678;
	//   //setEASParams.accessPassword = 0x00000000;
	//   setEASParams.EASState = true;
	//   //// Set EAS bit on the NXP tag providing AntennaInfo & Access filter as NULL
	//   api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NXPtagID,12,&setEASParams,NULL,NULL,NULL);
	//   
	//   //// Set EAS bit on the NXP tag providing Access filter & AntennaInfo as NULL
	//   TAG_PATTERN tpA = {MEMORY_BANK_EPC,32,Pattern1,32,TagMask,32,0};
	//   TAG_PATTERN tpB = {MEMORY_BANK_EPC,32,Pattern2,32,TagMask,32,0};
	//   ACCESS_FILTER aFilter ;aFilter.lpTagPatternA = &tpA;aFilter.lpTagPatternB = &tpB;
	//   aFilter.matchPattern = NOTA_AND_B;
	//   
	//   api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);	   		
	//   api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NULL,0,&setEASParams,&aFilter,0,NULL);
	//   WaitForSingleObject(hInventoryDone, 10000);

	//   //// Set EAS bit on the NXP tag providing AntennaInfo & Access filter as NULL		
	//   ANTENNA_INFO antennaInfo;
	//   UINT16 antennaIDList[2] = {1,3};
	//   OPERATION_QUALIFIER opq[2] = {C1G2_OPERATION,NXP_EAS_SCAN};
	//   antennaInfo.pAntennaList = antennaIDList;
	//   antennaInfo.length = 2;
	//   antennaInfo.pAntennaOpList = opq;
	//   
	//   api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);	   		
	//   api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NULL,0,&setEASParams,NULL,&antennaInfo,NULL);
	//   WaitForSingleObject(hInventoryDone, 10000);

	//   api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader,&nSuccess,&nFailure);
	//   
	//   ////negative cases
	//   api3Data.rfidStatus=RFID_NXPSetEAS(( RFID_HANDLE32)INVALID_HANDLE_VALUE,NXPtagID,12,&setEASParams,NULL,NULL,NULL);
	//   api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NXPtagID,12,NULL,NULL,NULL,NULL);
	//	if(api3Data.rfidStatus == RFID_API_SUCCESS)
	//	{
	//		TestRFID_RegisterEventNotification( api3Data.hReader,&api3Data.pInfo  );
	//	   //   // Fill up AntennaInfo structure to specify EAS operation to be performed on atleast on antenna during Inventory API call
	//		READER_CAPS readerCaps;
	//		RFID_GetReaderCaps(api3Data.hReader,&readerCaps);
	//		ANTENNA_INFO AntennaInfo;
	//		AntennaInfo.length = readerCaps.numAntennas;
	//		AntennaInfo.pAntennaList = new UINT16[AntennaInfo.length];
	//		AntennaInfo.pAntennaOpList = new OPERATION_QUALIFIER[AntennaInfo.length];
	//         for(UINT32 nIndex = 0; nIndex < AntennaInfo.length; nIndex++)
	//         {
	//             AntennaInfo.pAntennaList[nIndex] = nIndex+1;
	//             AntennaInfo.pAntennaOpList[nIndex] = NXP_EAS_SCAN;
	//         }
	//    
	//         HANDLE hEASAlarmEvent = CreateEvent(NULL,false,false,NULL);
	//		RFID_RegisterEventNotification(api3Data.hReader,NXP_EAS_ALARM_EVENT,hEASAlarmEvent);

	//         // Inventory API called here inititates EAS operation in all antennae and reader sets off EAS_ALARM_EVENT once it receives
	//         // EAS Alarm Notification from the tag whose EAS bit has just been set
	//		RFID_PerformInventory(api3Data.hReader,NULL,&AntennaInfo,NULL,NULL);
	//		if(WaitForSingleObject(hEASAlarmEvent, 10000 ) == WAIT_OBJECT_0)
	//		{
	//			RFID_StopInventory(api3Data.hReader);
	//			MessageBeep( 1 );
	//		}
	//				
	//        
	//		 //This code reset the EAS bit on the same tag to prevent raising false alarms
	//		setEASParams.EASState = false;
	//		api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NXPtagID,12,&setEASParams,NULL,NULL,NULL);
	//		TestRFID_RegisterEventNotification( api3Data.hReader,&api3Data.pInfo  );
	//   }
	//}
	
	//fprintf( api3Data.fpLog,"GetReaderStats.....for SetEAS()...\n");
	//GetReaderStats(api3Data.hReader);
}

void SetEAS_Negative(RFID_HANDLE32 hreader)
{
	// Code Snippets for setting tag's EAS bits and Running EAS on the supported reader
	  
   UINT8 NXPtagID[12] = {0x30,0x05,0xfb,0x63,0xac,0x1f,0x70,0x06,0xec,0x88,0x04,0x68};
   UINT8 NXPPasswordData[4] = {0x12,0x34,0x56,0x78};
   
   // Write Access-password into NXP tag
   WRITE_ACCESS_PARAMS writeAccessParams;
   writeAccessParams.accessPassword = 0x12345678;
   writeAccessParams.pWriteData = NXPPasswordData;
   writeAccessParams.writeDataLength  = 4;
   writeAccessParams.memoryBank = MEMORY_BANK_RESERVED;
   writeAccessParams.byteOffset = 4;
   
   NXP_SET_EAS_PARAMS setEASParams;
   setEASParams.accessPassword = 0x12345678;
   setEASParams.EASState = true;
   
   api3Data.rfidStatus=RFID_NXPSetEAS(( RFID_HANDLE32)INVALID_HANDLE_VALUE,NXPtagID,12,&setEASParams,NULL,NULL,NULL);
   api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NXPtagID,12,NULL,NULL,NULL,NULL);

   //api3Data.rfidStatus = RFID_Write(api3Data.hReader,NXPtagID,12,&writeAccessParams,NULL,NULL,NULL);
   //if(api3Data.rfidStatus == RFID_API_SUCCESS)
   //{
	  // NXP_SET_EAS_PARAMS setEASParams;
   //    setEASParams.accessPassword = 0x12345678;
	  // setEASParams.EASState = true;
		
	  // ////negative cases
	  // api3Data.rfidStatus=RFID_NXPSetEAS(( RFID_HANDLE32)INVALID_HANDLE_VALUE,NXPtagID,12,&setEASParams,NULL,NULL,NULL);
	  // api3Data.rfidStatus=RFID_NXPSetEAS(api3Data.hReader,NXPtagID,12,NULL,NULL,NULL,NULL);
			//
	  // if(api3Data.rfidStatus == RFID_API_SUCCESS)
   //    {
		 //	TestRFID_RegisterEventNotification( api3Data.hReader,&api3Data.pInfo  );
   //    }
   //}
}

void SetQuiet(RFID_HANDLE32 hreader)
{
	Login();
	api3Data.rfidStatus = RFID_ClearReaderStats(api3Data.hReaderMgmt);
	fprintf( api3Data.fpLog,"**********Reader Statistics cleared*********\n");
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	UINT8 NXPtagID[12] = {0x30,0x05,0xfb,0x63,0xac,0x1f,0x70,0x06,0xec,0x88,0x04,0x68};
	UINT8 NXPPasswordData[4] = {0x12,0x34,0x56,0x78};
	UINT32 nSuccess = 0, nFailure = 0;
	HANDLE hInventoryDone = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	UINT8 Pattern1[4] = { 0x30,0x05,0xFB,0x63 };
	UINT8 Pattern2[4] = { 0xAA,0xBB,0xCC,0xDD };
	UINT8 TagMask[4] = { 0xFF,0xFF,0xFF,0xFF };

	TAG_PATTERN tpA = {MEMORY_BANK_EPC,32,Pattern1,32,TagMask,32,0};
	TAG_PATTERN tpB = {MEMORY_BANK_EPC,32,Pattern2,32,TagMask,32,0};
	ACCESS_FILTER aFilter ;
	aFilter.lpTagPatternA = &tpA;
	aFilter.lpTagPatternB = &tpB;
	aFilter.matchPattern = A_AND_NOTB;

	NXP_READ_PROTECT_PARAMS setQuietParams;
	setQuietParams.accessPassword = 0;
	
	ANTENNA_INFO antennaInfo;
	UINT16 antennaIDList[2] = {1,3};
	OPERATION_QUALIFIER opq[2] = {NXP_EAS_SCAN,C1G2_OPERATION};
	antennaInfo.pAntennaList = antennaIDList;
	antennaInfo.length = 2;
	antennaInfo.pAntennaOpList = opq;
	
	api3Data.rfidStatus = RFID_NXPReadProtect(api3Data.hReader,NXPtagID,12,&setQuietParams,NULL,&antennaInfo,NULL);
	
	
	api3Data.rfidStatus = RFID_NXPReadProtect(api3Data.hReader,NXPtagID,12,&setQuietParams,&aFilter,&antennaInfo,NULL);

	NXP_RESET_READ_PROTECT_PARAMS resetQuietParams;
	resetQuietParams.accessPassword = 0x12345678;
	
	api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);
	api3Data.rfidStatus = RFID_NXPResetReadProtect(api3Data.hReader,&resetQuietParams,&antennaInfo,NULL);
	WaitForSingleObject(hInventoryDone, 10000);

	/*NXP_RESET_READ_PROTECT_PARAMS resetQuietParams;
	resetQuietParams.accessPassword = 0x12345678;
	api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);
	api3Data.rfidStatus = RFID_NXPResetReadProtect(api3Data.hReader,&resetQuietParams,NULL,NULL);
	WaitForSingleObject(hInventoryDone, 10000);
	api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader,&nSuccess,&nFailure);*/
	
	// Write Access-password into NXP tag
	/*WRITE_ACCESS_PARAMS writeAccessParams;
	writeAccessParams.accessPassword = 0;
	writeAccessParams.pWriteData = NXPPasswordData;
	writeAccessParams.writeDataLength  = 4;
	writeAccessParams.memoryBank = MEMORY_BANK_RESERVED;
	writeAccessParams.byteOffset = 4;
	api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);
	api3Data.rfidStatus = RFID_Write(api3Data.hReader,NXPtagID,12,&writeAccessParams,NULL,NULL,NULL);
	WaitForSingleObject(hInventoryDone, 10000);*/
	//api3Data.rfidStatus = RFID_GetLastAccessResult(api3Data.hReader,&nSuccess,&nFailure);
	//if(api3Data.rfidStatus == RFID_API_SUCCESS)
	//{
	//   NXP_READ_PROTECT_PARAMS setQuietParams;
	//   setQuietParams.accessPassword = 0;
	//   		
	//   ANTENNA_INFO antennaInfo;
	//   UINT16 antennaIDList[2] = {1,3};
	//   OPERATION_QUALIFIER opq[2] = {NXP_EAS_SCAN,C1G2_OPERATION};
	//   antennaInfo.pAntennaList = antennaIDList;
	//   antennaInfo.length = 2;
	//   antennaInfo.pAntennaOpList = opq;

	//   

	//   /*READER_CAPS readerCaps;
	//   RFID_GetReaderCaps(api3Data.hReader, &readerCaps);	*/

	//   api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);	   		
	//   api3Data.rfidStatus = RFID_NXPReadProtect(api3Data.hReader,NXPtagID,12,&setQuietParams,NULL,&antennaInfo,NULL);
	//   WaitForSingleObject(hInventoryDone, 10000);
	//	
	//   //api3Data.rfidStatus=RFID_GetLastAccessResult(api3Data.hReader,&nSuccess,&nFailure);
	//	
	//   
	//   ERROR_INFO errInfo;	
	//   RFID_GetLastErrorInfo(api3Data.hReader,&errInfo);
	//   wprintf(L"\n Status description: %S", errInfo.statusDesc);
	//   wprintf(L"\n Vendor message: %S", errInfo.vendorMessage);
	//   
	//   if(api3Data.rfidStatus == RFID_API_SUCCESS)
	//   {
	//	   NXPPasswordData[3] = 0x87;
 //          
	//	   api3Data.rfidStatus = RFID_Write(api3Data.hReader,NULL,0,&writeAccessParams,&aFilter,NULL,NULL);
	//	   // Access will fail on a quietened tag
	//	   
	//	   //TestRFID_RegisterEventNotification( api3Data.hReader,&api3Data.pInfo  );
	//	   //Perform Inventory
	//		
	//	   NXP_RESET_READ_PROTECT_PARAMS resetQuietParams;
	//	   resetQuietParams.accessPassword = 0x12345678;
	//	   api3Data.rfidStatus	= RFID_RegisterEventNotification(api3Data.hReader, ACCESS_STOP_EVENT, hInventoryDone);
	//	   api3Data.rfidStatus = RFID_NXPResetReadProtect(api3Data.hReader,&resetQuietParams,&antennaInfo,NULL);
	//	   WaitForSingleObject(hInventoryDone, 10000);
	//	   api3Data.rfidStatus=RFID_GetLastAccessResult(api3Data.hReader,&nSuccess,&nFailure);
	//	   TestRFID_RegisterEventNotification( api3Data.hReader,&api3Data.pInfo  );
	//	   
	//	   api3Data.rfidStatus = RFID_Write(api3Data.hReader,NULL,0,&writeAccessParams,&aFilter,NULL,NULL);
	//	   api3Data.rfidStatus=RFID_GetLastAccessResult(api3Data.hReader,&nSuccess,&nFailure);
	//	 //  // nSuccess -> greater than equal to 1

	//
	//	   api3Data.rfidStatus = RFID_Write(api3Data.hReader,NXPtagID,12,&writeAccessParams,NULL,NULL,NULL);
	//		//// Access will now succeed
 // 
	//   }
	//}
	fprintf( api3Data.fpLog,"GetReaderStats.....for SetQuiet() & ResetQuiet()...\n");
	GetReaderStats(api3Data.hReader);	   
}

void SetQuiet_Negative(RFID_HANDLE32 hreader)
{
	UINT8 NXPtagID[12] = {0x30,0x05,0xfb,0x63,0xac,0x1f,0x70,0x06,0xec,0x88,0xAA,0xAA};
	UINT8 NXPPasswordData[4] = {0x12,0x34,0x56,0x78};
	UINT32 nSuccess = 0, nFailure = 0;
	HANDLE hInventoryDone = CreateEvent(NULL, FALSE, FALSE, NULL);

	UINT8 Pattern1[4] = { 0x30,0x05,0xFB,0x63 };
	UINT8 Pattern2[4] = { 0xAA,0xBB,0xCC,0xDD };
	UINT8 TagMask[4] = { 0xFF,0xFF,0xFF,0xFF };

	TAG_PATTERN tpA = {MEMORY_BANK_EPC,32,Pattern1,32,TagMask,32,0};
	TAG_PATTERN tpB = {MEMORY_BANK_EPC,32,Pattern2,32,TagMask,32,0};
	ACCESS_FILTER aFilter ;
	aFilter.lpTagPatternA = &tpA;
	aFilter.lpTagPatternB = &tpB;
	aFilter.matchPattern = A_AND_NOTB;

	NXP_READ_PROTECT_PARAMS setQuietParams;
	setQuietParams.accessPassword = 0x12345678;

	ANTENNA_INFO antennaInfo;
	memset(&antennaInfo,0,sizeof(ANTENNA_INFO));
	UINT16 antennaIDList[2] = {1,3};
	OPERATION_QUALIFIER opq[2] = {NXP_EAS_SCAN,C1G2_OPERATION};
	antennaInfo.pAntennaList = antennaIDList;
	antennaInfo.length = 2;
	antennaInfo.pAntennaOpList = opq;



	/*READER_CAPS readerCaps;
	RFID_GetReaderCaps(api3Data.hReader, &readerCaps);	*/

	api3Data.rfidStatus = RFID_NXPReadProtect(api3Data.hReader,NULL,0,NULL,&aFilter,&antennaInfo,NULL);
	api3Data.rfidStatus = RFID_NXPReadProtect(( RFID_HANDLE32)INVALID_HANDLE_VALUE,NULL,0,&setQuietParams,&aFilter,&antennaInfo,NULL);
	
	NXP_RESET_READ_PROTECT_PARAMS resetQuietParams;
	resetQuietParams.accessPassword = 0x12345678;
	
	api3Data.rfidStatus = RFID_NXPResetReadProtect(api3Data.hReader,NULL,&antennaInfo,NULL);
	api3Data.rfidStatus = RFID_NXPResetReadProtect(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&resetQuietParams,&antennaInfo,NULL);

}
#endif 