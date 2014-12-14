/*
 * ClientFunctions.c
 *
 *  Created on: Dec 14, 2014
 *      Author: nikhil
 */
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include "ClientHeader.h"

int SetupTCPClientSocket(const char *serverIP, const char *serverPort)
{

	  in_port_t servPort = atoi(serverPort);
	  int clientSock = -1;
	  // Create a reliable, stream socket using TCP
	  clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	  if (clientSock < 0)
	    DieWithErrorMessage("Client Socket() creation failed","");

	  // Construct the server address structure
	  struct sockaddr_in serverAddr;            // Server address
	  memset(&serverAddr, 0, sizeof(serverAddr)); // Zero out structure
	  serverAddr.sin_family = AF_INET;          // IPv4 address family
	  // Convert address
	  int rtnVal = inet_pton(AF_INET, serverIP, &serverAddr.sin_addr.s_addr);
	  if (rtnVal == 0)
	    DieWithErrorMessage("inet_pton() failed", "invalid address string");
	  else if (rtnVal < 0)
	    DieWithErrorMessage("inet_pton() failed-","");
	  serverAddr.sin_port = htons(servPort);    // Server port

	  // Establish the connection to the DNS Dossier server
	  if (connect(clientSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
          return -1;
	  else
		  return clientSock;
  }


void DieWithErrorMessage(const char *msg, const char *detail) {
  perror(msg);
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

int validateString(char *toValidateString)
{
	int field1,field2,field3,field4;
	sscanf(toValidateString,"%d.%d.%d.%d",&field1,&field2,&field3,&field4);
	if(field1<255 && field1>0)
	{
		if(field2<255 && field2>0)
		{
			if(field3<255 && field3>0)
			{
				if(field4<255 && field4>0)
				{
					return 1;
				}
				else
					DieWithErrorMessage("Invalid IP","field4");
			}
			else
				DieWithErrorMessage("Invalid IP","field3");
		}
		else
			DieWithErrorMessage("Invalid IP","field2");
	}
	else
		DieWithErrorMessage("Invalid IP","field1");
	return 0;
}

