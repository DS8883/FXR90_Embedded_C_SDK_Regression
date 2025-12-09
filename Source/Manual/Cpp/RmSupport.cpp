#ifndef linux
#include "ManualTests.h"
#include <shellapi.h>
#include <TCHAR.h>
#include <atlbase.h>
#include <xmllite.h>

extern struct RFID3DATA api3Data;

// COM ptr definitions for XML parsing
//
CComPtr<IStream> pFileStream;	// ComPtr for XML file Stream
CComPtr<IStream> pOutFileStream;
CComPtr<IXmlWriter> pWriter;
CComPtr<IXmlReader> pReader;
XmlNodeType nodeType;
TCHAR szReaderIP[256];
char MBCSSessionID[32];		// for Session ID in MBCS format.
TCHAR Command[512];			// Command to send to the reader via CURL
char XMLPreAmble[4096];		// XML preamble to  form XML input
char XMLPostfix[1024];		// Post fix for the XML file
TCHAR XMLControlCommand[128];	
TCHAR exePath[256];			// Curl Path
TCHAR CurlDirectory[256];	// Curl Directory

// verify if the region is set correclty or not
//
BOOL VerifyResult( TCHAR* filename )
{
	HRESULT hr;
	XmlNodeType nodeType;
//	const WCHAR* pwszPrefix;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;
	BOOL Success = FALSE;
	pFileStream = NULL;
	pReader = NULL;
	
	if (FAILED(hr = SHCreateStreamOnFile(filename, STGM_READ, &pFileStream)))
	{
		wprintf(L"Error creating file reader, error is %08.8lx", hr);
	    return FALSE;
	}

	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
	{
		wprintf(L"Error creating xml reader, error is %08.8lx", hr);
	    return FALSE;
	}

	if (FAILED(hr = pReader->SetInput(pFileStream)))
	{
		wprintf(L"Error setting input for reader, error is %08.8lx", hr);
	    return FALSE;
	}

	while (S_OK == (hr = pReader->Read(&nodeType)))
    {
		
		if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
		{
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
		}

		if(lstrcmp(pwszLocalName,L"resultCode")== 0 )
        {
            pReader->Read(&nodeType);
			pReader->GetValue(&pwszValue,NULL);
			if(lstrcmp( pwszValue,L"0")==0)
			{
				Success = TRUE;
			}
				
			break;
        }
	}

	return Success;
}

// Save the Region( Commit )
//
void SaveRegion( )
{
	FILE* fpSave = fopen("SaveRegion.xml","w");
	fprintf( fpSave,"%s",XMLPreAmble);
	fprintf( fpSave,"<motorm:sessionID>%s</motorm:sessionID>",MBCSSessionID );
	fprintf( fpSave,"<motorm:saveConfigChanges></motorm:saveConfigChanges>" );
	fprintf( fpSave,"</motorm:readerDevice></rm:command>");
	fflush( fpSave);
	fclose( fpSave );
	wsprintf(XMLControlCommand,TEXT("%s %s%s%s"),TEXT("-d @SaveRegion.xml"),TEXT("http://"),api3Data.szIPAddress,TEXT("/control -o SaveReply.xml -u"));
	wsprintf(Command,TEXT("%s \"%s\"%s\"%s\""),XMLControlCommand,TEXT("admin"),TEXT(":"),TEXT("change"));
	HINSTANCE hInst = ShellExecute( NULL,TEXT("open"), exePath, Command,NULL,SW_HIDE);
	Sleep(30000);
	
	// Clean up
	DeleteFile(L"SaveRegion.xml");
}

