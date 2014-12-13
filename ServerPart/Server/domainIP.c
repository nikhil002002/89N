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
#include <netdb.h>
#include <arpa/inet.h>
#include "IpDomainDossierHeaders.h"


void domainIP(char *webName)
//char *domainIP(struct database *fRecord, char *webName, char *message)
{
//	struct database *currentRecord;
//	char domName[20], ipList[160];
//	int index = 0;
//	while(webName[index] != '\0')
//	{
//		webName[index] = toupper(webName[index]);
//		index++;
//	}
//	currentRecord = fRecord;
//	while(currentRecord != NULL)
//	{
//		index = 0;
//		strcpy(domName, currentRecord->domainName);
//		while(domName[index] != '\0')
//		{
//			domName[index] = toupper(domName[index]);
//			index++;
//		}
//		if(strcmp(domName, webName) == 0)
//		{
//			break;
//		}
//		currentRecord = currentRecord->nextRecord;
//	}
//	if(currentRecord != NULL)
//	{
//		index = 0;
//		strcpy(ipList, currentRecord->ipAddrs[index]);
//		index++;
//		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
//		{
//			strcat(ipList, " ");
//			strcat(ipList, currentRecord->ipAddrs[index]);
//			index++;
//		}
//		currentRecord->numTimes++;
//		strcpy(message, "The ip for the domain is returned.");
//		return &ipList[0];
//	}
//	else
//	{
		char** ipList=findIPfromDomainName(webName);
		for(; ipList != NULL; ipList++)
		{
			printf("%s ",&(**ipList));
		}
//	}
}

char **findIPfromDomainName(char *hostname)
{
	 // Tell the system what kind(s) of address info we want
	  struct addrinfo addrCriteria;                   // Criteria for address match
	  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
	  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
	  //addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
	  //addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

	  // Get address(es) associated with the specified name/service
	  struct addrinfo *addrList; // Holder for list of addresses returned
	  // Modify servAddr contents to reference linked list of addresses
	  int rtnVal = getaddrinfo(hostname, NULL, &addrCriteria, &addrList);
	  if (rtnVal != 0)
	    DieWithErrorMessage("getaddrinfo() failed", gai_strerror(rtnVal));

	  // returned addresses
	  char clntName[INET_ADDRSTRLEN];
	  char** maIPList=NULL;
//	  if((maIPList=malloc(1*sizeof(char*)))==NULL)
//	  	DieWithErrorMessage("Memory allocation Failed for 1st IP","");
	  int index=2;

	  for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next) {
		  if (inet_ntop(PF_INET, addr->ai_addr, clntName, sizeof(clntName)) != NULL)
		  {
			  if((maIPList=realloc(maIPList,index*sizeof(char*)))==NULL)
			  	DieWithErrorMessage("Memory allocation Failed for IPs","");
			  maIPList[index-2]=clntName;
			  maIPList[index-1]=NULL;
			  index++;
		  }
	  }
	  return maIPList;
}
