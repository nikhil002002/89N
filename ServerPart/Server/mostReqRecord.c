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
	char* recrd=NULL;
	recrd=printFunction(currentRecord,recrd);

	strcpy(message, recrd);
	return fMostRequest;
}

char *printFunction(struct database *first, char *recrd)
{
	char data[200], times[10];
	recrd=data;
	int index;
	while(first != NULL)
	{
		index = 0;
		strcat(data, first->domainName);
		strcat(data, " ");
		sprintf(times, "%d", first->numTimes);
		strcat(data, times);
		while((first->ipAddrs[index][0] != '\0') && (index < 10))
		{
			strcat(data, " ");
			strcat(data, first->ipAddrs[index]);
			index++;
		}
		strcat(data, "\n");
		first = first->nextRecord;
	}
	return recrd;
}