// Verify the Region by reading back
//
BOOL getRegion2Verify( char* regionName )
{
	HRESULT hr;
	XmlNodeType nodeType;
	char*  pRegionName;
//	const WCHAR* pwszPrefix;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;
	BOOL Success = FALSE;
	pFileStream = NULL;
	pReader = NULL;

	FILE* fpSave = fopen("getActive.xml","w");
	fprintf( fpSave,"%s",XMLPreAmble);
	fprintf( fpSave,"<motorm:sessionID>%s</motorm:sessionID>",MBCSSessionID);
	fprintf( fpSave,"<motorm:getActiveRegion />");
	fprintf( fpSave,"</motorm:readerDevice></rm:command>");
	fflush( fpSave );
	fclose( fpSave );
	wsprintf(XMLControlCommand,TEXT("%s %s%s%s"),TEXT("-d @getActive.xml"),TEXT("http://"),api3Data.szIPAddress,TEXT("/control -o getActiveReply.xml -u"));
	wsprintf(Command,TEXT("%s \"%s\"%s\"%s\""),XMLControlCommand,TEXT("admin"),TEXT(":"),TEXT("change"));
	HINSTANCE hInst = ShellExecute( NULL,TEXT("open"), exePath, Command,NULL,SW_HIDE);
	Sleep(5000);
	if (FAILED(hr = SHCreateStreamOnFile(L"getActiveReply.xml", STGM_READ, &pFileStream)))
	{
		wprintf(L"Error creating file reader, error is %08.8lx", hr);
	    return FALSE;
	}
	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
	{
		wprintf(L"Error creating xml reader, error is %08.8lx", hr);
	    return FALSE;
	}
	if (FAILED(hr = pReader->SetInput(pFileStream)))
	{
		wprintf(L"Error setting input for reader, error is %08.8lx", hr);
	    return FALSE;
	}

	while (S_OK == (hr = pReader->Read(&nodeType)))
    {
		
		if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
		{
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
		}

		if(lstrcmp(pwszLocalName,L"regionName")== 0 )
        {
            pReader->Read(&nodeType);
			pReader->GetValue(&pwszValue,NULL);
			
			// Since the value found during the XML parsing is Unicode String. This is need to be converted to MultiByte in order to
			// verify the correctness of the region
			//
			int bufferSpaceNeeded = WideCharToMultiByte(CP_ACP, 0, pwszValue, lstrlen(pwszValue), NULL, 0, NULL, NULL);
			if(bufferSpaceNeeded > 0)
			{

				pRegionName = new char[bufferSpaceNeeded];
				pRegionName[bufferSpaceNeeded] = '\0';  
				if(WideCharToMultiByte(CP_ACP, 0, pwszValue, lstrlen(pwszValue), pRegionName, bufferSpaceNeeded, NULL, NULL) > 0)
				{
					// WCTMB was successful, and multibyteBuffer should now hold the string.
					// Use it as you see fit here
				}

				if( strcmp( pRegionName,regionName ) == 0 )
				{
					Success = TRUE;
				}
				//delete[] pRegionName;	
			}

			break;
        }
	}

	DeleteFile(L"getActive.xml");
	DeleteFile(L"getActiveReply.xml");

	return Success;
}

