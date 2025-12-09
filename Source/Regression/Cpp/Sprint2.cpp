#include "Common.h"
#include "APITest.h"

extern struct RFID3DATA api3Data;

void testGetProfileList()
{
	rfid3Sleep (5000);
	PROFILE_LIST ProfileList;
	api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &ProfileList);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"Number of Profiles: %d\n",ProfileList.numProfiles );
		wprintf(L"\n Number of Profiles: %d\n",ProfileList.numProfiles );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog,"Active Profile INDEX --> NAME: %d --> %S\n",ProfileList.activeProfileIndex, ProfileList.pProfileName[ProfileList.activeProfileIndex] );
		wprintf(L"\n Active Profile INDEX --> NAME: %d --> %ls\n",ProfileList.activeProfileIndex, ProfileList.pProfileName[ProfileList.activeProfileIndex] );
		fprintf( api3Data.fpCurLog,"<br>");
		for ( int index=0;index<ProfileList.numProfiles;index++)
		{
			fprintf( api3Data.fpCurLog,"Profile INDEX --> NAME: %d --> %S\n",index, ProfileList.pProfileName[index] );
			wprintf( L"\n Profile INDEX --> NAME: %d --> %ls\n",index, ProfileList.pProfileName[index] );
			fprintf( api3Data.fpCurLog,"<br>" );
		}
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( 2 );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"FAIL:%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		ERROR_INFO errorInfo;
	
		RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
		fprintf( api3Data.fpCurLog,"rfidStatusCode - %d<br>",errorInfo.rfidStatusCode);
		fprintf( api3Data.fpCurLog,"statusDesc - %S <br>",errorInfo.statusDesc);
		fprintf( api3Data.fpCurLog,"vendorMessage - %S <br>", errorInfo.vendorMessage);
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-PROFILES");
}

void testDeleteProfile()
{
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Test RFID_DeleteProfile\n<br> Expected Output: RFID_API_SUCCESS</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_DeleteProfile( api3Data.hReaderMgmt, L"myprofile.xml");

	apiResultPassLog();
	logToResultFile(L"RMAPI-PROFILES");
}

void testProfiles()
{
	// Get the Current Directory 
	wchar_t pCurrentDirectory[512];
	char* pANSICurrentDirectory;
#ifdef linux
	pANSICurrentDirectory = GetCurrentDir( NULL,0 );
	if( pANSICurrentDirectory == NULL )
	{
		wprintf(L"\n Could not get Current Directory ");
		
	}
	else
	{
		mbstowcs( pCurrentDirectory, pANSICurrentDirectory,512);
		printf("\n ANSI CurrentDirectory %s",pANSICurrentDirectory);
		wprintf(L"\n WIDECHAR Current Directory is %ls",pCurrentDirectory);
	}
#endif

#if !WINCE && !linux
	GetCurrentDirectory(512,pCurrentDirectory);
#endif

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Gets the list of profiles available",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	testGetProfileList();
		
	//RFID_ImportProfileFromReader	
	//wcscpy( api3Data.tcProfileName,*(ProfileList.pProfileName) );
	wcscpy( api3Data.tcProfileName,L"Default.xml" );

	if( api3Data.readerType == FX )
	{
		fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_ImportProfileFromReader\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog, "\n Result: ");

		api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt,api3Data.tcProfileName,pCurrentDirectory);
		wchar_t filename1[512];
		wchar_t filename2[512];
#if (!WINCE && !linux)
		rfid_swprintf( filename1,512,L"%ls\\%ls",pCurrentDirectory,api3Data.tcProfileName );
		rfid_swprintf( filename2,512,L"%ls\\%ls",pCurrentDirectory,L"myprofile.xml");
		int result = _wrename( filename1,filename2 );
#endif

#ifdef linux
		char ansifilename1[512];
		char ansifilename2[512];
		sprintf( ansifilename1,"%s/%s",pANSICurrentDirectory,"Default.xml" );
		sprintf( ansifilename2,"%s/%s",pANSICurrentDirectory,"myprofile.xml");
		wprintf(L"\n File name 1 is  %ls ",MultiByte2WideChar(ansifilename1));
		wprintf(L"\n File name 2 is  %ls ",MultiByte2WideChar(ansifilename2));

		if( rename(ansifilename1,ansifilename2) != 0)
		{
			wprintf(L"\n Could not rename the file %ls ",MultiByte2WideChar(strerror(errno)));
		}
#endif
		apiResultPassLog();
		logToResultFile(L"RMAPI-PROFILES");
	}

	rfid3Sleep (10000);

	//RFID_ExportProfileToReader with Activation set to TRUE
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Test RFID_ExportProfileToReader with Activation set to TRUE\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"myprofile.xml",pCurrentDirectory,TRUE);
	rfid3Sleep (10000);
	
	apiResultPassLog();
	logToResultFile(L"RMAPI-PROFILES");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Verify the <b><i>EXPORTED PROFILE (myprofile.xml) is the ACTIVE PROFILE</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	
	//Verify the EXPORTED PROFILE is the ACTIVE PROFILE
	testGetProfileList();
	
	//Delete the EXPORTED PROFILE
	testDeleteProfile();
	rfid3Sleep (10000);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Verify the <b><i>EXPORTED profile (myprofile.xml) is deleted from the Reader</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	
	testGetProfileList();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_ExportProfileToReader with Activation set to False\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"myprofile.xml",pCurrentDirectory,FALSE);
	rfid3Sleep (10000);

	apiResultPassLog();
	logToResultFile(L"RMAPI-PROFILES");
	
	fprintf( api3Data.fpCurLog,"\n\n<<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Verify the <b><i>Profile EXPORTED (myprofile.xml) to the Reader is NOT ACTIVATED</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	testGetProfileList();

	//Test RFID_SetActiveProfile
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_SetActiveProfile\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetActiveProfile(api3Data.hReaderMgmt, L"myprofile.xml");
	apiResultPassLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Verify whether the <b><i>Profile (myprofile.xml) is ACTIVATED</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	testGetProfileList();

	testDeleteProfile();
	rfid3Sleep (10000);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Verify the <b><i>ACTIVATED profile (myprofile.xml) is deleted from the Reader</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	
	testGetProfileList();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_SetActiveProfile\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetActiveProfile(api3Data.hReaderMgmt, L"Default.xml");
	rfid3Sleep (10000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Test RFID_GetProfileList\n<br> Expected Output: Verify the <b><i>ACTIVE PROFILE set to 'Default.xml'</i></b>",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	testGetProfileList();
}

void testProfiles_Negative()
{
	Login();
	wchar_t tcPath[512];
	wcscpy(tcPath,L"C:\\");
	PROFILE_LIST ProfileList;
	
	//RFID_GetProfileList Negative case
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_GetProfileList by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetProfileList( ( RFID_HANDLE32)INVALID_HANDLE_VALUE, &ProfileList );
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_GetProfileList by passing NULL to PROFILE_LIST parameter\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, NULL );
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");
		
	//RFID_ImportProfileFromReader Negative case
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ImportProfileFromReader by providing INVALID local path\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt,api3Data.tcProfileName,L"C:\\Invalid");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ImportProfileFromReader by providing INVALID Profile Name\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt,L"C:\\Invalid",tcPath);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ImportProfileFromReader by passing INVALID_HANDLE_VALUE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ImportProfileFromReader( ( RFID_HANDLE32)INVALID_HANDLE_VALUE,api3Data.tcProfileName,tcPath);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ExportProfileToReader by passing INVALID profile name\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"invalid",tcPath,FALSE);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ExportProfileToReader by passing INVALID path\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"myprofile.xml",L"C:\\Invalid",FALSE);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");	

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ExportProfileToReader by passing INVALID_HANDLE_VALUE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ExportProfileToReader( ( RFID_HANDLE32)INVALID_HANDLE_VALUE,L"myprofile.xml",L"C:\\",FALSE);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_SetActiveProfile by passing INVALID profile\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetActiveProfile(api3Data.hReaderMgmt, L"Invalid");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_SetActiveProfile by passing INVALID_HANDLE_VALUE\n<br> Expected Output: INVALID_HANDLE_VALUE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetActiveProfile(( RFID_HANDLE32)INVALID_HANDLE_VALUE, L"Default.xml");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_DeleteProfile by passing INVALID_HANDLE_VALUE\n<br> Expected Output: INVALID_HANDLE_VALUE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_DeleteProfile( ( RFID_HANDLE32)INVALID_HANDLE_VALUE, L"myprofile.xml");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_DeleteProfile by passing Invalid profile\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_DeleteProfile( api3Data.hReaderMgmt, L"Invalid");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Execute RFID_DeleteProfile by attempting to delete default profile\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_DeleteProfile( api3Data.hReaderMgmt, L"Default.xml");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_GetProfileList without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &ProfileList);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ImportProfileFromReader without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ImportProfileFromReader( api3Data.hReaderMgmt,api3Data.tcProfileName,tcPath);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_ExportProfileToReader without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ExportProfileToReader( api3Data.hReaderMgmt,L"myprofile.xml",tcPath,TRUE);
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_SetActiveProfile without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetActiveProfile(api3Data.hReaderMgmt, L"Default.xml");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-PROFILES-%d></a> RMAPI-PROFILES-%d:\n\n<br> Description: Testing RFID_DeleteProfile without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_DeleteProfile( api3Data.hReaderMgmt, L"myprofile.xml");
	apiResultFailLog();
	logToResultFile(L"RMAPI-PROFILES");
}

