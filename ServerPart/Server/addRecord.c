#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IpDomainDossierHeaders.h"

struct database *addRecord(struct database *fRecord, const char *newData, char *message)
{
	struct database *currentRecord, *toAddRecord;
	char domName[20], ipAdd[16], webName[20], siteName[20];  //totalData[36],
	int first, second, third, fourth, index = 0;
	//strcpy(totalData, newData);

	strcpy(webName,newData);
	char const *ipAddp=newData+strlen(newData)+1;

	strcpy(ipAdd, ipAddp);
	//sscanf(newData, "%s%s", webName, ipAdd);
	sscanf(ipAdd, "%d.%d.%d.%d", &first, &second, &third, &fourth);
	if(first < 0 || first > 255 || second < 0 || second > 255 || third < 0 || third > 255 || fourth < 0 || fourth > 255)
	{
		strcpy(message, "The IP address format is wrong. The record could not be added.");
		return NULL;
	}
	strcpy(siteName,webName);
	while(siteName[index] != '\0')
	{
		siteName[index] = toupper(siteName[index]);
		index++;
	}
	currentRecord = fRecord;
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
	if(currentRecord == NULL)
	{
		toAddRecord = (struct database *)malloc(sizeof(struct database));
		if(toAddRecord == NULL)
		{
			strcpy(message, "Database is full. No space to add new records. The record could not be added.");
			return NULL;
		}
		toAddRecord->numTimes = 0;
		strcpy(toAddRecord->domainName, webName);
		strcpy(toAddRecord->ipAddrs[0], ipAdd);
		strcpy(toAddRecord->ipAddrs[1], "\0");
		toAddRecord->nextRecord = fRecord;
		fRecord = toAddRecord;
		strcpy(message, "The record added successfully.");
		return fRecord;
	}
	else
	{
		index = 0;
		while((currentRecord->ipAddrs[index][0] != '\0') && (index < 10))
		{
			if(strcmp(currentRecord->ipAddrs[index], ipAdd) == 0)
			{
				strcpy(message, "The record to be added already exists.");
				return fRecord;
			}
			else if(index == 9)
			{
				strcpy(message, "The record already existed, and the alternate IP address was not added to the record due to maximum IP addresses.");
				return fRecord;
			}
		index++;
		}
		strcpy(currentRecord->ipAddrs[index], ipAdd);
		if(index < 9)
		{
			strcpy(currentRecord->ipAddrs[index + 1], "\0");
		}
		strcpy(message, "The record already existed, but alternate IP address was added.");
		return fRecord;
	}
}