// Change Region to the Specified Region name and verify if the Region Set Correclty or not
//
BOOLEAN ChangeRegion( RegionInfo *pInfo,ChannelSelectInfo* pChannelsInfo,BOOLEAN LBT,BOOLEAN Hopping )
{
	FILE* fpRegion = fopen("Region.xml","w");

	// Create XML file for setting the region based region, channels etc.
	//
	sprintf(XMLPreAmble,"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s","<?xml version=","1.0"," encoding=","UTF-8","?><rm:command epcglobal:creationDate=","2001-12-17T09:30:47.0Z"," epcglobal:schemaVersion=","0.0"," xsi:schemaLocation=","urn:epcglobal:rm:xsd:1 ../../../schemas/RmCommand.xsd"," xmlns:xsi=","http://www.w3.org/2001/XMLSchema-instance"," xmlns:rm=","urn:epcglobal:rm:xsd:1"," xmlns:epcglobal=","urn:epcglobal:xsd:1"," xmlns:motorm=","urn:motorfid:rm:xsd:1","><rm:id>101</rm:id><rm:targetName></rm:targetName><motorm:readerDevice>");
	sprintf(XMLPostfix,"%s","</motorm:setActiveRegion></motorm:readerDevice></rm:command>");
	fprintf(fpRegion,"%s",XMLPreAmble);
	fprintf(fpRegion,"<motorm:sessionID>%s</motorm:sessionID>",MBCSSessionID);
		
	fprintf(fpRegion,"<motorm:setActiveRegion><motorm:regionName>%s</motorm:regionName>",pInfo->RegionName);
	fprintf(fpRegion,"<motorm:standardName>%s</motorm:standardName>",pInfo->StandardName );
	
	// Set the channel list based on the array passed.
	// Since Max number of channels that can be present in EU region is FOUR. So the array will be presented in the following fashion
	// Array Index	: 0	1	2	3
	// Values		: 0	0	1	1
	// In the above example the indexes Two and Three  should be set and the First two should be unset.
	//
	if( pChannelsInfo != NULL )
	{
		fprintf(fpRegion,"<motorm:list>");
		if(pChannelsInfo->ChannelInfo[0] == 1 )
		{
			fprintf(fpRegion,"<motorm:channelUsed>%s</motorm:channelUsed>","1");
		}
		if(pChannelsInfo->ChannelInfo[1] == 1 )
		{
			fprintf(fpRegion,"<motorm:channelUsed>%s</motorm:channelUsed>","2");
		}
		if(pChannelsInfo->ChannelInfo[2] == 1 )
		{
			fprintf(fpRegion,"<motorm:channelUsed>%s</motorm:channelUsed>","3");
		}
		if( pInfo->freqCount > 3 && pChannelsInfo[0].ChannelInfo[3] == 1 )
		{
			fprintf(fpRegion,"<motorm:channelUsed>%s</motorm:channelUsed>","4");
		}
		fprintf(fpRegion,"</motorm:list>");
	}
	else
	{
		fprintf(fpRegion,"<motorm:list>");
		fprintf(fpRegion,"<motorm:channelUsed>%s</motorm:channelUsed>","0");
		fprintf(fpRegion,"</motorm:list>");
	}
	
	// IF Channel information is not passed, then treat it as non EU region( US ).
	//
	if( pChannelsInfo != NULL )
	{
		fprintf(fpRegion,"%s%s",LBT?"<motorm:doLBT>true</motorm:doLBT>":"<motorm:doLBT>false</motorm:doLBT>",Hopping?"<motorm:doHopping>true</motorm:doHopping>":"<motorm:doHopping>false</motorm:doHopping>");
	}
	
	fprintf( fpRegion,XMLPostfix );
	fclose( fpRegion );
	
	// Command formation to set the region on the reader
	//
	wsprintf(XMLControlCommand,TEXT("%s %s%s%s"),TEXT("-d @Region.xml"),TEXT("http://"),api3Data.szIPAddress,TEXT("/control -o RegionReply.xml -u"));
	wsprintf(Command,TEXT("%s \"%s\"%s\"%s\""),XMLControlCommand,TEXT("admin"),TEXT(":"),TEXT("change"));
	HINSTANCE hInst = ShellExecute( NULL,TEXT("open"), exePath, Command,NULL,SW_HIDE);
	Sleep(3000);
	
	
	// Verify the result if region is set or not.
	//
	if (VerifyResult(L"RegionReply.xml") == TRUE )
	{
		// Could not set the region..error
		printf("\n Region Set %s === SUCCESS",pInfo->StandardName );
		SaveRegion( );
	}
	
	// Verify by getting back the Region info from the Reader
	//
	if( !getRegion2Verify( pInfo->RegionName ) )
	{
		printf("\n region %s could not be verified",pInfo->RegionName);
		//exit (0);
	}

	
	// Clean up the intermediate files
	//
	DeleteFile(L"RegionReply.xml");
	DeleteFile(L"Region.xml");

	return TRUE;

}

