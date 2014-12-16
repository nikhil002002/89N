/*
 * ClientProcess.c
 * The entire client process - the main program at the Client
 *  Created on: Dec 11, 2014
 *      Authors: Nikhil Rajendran and Pranav Sarda
 */

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include "ClientHeader.h"

// Accepts various parameters depending on the request for the server
int main(int argc, char *argv[]) {

  if (argc < 4) // Test for correct number of arguments
    DieWithErrorMessage("Parameter(s)","<Server Address> <Port> <command> <arg>");

  int command =atoi(argv[3]);

  if((command==1||command==3||command==6)&&argc!=5)
	  DieWithErrorMessage("Insufficient Parameter(s)","<Server Address> <Port> <command> <arg>");
  if((command==2 && argc!=6))
	  DieWithErrorMessage("Insufficient Parameter(s)","<Server Address> <Port> <command> <arg>");

  char *toSendBuffer=NULL;
  int lenArgOneAndTwo;
  if(command==4||command==5)
  {
	  toSendBuffer=argv[3];
	  lenArgOneAndTwo=strlen(argv[3])+1;
  }
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
		  if(validateString(argv[5])==1)
		  {
			  lenArgOneAndTwo+=(strlen(argv[5])+1);
			  if((toSendBuffer=realloc(toSendBuffer,lenArgOneAndTwo))==NULL)
					  DieWithErrorMessage("Insufficient Memory","");
			  tst+=strlen(argv[4])+1;
			  strcpy(tst,argv[5]);
		  }
	  }
   }


  char *serverIP = argv[1];     // Server address/name

// Create a  TCP socket
  int clientSock = SetupTCPClientSocket(serverIP, argv[2]);
  if (clientSock < 0)
    DieWithErrorMessage("SetupTCPClientSocket() failed", "unable to connect");

  size_t bufStringLen = lenArgOneAndTwo; // Determine input length

  // Send the string to the server
  ssize_t sentBytes = send(clientSock, toSendBuffer, bufStringLen, 0);

  if (sentBytes < 0)
    DieWithErrorMessage("send() failed","");
  else if (sentBytes != bufStringLen)
    DieWithErrorMessage("send()", "Message not transmitted Completely");

  // Receive from the server
  int receivedBytes = 0; // Count of total bytes received
  fputs("Received: ", stdout);
  char buffer[BUFSIZE]; // I/O buffer
  // Receive up to the buffer size (minus 1 to leave space for
  // a null terminator) bytes from the sender
  receivedBytes = recv(clientSock, buffer, BUFSIZE - 1, 0);
    if (receivedBytes < 0)
      DieWithErrorMessage("recv() failed","");
    else if (receivedBytes == 0)
      DieWithErrorMessage("recv()", "connection closed prematurely");
    
    fputs(buffer, stdout);      // Print the buffer

  fputc('\n', stdout); // Print a final linefeed

  close(clientSock);
  exit(0);
}
