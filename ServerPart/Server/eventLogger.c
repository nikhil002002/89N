#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int eventLogger(const char *data)
{
	char dateTime[17];
	int length = strlen(data) + strlen(dateTime);
	struct tm* currTime;

	time_t now=time(NULL);
	FILE *eventFile;
	char *eventMessage = malloc(length*sizeof(char));
	if(eventMessage == NULL)
	{
		fprintf(stdout, "Memory allocation for logging failed.");
		return 0;
	}
	eventFile = fopen("EventLog.txt", "a");
	if(eventFile == NULL)
	{
		fprintf(stdout, "The EventLog log file did not open.");
		return 0;
	}
	//time(&now);
	currTime = localtime(&now);
	strftime (dateTime, sizeof(dateTime), "%Y/%m/%d %H:%M", currTime);
	strcpy(eventMessage, dateTime);
	strcat(eventMessage, " ");
	strcat(eventMessage, data);
	fprintf(eventFile, "%s\n", eventMessage);
	fclose(eventFile);

	return 0;
}