// This will login and return the Session ID to the Caller.This will fill in the Session ID String
//
BOOLEAN RmLogin( wchar_t* IpAddress )
{
	HRESULT hr;
	XmlNodeType nodeType;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;
	if( pFileStream !=NULL ) pFileStream = NULL;;
	if( pReader != NULL ) pReader = NULL ;

	// format the login command with the given ip address
	//
	wsprintf(XMLControlCommand,TEXT("%s %s%s%s"),TEXT("-d @000doLogin.xml"),TEXT("http://"),api3Data.szIPAddress,TEXT("/control -o doLoginreply.xml -u"));
	DWORD errorInfo = GetCurrentDirectory( MAX_LENGTH ,CurlDirectory );
	wsprintf(exePath,TEXT("%s\\%s"),CurlDirectory,TEXT("Curl.exe"));
	wsprintf(Command,TEXT("%s \"%s\"%s\"%s\""),XMLControlCommand,TEXT("admin"),TEXT(":"),TEXT("change"));
	HINSTANCE hInst = ShellExecute( NULL,TEXT("open"), exePath, Command,NULL,SW_HIDE); 
	Sleep(2000);

	// Open the file for reading the session ID from the reader.
	//
	if (FAILED(hr = SHCreateStreamOnFile(L"doLoginreply.xml", STGM_READ, &pFileStream)))
	{
		wprintf(L"Error creating file reader, error is %08.8lx", hr);
	    return FALSE;
	}
	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
	{
		wprintf(L"Error creating xml reader, error is %08.8lx", hr);
	    return FALSE;
	}
	if (FAILED(hr = pReader->SetInput(pFileStream)))
	{
		wprintf(L"Error setting input for reader, error is %08.8lx", hr);
	    return FALSE;
	}

	while (S_OK == (hr = pReader->Read(&nodeType)))
    {
		
		//pReader->GetPrefix(&pwszPrefix,NULL);
		if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
		{
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
		}

		if(lstrcmp(pwszLocalName,L"sessionID")== 0 )
        {
            pReader->Read(&nodeType);
			pReader->GetValue(&pwszValue,NULL);

			// Convert the TCHAR Session ID to MBCS before copying to the session id string
			//
			int bufferSpaceNeeded = WideCharToMultiByte(CP_ACP, 0, pwszValue, lstrlen(pwszValue), NULL, 0, NULL, NULL);
			if(bufferSpaceNeeded > 0)
			{

				 char*  pMBCSSessionID = new char[bufferSpaceNeeded+1];
				 pMBCSSessionID[bufferSpaceNeeded] = '\0';  
				  if(WideCharToMultiByte(CP_ACP, 0, pwszValue, lstrlen(pwszValue), pMBCSSessionID, bufferSpaceNeeded, NULL, NULL) > 0)
				  {
					// WCTMB was successful, and multibyteBuffer should now hold the string.
					// Use it as you see fit here
				  }
				  strcpy(MBCSSessionID,pMBCSSessionID);
				  delete[] pMBCSSessionID;
			}
			break;
        }
    }
	DeleteFile(L"doLoginreply.xml");
	return TRUE;
}

