/*
 * AndroidLog.cpp
 *
 *  Created on: 2013年11月28日
 *      Author: veally@foxmail.com
 */
#include "Android_Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Common.h"
#define LOGPATH	"/sdcard/eps/log/"

void LOGI(const char* fmt, ...) {
	char buffer[1024];
	memset(buffer, 0, 1024);
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 1024, fmt, arg_ptr);
	va_end(arg_ptr);
	__android_log_vprint(ANDROID_LOG_INFO, TAG, fmt, arg_ptr);
}
void LOGW(const char* fmt, ...) {
	char buffer[1024];
	memset(buffer, 0, 1024);
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 1024, fmt, arg_ptr);
	va_end(arg_ptr);
	__android_log_vprint(ANDROID_LOG_WARN, TAG, fmt, arg_ptr);

}
void LOGD(const char* fmt, ...) {
	char buffer[1024];
		memset(buffer, 0, 1024);
		va_list arg_ptr;
		va_start(arg_ptr, fmt);
		vsnprintf(buffer, 1024, fmt, arg_ptr);
		va_end(arg_ptr);
		__android_log_vprint(ANDROID_LOG_ERROR, TAG, fmt, arg_ptr);

}
void LOGE(const char* fmt, ...) {
	char buffer[1024];
	memset(buffer, 0, 1024);
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buffer, 1024, fmt, arg_ptr);
	va_end(arg_ptr);
	__android_log_vprint(ANDROID_LOG_ERROR, TAG, fmt, arg_ptr);

}
void Logfile(const char* fmt, ...) {
	char filename[1024] = { 0, };
	char timestr[1024] = { 0, };
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(filename, "%4d-%02d-%02d", 1900 + timeinfo->tm_year,
			1 + timeinfo->tm_mon, timeinfo->tm_mday);
	sprintf(timestr, "[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min,
			timeinfo->tm_sec);
	char fullpath[1024] = LOGPATH;
	strcat(fullpath, filename);
	if (CreateMultDir(LOGPATH) != 1)
		return;
	FILE *f = fopen(fullpath, "a+");
	if (f) {
//		fprintf(f, timestr);
		char buffer[1024];
		memset(buffer, 0, 1024);
		va_list arg_ptr;
		va_start(arg_ptr, fmt);
		vsnprintf(buffer, 1024, fmt, arg_ptr);
		va_end(arg_ptr);
//		fprintf(f, buffer);
		fprintf(f, "\n");
		fclose(f);

	} else {
		LOGW("open logfile:%s failed", fullpath);
	}

}