void getUSBoperation( USB_OPERATION_INFO* pusbOPerationInfo )
{
	////RFID_GetUSBOperationMode()
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetUSBOperationMode\n<br> Expected Output: ActiveSync or NETWORK",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	
	api3Data.rfidStatus=RFID_GetUSBOperationMode(api3Data.hReaderMgmt, pusbOPerationInfo);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		if( pusbOPerationInfo->mode == ACTIVE_SYNC )
		{
			fprintf( api3Data.fpCurLog,"Operation Mode is \t:ACTIVE_SYNC\t Allow LLRP Connection Over ride \t: %s",  pusbOPerationInfo->allowLLRPConnectionOverride ? "true" : "false");
			wprintf( L"\n Operation Mode\t:ACTIVE_SYNC \t Allow LLRP Connection Over ride \t: %ls",  pusbOPerationInfo->allowLLRPConnectionOverride ? L"TRUE" : L"FALSE");
		}
        else if( pusbOPerationInfo->mode == NETWORK)
		{
			fprintf( api3Data.fpCurLog,"Operation Mode is \t: NETWORK \t Allow LLRP Connection Over ride \t: %s",  pusbOPerationInfo->allowLLRPConnectionOverride ? "TRUE" : "FALSE");
			wprintf( L"\n Operation Mode\t: NETWORK \t Allow LLRP Connection Over ride \t: %ls",  pusbOPerationInfo->allowLLRPConnectionOverride ? L"TRUE" : L"FALSE");
		}
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"FAIL:%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		DisplayLastErrorInfo();
		fprintf( api3Data.fpCurLog,"<br><br>");
	}

	logToResultFile(L"RMAPI-FUN");
}

void testUSBoperation()
{
	USB_OPERATION_INFO usb;
	memset(&usb,0,sizeof(USB_OPERATION_INFO));

	// get USB Operation and Mode
	getUSBoperation( &usb );
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_SetUSBOperationMode(ACTIVE_SYNC) with allowLLRPConnectionOverride DISABLED\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	usb.mode = ACTIVE_SYNC;
	usb.allowLLRPConnectionOverride = FALSE;

	api3Data.rfidStatus = RFID_SetUSBOperationMode(api3Data.hReaderMgmt,&usb);
	getUSBoperation( &usb );
	if( usb.mode == ACTIVE_SYNC || usb.allowLLRPConnectionOverride == FALSE )
	{
		SetLogResult( TRUE ); 
	}
	
	logToResultFile(L"RMAPI-USB");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_SetUSBOperationMode(ACTIVE_SYNC) with allowLLRPConnectionOverride ENABLED\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	usb.allowLLRPConnectionOverride = true;

	api3Data.rfidStatus=RFID_SetUSBOperationMode(api3Data.hReaderMgmt,&usb);
	getUSBoperation( &usb );
	if( usb.mode == ACTIVE_SYNC || usb.allowLLRPConnectionOverride == TRUE )
	{
		SetLogResult( TRUE ); 
	}
	logToResultFile(L"RMAPI-USB");
	

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_SetUSBOperationMode(NETWORK) with allowLLRPConnectionOverride DISABLED\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	usb.mode = NETWORK;
	usb.allowLLRPConnectionOverride = TRUE;

	api3Data.rfidStatus = RFID_SetUSBOperationMode( api3Data.hReaderMgmt,&usb );
	getUSBoperation( &usb );
	if( usb.mode == NETWORK || usb.allowLLRPConnectionOverride == FALSE )
	{
		SetLogResult( TRUE ); 
	}
	logToResultFile(L"RMAPI-USB");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_SetUSBOperationMode(NETWORK) with allowLLRPConnectionOverride ENABLED\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	usb.allowLLRPConnectionOverride = FALSE;

	api3Data.rfidStatus = RFID_SetUSBOperationMode( api3Data.hReaderMgmt,&usb );
	getUSBoperation( &usb );
	if( usb.mode == NETWORK || usb.allowLLRPConnectionOverride == TRUE )
	{
		SetLogResult( TRUE ); 
	}
	logToResultFile(L"RMAPI-USB");
}

void testUSBoperation_Negative( )
{
	USB_OPERATION_INFO usb;
	Login();
		
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_SetUSBOperationMode(NETWORK) by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	usb.mode = NETWORK;
	usb.allowLLRPConnectionOverride = true;

	api3Data.rfidStatus=RFID_SetUSBOperationMode((RFID_HANDLE32)INVALID_HANDLE_VALUE,&usb);
	apiResultFailLog();
	logToResultFile(L"RMAPI-USB");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_GetUSBOperationMode by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetUSBOperationMode((RFID_HANDLE32)INVALID_HANDLE_VALUE, &usb);
	apiResultFailLog();
	logToResultFile(L"RMAPI-USB");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_GetUSBOperationMode by passing NULL to USB parameter\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetUSBOperationMode(api3Data.hReaderMgmt,NULL);
	apiResultFailLog();
	logToResultFile(L"RMAPI-USB");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_SetUSBOperationMode without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_SetUSBOperationMode(api3Data.hReaderMgmt,&usb);
	apiResultFailLog();
	logToResultFile(L"RMAPI-USB");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USB-%d></a> RMAPI-USB-%d:\n\n<br> Description: Testing RFID_GetUSBOperationMode without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetUSBOperationMode(api3Data.hReaderMgmt, &usb);
	apiResultFailLog();
	logToResultFile(L"RMAPI-USB");
}

