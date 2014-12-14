#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IpDomainDossierHeaders.h"

struct database *readFile(char *fName, char *message)
{
	struct database *firstRecord = NULL;
	struct database *newRecord;
	char line[200], ipList[160], *tempList, dummyIP[16];
	int addr, index;
	FILE *dataReadFile;
	dataReadFile = fopen(fName, "r");
	if(dataReadFile == NULL)
	{
		strcpy(message, "File Not present/cannot be opened.");
		return NULL;
	}
	while(fgets(line, sizeof(line), dataReadFile) != NULL)
	{
		newRecord = (struct database *)malloc(sizeof(struct database));
		if(newRecord == NULL)
		{
			strcpy(message, "No memory left. File could not be read properly.");
			return NULL;
		}
		sscanf(line, "%s %d %[0-9. ]", newRecord->domainName, &newRecord->numTimes, ipList);
		index = 0;
		addr = 0;
		tempList = ipList;
		while(*tempList)
		{
			if(*tempList == ' ')
			{
				dummyIP[index] = '\0';
				index = 0;
				strcpy(newRecord->ipAddrs[addr], dummyIP);
				addr++;
				tempList++;
				continue;
			}
			dummyIP[index] = *tempList;
			tempList++;
			index++;
			if(*tempList == '\0')
			{
				dummyIP[index] = '\0';
				strcpy(newRecord->ipAddrs[addr], dummyIP);
				if(addr < 9)
				{
					strcpy(newRecord->ipAddrs[addr + 1], "\0");
				}
			}
		}
		newRecord->nextRecord = firstRecord;
		firstRecord = newRecord;
	}
	fclose(dataReadFile);
	strcpy(message, "The file has been read properly.");
	return firstRecord;
}
