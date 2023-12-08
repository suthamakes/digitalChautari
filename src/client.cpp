#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define servPORT 8080

using namespace std;

int initializeSocket(const char *servIP)
{
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        cerr << "[SYSTEM] Error creating socket\n";
        return -1;
    }
    else
    {
        cout << "[SYSTEM] Socket Created Successfully\n";
    }

    // Set up the address family
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(servPORT);
    serverAddress.sin_addr.s_addr = inet_addr(servIP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "[SYSTEM] Error connecting to the server\n";
        close(clientSocket);
        return -1;
    }
    else
    {
        cout << "[SYSTEM] Connected to the server at " << servIP << ":" << ntohs(serverAddress.sin_port) << "\n";
    }

    return clientSocket;
}

void sendMessageLoop(int clientSocket)
{
    while (true)
    {
        char buff[400];

        cout << "Enter your message: ";
        cin.getline(buff, 400);

        size_t msgLen = strlen(buff);

        int msg = send(clientSocket, &msgLen, sizeof(msgLen), 0);

        if (msg == -1)
        {
            cerr << "[SYSTEM] Error sending message\n";
            exit(-1);
        }

        msg = send(clientSocket, buff, msgLen, 0);

        if (msg == -1)
        {
            cerr << "[SYSTEM] Error sending message data \n";
            exit(-1);
        }
        else
        {
            cout << "sent:  " << buff << '\n';
        }
    }
}

int main()
{
    const char *servIP = "127.0.0.1";

    // Initialize socket and connect to the server
    int clientSocket = initializeSocket(servIP);

    if (clientSocket != -1)
    {
        // Enter the message sending loop
        sendMessageLoop(clientSocket);

        // Close the socket when the loop ends
        close(clientSocket);
    }

    return 0;
}
