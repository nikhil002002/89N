#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

struct database *leastReqRecord(struct database *fRecord, char *message)
{
	int numReq, index;
	struct database *currentRecord, *fLeastRequest, *leastRequest;
	currentRecord = fRecord;
	if(currentRecord == NULL)
	{
		strcpy(message, "No Records to find Least Requested Records.");
		eventLogger(message);
		return currentRecord;
	}
	numReq = currentRecord->numTimes;
	currentRecord = currentRecord->nextRecord;
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes < numReq)
		{
			numReq = currentRecord->numTimes;
		}
		currentRecord = currentRecord->nextRecord;
	}
	currentRecord = fRecord;
	fLeastRequest = NULL;
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes == numReq)
		{
			leastRequest = (struct database *)malloc(sizeof(struct database));
			if(leastRequest == NULL)
			{
				strcpy(message, "leastReqRecord->No memory left to store extracted record.");
				errorLogger(message);
				return NULL;
			}
			strcpy(leastRequest->domainName, currentRecord->domainName);
			leastRequest->numTimes = currentRecord->numTimes;
			index = 0;
			while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
			{
				strcpy(leastRequest->ipAddrs[index], currentRecord->ipAddrs[index]);
				index++;
			}
			if(index < 10)
			{
				strcpy(leastRequest->ipAddrs[index], "\0");
			}
			leastRequest->nextRecord = fLeastRequest;
			fLeastRequest = leastRequest;
		}
		currentRecord = currentRecord->nextRecord;
	}

	char recrd[1000];
	printFunction(leastRequest,recrd);

	strcpy(message, recrd);
	return fLeastRequest;
}
