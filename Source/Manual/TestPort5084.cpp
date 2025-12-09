#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Simple TCP port connectivity test for port 5084
int main()
{
    const char* readerIP = "169.254.10.1";
    int ports[] = {80, 443, 5084, 14150}; // HTTP, HTTPS, RFID Data, RFID Management
    
    printf("\n=== Testing TCP Port Connectivity to %s ===\n\n", readerIP);
    
    for (int i = 0; i < 4; i++)
    {
        int port = ports[i];
        printf("Testing port %d... ", port);
        fflush(stdout);
        
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            printf("ERROR: socket() failed: %s\n", strerror(errno));
            continue;
        }
        
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, readerIP, &addr.sin_addr);
        
        int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        
        if (result == 0)
        {
            printf("✓ OPEN - Connection successful!\n");
        }
        else
        {
            printf("✗ CLOSED - %s (errno=%d)\n", strerror(errno), errno);
        }
        
        close(sock);
    }
    
    printf("\n=== Test Complete ===\n");
    return 0;
}
