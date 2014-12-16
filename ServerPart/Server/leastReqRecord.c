/*
 * leastReqRecord.c
 * To find the least requested record(s) in the linked list database
 *	Created on: Dec 10, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

// function to find least requested record(s) - returns a pointer to first node in the linked list of least requested records
// parameter 1 - pointer to first node in the linked list database
// parameter 2 - pointer to message about the return value
struct database *leastReqRecord(struct database *fRecord, char *message)
{
	int numReq, index;
	char recrd[1000];
	struct database *currentRecord, *fLeastRequest, *leastRequest;
	currentRecord = fRecord;
	if(currentRecord == NULL)
	{
		strcpy(message, "No Records to find Least Requested Records.");
		eventLogger(message);
		return currentRecord;
	}

	// considering the least requested record as the first in the linked list
	numReq = currentRecord->numTimes;
	currentRecord = currentRecord->nextRecord;

	// while loop to check for the least requested record
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes < numReq)
		{
			numReq = currentRecord->numTimes;
		}
		currentRecord = currentRecord->nextRecord;
	}

	// while loop to create a linked list containing least requested records
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

			// while loop condition to check '\0' in the next row after valid IP address rows
			// index < 10 as max number of IP addresses is assumed to be 10
			while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
			{
				strcpy(leastRequest->ipAddrs[index], currentRecord->ipAddrs[index]);
				index++;
			}

			// assigning '\0' to the next row after valid IP address rows for easy detection of total valid IP address rows
			if(index < 10)
			{
				strcpy(leastRequest->ipAddrs[index], "\0");
			}
			leastRequest->nextRecord = fLeastRequest;
			fLeastRequest = leastRequest;
		}
		currentRecord = currentRecord->nextRecord;
	}

	// function call to get all least requested records in one string
	// function definition is given in file mostReqRecord.c
	printFunction(leastRequest,recrd);
	strcpy(message, recrd);
	return fLeastRequest;
}
