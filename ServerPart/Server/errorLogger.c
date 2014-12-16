/*
 * errorLogger.c
 * To log errors in the ErrorLog.txt file
 *	Created on: Dec 14, 2014
 *	 Authors: Pranav Sarda and Nikhil Rajendran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// function to log errors in error log file
// parameter 1 - pointer to data to be written in the error log file
int errorLogger(const char *data)
{
	char dateTime[20];

	struct tm *currTime;
	time_t now1 = time(NULL);
	FILE *errorFile;
	char errorMessage[1000] = {0};

	// to get the current date and time
	currTime = localtime(&now1);

	// to get the current date and time in the required format
	strftime (dateTime, sizeof(dateTime), "%Y/%m/%d %H:%M", currTime);

	strcpy(errorMessage, dateTime);
	strcat(errorMessage, " ");
	strcat(errorMessage, data);

	errorFile = fopen("ErrorLog.txt", "a");
	if(errorFile == NULL)
	{
		fprintf(stdout, "The ErrorLog log file did not open.");
		return 0;
	}
	fprintf(errorFile, "%s\n", errorMessage);

	fclose(errorFile);

	return 0;
}
