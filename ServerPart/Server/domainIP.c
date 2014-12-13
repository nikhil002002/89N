#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "database.h"

char *domainIP(struct database *fRecord, char *webName, char *message)
{
	struct database *currentRecord;
	char domName[20], ipList[160];
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
//	if(currentRecord != NULL)
//	{
		index = 0;
		strcpy(ipList, currentRecord->ipAddrs[index]);
		index++;
		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
		{
			strcat(ipList, " ");
			strcat(ipList, currentRecord->ipAddrs[index]);
			index++;
		}
		currentRecord->numTimes++;
		strcpy(message, "The ip for the domain is returned.");
		return &ipList[0];
//	}
//	else
//	{
		// to put code for gethostbyname();
//	}
}