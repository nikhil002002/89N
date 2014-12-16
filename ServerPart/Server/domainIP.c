/*
 * domainIP.C
 * To find the IP address of domain from Database or Web
 *  Created on: Dec 08, 2014
 *      Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include "netdb.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include "IpDomainDossierHeaders.h"

// function to find IP address of a domain - returns a pointer to first node in the linked list database
// parameter 1 - pointer to first node in the linked list
// parameter 2 - pointer to domain name whose IP address is requested
// parameter 3 - pointer to message about the return value
struct database *domainIP(struct database *fRecord, char *webNamep, char *message)
{
	dbLstPtr=fRecord;
	char** ipList;

	struct database *currentRecord, *webRecord;
	char domName[20],webName[20];
	strncpy(webName, webNamep, sizeof(webName));
	int index = 0;

	// while loop to convert domain name to upper case for case insensitive comparison
	while(webName[index] != '\0')
	{
		webName[index] = toupper(webName[index]);
		index++;
	}

	// while loop to check if domain name is already present
	currentRecord = fRecord;
	while(currentRecord != NULL)
	{
		index = 0;
		strcpy(domName, currentRecord->domainName);
		while(domName[index] != '\0')
		{
			domName[index] = toupper(domName[index]);
			index++;
		}
		if(strcmp(domName, webName) == 0)
		{
			break;
		}
		currentRecord = currentRecord->nextRecord;
	}

	// if to return IP addresses of already present domain name
	if(currentRecord != NULL)
	{
		index = 0;
		ipList=calloc(10, sizeof(char*));

		// while loop condition to check '\0' in the next row after valid IP address rows
		// index < 10 as max number of IP addresses is assumed to be 10
		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
		{
			ipList[index]=currentRecord->ipAddrs[index];
			index++;
		}

		// to increment number of requests for the IP address of the domain by 1
		currentRecord->numTimes++;

		char *eventmssg = malloc(100);
		sprintf(eventmssg,"IP for domain %s requested", webNamep);
		eventLogger(eventmssg);
		free(eventmssg);
	}

	// else to get the IP address of the domain from the web
	else
	{
		// function call to function defined to get the IP from the Web
		ipList = findIPfromDomainName(webNamep);

		webRecord = (struct database *)malloc(sizeof(struct database));
		if(webRecord == NULL)
		{
			strcpy(message, "Database is full. The record to be added after searching IP from the web could not be added.");
			eventLogger(message);
			return NULL;
		}

		// to initialize number of requests for the IP address of the domain by 1
		webRecord->numTimes = 1;
		strcpy(webRecord->domainName, webNamep);
		index = 0;
		char** tempIpList = ipList;

		// index < 10 as max number of IP addresses is assumed to be 10
		while((*(tempIpList) != NULL) && (index < 10))
		{
			strcpy(webRecord->ipAddrs[index], ipList[index]);
			index++;
			tempIpList++;
		}

		// assigning '\0' to the next row after valid IP address rows for easy detection of total valid IP address rows
		if(index < 9)
		{
			strcpy(webRecord->ipAddrs[index], "\0");
		}
		webRecord->nextRecord = fRecord;
		fRecord = webRecord;

		char *eventmssg=malloc(100);
		sprintf(eventmssg, "IP for domain %s added.", webNamep);
		eventLogger(eventmssg);
		free(eventmssg);
	}

	int ipcount = 0;
	char** tempIpList;

	// to get the count of IP addresses
	for(tempIpList = ipList; *(tempIpList) != NULL; tempIpList++)
	{
		ipcount++;
	}

	int lenBuff = strlen(webNamep) + (ipcount*16) + 50;
	char *toSendBuffer = calloc(lenBuff, sizeof(char));

	strcat(toSendBuffer, "Requested Domain :");
	strcat(toSendBuffer, webNamep);
	strcat(toSendBuffer, " IP Add: ");

	// to concatenate IP addresses for the domain
	for(tempIpList = ipList; *(tempIpList) != NULL; tempIpList++)
	{
		strcat(toSendBuffer,ipList[--ipcount]);
		strcat(toSendBuffer," ");
	}

	free(ipList);
	strcpy(message, toSendBuffer);

	char *eventmssg = malloc(150);
	sprintf(eventmssg, "Record added/requested: %s ", message);
	eventLogger(eventmssg);
	free(eventmssg);
	return fRecord;
}

// function to get the IP address of a domain from the web
// parameter 1 - pointer to domain name whose IP address is requested
char **findIPfromDomainName(char *hostname)
{
	struct in_addr **addr_list;

	// Get IP from Domain Name
	struct hostent *lh = gethostbyname(hostname);

	char clntName[INET_ADDRSTRLEN];
	char** maIPList=NULL;
	int index=2;

	addr_list = (struct in_addr **)lh->h_addr_list;
	char **test=lh->h_addr_list;

	int i=0;

	//Create Array of string pointers containing retrieved IP addresses
	    for(i = 0; addr_list[i] != NULL; i++) {
	    	  if (inet_ntop(PF_INET, test[i], clntName, sizeof(clntName)) != NULL)
			  {
				  if((maIPList=realloc(maIPList,index*sizeof(char*)))==NULL)
				  {
				    if(dbLstPtr!=NULL)
						free(dbLstPtr);
					free(maIPList);
					DieWithErrorMessage("Memory allocation Failed for IPs",NULL);
				  }

				  int len;
				  len = strlen(clntName);
				  if((maIPList[index-2] = (char *)malloc(len + 1))==NULL)
				  {
				    if(dbLstPtr!=NULL)
						free(dbLstPtr);
					free(maIPList);
					DieWithErrorMessage("Memory allocation Failed for IPs",NULL);
				  }
				  strcpy(maIPList[index-2], clntName);

				  maIPList[index-1]= NULL;
				  index++;
			  }
	    	printf("%s ", inet_ntoa(*addr_list[i]));
	    }
	  return maIPList;	//Return List of IP
}
