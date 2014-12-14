/*
 * IpDomainDossierHeaders.h
 * Declarations of Support functions required for Server Process
 *  Created on: Dec 07, 2014
 *      Author: Nikhil Rajendran and Pranav Sarda
 */

//#pragma once
#ifndef IPDOMAINDOSSIERHEADERS_H_
#define IPDOMAINDOSSIERHEADERS_H_
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

struct database
{
	char domainName[20];
	int numTimes;
	char ipAddrs[10][16];
	struct database *nextRecord;
};

# define SERVERSHUTDOWNCODE "2A670"
// Function to Handle errors with custom Message
void DieWithErrorMessage(const char *msg, const char *detail);
// Handle error with sys msg
//void DieWithSystemMessage(const char *msg);


//char** domainIP(char *domainName);
char **domainIP(struct database *fRecord, char *webName);
void ProcessTCPClient(int clientSocket,struct database *fRecord);
char **findIPfromDomainName(char *hostname);
char* processCommand(const char *dataBuffer, const int dataLength,struct database *fRecord);
char **processData(const char *dataBuffer, const int dataLength,int command);
struct database *readFile(char *fName, char *message);
struct database *deleteRecord(struct database *fRecord, const char *webName, char *message);
struct database *addRecord(struct database *fRecord, const char *newData, char *message);
struct database *mostReqRecord(struct database *fRecord, char *message);
struct database *leastReqRecord(struct database *fRecord, char *message);
char *writeFile(struct database *fRecord, char *fName);

char *fileNamePtr;


enum sizeConstants {
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 512,
};


#endif /* IPDOMAINDOSSIERHEADERS_H_ */
