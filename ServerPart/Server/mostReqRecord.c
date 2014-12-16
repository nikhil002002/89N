/*
 * mostReqRecord.c
 * To find the most requested record(s) in the linked list database
 *	Created on: Dec 10, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

// function to find most requested record(s) - returns a pointer to first node in the linked list of most requested records
// parameter 1 - pointer to first node in the linked list database
// parameter 2 - pointer to message about the return value
struct database *mostReqRecord(struct database *fRecord, char *message)
{
	int numReq, index;
	char recrd[1000];
	struct database *currentRecord, *fMostRequest, *mostRequest;
	currentRecord = fRecord;
	if(currentRecord == NULL)
	{
		strcpy(message, "No Records to find Most Requested Records.");
		eventLogger(message);
		return currentRecord;
	}

	// considering the most requested record as the first in the linked list
	numReq = currentRecord->numTimes;
	currentRecord = currentRecord->nextRecord;

	// while loop to check for the most requested record
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes > numReq)
		{
			numReq = currentRecord->numTimes;
		}
		currentRecord = currentRecord->nextRecord;
	}

	// while loop to create a linked list containing most requested records
	currentRecord = fRecord;
	fMostRequest = NULL;
	while(currentRecord != NULL)
	{
		if(currentRecord->numTimes == numReq)
		{
			mostRequest = (struct database *)malloc(sizeof(struct database));
			if(mostRequest == NULL)
			{
				strcpy(message, "mostReqRecords()->No memory left to store extracted record.");
				errorLogger(message);
				return NULL;
			}
			strcpy(mostRequest->domainName, currentRecord->domainName);
			mostRequest->numTimes = currentRecord->numTimes;
			index = 0;

			// while loop condition to check '\0' in the next row after valid IP address rows
			// index < 10 as max number of IP addresses is assumed to be 10
			while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
			{
				strcpy(mostRequest->ipAddrs[index], currentRecord->ipAddrs[index]);
				index++;
			}

			// assigning '\0' to the next row after valid IP address rows for easy detection of total valid IP address rows
			if(index < 10)
			{
				strcpy(mostRequest->ipAddrs[index], "\0");
			}
			mostRequest->nextRecord = fMostRequest;
			fMostRequest = mostRequest;
		}
		currentRecord = currentRecord->nextRecord;
	}

	// function call to get all most requested records in one string
	printFunction(fMostRequest,recrd);
	strcpy(message, recrd);
	return fMostRequest;
}

// function to get all the requested records in one string
// parameter 1 - pointer to first node in the linked list of the requested records
// parameter 2 - pointer to string containing all the requested records
void printFunction(struct database *first, char *recrd)
{
	char data[1000]={0}, times[10];
	int index;

	// while loop to concatenate most requested records
	while(first != NULL)
	{
		index = 0;
		strcat(data, first->domainName);
		strcat(data, " ");
		sprintf(times, "%d", first->numTimes);
		strcat(data, times);

		// while loop condition to check '\0' in the next row after valid IP address rows
		// index < 10 as max number of IP addresses is assumed to be 10
		while((first->ipAddrs[index][0] != '\0') && (index < 10))
		{
			strcat(data, " ");
			strcat(data, first->ipAddrs[index]);
			index++;
		}
		strcat(data, "\n");
		first = first->nextRecord;
	}
	strcpy(recrd,data);
}
