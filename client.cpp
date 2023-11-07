#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define servPORT 8080

using namespace std;

int main()
{

      const char *servIP = "127.0.0.1";

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
                  return -1;
            }
            msg = send(clientSocket, buff, msgLen, 0);

            if (msg == -1)
            {
                  cerr << "[SYSTEM] Error sending message data \n";
                  return -1;
            }

            else
            {
                  cout << "sent:  " << buff << '\n';
            }
      }

      close(clientSocket);

      return 0;
}