void testGPIoperation_MANUAL()
{
	UINT32 DebounceTime;
	BOOLEAN status;
	GPI_PORT_STATE gpi;
	//Get GPI state for port 0 & 1
	api3Data.rfidStatus=RFID_GetGPIState(api3Data.hReader,1,&status,&gpi);
	api3Data.rfidStatus=RFID_GetGPIState(api3Data.hReader,2,&status,&gpi);

	api3Data.rfidStatus=RFID_EnableGPIPort(api3Data.hReader,1,false);
	api3Data.rfidStatus=RFID_EnableGPIPort(api3Data.hReader,2,true);
	
	api3Data.rfidStatus=RFID_GetGPIState(api3Data.hReader,1,&status,&gpi);
	api3Data.rfidStatus=RFID_GetGPIState(api3Data.hReader,2,&status,&gpi);

	api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,&DebounceTime,0);
	api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,10000,0);
	api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,&DebounceTime,0);
	
	

	//Negative case
	api3Data.rfidStatus=RFID_SetGPIDebounceTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,1000,0);
	api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,61000,0);
	api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);
	api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);
}

void testGetGPIdebounce()
{
	UINT32 DebounceTime;
		
	api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,&DebounceTime,0);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br>");
		fprintf( api3Data.fpCurLog,"GPIDebounce set to %d msec",DebounceTime);
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-USB");
}

void testGPIoperation()
{
	//Test RFID_GetGPIDebounceTime()
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_GetGPIDebounceTime\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	testGetGPIdebounce();

	//Test RFID_SetGPIDebounceTime
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_GetGPIDebounceTime\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,10000,0);

	apiResultPassLog();
	logToResultFile(L"RMAPI-GPI");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Verify the RFID_SetGPIDebounceTime set to 10000 msec\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	testGetGPIdebounce();
	
	////Negative case
	//api3Data.rfidStatus=RFID_SetGPIDebounceTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,1000,0);
	//api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,61000,0);
	//api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);
	//api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);
}

void testGPIoperation_Negative()
{
	UINT32 DebounceTime;
	Login();
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_GetGPIDebounceTime by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetGPIDebounceTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&DebounceTime,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-GPI");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_GetGPIDebounceTime by passing NULL paramter to Debounce time\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-GPI");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_SetGPIDebounceTime by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_SetGPIDebounceTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,1000,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-GPI");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_SetGPIDebounceTime by passing out of range debounceTime (msec) value\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,61000,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-GPI");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_GetGPIDebounceTime without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,&DebounceTime,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-GPI");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-GPI-%d></a> RMAPI-GPI-%d:\n\n<br> Description: Testing RFID_SetGPIDebounceTime without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,10000,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-GPI");


	//api3Data.rfidStatus=RFID_SetGPIDebounceTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,1000,0);
	//api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,61000,0);
	//api3Data.rfidStatus=RFID_SetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);
	//api3Data.rfidStatus=RFID_GetGPIDebounceTime(api3Data.hReaderMgmt,NULL,0);

}

void initiateLLRPconnTRUE()
{
	LLRP_CONNECTION_CONFIG conn;
	wcscpy(conn.hostServerIP, L"192.168.2.106");
	conn.isClient=false;
	conn.port=5084;
	//conn.reserved=0;
	//api3Data.rfidStatus=RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&conn);
	api3Data.rfidStatus=RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,&conn);
	api3Data.rfidStatus=RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&conn);

	api3Data.rfidStatus=RFID_DisconnectLLRPConnectionFromReader(api3Data.hReaderMgmt);
	api3Data.rfidStatus=RFID_InitiateLLRPConnectionFromReader(api3Data.hReaderMgmt,0);

	////Negative cases
	api3Data.rfidStatus=RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,NULL);
	api3Data.rfidStatus=RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,NULL);
}

void initiateLLRPconnFALSE()
{
	LLRP_CONNECTION_CONFIG conn;
	wcscpy(conn.hostServerIP, L"192.168.2.106");
	conn.isClient=false;
	conn.port=5084;
	//conn.reserved=0;
	//api3Data.rfidStatus=RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&conn);
	api3Data.rfidStatus=RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,&conn);
	api3Data.rfidStatus=RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&conn);

	api3Data.rfidStatus=RFID_DisconnectLLRPConnectionFromReader(api3Data.hReaderMgmt);
	api3Data.rfidStatus=RFID_InitiateLLRPConnectionFromReader(api3Data.hReaderMgmt,0);

	////Negative cases
	api3Data.rfidStatus=RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,NULL);
	api3Data.rfidStatus=RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,NULL);
}

void getHealthStatus()
{
	HEALTH_STATUS sts;
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-HEALTH-%d></a> RMAPI-HEALTH-%d:\n\n<br> Description: Testing RFID_GetHealthStatus of <b>RM</b> interface\n<br> Expected Output: UP or DOWN",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,RM,&sts,0);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"RM status is -> %S",sts?L"UP":L"DOWN");
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-HEALTH");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-HEALTH-%d></a> RMAPI-HEALTH-%d:\n\n<br> Description: Testing RFID_GetHealthStatus of <b>LLRP_SERVER</b> interface\n<br> Expected Output: UP or DOWN",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,LLRP_SERVER,&sts,0);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"RM status is -> %S",sts?L"UP":L"DOWN");
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-HEALTH");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	
	////Negative case

	fprintf( api3Data.fpCurLog,"\n**************************<b>Test HealthStatus of the Reader - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-HEALTH-%d></a> RMAPI-HEALTH-%d:\n\n<br> Description: Testing RFID_GetHealthStatus without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,RM,&sts,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-HEALTH");

	Login();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-HEALTH-%d></a> RMAPI-HEALTH-%d:\n\n<br> Description: Testing RFID_GetHealthStatus by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetHealthStatus(( RFID_HANDLE32)INVALID_HANDLE_VALUE,RM,&sts,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-HEALTH");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-HEALTH-%d></a> RMAPI-HEALTH-%d:\n\n<br> Description: Testing RFID_GetHealthStatus by passing NULL parameter\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus=RFID_GetHealthStatus(api3Data.hReaderMgmt,RM,NULL,0);
	apiResultFailLog();
	logToResultFile(L"RMAPI-HEALTH");
}

