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
#include <stdio.h>
#include "IpDomainDossierHeaders.h"

char *mssg;


struct database *ProcessTCPClient(int clientSocket,struct database *fRecord) {
  char buffer[BUFSIZE]; // Buffer for storing incoming data

  // Receive transmitted message from client
  ssize_t numBytesRcvd = recv(clientSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithErrorMessage("Server Receive failed","");

  char *toSendBuffer= processCommand(buffer,numBytesRcvd,fRecord);
  ssize_t sendBufferLength=strlen(toSendBuffer)+1;

    ssize_t numBytesSent = send(clientSocket, toSendBuffer,sendBufferLength , 0);
    if (numBytesSent < 0)
      DieWithErrorMessage("send() failed","");
    else if (numBytesSent != sendBufferLength)
    	DieWithErrorMessage("send()", "sent unexpected number of bytes");

  free(toSendBuffer);
  close(clientSocket); // Close client socket
  return dbLstPtr;
}

char* processCommand(const char *dataBuffer, const int dataLength,struct database *fRecord)
{
	//char *chDataBufferptr=dataBuffer;
	mssg= malloc(150);
	int command=atoi(dataBuffer);
	int length=dataLength-2;
	switch (command)
	{
		case 1:
		{
			char **param=processData(dataBuffer+2,length,1);
			dbLstPtr= domainIP(fRecord,param[0],mssg);
			return mssg;
		}
			break;
		case 2:	//Add Record
			{
				dbLstPtr = addRecord(fRecord,dataBuffer+2,mssg);
				return mssg;
				//processData(dataBuffer+2,length,2);
			}
			break;
		case 3:	//Delete Record
			{
				dbLstPtr=deleteRecord(fRecord,dataBuffer+2,mssg);
				return mssg;
				//processData(dataBuffer+2,length,3);
			}
			break;
		case 4:		//Most Requested Records
		{
			mostReqRecord(fRecord,mssg);
			return mssg;
			break;
		}
		case 5:		//Least Requested Records
		{
			leastReqRecord(fRecord,mssg);
			return mssg;
			break;
		}
		case 6:		//ShutDown Server
		{
			if(strcmp(SERVERSHUTDOWNCODE,dataBuffer+2)==0)
			{
				char *sysmssg= writeFile(fRecord,fileNamePtr);
				exit(0);
			}
			strcpy(mssg,"Unauthorized");
			return mssg;
			break;
		}
		default:
		{
			strcpy(mssg,"Command not recognized");
			return mssg;
		}
			break;
	}
	strcpy(mssg,"Command not processed");
	return mssg;
}

char **processData(const char *dataBufferp, const int dataLength,int command)
{
	const char *dataBuffer=dataBufferp;
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
			maPtrData[counter]=(char *)dataBuffer;
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

