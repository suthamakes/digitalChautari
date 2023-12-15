#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>

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
    fd_set readFds;
    FD_ZERO(&readFds);
    FD_SET(clientSocket, &readFds);
    FD_SET(STDIN_FILENO, &readFds);

    while (true)
    {
        fd_set tmpReadFds = readFds;
        int maxFd = max(clientSocket, STDIN_FILENO);

        // Use select to monitor file descriptors for readiness
        int readySock = select(maxFd + 1, &tmpReadFds, NULL, NULL, NULL);

        if (readySock == -1)
        {
            cerr << "[SYSTEM] Error in select\n";
            break;
        }
        else if (FD_ISSET(clientSocket, &tmpReadFds))
        {
            // Data available from the server
            size_t messageLen;
            int lenBytesRead = recv(clientSocket, &messageLen, sizeof(messageLen), 0);

            if (lenBytesRead <= 0)
            {
                cerr << "[SYSTEM] Server disconnected.\n";
                break;
            }
            else
            {
                char buffer[1024];
                int bytesRead = recv(clientSocket, buffer, messageLen, 0);

                if (bytesRead <= 0)
                {
                    cerr << "[SYSTEM] Server disconnected.\n";
                    break;
                }
                else
                {
                    buffer[bytesRead] = '\0';
                    cout << "[SYSTEM] Received broadcast from server: " << buffer << endl;
                }
            }
        }
        else if (FD_ISSET(STDIN_FILENO, &tmpReadFds))
        {
            // User input available
            char buff[400];
            cin.getline(buff, 400);

            size_t msgLen = strlen(buff);

            int msg = send(clientSocket, &msgLen, sizeof(msgLen), 0);

            if (msg == -1)
            {
                cerr << "[SYSTEM] Error sending message\n";
                break;
            }

            msg = send(clientSocket, buff, msgLen, 0);

            if (msg == -1)
            {
                cerr << "[SYSTEM] Error sending message data \n";
                break;
            }
            else
            {
                cout << "[SYSTEM] Sent: " << buff << '\n';
            }
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
