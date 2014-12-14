#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

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
