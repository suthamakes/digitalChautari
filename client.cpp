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
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return -1;
    }
    else
    {
        cout << "Socket Created Successfully\n";
    }

    // Set up the address family
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(customIP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error connecting to the server\n";
        close(clientSocket);
        return -1;
    }
    else
    {
        std::cout << "Connected to the server at " << customIP << ":" << ntohs(serverAddress.sin_port) << "\n";
    }
    // ... rest of the code (send/receive data)

    // Close the socket when done
    close(clientSocket);

    return 0;
}
