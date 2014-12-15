#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void errorLogger(char *data)
{
	char dateTime[17];
	int length = strlen(data) + strlen(dateTime);
	struct tm *currTime;
	time_t now;
	FILE *errorFile;
	char *errorMessage = (char *)malloc(length*sizeof(char));
	if(errorMessage == NULL)
	{
		fprintf(stdout, "Memory allocation for logging failed.");
		return;
	}
	errorFile = fopen("ErrorLog.txt", "a");
	if(errorFile == NULL)
	{
		fprintf(stdout, "The ErrorLog log file did not open.");
		return;
	}
	now = time(0);
	currTime = localtime(&now);
	strftime (dateTime, sizeof(dateTime), "%Y/%m/%d %H:%M", currTime);
	strcpy(errorMessage, dateTime);
	strcat(errorMessage, " ");
	strcat(errorMessage, data);
	fprintf(errorFile, "%s\n", errorMessage);
	fclose(errorFile);
}