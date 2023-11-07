#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

using namespace std;

int main()
{

      const char *servIP = "127.0.0.1";

      // Create a socket
      int listening = socket(AF_INET, SOCK_STREAM, 0);

      if (listening == -1)
      {
            cerr << "[SYSTEM] Error creating socket\n";
            return -1;
      }
      else
      {
            cout << "[SYSTEM] Socket Created Successfully\n";
      }

      // Bind the socket to a specific address and port
      sockaddr_in serverAddress;
      serverAddress.sin_family = AF_INET;
      serverAddress.sin_port = htons(PORT);
      serverAddress.sin_addr.s_addr = inet_addr(servIP);

      if (bind(listening, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
      {
            cerr << "[SYSTEM] Error binding socket\n";
            close(listening);
            return -1;
      }
      else
      {
            cout << "[SYSTEM] Successfully binded to local port\n";
      }

      if (listen(listening, SOMAXCONN) < 0)
      {
            cout << "[SYSTEM] Failed to start\n";
      }
      else
      {
            cout << "[SYSTEM] Started listening to local port\n";
      }

      while (true)
      {

            // accepting connection from the client
            sockaddr_in clientAddress;
            socklen_t clientAddrSize = sizeof(clientAddress);

            // Accept a connection from the client
            int clientSocket = accept(listening, (struct sockaddr *)&clientAddress, &clientAddrSize);

            if (clientSocket == -1)
            {
                  cerr << "[SYSTEM] Error accepting connection";
            }

            cout << "[SYSTEM] Accepted connection from "
                 << inet_ntoa(clientAddress.sin_addr) // Convert client IP to string
                 << ":" << ntohs(clientAddress.sin_port) << "\n";

            while (true)
            {
                  size_t messageLen;
                  int msg = recv(clientSocket, &messageLen, sizeof(messageLen), 0);

                  if (msg < 0)
                  {
                        cerr << "[SYSTEM] Error receiving message length\n";
                        break;
                  }
                  else if (msg == 0)
                  {
                        cout << "[SYSTEM] Client disconnected\n";
                        break;
                  }

                  char recBuff[messageLen];
                  msg = recv(clientSocket, recBuff, messageLen, 0);

                  if (msg > 0)
                  {
                        recBuff[msg] = '\0'; // Null-terminate the received data
                        cout << "Message received: " << recBuff << '\n';
                  }
                  else
                  {
                        cerr << "[SYSTEM] Error receiving message data\n";
                        break;
                  }
            }

            close(clientSocket);
      }
      close(listening);
      return 0;
}