void TestProfileAPI( RFID_HANDLE32 hReader )
{
	SetLogText("Test_PROFILES");
	logCurrentExecution("Test_PROFILES");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 32;
	Login();
	READER_STATS stats;
	api3Data.rfidStatus = RFID_GetReaderStats(api3Data.hReaderMgmt,-1,&stats);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Profiles - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	testProfiles();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Profiles - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	testProfiles_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void TestUSB( RFID_HANDLE32 hReader )
{
	SetLogText("Test_USB()");
	logCurrentExecution( "Test_USB");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 14;
	
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test USB Operation mode - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	if( Login() )
	{
		testUSBoperation();
	}
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test USB Operation mode - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	testUSBoperation_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void TestGPIdebounce(RFID_HANDLE32 hreader)
{
	SetLogText("Test_GPIdebounce()");
	logCurrentExecution( "Test_GPIdebounce");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 9;
	Login();
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test GPIdebounce - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	//testGPIoperation_MANUAL();
	testGPIoperation();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test GPIdebounce - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	testGPIoperation_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void TestInitiateLLRPconn(RFID_HANDLE32 hreader)
{
	Login();
	initiateLLRPconnTRUE();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	Login();
	initiateLLRPconnFALSE();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
}

void TestGetHealthStatus(RFID_HANDLE32 hReader)
{
	SetLogText("Test_GetHealthStatus");
	logCurrentExecution( "Test_GetHealthStatus");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.dwTestCaseNo = 01;
	api3Data.itemTestCount = 5;
	Login();
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test HealthStatus of the Reader - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	getHealthStatus();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void testGetReaderInfo()
{
	READER_INFO readerInfo;
	
	api3Data.rfidStatus = RFID_GetReaderInfo(api3Data.hReaderMgmt,&readerInfo);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"Name -> %S<br>",readerInfo.name);
		fprintf( api3Data.fpCurLog,"Description -> %S<br>",readerInfo.description);
		fprintf( api3Data.fpCurLog,"Location -> %S<br>",readerInfo.location);
		fprintf( api3Data.fpCurLog,"Contact -> %S<br>",readerInfo.contact);
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-READERINFO");
}

void testReaderInfo()
{
	READER_INFO readerInfo;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERINFO-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetReaderInfo\n<br> Expected Output: ReaderInfo details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	testGetReaderInfo();

	wcscpy(readerInfo.contact, L"Zebra Technologies Corporation");
	wcscpy(readerInfo.description, L"FX9600 Reader");
	wcscpy(readerInfo.location, L"RFID Lab");
	wcscpy(readerInfo.name, L"FX9600 Series Reader");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetReaderInfo with the valid ReaderInfo details\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	
	api3Data.rfidStatus = RFID_SetReaderInfo(api3Data.hReaderMgmt,&readerInfo);
	apiResultPassLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Verify the ReaderInfo details set\n<br> Expected Output: Vaild ReaderInfo details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	testGetReaderInfo();
}

void testGetReaderInfoA()
{
	READER_INFOA readerInfo;

	api3Data.rfidStatus = RFID_GetReaderInfoA(api3Data.hReaderMgmt, &readerInfo);

	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		fprintf(api3Data.fpCurLog, "Name -> %s<br>", readerInfo.name);
		fprintf(api3Data.fpCurLog, "Description -> %s<br>", readerInfo.description);
		fprintf(api3Data.fpCurLog, "Location -> %s<br>", readerInfo.location);
		fprintf(api3Data.fpCurLog, "Contact -> %s<br>", readerInfo.contact);

		printf("\n Name -> %s", readerInfo.name);
		printf("\n Description -> %s", readerInfo.description);
		printf("\n Location -> %s", readerInfo.location);
		printf("\n Contact -> %s", readerInfo.contact);
		SetLogResult(TRUE);
	}
	else
	{
		fprintf(api3Data.fpCurLog, "<b>FAIL:</b>%S", RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf(api3Data.fpCurLog, "<br><br>");
	}
	logToResultFile(L"RMAPI-READERINFO");
}

void testReaderInfoA()
{
	READER_INFOA readerInfo;

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-READERINFO-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetReaderInfo\n<br> Expected Output: ReaderInfo details", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");

	testGetReaderInfoA();

	strcpy(readerInfo.contact, "Zebra Technologies Corporation");
	strcpy(readerInfo.description, "FX9600 Reader");
	strcpy(readerInfo.location, "RFID Lab");
	strcpy(readerInfo.name, "FX9600 Series Reader");

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetReaderInfo with the valid ReaderInfo details\n<br> Expected Output: RFID_API_SUCCESS", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");
	fprintf(api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetReaderInfoA(api3Data.hReaderMgmt, &readerInfo);
	apiResultPassLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf(api3Data.fpCurLog, "\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Verify the ReaderInfo details set\n<br> Expected Output: Vaild ReaderInfo details", api3Data.dwTestCaseNo, api3Data.dwTestCaseNo);
	fprintf(api3Data.fpCurLog, "<br>");

	testGetReaderInfoA();
}

void testReaderInfo_Negative()
{
	READER_INFO readerInfo;
	Login();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetReaderInfo by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderInfo ( ( RFID_HANDLE32)INVALID_HANDLE_VALUE, &readerInfo );
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetReaderInfo by passing NULL parameter to READER_INFO\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderInfo ( api3Data.hReaderMgmt, NULL );
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetReaderInfo by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetReaderInfo ( ( RFID_HANDLE32)INVALID_HANDLE_VALUE, &readerInfo );
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetReaderInfo by passing NULL parameter to READER_INFO\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetReaderInfo ( api3Data.hReaderMgmt, NULL );
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Pass large String values to Set Reader Info\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	wcscpy(readerInfo.contact, L"fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	wcscpy(readerInfo.description, L"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	wcscpy(readerInfo.location, L"fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	wcscpy(readerInfo.name, L"fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	
	api3Data.rfidStatus = RFID_SetReaderInfo(api3Data.hReaderMgmt,&readerInfo);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetReaderInfo without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	wcscpy(readerInfo.contact, L"Zebra Technologies Corporation");
	wcscpy(readerInfo.description, L"FX9600 Reader");
	wcscpy(readerInfo.location, L"RFID Lab");
	wcscpy(readerInfo.name, L"FX9600 Series Reader");

	api3Data.rfidStatus = RFID_SetReaderInfo(api3Data.hReaderMgmt,&readerInfo);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetReaderInfo without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderInfo(api3Data.hReaderMgmt,&readerInfo);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");
}

void TestReaderInfo( RFID_HANDLE32 hReader )
{
	SetLogText("Test_ReaderInfo");
	logCurrentExecution( "Test_ReaderInfo");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 13;
	api3Data.dwTestCaseNo = 01;
	Login();
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test TestReaderInfo - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	testReaderInfo();
	testReaderInfoA();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test TestReaderInfo - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	testReaderInfo_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void testSetUserLed()
{
	Login();

	LED_INFO  LedInfo ;
	LedInfo.ledColor = LED_RED;
	LedInfo.blink = true;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED color RED and LED blink set to <i>TRUE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.blink = false;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED color RED and LED blink set to <i>FALSE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.ledColor = LED_OFF;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED OFF and LED blink set to <i>FALSE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");
	
	LedInfo.ledColor = LED_GREEN;
	LedInfo.blink = true;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED color GREEN and LED blink set to <i>TRUE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.blink = false;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED color GREEN and LED blink set to <i>FALSE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.ledColor = LED_OFF;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED OFF and LED blink set to <i>FALSE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.ledColor = LED_YELLOW;
	LedInfo.blink = true;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED color YELLOW and LED blink set to <i>TRUE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.blink = false;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED color YELLOW and LED blink set to <i>FALSE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");

	LedInfo.ledColor = LED_OFF;
	LedInfo.durationSeconds = 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED with LED OFF and LED blink set to <i>FALSE</i>\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	rfid3Sleep(2000);
	apiResultPassLog();
	logToResultFile(L"RMAPI-USERLED");
	
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test SetUserLed operation - <i>NEGATIVE Test cases</i></b>**************************\n<br>");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED( ( RFID_HANDLE32)INVALID_HANDLE_VALUE, &LedInfo );
	apiResultFailLog();
	logToResultFile(L"RMAPI-USERLED");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED by passing NULL parameter to LED info\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetUserLED( api3Data.hReaderMgmt, NULL );
	apiResultFailLog();
	logToResultFile(L"RMAPI-USERLED");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-USERLED-%d></a> RMAPI-USERLED-%d:\n\n<br> Description: Testing RFID_SetUserLED without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	LedInfo.ledColor = LED_GREEN;
	LedInfo.blink = true;
	LedInfo.durationSeconds = 1;

	api3Data.rfidStatus = RFID_SetUserLED(api3Data.hReaderMgmt,&LedInfo );
	apiResultFailLog();
	logToResultFile(L"RMAPI-USERLED");
}
void TestSetUserLedAPI( RFID_HANDLE32 hReader)
{
	SetLogText("Test_SetUserLed");
	logCurrentExecution( "Test_SetUserLed");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test SetUserLed operation - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	api3Data.itemTestCount = 12;
	api3Data.dwTestCaseNo = 01;
	testSetUserLed();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void testGetLocalTime()
{
	SYSTEMTIME systemTime;
	
	api3Data.rfidStatus = RFID_GetLocalTime(api3Data.hReaderMgmt,&systemTime );
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"Month -> %d<br>",systemTime.wMonth);
		fprintf( api3Data.fpCurLog,"Day -> %d<br>",systemTime.wDay);
		fprintf( api3Data.fpCurLog,"Year -> %d<br>",systemTime.wYear);
		fprintf( api3Data.fpCurLog,"Hour -> %d<br>",systemTime.wHour);
		fprintf( api3Data.fpCurLog,"Minute -> %d<br>",systemTime.wMinute);
		fprintf( api3Data.fpCurLog,"Second -> %d<br>",systemTime.wSecond);
		fprintf( api3Data.fpCurLog,"DayOfWeek -> %d<br>",systemTime.wDayOfWeek);
		fprintf( api3Data.fpCurLog,"Milliseconds -> %d<br>",systemTime.wMilliseconds);
		wprintf( L"\nMonth -> %ld",systemTime.wMonth);
		wprintf( L"\nDay -> %ld",systemTime.wDay);
		wprintf( L"\nYear -> %ld",systemTime.wYear);
		wprintf( L"\nHour -> %ld",systemTime.wHour);
		wprintf( L"\nMinute -> %ld",systemTime.wMinute);
		wprintf( L"\nSecond -> %ld",systemTime.wSecond);
		wprintf( L"\nDayOfWeek -> %ld",systemTime.wDayOfWeek);
		wprintf( L"\nMilliseconds -> %ld",systemTime.wMilliseconds);

		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-FUN");
}

void testLocalTime()
{
	SYSTEMTIME systemTime;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_GetLocalTime\n<br> Expected Output: Local time of the Reader",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");

	testGetLocalTime();
	
	api3Data.rfidStatus = RFID_GetLocalTime(api3Data.hReaderMgmt,&systemTime);

	// Modify the time to Set to a different date and time
	systemTime.wDay -= 1;
	systemTime.wHour -= 1;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_SetLocalTime with valid systemTime data\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLocalTime(api3Data.hReaderMgmt,&systemTime);
	apiResultPassLog();
	logToResultFile(L"RMAPI-LOCALTIME");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing Verify the RFID_GetLocalTime\n<br> Expected Output: Reader Local time should be set with the valid data",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");

	testGetLocalTime();
}

void testLocalTime_Negative()
{
	SYSTEMTIME systemTime;

	Login();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_SetLocalTime by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLocalTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&systemTime );
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_SetLocalTime by passing NULL paramter\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLocalTime(api3Data.hReaderMgmt,NULL );
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_SetLocalTime by passing invalid system time data\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	systemTime.wDay = 110;
	systemTime.wDayOfWeek = 1;
	systemTime.wHour = 120;
	systemTime.wMilliseconds = 1000;
	systemTime.wMinute = 30;
	systemTime.wMonth = 1;
	systemTime.wSecond = 30;
	systemTime.wYear = 2010;
	
	api3Data.rfidStatus = RFID_SetLocalTime(api3Data.hReaderMgmt,&systemTime );
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_GetLocalTime by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetLocalTime(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&systemTime );
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");


	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_GetLocalTime by passing NULL paramter\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetLocalTime(api3Data.hReaderMgmt,NULL );
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_GetLocalTime without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetLocalTime(api3Data.hReaderMgmt,&systemTime );
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-LOCALTIME-%d></a> RMAPI-LOCALTIME-%d:\n\n<br> Description: Testing RFID_GetLocalTime without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLocalTime(api3Data.hReaderMgmt,&systemTime);
	apiResultFailLog();
	logToResultFile(L"RMAPI-LOCALTIME");
}

void Test_LocalTime( RFID_HANDLE32 hReader )
{
	SetLogText("Test_LocalTime");
	logCurrentExecution( "Test_LocalTime");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount = 10;
	api3Data.dwTestCaseNo = 01;
	Login();
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Local time of the Reader - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	testLocalTime();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Local time of the Reader - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	testLocalTime_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void testGetTimeZone()
{
	TIME_ZONE_LIST timeZoneList;

	api3Data.rfidStatus = RFID_GetTimeZoneList(api3Data.hReaderMgmt,&timeZoneList);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"Number of TimeZones: %d<br>",timeZoneList.numTimeZones);
		fprintf( api3Data.fpCurLog,"ActiveTimeZone %d -- %S<br>",timeZoneList.activeTimeZoneIndex,timeZoneList.pTimeZone[timeZoneList.activeTimeZoneIndex]);
		for (int i=0; i<timeZoneList.numTimeZones ; i++ )
		{
			fprintf( api3Data.fpCurLog,"\nTime Zone %d -- %S <br>", i, timeZoneList.pTimeZone[i]);
			wprintf( L"\nTime Zone %d -- %ls ", i, timeZoneList.pTimeZone[i]);
		}
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-TIMEZONE");
}

void testTimeZone()
{
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_GetTimeZoneList\n<br> Expected Output: Time Zone details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	testGetTimeZone();
	// Get the number of time zones supported.
	// and picka random number with in the zone limit to set
	//
	TIME_ZONE_LIST timeZoneList;
	api3Data.rfidStatus = RFID_GetTimeZoneList(api3Data.hReaderMgmt,&timeZoneList);
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_SetTimeZone with Random TimeZone Selected \n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	for( int i = 0;i < timeZoneList.numTimeZones;i++)
	{
		api3Data.rfidStatus = RFID_SetTimeZone( api3Data.hReaderMgmt,i); 
		api3Data.rfidStatus = RFID_GetTimeZoneList( api3Data.hReaderMgmt,&timeZoneList );
		if (api3Data.rfidStatus != RFID_READER_FUNCTION_UNSUPPORTED)
		{
			wprintf(L"\n Set TimeZone Index %ld\t GetTMZoneIndex is %ld\t ActiveTimeZone is %ls", i, timeZoneList.activeTimeZoneIndex, timeZoneList.pTimeZone[timeZoneList.activeTimeZoneIndex]);
		}
		if(api3Data.rfidStatus != RFID_API_SUCCESS && i != timeZoneList.activeTimeZoneIndex  )
		{
			apiResultPassLog();
		}
		else if( api3Data.rfidStatus == RFID_API_SUCCESS )
		{
			SetLogResult( TRUE );
		}
	}
	
	logToResultFile(L"RMAPI-TIMEZONE");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Verify is the time zone set successfully\n<br> Expected Output: Time Zone details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	testGetTimeZone();
}

void testTimeZone_Negative()
{
	TIME_ZONE_LIST timeZoneList;

	Login();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_GetTimeZoneList by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetTimeZoneList(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&timeZoneList);
	apiResultFailLog();
	logToResultFile(L"RMAPI-TIMEZONE");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_GetTimeZoneList by passing NULL parameter to the timeZonelist\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetTimeZoneList(api3Data.hReaderMgmt,NULL);
	apiResultFailLog();
	logToResultFile(L"RMAPI-TIMEZONE");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_SetTimeZone by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetTimeZone(( RFID_HANDLE32)INVALID_HANDLE_VALUE,76);
	apiResultFailLog();
	logToResultFile(L"RMAPI-TIMEZONE");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_SetTimeZone by passing invalid (out of range) parameter\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetTimeZone(api3Data.hReaderMgmt,1000);
	apiResultFailLog();
	logToResultFile(L"RMAPI-TIMEZONE");
	
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_GetTimeZoneList without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetTimeZoneList(api3Data.hReaderMgmt,&timeZoneList);
	apiResultFailLog();
	logToResultFile(L"RMAPI-TIMEZONE");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-TIMEZONE-%d></a> RMAPI-TIMEZONE-%d:\n\n<br> Description: Testing RFID_SetTimeZone without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetTimeZone(api3Data.hReaderMgmt,10);
	apiResultFailLog();
	logToResultFile(L"RMAPI-TIMEZONE");
}

void Test_TimeZone( RFID_HANDLE32 hReader )
{
	SetLogText("Test_TimeZone");
	logCurrentExecution( "Test_TimeZone");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	api3Data.itemTestCount =11;
	api3Data.dwTestCaseNo = 01;
	Login();
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Time Zone - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	testTimeZone();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Time Zone - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	testTimeZone_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void testGetReaderStatistics(int antennaId)
{
	READER_STATS readerStats;
	memset(&readerStats,0,sizeof(readerStats));
	api3Data.rfidStatus = RFID_GetReaderStats(api3Data.hReaderMgmt,antennaId,&readerStats);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"nxpCalibrateSuccessCount = %d<br>", readerStats.nxpCalibrateSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpCalibrateFailureCount = %d<br>", readerStats.nxpCalibrateFailureCount);
		fprintf( api3Data.fpCurLog,"nxpChangeEASSuccessCount = %d<br>", readerStats.nxpChangeEASSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpChangeEASFailureCount = %d<br>", readerStats.nxpChangeEASFailureCount);
		fprintf( api3Data.fpCurLog,"nxpEASAlarmSuccessCount = %d<br>", readerStats.nxpEASAlarmSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpEASAlarmFailureCount = %d<br>", readerStats.nxpEASAlarmFailureCount);
		fprintf( api3Data.fpCurLog,"nxpReadProtectSuccessCount = %d<br>", readerStats.nxpReadProtectSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpReadProtectFailureCount = %d<br>", readerStats.nxpReadProtectFailureCount);
		fprintf( api3Data.fpCurLog,"nxpResetReadProtectSuccessCount = %d<br>", readerStats.nxpResetReadProtectSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpResetReadProtectFailureCount = %d<br>", readerStats.nxpResetReadProtectFailureCount);
		fprintf( api3Data.fpCurLog,"writeSuccessCount = %d<br>", readerStats.writeSuccessCount);
		fprintf( api3Data.fpCurLog,"writeFailureCount = %d<br>", readerStats.writeFailureCount);
		fprintf( api3Data.fpCurLog,"readSuccessCount = %d<br>", readerStats.readSuccessCount);
		fprintf( api3Data.fpCurLog,"readFailureCount = %d<br>", readerStats.readFailureCount);
		fprintf(api3Data.fpCurLog, "ambientTemperatureHighAlarmCount = %d\n", readerStats.ambientTemperatureHighAlarmCount);
		fprintf(api3Data.fpCurLog, "ambientTemperatureCriticalAlarmCount = %d\n", readerStats.ambientTemperatureCriticalAlarmCount);
		fprintf(api3Data.fpCurLog, "paTemperatureHighAlarmCount = %d\n", readerStats.paTemperatureHighAlarmCount);
		fprintf(api3Data.fpCurLog, "paTemperatureCriticalAlarmCount = %d\n", readerStats.paTemperatureCriticalAlarmCount);
		fprintf(api3Data.fpCurLog, "forwardPowerHighAlarmCount = %d\n", readerStats.forwardPowerHighAlarmCount);
		fprintf(api3Data.fpCurLog, "forwardPowerLowAlarmCount = %d\n", readerStats.forwardPowerLowAlarmCount);
		fprintf(api3Data.fpCurLog, "reversePowerHighAlarmCount = %d\n", readerStats.reversePowerHighAlarmCount);
		fprintf(api3Data.fpCurLog, "echoThresholdAlarmCount = %d\n", readerStats.echoThresholdAlarmCount);
		fprintf(api3Data.fpCurLog, "databaseWarningCount = %d\n", readerStats.databaseWarningCount);
		fprintf(api3Data.fpCurLog, "databaseErrorCount = %d\n", readerStats.databaseErrorCount);
		fprintf(api3Data.fpCurLog, "gpioInformationCount = %d\n", readerStats.gpioInformationCount);
		fprintf( api3Data.fpCurLog,"<br><br>");
		SetLogResult( TRUE );
		wprintf(L"nxpCalibrateSuccessCount = %d", readerStats.nxpCalibrateSuccessCount);
		wprintf(L"nxpCalibrateFailureCount = %d", readerStats.nxpCalibrateFailureCount);
		wprintf(L"nxpChangeEASSuccessCount = %d", readerStats.nxpChangeEASSuccessCount);
		wprintf(L"nxpChangeEASFailureCount = %d", readerStats.nxpChangeEASFailureCount);
		wprintf(L"nxpEASAlarmSuccessCount = %d", readerStats.nxpEASAlarmSuccessCount);
		wprintf(L"nxpEASAlarmFailureCount = %d", readerStats.nxpEASAlarmFailureCount);
		wprintf(L"nxpReadProtectSuccessCount = %d", readerStats.nxpReadProtectSuccessCount);
		wprintf(L"nxpReadProtectFailureCount = %d", readerStats.nxpReadProtectFailureCount);
		wprintf(L"nxpResetReadProtectSuccessCount = %d", readerStats.nxpResetReadProtectSuccessCount);
		wprintf(L"nxpResetReadProtectFailureCount = %d", readerStats.nxpResetReadProtectFailureCount);
		wprintf(L"writeSuccessCount = %d", readerStats.writeSuccessCount);
		wprintf(L"writeFailureCount = %d", readerStats.writeFailureCount);
		wprintf(L"readSuccessCount = %d", readerStats.readSuccessCount);
		wprintf(L"readFailureCount = %d", readerStats.readFailureCount);
		wprintf(L"ambientTemperatureHighAlarmCount = %d\n", readerStats.ambientTemperatureHighAlarmCount);
		wprintf(L"ambientTemperatureCriticalAlarmCount = %d\n", readerStats.ambientTemperatureCriticalAlarmCount);
		wprintf(L"paTemperatureHighAlarmCount = %d\n", readerStats.paTemperatureHighAlarmCount);
		wprintf(L"paTemperatureCriticalAlarmCount = %d\n", readerStats.paTemperatureCriticalAlarmCount);
		wprintf(L"forwardPowerHighAlarmCount = %d\n", readerStats.forwardPowerHighAlarmCount);
		wprintf(L"forwardPowerLowAlarmCount = %d\n", readerStats.forwardPowerLowAlarmCount);
		wprintf(L"reversePowerHighAlarmCount = %d\n", readerStats.reversePowerHighAlarmCount);
		wprintf(L"echoThresholdAlarmCount = %d\n", readerStats.echoThresholdAlarmCount);
		wprintf(L"databaseWarningCount = %d\n", readerStats.databaseWarningCount);
		wprintf(L"databaseErrorCount = %d\n", readerStats.databaseErrorCount);
		wprintf(L"gpioInformationCount = %d\n", readerStats.gpioInformationCount);
		wprintf(L"ambientTemperatureHighAlarmCount = %d\n", readerStats.ambientTemperatureHighAlarmCount);
		wprintf(L"ambientTemperatureCriticalAlarmCount = %d\n", readerStats.ambientTemperatureCriticalAlarmCount);
		wprintf(L"paTemperatureHighAlarmCount = %d\n", readerStats.paTemperatureHighAlarmCount);
		wprintf(L"paTemperatureCriticalAlarmCount = %d\n", readerStats.paTemperatureCriticalAlarmCount);
		wprintf(L"forwardPowerHighAlarmCount = %d\n", readerStats.forwardPowerHighAlarmCount);
		wprintf(L"forwardPowerLowAlarmCount = %d\n", readerStats.forwardPowerLowAlarmCount);
		wprintf(L"reversePowerHighAlarmCount = %d\n", readerStats.reversePowerHighAlarmCount);
		wprintf(L"echoThresholdAlarmCount = %d\n", readerStats.echoThresholdAlarmCount);
		wprintf(L"databaseWarningCount = %d\n", readerStats.databaseWarningCount);
		wprintf(L"databaseErrorCount = %d\n", readerStats.databaseErrorCount);
		wprintf(L"gpioInformationCount = %d\n", readerStats.gpioInformationCount);
		wprintf(L"\n\n");
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S<br>",RFID_GetErrorDescription(api3Data.rfidStatus));
		ERROR_INFO errorInfo;
	
		RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
		fprintf( api3Data.fpCurLog,"rfidStatusCode - %d<br>",errorInfo.rfidStatusCode);
		fprintf( api3Data.fpCurLog,"statusDesc - %S <br>",errorInfo.statusDesc);
		fprintf( api3Data.fpCurLog,"vendorMessage - %S <br>", errorInfo.vendorMessage);
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	logToResultFile(L"RMAPI-FUN");
}

void testReaderStatistics()
{
	Login();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_GetReaderStats<br> Expected Output: Reader Statistics details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	testGetReaderStatistics(1);
	testGetReaderStatistics(2);
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_ClearReaderStats\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ClearReaderStats(api3Data.hReaderMgmt);
	apiResultPassLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Verify the Reader statistics is cleared<br> Expected Output: Reader Statistics details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	testGetReaderStatistics(1);

}
void testReaderStatistics_Negative()
{
	Login();

	READER_STATS readerStats;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_GetReaderStats by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderStats(( RFID_HANDLE32)INVALID_HANDLE_VALUE,0,&readerStats);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_GetReaderStats by passing NULL paramter to the READER_STATS parameter\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderStats(( RFID_HANDLE32)INVALID_HANDLE_VALUE,0,NULL);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_GetReaderStats by passing Antenna ID -> 0\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderStats(api3Data.hReaderMgmt,0,&readerStats);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_GetReaderStats by passing Antenna ID -> 20\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderStats(api3Data.hReaderMgmt,20,&readerStats);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");


	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_ClearReaderStats by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderStats( (RFID_HANDLE32)INVALID_HANDLE_VALUE,20,&readerStats);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");
	
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_GetReaderStats without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetReaderStats(api3Data.hReaderMgmt,0,&readerStats);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-READERSTATISTICS-%d></a> RMAPI-READERSTATISTICS-%d:\n\n<br> Description: Testing RFID_ClearReaderStats without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_ClearReaderStats(api3Data.hReaderMgmt);
	apiResultFailLog();
	logToResultFile(L"RMAPI-READERSTATISTICS");
}
void Test_ReaderStatistics( RFID_HANDLE32 hReader )
{
	SetLogText("Test_ReaderStatistics");
	logCurrentExecution( "Test_ReaderStatistics");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Reader Statistics - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	api3Data.itemTestCount = 11;
	api3Data.dwTestCaseNo = 01;
	testReaderStatistics();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test Reader Statistics - <i>Negative Test cases</i></b>**************************\n<br>");
	testReaderStatistics_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	api3Data.dwSkipCount = (api3Data.itemTestCount - (api3Data.dwPassCount+api3Data.dwFailCount));
	LogPassFailCount();
}

void getLLRPConnectionConfig( LLRP_CONNECTION_CONFIG* pllrpConnectionConfig )
{
	api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,pllrpConnectionConfig );
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		fprintf( api3Data.fpCurLog,"hostServerIP: %S<br>",pllrpConnectionConfig->hostServerIP);
		fprintf( api3Data.fpCurLog,"isClient: %d<br>",pllrpConnectionConfig->isClient);
		fprintf( api3Data.fpCurLog,"port: %d",pllrpConnectionConfig->port);
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
	else
	{
		fprintf( api3Data.fpCurLog,"<b>FAIL:</b>%S",RFID_GetErrorDescription(api3Data.rfidStatus));
		ERROR_INFO errorInfo;
	
		RFID_GetLastErrorInfo(api3Data.hReaderMgmt, &errorInfo);
		fprintf( api3Data.fpCurLog,"rfidStatusCode - %d<br>",errorInfo.rfidStatusCode);
		fprintf( api3Data.fpCurLog,"statusDesc - %S <br>",errorInfo.statusDesc);
		fprintf( api3Data.fpCurLog,"vendorMessage - %S <br>", errorInfo.vendorMessage);
		fprintf( api3Data.fpCurLog,"<br><br>");
		fprintf( api3Data.fpCurLog,"<br><br>");
	}
}

void testLLRPConnectionConfig()
{
	LLRP_CONNECTION_CONFIG g_llrpConnectionConfig;
	LLRP_CONNECTION_CONFIG s_llrpConnectionConfig;
	memset(&g_llrpConnectionConfig,0,sizeof(LLRP_CONNECTION_CONFIG));
	memset(&s_llrpConnectionConfig,0,sizeof(LLRP_CONNECTION_CONFIG));

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetLLRPConnectionConfig\n<br> Expected Output: LLRP configuration details",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	getLLRPConnectionConfig( &g_llrpConnectionConfig );
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RMAPI-FUN");

	wcscpy( s_llrpConnectionConfig.hostServerIP, L"10.10.10.1"); 
	s_llrpConnectionConfig.isClient = true;
	s_llrpConnectionConfig.port = 5084;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetLLRPConnectionConfig with <i>clientMode</i> set to TRUE\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,&s_llrpConnectionConfig);
	apiResultPassLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Verify the RFID_GetLLRPConnectionConfig\n<br> Expected Output: LLRP should be configured with the client Mode with an appropriate IP",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	getLLRPConnectionConfig( &g_llrpConnectionConfig );
	if( wcscmp(g_llrpConnectionConfig.hostServerIP,s_llrpConnectionConfig.hostServerIP) == 0 && g_llrpConnectionConfig.isClient == s_llrpConnectionConfig.isClient )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RMAPI-FUN");
	
	wcscpy(s_llrpConnectionConfig.hostServerIP, L"10.10.10.1"); 
	s_llrpConnectionConfig.isClient = false;
	s_llrpConnectionConfig.port = 5084;

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetLLRPConnectionConfig with <i>clientMode</i> set to FALSE\n<br> Expected Output: RFID_API_SUCCESS",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,&s_llrpConnectionConfig);
	apiResultPassLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Verify the RFID_GetLLRPConnectionConfig\n<br> Expected Output: LLRP should be configured with the server Mode with an appropriate IP",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	
	getLLRPConnectionConfig( &g_llrpConnectionConfig );
	if( wcscmp(g_llrpConnectionConfig.hostServerIP,s_llrpConnectionConfig.hostServerIP) == 0 && (g_llrpConnectionConfig.isClient == s_llrpConnectionConfig.isClient) )
	{
		SetLogResult( TRUE );
	}

	logToResultFile(L"RMAPI-FUN");
	
}

void testLLRPConnectionConfig_Negative()
{
	LLRP_CONNECTION_CONFIG llrpConnectionConfig;
	memset(&llrpConnectionConfig,0,sizeof(LLRP_CONNECTION_CONFIG));
	
	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetLLRPConnectionConfig by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&llrpConnectionConfig);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetLLRPConnectionConfig by passing NULL parameter to the LLRP_CONNECTION_CONFIG\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,NULL);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetLLRPConnectionConfig by passing INVALID_HANDLE\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(( RFID_HANDLE32)INVALID_HANDLE_VALUE,&llrpConnectionConfig);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetLLRPConnectionConfig by passing NULL parameter to the LLRP_CONNECTION_CONFIG\n<br> Expected Output: RFID_PARAM_ERROR",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,NULL);
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetLLRPConnectionConfig by passing <i>Invalid IP address</i> to the LLRP_CONNECTION_CONFIG\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");
	Login();
	memset(&llrpConnectionConfig,0,sizeof(LLRP_CONNECTION_CONFIG));
	wcscpy(llrpConnectionConfig.hostServerIP, L"10.10.10.1.1"); 
	llrpConnectionConfig.isClient = TRUE;
	llrpConnectionConfig.port = 5084;
	
	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig( api3Data.hReaderMgmt,&llrpConnectionConfig );
	
	apiResultFailLog();
	logToResultFile(L"RMAPI-FUN");

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_SetLLRPConnectionConfig by passing <i>the port 80 (already in use)</i> to the LLRP_CONNECTION_CONFIG\n<br> Expected Output: RFID_RM_COMMAND_FAILED",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	wcscpy(llrpConnectionConfig.hostServerIP, L"127.0.0.1"); 
	llrpConnectionConfig.isClient = FALSE;
	llrpConnectionConfig.port = 5084;
	
	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig( api3Data.hReaderMgmt,&llrpConnectionConfig);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RMAPI-FUN");

	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	Login();

	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetLLRPConnectionConfig without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	api3Data.rfidStatus = RFID_GetLLRPConnectionConfig(api3Data.hReaderMgmt,&llrpConnectionConfig);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RMAPI-FUN");


	fprintf( api3Data.fpCurLog,"\n\n<a name=RMAPI-FUN-%d></a> RMAPI-FUN-%d:\n\n<br> Description: Testing RFID_GetLLRPConnectionConfig without RM_Login()\n<br> Expected Output: RFID_INVALID_HANDLE",api3Data.dwTestCaseNo,api3Data.dwTestCaseNo );
	fprintf( api3Data.fpCurLog,"<br>");
	fprintf( api3Data.fpCurLog, "\n Result: ");

	llrpConnectionConfig.port = 5084;
	llrpConnectionConfig.isClient = FALSE;

	api3Data.rfidStatus = RFID_SetLLRPConnectionConfig(api3Data.hReaderMgmt,&llrpConnectionConfig);
	if( api3Data.rfidStatus == RFID_API_SUCCESS )
	{
		SetLogResult( TRUE );
	}
	logToResultFile(L"RMAPI-FUN");
}

void Test_LLRPConnectionConfig( RFID_HANDLE32 hReader )
{
	SetLogText("Test_LLRPConnectionConfig");
	logCurrentExecution("Test_LLRPConnectionConfig");
	fprintf( api3Data.fpCurLog,"<html>\n");
	fprintf( api3Data.fpCurLog,"<body>\n");
	Login();
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test LLRP configuration - <i>FUNCTIONAL Test cases</i></b>**************************\n<br>");
	testLLRPConnectionConfig();
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
	fprintf( api3Data.fpCurLog,"\n**************************<b>Test LLRP configuration - <i>NEGATIVE Test cases</i></b>**************************\n<br>");
	//testLLRPConnectionConfig_Negative();
	fprintf( api3Data.fpCurLog,"</body>\n");
	fprintf( api3Data.fpCurLog,"</html>\n");
	LogPassFailCount();

}

void GetReaderStats(RFID_HANDLE32 hreader)
{
	Login();
	READER_STATS readerStats;
	api3Data.rfidStatus = RFID_GetReaderStats(api3Data.hReaderMgmt,1,&readerStats);
	fprintf( api3Data.fpLog,"************Reader statistics of ANTENNA ID 1************\n");
	if (api3Data.rfidStatus == RFID_API_SUCCESS)
	{
		wprintf(L"\nnxpCalibrateSuccessCount = %d<br>", readerStats.nxpCalibrateSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpCalibrateSuccessCount = %d<br>", readerStats.nxpCalibrateSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpCalibrateFailureCount = %d<br>", readerStats.nxpCalibrateFailureCount);
		fprintf( api3Data.fpCurLog,"nxpChangeEASSuccessCount = %d<br>", readerStats.nxpChangeEASSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpChangeEASFailureCount = %d<br>", readerStats.nxpChangeEASFailureCount);
		fprintf( api3Data.fpCurLog,"nxpEASAlarmSuccessCount = %d<br>", readerStats.nxpEASAlarmSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpEASAlarmFailureCount = %d<br>", readerStats.nxpEASAlarmFailureCount);
		fprintf( api3Data.fpCurLog,"nxpReadProtectSuccessCount = %d<br>", readerStats.nxpReadProtectSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpReadProtectFailureCount = %d<br>", readerStats.nxpReadProtectFailureCount);
		fprintf( api3Data.fpCurLog,"nxpResetReadProtectSuccessCount = %d<br>", readerStats.nxpResetReadProtectSuccessCount);
		fprintf( api3Data.fpCurLog,"nxpResetReadProtectFailureCount = %d<br>", readerStats.nxpResetReadProtectFailureCount);
		fprintf( api3Data.fpCurLog,"writeSuccessCount = %d<br>", readerStats.writeSuccessCount);
		fprintf( api3Data.fpCurLog,"writeFailureCount = %d<br>", readerStats.writeFailureCount);
		fprintf( api3Data.fpCurLog,"readSuccessCount = %d<br>", readerStats.readSuccessCount);
		fprintf( api3Data.fpCurLog,"readFailureCount = %d", readerStats.readFailureCount);
	}
	api3Data.rfidStatus = RFID_Logout(api3Data.hReaderMgmt);
}
void VerifyRMProfileTimeZoneAPI( )
{
	wprintf(L"\n Executing %ls Started ",MultiByte2WideChar( __FUNCTION__));
	TestProfileAPI( api3Data.hReader );
	TestUSB( api3Data.hReader );
	TestGPIdebounce( api3Data.hReader );
	//
	//TestInitiateLLRPconn(api3Data.hReader);//Manual
	TestGetHealthStatus( api3Data.hReader );
	TestReaderInfo ( api3Data.hReader );

	TestSetUserLedAPI( api3Data.hReader );
	Test_LocalTime( api3Data.hReader );
	Test_TimeZone( api3Data.hReader );
	Test_ReaderStatistics( api3Data.hReader );
	//Test_LLRPConnectionConfig( api3Data.hReader );//Crashing LLRP Need to check
	//GetReaderStats( api3Data.hReader );

	// Attempt Reconnection after profile API, since the LLRP will be re initialized and Conenction reset 
	// happens
	wprintf(L"\n Executing %ls Comleted ",MultiByte2WideChar( __FUNCTION__));
}
