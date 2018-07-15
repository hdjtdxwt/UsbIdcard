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

#ifndef __COMMON_CLASS__
#define __COMMON_CLASS__

#include <time.h>
#include <jni.h>
#include <stdio.h>

#include <string.h>
#define CONFIGPATH	"/sdcard/eps/config/"


//char* to jstring
jstring stringTojstring(JNIEnv* env, const char* pat);

jstring charTojstringWithCharset(JNIEnv* env, const char* pat, char* charset);

//jstring to char*
char* jstringToChar(JNIEnv* env, jstring jstr);

char* jstringToCharWithCharset(JNIEnv* env, jstring jstr, char* charset);

//		int  jstringTostring(JNIEnv* env, jstring jstr, char* desc);

void charToJchar(char* src, jchar* desc, int srcLen);

void jcharTochar(jchar* src, char* desc, int srcLen);

char char2Hex(unsigned char c);
/*
 * From byte to hex char
 * eg: {0xA0, 0xA4} => "A0A4"
 */
void byte2HexChar(char* desc, jbyte * src, int srcLen);

void ASCIIChar2Hex(char* src, int srcLen, char * result);

int char2Int(char c);

//		void byteTojbyteArray(char *src, jbyteArray desc, int srcLen);
int PathExist(char *strPath);
int CreateMultDir(const char *dir);
jstring byte2jstring(JNIEnv* env, const char* byte);
jstring byte2jstring(JNIEnv* env, const char* byte, int byteLen);
jstring trim(JNIEnv* env,jstring string);

/*
 * From byte to hex char
 * eg: {0xA0, 0xA4} => "A0A4"
 */
#endif
