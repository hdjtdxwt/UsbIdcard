/*
 * Copyright (C) 2012-2013@veally.com SerialPortJAR
 * Author :  <veally@foxmail.com>
 *
 *
 * Licensed under the veally License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.veally.com
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef ANDROID_LOG_H_
#define ANDROID_LOG_H_

#include "android/log.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

static const char * TAG="TERMINAL";
//#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG, fmt, ##args)
//#define LOGW(fmt, args...) __android_log_print(ANDROID_LOG_WARN,  TAG, fmt, ##args)
//#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
//#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

void LOGI(const char* fmt, ...) ;
void LOGW(const char* fmt, ...);
void LOGD(const char* fmt, ...);
void LOGE(const char* fmt, ...);

void Logfile(const char* fmt,...);
bool Read2WriteConfig(bool isRead,char *tag,char *result);

/*void LOGD(const char *fmt, int len, char *args){
	__android_log_print(ANDROID_LOG_WARN, TAG, fmt, len, args);
};*/


#endif

