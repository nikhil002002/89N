#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "IpDomainDossierHeaders.h"
#include <time.h>

static char lastReqIp[20]={0};
static time_t instance1, instance2;
static const int MAXPENDINGREQ = 5; // Maximum outstanding connection requests allowed on server
//static struct database *dbLstPtr;
//Accepts Port Number, File Name and Accepted Time gap.
int main(int argc, char *argv[]) {

	instance1=0;

  if (argc != 4) // Test for correct number of arguments else Exit
	  DieWithErrorMessage("Parameter(s)", "<Server Port> <File Name> <Time Gap>");

  in_port_t serverPortNo = atoi(argv[1]); // First argument:  local port

  fileNamePtr=argv[2];	//Second Argument: File Name

  double timeGap= (double)atoi(argv[3]); //Third Argument: TIME Gap

  char *mssg =malloc(100) ;								//TODO
  //Read DataBase

  if((dbLstPtr=readFile(fileNamePtr,mssg))==NULL)
  {
	  //report File not found Event;
  }


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
    DieWithErrorMessage("Server bind() operation failed","");

  // Listen from socket for incoming connections
  if (listen(servSock, MAXPENDINGREQ) < 0)
	DieWithErrorMessage("Server listen() operation failed","");

  for (;;) {

	// Start Server
    struct sockaddr_in clientAddr; // Client address
    // Set length of client address structure
    socklen_t clntAddrLen = sizeof(clientAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clientAddr, &clntAddrLen);

    if (clntSock < 0)
      DieWithErrorMessage("Server accept() operation failed","");

    //Record Time
    time(&instance2);
    double seconds = difftime(instance2, instance1);

    char clntName[INET_ADDRSTRLEN]; // String to contain client address in Dotted Decimal
    if (inet_ntop(PF_INET, &clientAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
    {
      printf("Receiving from Client IP %s/ Port: %d\n", clntName, ntohs(clientAddr.sin_port));
    }
    else
    {
      perror("Unable to Retrieve Client address");
      //TODO Continue loop to listen again
      continue;
    }

    if((strcmp(clntName,lastReqIp)==0) && seconds<timeGap)
    {
    	char msg[20];
    	sprintf(msg,"Please wait for %.1f seconds",timeGap);
    	ssize_t msglen=strlen(msg);
    	sendBuffer(clntSock,msg,msglen);
    	continue;
    }
    else
    {
    	strcpy(lastReqIp,clntName);
    	instance1=instance2;
    }
    //printf("%p\n", dbLstPtr);
    dbLstPtr= ProcessTCPClient(clntSock,dbLstPtr);

  }

}
