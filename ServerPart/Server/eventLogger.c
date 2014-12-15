#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void eventLogger(char *data)
{
	char dateTime[17];
	int length = strlen(data) + strlen(dateTime);
	struct tm *currTime;
	time_t now;
	FILE *eventFile;
	char *eventMessage = (char *)malloc(length*sizeof(char));
	if(eventMessage == NULL)
	{
		fprintf(stdout, "Memory allocation for logging failed.");
		return;
	}
	eventFile = fopen("EventLog.txt", "a");
	if(eventFile == NULL)
	{
		fprintf(stdout, "The EventLog log file did not open.");
		return;
	}
	now = time(0);
	currTime = localtime(&now);
	strftime (dateTime, sizeof(dateTime), "%Y/%m/%d %H:%M", currTime);
	strcpy(eventMessage, dateTime);
	strcat(eventMessage, " ");
	strcat(eventMessage, data);
	fprintf(eventFile, "%s\n", eventMessage);
	fclose(eventFile);
}