/*
 * domainIP.C
 * Find IP of Domain from Database or Web
 *  Created on: Dec 08, 2014
 *      Author: Pranav Sarda and Nikhil Rajendran
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


//char** domainIP(char *webName)
//char **domainIP(struct database *fRecord, char *webNamep)
struct database *domainIP(struct database *fRecord, char *webNamep, char *message)
{
	dbLstPtr=fRecord;
	char** ipList;

	struct database *currentRecord, *webRecord;
	char domName[20],webName[20]; //ipList[160];
	strncpy(webName,webNamep,sizeof(webName));
	int index = 0;
	while(webName[index] != '\0')
	{
		webName[index] = toupper(webName[index]);
		index++;
	}
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
	if(currentRecord != NULL)
	{
		index = 0;
		ipList=calloc(10,sizeof(char*));
		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))	//TODO : ask Index<10
		{
			ipList[index]=currentRecord->ipAddrs[index];
			index++;
		}
//		index = 0;
//		strcpy(ipList, currentRecord->ipAddrs[index]);
//		index++;
//		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
//		{
//			strcat(ipList, " ");
//			strcat(ipList, currentRecord->ipAddrs[index]);
//			index++;
//		}
		currentRecord->numTimes++;
//		strcpy(message, "The ip for the domain is returned.");
//		return &ipList[0];
//		return ipList;
		char *eventmssg=malloc(100);
		sprintf(eventmssg,"IP for domain %s requested",webNamep);
		eventLogger(eventmssg);
		free(eventmssg);
	}
	else
	{
		ipList=findIPfromDomainName(webNamep);

		webRecord = (struct database *)malloc(sizeof(struct database));
		if(webRecord == NULL)
		{
			strcpy(message, "Database is full. The record to be added after searching IP from the web could not be added.");
			eventLogger(message);
			return NULL;
		}
		webRecord->numTimes = 1;
		strcpy(webRecord->domainName, webNamep);
		index = 0;
		char** tempIpList=ipList;
		while((*(tempIpList) != NULL) && (index < 10))
		{
			strcpy(webRecord->ipAddrs[index], ipList[index]);
			index++;
			tempIpList++;
		}
		if(index < 9)
		{
			strcpy(webRecord->ipAddrs[index], "\0");
		}
		webRecord->nextRecord = fRecord;
		fRecord = webRecord;

		char *eventmssg=malloc(100);
		sprintf(eventmssg,"IP for domain %s added.",webNamep);
		eventLogger(eventmssg);
		free(eventmssg);
		//return ipList;
	}

	int ipcount=0;
	char** tempIpList;
	for(tempIpList=ipList;*(tempIpList)!=NULL;tempIpList++)
	{
		ipcount++;
	}

	int lenBuff=strlen(webNamep)+(ipcount*16)+50;
	char *toSendBuffer=calloc(lenBuff,sizeof(char));

	strcat(toSendBuffer,"Requested Domain :");
	strcat(toSendBuffer,webNamep);
	strcat(toSendBuffer," IP Add: ");

	for(tempIpList=ipList;*(tempIpList)!=NULL;tempIpList++)
	{
		strcat(toSendBuffer,ipList[--ipcount]);
		strcat(toSendBuffer," ");
	}


	free(ipList);
	strcpy(message, toSendBuffer);

	char *eventmssg=malloc(150);
	sprintf(eventmssg,"Record added/requested: %s ",message);
	eventLogger(eventmssg);
	free(eventmssg);
	return fRecord;
}

char **findIPfromDomainName(char *hostname)
{
//	 // Tell the system what kind(s) of address info we want
//	  struct addrinfo addrCriteria;                   // Criteria for address match
//	  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
//	  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
//	  addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
//	  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol
//
//	  // Get address(es) associated with the specified name/service
//	  struct addrinfo *addrList; // Holder for list of addresses returned
//	  // Modify servAddr contents to reference linked list of addresses
//	  int rtnVal = getaddrinfo(hostname, NULL, &addrCriteria, &addrList);
//	  if (rtnVal != 0)
//	    DieWithErrorMessage("getaddrinfo() failed", gai_strerror(rtnVal));
//
//	  // returned addresses
//	  char clntName[INET_ADDRSTRLEN];
//	  char** maIPList=NULL;
////	  if((maIPList=malloc(1*sizeof(char*)))==NULL)
////	  	DieWithErrorMessage("Memory allocation Failed for 1st IP","");
//	  int index=2;
//
//	  struct addrinfo *addr;
//
//	  for ( addr= addrList; addr != NULL; addr = addr->ai_next) {
//		  if (inet_ntop(PF_INET, addr->ai_addr, clntName, sizeof(clntName)) != NULL)
//		  {
//			  if((maIPList=realloc(maIPList,index*sizeof(char*)))==NULL)
//			  	DieWithErrorMessage("Memory allocation Failed for IPs","");
//			  maIPList[index-2]=clntName;
//			  maIPList[index-1]=NULL;
//			  index++;
//		  }
//	  }
	struct in_addr **addr_list;
	struct hostent *lh = gethostbyname(hostname);

	char clntName[INET_ADDRSTRLEN];
	char** maIPList=NULL;
	int index=2;

	addr_list = (struct in_addr **)lh->h_addr_list;
	char **test=lh->h_addr_list;

	int i=0;

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
	  return maIPList;
}
