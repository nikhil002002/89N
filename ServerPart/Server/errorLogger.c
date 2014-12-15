#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int errorLogger(const char *data)
{
	static char dateTime[20];

	struct tm *currTime;
	time_t now1;
	FILE *errorFile;
	char errorMessage[1000]={0};

	now1 = time(0);
	currTime = localtime(&now1);
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
