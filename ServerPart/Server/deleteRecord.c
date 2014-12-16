/*
 * deleteRecord.c
 * To delete a requested record in the linked list database
 *	Created on: Dec 08, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IpDomainDossierHeaders.h"

// function to delete record - returns a pointer to first node in the linked list database
// parameter 1 - pointer to first node in the linked list
// parameter 2 - new data consisting of domain name to be deleted
// parameter 3 - pointer to message about the return value
struct database *deleteRecord(struct database *fRecord, const char *domainName, char *message)
{
	struct database *currentRecord, *previousRecord;
	char domName[20],webName[20];
	int index = 0;
	strncpy(webName,domainName,sizeof(webName)-1);

	// while loop to convert domain name to delete to upper case for case insensitive comparison
	while(webName[index] != '\0')
	{
		webName[index] = toupper(webName[index]);
		index++;
	}

	// for loop to find the location of the domain name to be deleted in the linked list
	currentRecord = fRecord;
	previousRecord = NULL;
	for(; currentRecord != NULL; previousRecord = currentRecord, currentRecord = currentRecord->nextRecord)
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
	}

	// condition to check if record to be deleted exists
	if(currentRecord == NULL)
	{
		strcpy(message, "The record to be deleted does not exist.");

		char *eventmssg=malloc(150);
		sprintf(eventmssg,"Record Does not exist for deletion, domain: %s ",domainName);
		eventLogger(eventmssg);
		free(eventmssg);

		return NULL;
	}

	// condition if record to be deleted is the first in the linked list
	else if(previousRecord == NULL)
	{
		previousRecord = currentRecord->nextRecord;
		fRecord = previousRecord;
		free(currentRecord);
		strcpy(message, "The record to be deleted is deleted successfully.");

		char *eventmssg=malloc(150);
		sprintf(eventmssg,"Record deleted, domain: %s ",domainName);
		eventLogger(eventmssg);
		free(eventmssg);

		return fRecord;
	}

	// condition if record to be deleted is not the first in the linked list
	else
	{
		previousRecord->nextRecord = currentRecord->nextRecord;
		free(currentRecord);
		strcpy(message, "The record to be deleted is deleted successfully.");
		return fRecord;
	}
}
