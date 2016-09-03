#include "debug.h"
#include <stdio.h> 
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int DEBUG::getLastErrorCode()
{
	return(errorCode);
};

const char* DEBUG::getLastErrorMessage()
{
	return &(errorMessage[0]);
};

void DEBUG::toLog(int code, const char* msg, ...)
{
	va_list args;
	char buf[1024];
	FILE* pFile;
	time_t tnow;
	struct tm *tmnow;

	if(!(pFile=fopen("error.log","a")))
		return;

	va_start(args, msg);
	vsprintf(buf,msg,args);

	time(&tnow);
	tmnow=localtime(&tnow);

	if(tmnow->tm_min<10)
		fprintf(pFile,"[%d.%d. %d:0%d]:%s\n",tmnow->tm_mday, tmnow->tm_mon+1,tmnow->tm_hour,tmnow->tm_min,buf);
	else
		fprintf(pFile,"[%d.%d. %d:%d]:%s\n",tmnow->tm_mday, tmnow->tm_mon+1,tmnow->tm_hour,tmnow->tm_min,buf);

	strcpy(errorMessage,buf);

	va_end(args);
	fclose(pFile);
};

DEBUG::DEBUG()
{
};

DEBUG::~DEBUG()
{
};

