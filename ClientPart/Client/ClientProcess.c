/*
 * ClientProcess.c
 *
 *  Created on: Dec 11, 2014
 *      Author: nikhil
 */

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include "ClientHeader.h"

int main(int argc, char *argv[]) {

  if (argc < 4) // Test for correct number of arguments
    DieWithErrorMessage("Parameter(s)","<Server Address> <Port> <command> <arg>");

  int command =atoi(argv[3]);

  if((command==1||command==3||command==6)&&argc!=5)
	  DieWithErrorMessage("Insufficient Parameter(s)","<Server Address> <Port> <command> <arg>");
  if((command==2 && argc<6))
	  DieWithErrorMessage("Insufficient Parameter(s)","<Server Address> <Port> <command> <arg>");

  char *toSendBuffer=NULL;
  int lenArgOneAndTwo;
  if(command==4||command==5)
	  toSendBuffer=argv[3];
  else
  {
	  lenArgOneAndTwo= strlen(argv[3])+strlen(argv[4])+2;
	  if((toSendBuffer=realloc(toSendBuffer,lenArgOneAndTwo))==NULL)
		  DieWithErrorMessage("Insufficient Memory","");
	  strcpy(toSendBuffer,argv[3]);
	  char *tst=toSendBuffer+strlen(argv[3])+1;
	  strcpy(tst,argv[4]);

	  if(command==2)
	  {
		  int index;
		  for(index=5;index<argc;index++)
		  {
			  if(validateString(argv[index])==1)
			  {
				  lenArgOneAndTwo+=(strlen(argv[index])+1);
				  if((toSendBuffer=realloc(toSendBuffer,lenArgOneAndTwo))==NULL)
						  DieWithErrorMessage("Insufficient Memory","");
				  tst+=strlen(argv[index-1])+2;
				  strcpy(tst,argv[index]);
			  }
		  }
	  }

  }


  char *serverIP = argv[1];     // Server address/name
  //int serverPort = atoi(argv[2]); // Server Port number

// Create a  TCP socket
  int clientSock = SetupTCPClientSocket(serverIP, argv[2]);
  if (clientSock < 0)
    DieWithErrorMessage("SetupTCPClientSocket() failed", "unable to connect");

  //size_t bufStringLen = strlen(toSendBuffer); // Determine input length
  size_t bufStringLen = lenArgOneAndTwo; // Determine input length

  // Send the string to the server
  ssize_t sentBytes = send(clientSock, toSendBuffer, bufStringLen, 0);

  if (sentBytes < 0)
    DieWithErrorMessage("send() failed","");
  else if (sentBytes != bufStringLen)
    DieWithErrorMessage("send()", "Message not transmitted Completely");


  // Receive from the server
  int receivedBytes = 0; // Count of total bytes received
  fputs("Received: ", stdout);     // Setup to print the echoed string
  char buffer[BUFSIZE]; // I/O buffer
  // Receive up to the buffer size (minus 1 to leave space for
  // a null terminator) bytes from the sender
  receivedBytes = recv(clientSock, buffer, BUFSIZE - 1, 0);
    if (receivedBytes < 0)
      DieWithErrorMessage("recv() failed","");
    else if (receivedBytes == 0)
      DieWithErrorMessage("recv()", "connection closed prematurely");
    //buffer[receivedBytes] = '\0';    // Terminate the string!
    fputs(buffer, stdout);      // Print the buffer

  fputc('\n', stdout); // Print a final linefeed

  close(clientSock);
  exit(0);
}