BOOL getReaderConfig( )
{
	HRESULT hr;
	XmlNodeType nodeType;
    const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;
	char*  pRegionName;
	
	// login to the Reader with ip address given.
	//
	if( !RmLogin( api3Data.szIPAddress ) )
	{
		printf("\n Count not Login to the Reader.");
	    return FALSE;
	}
	// read the regions supported from the reader and determine if the reader is US only Reader or WorldReader
	FILE* fpSupportedRegions = fopen("support.xml","w");

	// Create XML file for setting the region based region, channels etc.
	//
	sprintf(XMLPreAmble,"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"%s","<?xml version=","1.0"," encoding=","UTF-8","?><rm:command epcglobal:creationDate=","2001-12-17T09:30:47.0Z"," epcglobal:schemaVersion=","0.0"," xsi:schemaLocation=","urn:epcglobal:rm:xsd:1 ../../../schemas/RmCommand.xsd"," xmlns:xsi=","http://www.w3.org/2001/XMLSchema-instance"," xmlns:rm=","urn:epcglobal:rm:xsd:1"," xmlns:epcglobal=","urn:epcglobal:xsd:1"," xmlns:motorm=","urn:motorfid:rm:xsd:1","><rm:id>101</rm:id><rm:targetName></rm:targetName><motorm:readerDevice>");
	sprintf(XMLPostfix,"%s","</motorm:setActiveRegion></motorm:readerDevice></rm:command>");
	fprintf(fpSupportedRegions,"%s",XMLPreAmble);
	fprintf(fpSupportedRegions,"<motorm:sessionID>%s</motorm:sessionID>",MBCSSessionID);
	fprintf(fpSupportedRegions,"<motorm:getSupportedRegionList></motorm:getSupportedRegionList></motorm:readerDevice></rm:command>");
	fclose(fpSupportedRegions);
	
	wsprintf(XMLControlCommand,TEXT("%s %s%s%s"),TEXT("-d @support.xml"),TEXT("http://"),api3Data.szIPAddress,TEXT("/control -o supportreply.xml -u"));
	wsprintf(Command,TEXT("%s \"%s\"%s\"%s\""),XMLControlCommand,TEXT("admin"),TEXT(":"),TEXT("change"));
	HINSTANCE hInst = ShellExecute( NULL,TEXT("open"), exePath, Command,NULL,SW_HIDE);
	Sleep(3000);
	pFileStream = NULL;
	pReader = NULL;
	if (FAILED(hr = SHCreateStreamOnFile(L"supportreply.xml", STGM_READ, &pFileStream)))
	{
		wprintf(L"Error creating file reader, error is %08.8lx", hr);
	    return FALSE;
	}
	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
	{
		wprintf(L"Error creating xml reader, error is %08.8lx", hr);
		return FALSE;
	}
	if (FAILED(hr = pReader->SetInput(pFileStream)))
	{
		wprintf(L"Error setting input for reader, error is %08.8lx", hr);
		return FALSE;
	}

	while (S_OK == (hr = pReader->Read(&nodeType)))
    {
		
		if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
		{
            wprintf(L"Error getting local name, error is %08.8lx", hr);
            return -1;
		}

		if(lstrcmp(pwszLocalName,L"value")== 0 )
        {
			pReader->Read(&nodeType);
			
			do
			{
				pReader->GetValue(&pwszValue,NULL);

				// Since the value found during the XML parsing is Unicode String. This is need to be converted to MultiByte in order to
				// verify the correctness of the region
				//
				int bufferSpaceNeeded = WideCharToMultiByte(CP_ACP, 0, pwszValue, lstrlen(pwszValue), NULL, 0, NULL, NULL);
				if(bufferSpaceNeeded > 0)
				{

					pRegionName = new char[bufferSpaceNeeded];
					pRegionName[bufferSpaceNeeded] = '\0';  
					if(WideCharToMultiByte(CP_ACP, 0, pwszValue, lstrlen(pwszValue), pRegionName, bufferSpaceNeeded, NULL, NULL) > 0)
					{
						// WCTMB was successful, and multibyteBuffer should now hold the string.
						// Use it as you see fit here
					}

					//strcpy( RdrSupportedRegions.RegionNames[RdrSupportedRegions.regionCount],pRegionName);
					//RdrSupportedRegions.regionCount++;
				}
				//delete[] pRegionName;	
			}while(S_OK != pReader->MoveToNextAttribute() );

        }
	}

	DeleteFile(L"support.xml");
	DeleteFile(L"supportreply.xml");

	return TRUE;
}
#endif