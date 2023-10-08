#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

using namespace std;

int main()
{

    const char *customIP = "127.0.0.1";

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        cerr << "Error creating socket\n";
        return -1;
    }
    else
    {
        cout << "Socket Created Successfully\n";
    }

    // Bind the socket to a specific address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(customIP);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }
    else
    {
        cout << "Successfully bind to local port\n";
    }

    if (listen(serverSocket, 1) < 0)
    {
        cout << "Failed to start\n";
    }
    else
    {
        cout << "Started listening to local port\n";
    }

    while (true)
    {

        // accepting connection from the client
        sockaddr_in clientAddress;
        socklen_t clientAddrSize = sizeof(clientAddress);

        // Accept a connection from the client
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddrSize);

        if (clientSocket == -1)
        {
            cerr << "Error accepting connection";
        }

        cout << "Accepted connection from "
             << inet_ntoa(clientAddress.sin_addr) // Convert client IP to string
             << ":" << ntohs(clientAddress.sin_port) << "\n";

        close(clientSocket);
    }
    close(serverSocket);
    return 0;
}