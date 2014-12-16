/*
 * addRecord.c
 * To add a requested new record in the linked list database
 *	Created on: Dec 08, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IpDomainDossierHeaders.h"

// function to add record - returns a pointer to first node in the linked list database
// parameter 1 - pointer to first node in the linked list
// parameter 2 - new data consisting of domain name and ip to be added
// parameter 3 - pointer to message about the return value
struct database *addRecord(struct database *fRecord, const char *newData, char *message)
{
	struct database *currentRecord, *toAddRecord;
	char domName[20], ipAdd[16], webName[20], siteName[20];
	int first, second, third, fourth, index = 0;

	strcpy(webName,newData);
	char const *ipAddp=newData+strlen(newData)+1;

	strcpy(ipAdd, ipAddp);
	sscanf(ipAdd, "%d.%d.%d.%d", &first, &second, &third, &fourth);

	// to check if the IP of the domain to be added is correct
	if(first < 0 || first > 255 || second < 0 || second > 255 || third < 0 || third > 255 || fourth < 0 || fourth > 255)
	{
		strcpy(message, "The IP address format is wrong. The record could not be added.");
		eventLogger(message);
		return NULL;
	}
	strcpy(siteName,webName);

	// while loop to convert domain name to add to upper case for case insensitive comparison
	while(siteName[index] != '\0')
	{
		siteName[index] = toupper(siteName[index]);
		index++;
	}
	currentRecord = fRecord;

	// while loop to check if domain name to add is already present
	while(currentRecord != NULL)
	{
		index = 0;
		strcpy(domName, currentRecord->domainName);
		while(domName[index] != '\0')
		{
			domName[index] = toupper(domName[index]);
			index++;
		}
		if(strcmp(domName, siteName) == 0)
		{
			break;
		}
		currentRecord = currentRecord->nextRecord;
	}

	// if to add the new record
	if(currentRecord == NULL)
	{
		toAddRecord = (struct database *)malloc(sizeof(struct database));
		if(toAddRecord == NULL)
		{
			strcpy(message, "Database is full. No space to add new records. The record could not be added.");
			eventLogger(message);
			return NULL;
		}
		toAddRecord->numTimes = 0;
		strcpy(toAddRecord->domainName, webName);
		strcpy(toAddRecord->ipAddrs[0], ipAdd);

		// assigning '\0' to the next row after valid IP address rows for easy detection of total valid IP address rows
		strcpy(toAddRecord->ipAddrs[1], "\0");
		toAddRecord->nextRecord = fRecord;
		fRecord = toAddRecord;
		strcpy(message, "The record was added successfully.");

		char *eventmssg=malloc(150);
		sprintf(eventmssg,"Record added: Domain: %s IP: %s ",webName, ipAdd);
		eventLogger(eventmssg);
		free(eventmssg);

		return fRecord;
	}

	// else to check if IP to add exists for domain name already in the database
	else
	{
		index = 0;

		// while loop condition to check '\0' in the next row after valid IP address rows
		// index < 10 as max number of IP addresses is assumed to be 10
		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
		{
			if(strcmp(currentRecord->ipAddrs[index], ipAdd) == 0)
			{
				strcpy(message, "The record to be added already exists.");
				eventLogger(message);
				return fRecord;
			}
			else if(index == 9)
			{
				strcpy(message, "The record already existed, alternate IP address was not added to the record due to maximum IP limit.");
				eventLogger(message);
				return fRecord;
			}
		index++;
		}
		strcpy(currentRecord->ipAddrs[index], ipAdd);

		// assigning '\0' to the next row after valid IP address rows for easy detection of total valid IP address rows
		if(index < 9)
		{
			strcpy(currentRecord->ipAddrs[index + 1], "\0");
		}
		strcpy(message, "The record already existed, but alternate IP address was added.");

		char *eventmssg=malloc(150);
		sprintf(eventmssg,"Alternate IP added: %s ",ipAdd);
		eventLogger(eventmssg);
		free(eventmssg);

		return fRecord;
	}
}
