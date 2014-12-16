/*
 * eventLogger.c
 * To log errors in the EventLog.txt file
 *	Created on: Dec 14, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// function to log events in event log file
// parameter 1 - pointer to data to be written in the event log file
int eventLogger(const char *data)
{
	char dateTime[20];

	struct tm *currTime;
	time_t now = time(NULL);
	FILE *eventFile;
	char eventMessage[1000] = {0};

	// to get the current date and time
	currTime = localtime(&now);

	// to get the current date and time in the required format
	strftime (dateTime, sizeof(dateTime), "%Y/%m/%d %H:%M", currTime);

	strcpy(eventMessage, dateTime);
	strcat(eventMessage, " ");
	strcat(eventMessage, data);

	eventFile = fopen("EventLog.txt", "a");
	if(eventFile == NULL)
	{
		fprintf(stdout, "The EventLog log file did not open.");
		return 0;
	}
	fprintf(eventFile, "%s\n", eventMessage);

	fclose(eventFile);

	return 0;
}
