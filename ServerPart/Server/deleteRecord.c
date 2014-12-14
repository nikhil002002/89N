#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IpDomainDossierHeaders.h"

struct database *deleteRecord(struct database *fRecord, char *webName, char *message)
{
	struct database *currentRecord, *previousRecord;
	char domName[20];
	int index = 0;
	while(webName[index] != '\0')
	{
		webName[index] = toupper(webName[index]);
		index++;
	}
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
	if(currentRecord == NULL)
	{
		strcpy(message, "The record to be deleted does not exist.");
		return NULL;
	}
	else if(previousRecord == NULL)
	{
		previousRecord = currentRecord->nextRecord;
		fRecord = previousRecord;
		free(currentRecord);
		strcpy(message, "The record to be deleted is deleted successfully.");
		return fRecord;
	}
	else
	{
		previousRecord->nextRecord = currentRecord->nextRecord;
		free(currentRecord);
		strcpy(message, "The record to be deleted is deleted successfully.");
		return fRecord;
	}
}
