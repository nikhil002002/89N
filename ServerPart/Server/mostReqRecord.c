#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

struct database *mostReqRecord(struct database *fRecord, char *message)
{
	int numReq, index;
	struct database *currentRecord, *fMostRequest, *mostRequest;
	currentRecord = fRecord;
	if(currentRecord == NULL)
	{
		strcpy(message, "The database is empty.");
		return currentRecord;
	}
	numReq = currentRecord->numTimes;
	currentRecord = currentRecord->nextRecord;
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes > numReq)
		{
			numReq = currentRecord->numTimes;
		}
		currentRecord = currentRecord->nextRecord;
	}
	currentRecord = fRecord;
	fMostRequest = NULL;
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes == numReq)
		{
			mostRequest = (struct database *)malloc(sizeof(struct database));
			if(mostRequest == NULL)
			{
				strcpy(message, "No memory left to store extracted record.");
				return NULL;
			}
			strcpy(mostRequest->domainName, currentRecord->domainName);
			mostRequest->numTimes = currentRecord->numTimes;
			index = 0;
			while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
			{
				strcpy(mostRequest->ipAddrs[index], currentRecord->ipAddrs[index]);
				index++;
			}
			if(index != 10)
			{
				strcpy(mostRequest->ipAddrs[index], "\0");
			}
			mostRequest->nextRecord = fMostRequest;
			fMostRequest = mostRequest;
		}
		currentRecord = currentRecord->nextRecord;
	}
	strcpy(message, "The most requested record(s) are returned.");
	return fMostRequest;
}
