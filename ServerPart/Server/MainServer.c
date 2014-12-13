#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "IpDomainDossierHeaders.h"

static const int MAXPENDINGREQ = 5; // Maximum outstanding connection requests allowed on server

//Accepts Port Number, File Name and Accepted Time gap.
int main(int argc, char *argv[]) {

  if (argc != 4) // Test for correct number of arguments else Exit
	  DieWithErrorMessage("Parameter(s)", "<Server Port> <File Name> <Time Gap>");

  in_port_t serverPortNo = atoi(argv[1]); // First argument:  local port

  char *fileNamePtr=argv[2];	//Second Argument: File Name

  int timeGap= atoi(argv[3]); //Third Argument: TIME Gap


  // Create Server Socket
  int servSock; // Socket descriptor for server
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithErrorMessage("Server Socket() creation failed","");

  // Construct local address structure
  struct sockaddr_in serverAddr;                  // Local address
  memset(&serverAddr, 0, sizeof(serverAddr));     // Initialize Structure to Zero
  serverAddr.sin_family = AF_INET;                // Set Address Family Type
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen to any incoming interface
  serverAddr.sin_port = htons(serverPortNo);      // Server port

  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0)
    DieWithErrorMessage("Server bind() operation failed");

  // Listen the socket for incoming connections
  if (listen(servSock, MAXPENDINGREQ) < 0)
	DieWithErrorMessage("Server listen() operation failed");

  for (;;) {

	// Start Server
    struct sockaddr_in clientAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clientAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clientAddr, &clntAddrLen);

    if (clntSock < 0)
      DieWithErrorMessage("Server accept() operation failed");


    char clntName[INET_ADDRSTRLEN]; // String to contain client address in Dotted Decimal
    if (inet_ntop(PF_INET, &clientAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
      printf("Receiving from Client IP %s/ Port: %d\n", clntName, ntohs(clientAddr.sin_port));
    else
      perror("Unable to Retrieve Client address");
      //TODO Continue loop to listen again

    ProcessTCPClient(clntSock);

  }

}
