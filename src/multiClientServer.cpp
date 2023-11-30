#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

using namespace std;

constexpr int MAX_CLIENTS = 10; // Adjust the maximum number of clients as needed

int main()
{
    fd_set master_fds, readfds;
    const char *servIP = "127.0.0.1";
    FD_ZERO(&master_fds);

    // Create a TCP socket and add it to the set
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1)
    {
        cerr << "[SYSTEM] Error creating socket\n";
        return -1;
    }
    else
    {
        cout << "[SYSTEM] Socket Created Successfully\n";
    }
    FD_SET(listen_socket, &master_fds);

    // Set the socket to the listening state
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(servIP);
    server_address.sin_port = htons(8080);

    if (bind(listen_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "[SYSTEM] Error binding socket\n";
        close(listen_socket);
        return -1;
    }
    else
    {
        cout << "[SYSTEM] Successfully binded to local port\n";
    }

    if (listen(listen_socket, SOMAXCONN) < 0)
    {
        cerr << "[SYSTEM] Failed to start\n";
        return -1;
    }
    else
    {
        cout << "[SYSTEM] Started listening to local port\n";
    }

    int client_sockets[MAX_CLIENTS];
    memset(client_sockets, 0, sizeof(client_sockets));

    while (true)
    {
        readfds = master_fds;

        int max_fd = listen_socket;
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            int client_socket = client_sockets[i];
            if (client_socket > 0)
            {
                FD_SET(client_socket, &readfds);
                if (client_socket > max_fd)
                {
                    max_fd = client_socket;
                }
            }
        }

        int readySock = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (readySock == -1)
        {
            cerr << "[SYSTEM] Error\n";
        }
        else
        {
            // Check the state of each socket in the set
            if (FD_ISSET(listen_socket, &readfds))
            {
                cout << "[SYSTEM] The listening socket is ready to accept a connection.\n";
                sockaddr_in clientAddress;
                socklen_t clientAddrSize = sizeof(clientAddress);
                int client_socket = accept(listen_socket, (struct sockaddr *)&clientAddress, &clientAddrSize);

                if (client_socket > 0)
                {
                    cout << "[SYSTEM] Accepted connection from "
                         << inet_ntoa(clientAddress.sin_addr) // Convert client IP to string
                         << ":" << ntohs(clientAddress.sin_port) << "\n";

                    // Add the new client socket to the set
                    for (int i = 0; i < MAX_CLIENTS; ++i)
                    {
                        if (client_sockets[i] == 0)
                        {
                            client_sockets[i] = client_socket;
                            break;
                        }
                    }
                }
                else
                {
                    cerr << "[SYSTEM] Error accepting connection\n";
                }
            }

            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                int client_socket = client_sockets[i];
                if (client_socket > 0 && FD_ISSET(client_socket, &readfds))
                {
                    // Client socket is ready for reading

                    // First, receive the message length
                    size_t messageLen;
                    int lenBytesRead = recv(client_socket, &messageLen, sizeof(messageLen), 0);

                    if (lenBytesRead <= 0)
                    {
                        // Handle disconnect or error
                        close(client_socket);
                        client_sockets[i] = 0;
                        printf("[SYSTEM] Socket %d disconnected.\n", client_socket);
                    }
                    else
                    {
                        // Now, receive the actual message based on the received length
                        char buffer[1024];
                        int bytesRead = recv(client_socket, buffer, messageLen, 0);

                        if (bytesRead <= 0)
                        {
                            // Handle disconnect or error
                            close(client_socket);
                            client_sockets[i] = 0;
                            printf("[SYSTEM] Socket %d disconnected.\n", client_socket);
                        }
                        else
                        {
                            // Process received data
                            buffer[bytesRead] = '\0'; // Null-terminate the received data
                            cout << "[SYSTEM] Received data from socket " << client_socket << ": " << buffer << endl;
                        }
                    }
                }
            }
        }
    }

    // Close the sockets and clean up
    close(listen_socket);

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (client_sockets[i] > 0)
        {
            close(client_sockets[i]);
        }
    }
}
