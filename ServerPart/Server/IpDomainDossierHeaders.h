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

// Function to Handle errors with custom Message
void DieWithErrorMessage(const char *msg, const char *detail);
// Handle error with sys msg
//void DieWithSystemMessage(const char *msg);
// Print socket address

void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
// Test socket address equality
bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);
// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service);
// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock);
// Handle new TCP client
void HandleTCPClient(int clntSocket);
// Create and connect a new TCP client socket
int SetupTCPClientSocket(const char *server, const char *service);

char** domainIP(char *domainName);

void ProcessTCPClient(int clientSocket);
char **findIPfromDomainName(char *hostname);
char* processCommand(const char *dataBuffer, const int dataLength);
char **processData(const char *dataBuffer, const int dataLength,int command);

enum sizeConstants {
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 512,
};

struct database
{
	char domainName[20];
	int numTimes;
	char ipAddrs[10][16];
	struct database *nextRecord;
};
#endif /* IPDOMAINDOSSIERHEADERS_H_ */
