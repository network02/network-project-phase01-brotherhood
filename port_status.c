#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string.h>

#define HOST argv[1]
// input example : [appname] 127.0.0.1 80 443 8080 8081
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <host> <port1> <port2> ... <portN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to initialize Winsock.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    int sockfd = SOCKET_ERROR;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(HOST);

    for (int i = 2; i < argc; i++)
    {
        int port = atoi(argv[i]);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == SOCKET_ERROR)
        {
            perror("Failed to create socket");
            WSACleanup();
            return EXIT_FAILURE;
        }

        server_addr.sin_port = htons(port);

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            printf("Port %d is closed\n", port);
        }
        else
        {
            printf("Port %d is open\n", port);
            struct servent *service = getservbyport(htons(port), "tcp");
            if (service != NULL)
                printf("Service: %s\n", service->s_name);
            else
                printf("Service: Unknown\n");
        }

        closesocket(sockfd);
    }

    WSACleanup();
    return 0;
}