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
		strcpy(message, "The database is empty.");
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
				strcpy(message, "No memory left to store extracted record.");
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
			if(index != 10)
			{
				strcpy(leastRequest->ipAddrs[index], "\0");
			}
			leastRequest->nextRecord = fLeastRequest;
			fLeastRequest = leastRequest;
		}
		currentRecord = currentRecord->nextRecord;
	}

	char recrd[200];
	printFunction(leastRequest,recrd);

	strcpy(message, recrd);
	return fLeastRequest;
}
//
//void printFunction(struct database *first, char *recrd)
//{
//	char data[200]={0}, times[10];
//	int index;
//
//	while(first != NULL)
//	{
//		index = 0;
//		strcat(data, first->domainName);
//		strcat(data, " ");
//		sprintf(times, "%d", first->numTimes);
//		strcat(data, times);
//		while((first->ipAddrs[index][0] != '\0') && (index < 10))
//		{
//			strcat(data, " ");
//			strcat(data, first->ipAddrs[index]);
//			index++;
//		}
//		strcat(data, "\n");
//		first = first->nextRecord;
//	}
//	strcpy(recrd,data);
//}
