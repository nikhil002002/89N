/*
 * ClientHeader.h
 *
 *  Created on: Dec 13, 2014
 *      Author: nikhil
 */

#ifndef CLIENTHEADER_H_
#define CLIENTHEADER_H_
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

// Handle error with user msg
void DieWithErrorMessage(const char *msg, const char *detail);
// Handle error with sys msg
void DieWithSystemMessage(const char *msg);
// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service);
// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock);
// Handle new TCP client
void HandleTCPClient(int clntSocket);
// Create and connect a new TCP client socket
int SetupTCPClientSocket(const char *server, const char *service);

int validateString(char *toValidateString);

enum sizeConstants {
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 512,
};



#endif /* CLIENTHEADER_H_ */
