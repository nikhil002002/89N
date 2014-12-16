/*
 * ServerFnSupport.C
 * Definitions of Support Functions Required in the Server Process
 *  Created on: Dec 07, 2014
 *      Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "IpDomainDossierHeaders.h"

char *mssg;

//Process Input Command and send message to client
struct database *ProcessTCPClient(int clientSocket,struct database *fRecord) {

  dbLstPtr=fRecord;
  char buffer[BUFSIZE]; // Buffer for storing incoming data
  char *toSendBuffer;
  // Receive transmitted message from client
  ssize_t numBytesRcvd = recv(clientSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
  {
	toSendBuffer=malloc(20);
    toSendBuffer="Server Receive failed..Try again";
    errorLogger("Server Receive failed");
  }
  else
  {
	  toSendBuffer= processCommand(buffer,numBytesRcvd,fRecord);
  }
  ssize_t sendBufferLength=strlen(toSendBuffer)+1;

  //Send Message to client
    ssize_t numBytesSent = send(clientSocket, toSendBuffer,sendBufferLength , 0);
    if (numBytesSent < 0)
    {
      if(dbLstPtr!=NULL)
		free(dbLstPtr);
      if(toSendBuffer!=NULL)
      		free(toSendBuffer);
      DieWithErrorMessage("send() failed",NULL);
    }
     else if (numBytesSent != sendBufferLength)
     {
         if(dbLstPtr!=NULL)
   		free(dbLstPtr);
         if(toSendBuffer!=NULL)
         		free(toSendBuffer);
    	DieWithErrorMessage("send() failed", "sent unexpected number of bytes");
     }
  free(toSendBuffer);	//frees allocated mssg ptr in sendBuffer()
  close(clientSocket); // Close client socket
  return dbLstPtr;
}

//transmit buffer contents
void sendBuffer(int clientSocket,char *sendBuf, ssize_t bufLen)
{
	ssize_t numBytesSent = send(clientSocket, sendBuf,bufLen , 0);
	if (numBytesSent < 0)
	  DieWithErrorMessage("send() failed",NULL);
	else if (numBytesSent != bufLen)
		DieWithErrorMessage("send()", "sent unexpected number of bytes");
	close(clientSocket);
}

//Recognize client command and process accordingly
char* processCommand(const char *dataBuffer, const int dataLength,struct database *fRecord)
{

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
			}
			break;
		case 3:	//Delete Record
			{
				dbLstPtr=deleteRecord(fRecord,dataBuffer+2,mssg);
				return mssg;
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
				eventLogger("Server Shut Down Request from Client");
				char *sysmssg;
				sysmssg= writeFile(fRecord,fileNamePtr);
				eventLogger(sysmssg);
				exit(0);
			}
			strcpy(mssg,"Unauthorized Command");
			return mssg;
			break;
		}
		default:
		{
			strcpy(mssg,"Command not recognized");
			eventLogger("Unauthorized Shut Down Request");
			return mssg;
		}
			break;
	}
	strcpy(mssg,"Command not processed");
	return mssg;
}


//Get Domain and IP details from Input command
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


//User defined Error logging
void DieWithErrorMessage(const char *msg, const char *detail) {

  fputs(msg, stderr);
  fputs(": ", stderr);
  if(detail!=NULL)
   {
	  fputs(detail, stderr);
   }
  fputc('\n', stderr);
  errorLogger(msg);
  eventLogger(msg);
  if(detail!=NULL)
  {
	  errorLogger(detail);
	  eventLogger(detail);
  }
  exit(1);
}
