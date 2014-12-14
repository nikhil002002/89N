/*
 * ServerFnSupport.C
 * Definitions of Support Functions Required in the Server Process
 *  Created on: Dec 07, 2014
 *      Author: Pranav Sarda and Nikhil Rajendran
 */

#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <netdb.h>
#include <string.h>

#include "IpDomainDossierHeaders.h"


void ProcessTCPClient(int clientSocket) {
  char buffer[BUFSIZE]; // Buffer for storing incoming data

  // Receive transmitted message from client
  ssize_t numBytesRcvd = recv(clientSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithErrorMessage("Server Receive failed","");

  processCommand(buffer,numBytesRcvd);

//  // Send received string and receive again until end of stream
//  while (numBytesRcvd > 0) { // 0 indicates end of stream
//    // Echo message back to client
//    ssize_t numBytesSent = send(clientSocket, buffer, numBytesRcvd, 0);
//
//    if (numBytesSent < 0)
//      DieWithErrorMessage("send() failed","");
//    else if (numBytesSent != numBytesRcvd)
//    	DieWithErrorMessage("send()", "sent unexpected number of bytes");

    // See if there is more data to receive
//    numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
//    if (numBytesRcvd < 0)
//      DieWithSystemMessage("recv() failed");
//  }

  close(clientSocket); // Close client socket
}

void processCommand(const char *dataBuffer, const int dataLength)
{
	//char *chDataBufferptr=dataBuffer;

	int command=atoi(dataBuffer);
	int length=dataLength-2;
	switch (command)
	{
				case 1:
				{
					char **param=processData(dataBuffer+2,length,1);
					domainIP(param[0]);
					break;
				}
				case 2:
					processData(dataBuffer+2,length,2);
					break;
				case 3:
					processData(dataBuffer+2,length,3);
					break;
				case 4:
					//TODO call mrr
					break;
				case 5:
					//TODO call lrr
					break;
				case 6:
					//TODO Call End Function
					printf("End");
					exit(0);
					break;
				default:
					break;
	}
	int bufferLen=dataLength-1;
	const char *endlength;
	for(;(*(dataBuffer+1))!= 32 && --bufferLen>=0;)
	{
		endlength=dataBuffer+1;
	}
}

char **processData(const char *dataBuffer, const int dataLength,int command)
{
	const char* endOfDataBuf=dataBuffer+dataLength;
	char** maPtrData;
	if(command==1 || command==3)
	{
		//Initializing Pointer to an array of String Pointers
		if((maPtrData=malloc(1*sizeof(char*)))==NULL)
			DieWithErrorMessage("Memory allocation Failed for 1st parameter","");
		maPtrData[0]=(char *)dataBuffer;
	}else
	{
		if((maPtrData=malloc(1*sizeof(char*)))==NULL)
			DieWithErrorMessage("Memory allocation Failed for 1st parameter","");
		int counter =0;
		maPtrData[counter]=(char *)dataBuffer;
		dataBuffer++;

		while(dataBuffer!=endOfDataBuf)
		{
			counter++;
			if((maPtrData= realloc(maPtrData,1*sizeof(char*)))==NULL)
				DieWithErrorMessage("Memory allocation Failed for additional parameter","");
			maPtrData[counter]=(char *)*dataBuffer;
			dataBuffer++;
		}
	}
	return maPtrData;
}
void DieWithErrorMessage(const char *msg, const char *detail) {
  perror(msg);
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

//void DieWithSystemMessage(const char *msg) {
//  perror(msg);
//  exit(1);
//}

