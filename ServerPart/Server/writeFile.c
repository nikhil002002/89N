/*
 * writeFile.c
 * To write the linked list database in the file
 *	Created on: Dec 07, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

// function to write the linked list database in the file - returns a message about the success of the write operation
// parameter 1 - pointer to first node in the linked list database
// parameter 2 - pointer to name of the file
char *writeFile(struct database *fRecord, char *fName)
{
	char line[200], timesReq[10];
	int index = 0;
	struct database *currentRecord, *previousRecord, *tempRecord;
	FILE *dataWriteFile;
	dataWriteFile = fopen(fName, "w");
	if(dataWriteFile == NULL)
	{
		return "File cannot be opened.";
	}

	// while loop to copy each record in a single string and then write in the file
	currentRecord = fRecord;
	previousRecord = currentRecord;
	while(currentRecord != NULL)
	{
		index = 0;
		currentRecord = currentRecord->nextRecord;
		strcpy(line, previousRecord->domainName);
		strcat(line, " ");
		sprintf(timesReq, "%d", previousRecord->numTimes);
		strcat(line, timesReq);

		// while loop condition to check '\0' in the next row after valid IP address rows
		// index < 10 as max number of IP addresses is assumed to be 10
		while((previousRecord->ipAddrs[index][0] != '\0') && (index < 10))
		{
			strcat(line, " ");
			strcat(line, previousRecord->ipAddrs[index]);
			index++;
		}
		fprintf(dataWriteFile, "%s\n", line);
		tempRecord = previousRecord;
		previousRecord = currentRecord;
		free(tempRecord);
	}
	fclose(dataWriteFile);
	return "The file has been populated with the current database.";
}
