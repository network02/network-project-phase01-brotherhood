#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int main(int argc, char const *argv[])
{
    struct WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Initializing windows socket failed.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buffer[256] = {0};
    char response[512] = {0};
    SOCKET sockfd = -1;
    while (1)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            printf("Socket creation failed.\n");
            return EXIT_FAILURE;
        }
        
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            printf("Connection failed.\n");
            return EXIT_FAILURE;
        }

        printf("Enter the command: ");
        gets(buffer);
        send(sockfd, buffer, strlen(buffer), 0);
        recv(sockfd, response, 512, 0);
        printf("Response: \n%s\n", response);
        memset(buffer, 0, 256);
        memset(response, 0, 512);

        closesocket(sockfd);
    }

    return 0;
}
