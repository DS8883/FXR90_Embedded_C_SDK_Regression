#include "ManualTests.h"

extern struct RFID3DATA api3Data;

void SetRandomProfile()
{
	PROFILE_LIST lpProfileList;	
	LOGIN_INFO loginfo;
	wcscpy( loginfo.hostName,api3Data.szIPAddress );
	wcscpy( loginfo.userName,L"admin" );
	wcscpy( loginfo.password,L"change" );
	loginfo.forceLogin = TRUE;
	loginfo.version = RFID_API3_5_5;

	api3Data.rfidStatus = RFID_Login( &api3Data.hReaderMgmt,&loginfo,FX,FALSE,0 );
	api3Data.rfidStatus = RFID_GetProfileList( api3Data.hReaderMgmt, &lpProfileList);
	wprintf(L"\n RFID_GetProfileList: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	if( lpProfileList.pProfileName == NULL )
	{
		wprintf(L"\n Profile List is NULL");
	}
	if( lpProfileList.activeProfileIndex == 0) lpProfileList.activeProfileIndex++;
	api3Data.rfidStatus = RFID_SetActiveProfile( api3Data.hReaderMgmt, lpProfileList.pProfileName[lpProfileList.activeProfileIndex-1]);
	wprintf(L"\n RFID_SetActiveProfile: %ls \t Status :%ls",lpProfileList.pProfileName[lpProfileList.activeProfileIndex-1],RFID_GetErrorDescription(api3Data.rfidStatus));
	DisplayLastErrorInfo();
	api3Data.rfidStatus = RFID_Logout( api3Data.hReaderMgmt);
}
#ifdef linux
int getLinuxAcceptSocketInfo( int* myAcceptSocket, int Port  )
{
	wprintf(L"\n Inside getLinuxAcceptSocketInfo");
	int sd=-1;
	int rc, on=1, rcdsize=250;
	char buffer[250];
	struct sockaddr_in6 serveraddr, clientaddr;
	int addrlen=sizeof(clientaddr);
	char str[INET6_ADDRSTRLEN];
	if ((sd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)  
	{
		 wprintf(L"\nsocket() failed");
		 return -1;
	}

	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0) 
	{
		wprintf(L"\nsetsockopt(SO_REUSEADDR) failed");
		return -1; 
	}
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_port   = htons(Port);
	if(bind(sd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0) 
	{
		wprintf(L"\nbind() failed");
		return -1;
	}
	if(listen(sd, 10) < 0) 
	{
		wprintf(L"\nlisten() failed");
		return -1;
	}

	if(( *myAcceptSocket = accept(sd, NULL, NULL)) < 0)
	{
		wprintf(L"\naccept() failed");
		return -1;
	}
	else
	{
		/*****************************************************************/
		/* Display the client address.  Note that if the client is       */
		/* an IPv4 client, the address will be shown as an IPv4 Mapped   */
		/* IPv6 address.                                                 */
		/*****************************************************************/
		getpeername(*myAcceptSocket, (struct sockaddr *)&clientaddr, (socklen_t*)&addrlen);
		if(inet_ntop(AF_INET6, &clientaddr.sin6_addr, str, sizeof(str)))
		{
			wprintf(L"\nClient address is %ls\n", MultiByte2WideChar(str));
			wprintf(L"\nClient port is %ld\n", ntohs(clientaddr.sin6_port));
		}
		
		return 0;
	}
}
#endif
#ifndef linux
int InitializeSocketInfo( SOCKET* myAcceptSocket, int Port  )
{
	// Initialize Winsock.
	WSADATA wsaData;
	int Family = PF_INET6;
    int SocketType = SOCK_STREAM;
    char *Address = NULL;
	char myBuffer[16];
	int i,  RetVal;
	ADDRINFO Hints, *AddrInfo, *AI;
    SOCKET ServSock[FD_SETSIZE];;

	memset(&wsaData,0,sizeof(WSADATA));
	
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
	if (iResult != (int)NO_ERROR)
	{
		wprintf(L"\nError at WSAStartup()%ld\n", WSAGetLastError());
	}

	// By setting the AI_PASSIVE flag in the hints to getaddrinfo, we're
    // indicating that we intend to use the resulting address(es) to bind
    // to a socket(s) for accepting incoming connections.  This means that
    // when the Address parameter is NULL, getaddrinfo will return one
    // entry per allowed protocol family containing the unspecified address
    // for that family.
    //
    memset(&Hints, 0, sizeof (Hints));
    Hints.ai_family = Family;
    Hints.ai_socktype = SocketType;
    Hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
	RetVal = getaddrinfo(Address, _itoa(Port,myBuffer,10), &Hints, &AddrInfo);
    if (RetVal != 0)
	{
        wprintf(L"\ngetaddrinfo failed with error %d: \n",RetVal);
        WSACleanup();
        return -1;
    }

    // For each address getaddrinfo returned, we create a new socket,
    // bind that address to it, and create a queue to listen on.
    //
    for (i = 0, AI = AddrInfo; AI != NULL; AI = AI->ai_next)
	{
        // Highly unlikely, but check anyway.
        if (i == FD_SETSIZE) 
		{
            wprintf(L"\ngetaddrinfo returned more addresses than we could use.\n");
            break;
        }

        // This example only supports PF_INET and PF_INET6.
        if((AI->ai_family != PF_INET) && (AI->ai_family != PF_INET6))
		{
            continue;
		}

        // Open a socket with the correct address family for this address.
        ServSock[i] = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);

        if (ServSock[i] == INVALID_SOCKET) 
		{
            wprintf(L"\nSocket() failed with error %d:\n",WSAGetLastError());
            continue;
        }

        if ((AI->ai_family == PF_INET6) && IN6_IS_ADDR_LINKLOCAL((IN6_ADDR *) INETADDR_ADDRESS(AI->ai_addr)) && (((SOCKADDR_IN6 *) (AI->ai_addr))->sin6_scope_id == 0) )
		{
            wprintf(L"\nIPv6 link local addresses should specify a scope ID!\n");
        }
	
        // bind() associates a local address and port combination
        // with the socket just created. This is most useful when
        // the application is a server that has a well-known port
        // that clients know about in advance.
        //
        if(bind(ServSock[i], AI->ai_addr, (int) AI->ai_addrlen) == SOCKET_ERROR) 
		{
            wprintf(L"\nbind() failed with error %d: \n",WSAGetLastError());
            closesocket(ServSock[i]);
            continue;
        }
		if(SocketType == SOCK_STREAM) 
		{
            if(listen(ServSock[i], 5) == SOCKET_ERROR) 
			{
                wprintf(L"\nlisten() failed with error %d: \n",WSAGetLastError());
                closesocket(ServSock[i]);
                continue;
            }
		}

		*myAcceptSocket = accept(ServSock[i],NULL,NULL);
		if (*myAcceptSocket == INVALID_SOCKET) 
		{
			wprintf(L"\naccept() failed with error %d: \n",WSAGetLastError());
			WSACleanup();
			return -1;
		}
		else
		{
			char AddrName[NI_MAXHOST];
			struct sockaddr_storage Addr;
			int AddrLen = sizeof(Addr);
			if (getpeername(*myAcceptSocket, (LPSOCKADDR)&Addr, &AddrLen) == SOCKET_ERROR)
			{
				printf("Client: getpeername() failed with error %d: %s\n", WSAGetLastError());
			}
			else
			{
				if (getnameinfo((LPSOCKADDR)&Addr, AddrLen, AddrName, sizeof(AddrName), NULL, 0, NI_NUMERICHOST) == 0)
				{
					printf("Client: Connected to %s, port %d, protocol %s, protocol family %s\n", AddrName, ntohs(SS_PORT(&Addr)), (AI->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP", (AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
				}
			}
			return TRUE;
		 }
	}

	return 1;
}
#endif

// Test AcceptConnection on Host / MC Reader
// [in] Port -- port on which to accept the connection

int AcceptConnectionInRegularMode( UINT32 Port  )
{
	BOOLEAN bPowerstate;
#ifdef linux
	wprintf(L"\n AcceptConnectionInRegularMode");
	int acceptSocket;
	if( getLinuxAcceptSocketInfo(&acceptSocket,Port) != 0 )
	{
		wprintf(L"\n Error makeing connection");
		return FALSE;
	}
#else
	SOCKET acceptSocket;
	InitializeSocketInfo( &acceptSocket,Port );
	
#endif
	SERVER_INFO serverInfo;
	serverInfo.timeoutMilliseconds = 20000;
	serverInfo.version = RFID_API3_5_5;
	serverInfo.lpSecConInfo = NULL;

	api3Data.rfidStatus = RFID_AcceptConnection( &api3Data.hReader,acceptSocket,&serverInfo,NULL );
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		wprintf(L"\n API Result Description: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	}
	else
	{
		wprintf(L"\n SuccessFully Connected to the Client.. Doing Inventory");
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
		rfid3Sleep( 5000 );	
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
		while( (api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{		
			printTagDataWithResults(pTagData);
		}

/*		if( api3Data.readerType == FX )
		{
			SetRandomProfile();
			rfid3Sleep( 6000 );
		}
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
	
		api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,FALSE);
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n set Radio --> TRUE status:%ls",bPowerstate?L"ON":L"OFF");*/

		
#ifndef linux
		closesocket(acceptSocket);
#endif
		RFID_DeallocateTag(api3Data.hReader,pTagData);
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
	}
	
	return TRUE;
}

// Test AcceptConnection on Host / MC Reader
// [in] Port -- port on which to accept the connection

int AcceptConnectioninSecureMode( UINT32 Port )
{
	BOOLEAN bPowerstate;
	BYTE g_ClientCertContent[10240];
	int	 g_ClientCertSize;
	BYTE g_ClientKeyContent[10240];
	int	g_ClientKeySize;
	BYTE g_RootCertContent[10240];
	int	 g_RootCertSize;
#ifdef linux
	wprintf(L"\n accept Connection - Secure Mode");
	int acceptSocket;
	if( getLinuxAcceptSocketInfo(&acceptSocket,Port) != 0 )
	{
		wprintf(L"\n Error makeing connection");
		return FALSE;
	}

#else
	SOCKET acceptSocket;
	InitializeSocketInfo( &acceptSocket,Port );

#endif
	// Initialize the Certificate Info
	SEC_CONNECTION_INFO SecureInfo;
	memset(&SecureInfo,0,sizeof(SEC_CONNECTION_INFO));
	
#ifdef linux	
	if(GetFileContent("./client_crt.pem", g_ClientCertContent, &g_ClientCertSize) > 0)
#else
	if(GetFileContent("C:\\FX7500\\CertDir\\client_crt.pem", g_ClientCertContent, &g_ClientCertSize) > 0)
#endif
	{
		SecureInfo.clientCertBuff = g_ClientCertContent;
		SecureInfo.sizeCertBuff = g_ClientCertSize;
	}
#ifdef linux
	if (GetFileContent("./client_key.pem", g_ClientKeyContent, &g_ClientKeySize) > 0)
#else
	if (GetFileContent("C:\\FX7500\\CertDir\\client_key.pem", g_ClientKeyContent, &g_ClientKeySize) > 0)

#endif
	{
		SecureInfo.clientKeyBuff = g_ClientKeyContent;
		SecureInfo.sizeKeyBuff = g_ClientKeySize;
	}

	SecureInfo.phraseBuff = (BYTE*)"abcd12345";
	SecureInfo.sizePhraseBuff = strlen((char *)"abcd12345");
#ifdef linux
	if (GetFileContent("./cacert.pem", g_RootCertContent, &g_RootCertSize) > 0)
#else
	if (GetFileContent("C:\\FX7500\\CertDir\\cacert.pem", g_RootCertContent, &g_RootCertSize) > 0)
#endif
	{
		SecureInfo.rootCertBuff = g_RootCertContent;
		SecureInfo.sizeRootCertBuff = g_RootCertSize;
	}
		
	SecureInfo.secureMode = TRUE;	
	SecureInfo.validatePeerCert = TRUE;
	wprintf(L"\n Secure Mode :%ls \t Validate Peer:%ls",SecureInfo.secureMode?L"YES":L"NO",SecureInfo.validatePeerCert?L"YES":L"NO");
	SERVER_INFO serverInfo;
	serverInfo.timeoutMilliseconds = 20000;
	serverInfo.version = RFID_API3_5_5;
	serverInfo.lpSecConInfo = &SecureInfo;

	api3Data.rfidStatus = RFID_AcceptConnection( &api3Data.hReader,acceptSocket,&serverInfo,NULL );
	if( api3Data.rfidStatus != RFID_API_SUCCESS )
	{
		wprintf(L"\n API Result Description: %ls",RFID_GetErrorDescription(api3Data.rfidStatus));
	}
	else
	{
		wprintf(L"\n SuccessFully Connected to the Client.. Doing Inventory");
		rfid3Sleep(2000);
		LPTAG_DATA pTagData = RFID_AllocateTag( api3Data.hReader );	
		api3Data.rfidStatus = RFID_PerformInventory(api3Data.hReader,NULL,NULL,NULL,NULL );
		rfid3Sleep( 6000 );
		api3Data.rfidStatus = RFID_StopInventory( api3Data.hReader );
	/*	if( api3Data.readerType == FX )
		{
			SetRandomProfile();
			rfid3Sleep( 6000 );
		}
			
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n Radio Power State is %ls",bPowerstate?L"ON":L"OFF");
		api3Data.rfidStatus = RFID_SetRadioPowerState(api3Data.hReader,FALSE);
		api3Data.rfidStatus = RFID_GetRadioPowerState( api3Data.hReader, &bPowerstate);
		wprintf(L"\n set Radio --> TRUE status:%ls",bPowerstate?L"ON":L"OFF");*/

		while( (api3Data.rfidStatus = RFID_GetReadTag(api3Data.hReader, pTagData)) == RFID_API_SUCCESS)
		{		
			printTagDataWithResults(pTagData);
		}

		RFID_DeallocateTag(api3Data.hReader,pTagData);
		api3Data.rfidStatus = RFID_Disconnect( api3Data.hReader );
		//Socket Cleanup
#ifdef linux
	close(acceptSocket);
#else
	WSACleanup();
#                           endif
	}
	
	return TRUE;
}
