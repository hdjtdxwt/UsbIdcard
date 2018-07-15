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
 */

#include "common.h"
#include <jni.h>
#include "android/log.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

//char* to jstring
jstring stringTojstring(JNIEnv* env, const char* pat) {
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);

	jstring encoding = env->NewStringUTF("utf-8");
	jstring result = (jstring) env->NewObject(strClass, ctorID, bytes,
			encoding);
	env->DeleteLocalRef((jobject) strClass);
	env->DeleteLocalRef((jobject) bytes);
	env->DeleteLocalRef((jobject) encoding);

	return result;
}
jstring charTojstringWithCharset(JNIEnv* env, const char* pat, char* charset) {

	jclass strClass = env->FindClass("java/lang/String");
	jmethodID init = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
//	__android_log_print(ANDROID_LOG_DEBUG, "TERMINAL", "string2jstring:%d", strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
	jstring encoding = env->NewStringUTF(charset);

	jstring result = (jstring) env->NewObject(strClass, init, bytes, encoding);
	env->DeleteLocalRef((jobject) strClass);
	env->DeleteLocalRef((jobject) bytes);
	env->DeleteLocalRef((jobject) encoding);
	return result;
}

//jstring to char*
char* jstringToChar(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jmethodID getBytes = env->GetMethodID(clsstring, "getBytes", "()[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod((jobject) jstr,
			getBytes);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memset(rtn, '\0', alen + 1);
		memcpy(rtn, ba, alen);
	}
	env->ReleaseByteArrayElements(barr, ba, JNI_COMMIT);
	env->DeleteLocalRef((jobject) clsstring);
	env->DeleteLocalRef((jobject) barr);
	return rtn;
}

//jstring to char*
char* jstringToCharWithCharset(JNIEnv* env, jstring jstr, char* charset) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF(charset);
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod((jobject) jstr, mid,
			strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memset(rtn, '\0', alen + 1);
		memcpy(rtn, ba, alen);
	}
	env->ReleaseByteArrayElements(barr, ba, JNI_COMMIT);
	env->DeleteLocalRef((jobject) clsstring);
	env->DeleteLocalRef((jobject) strencode);
	env->DeleteLocalRef((jobject) barr);

	return rtn;
}

/*int  jstringTostring(JNIEnv* env, jstring jstr, char* desc)
 {
 if(desc){
 desc = NULL;
 }
 jclass clsstring = env->FindClass("java/lang/String");
 jstring strencode = env->NewStringUTF("utf-8");
 jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
 jbyteArray barr=  (jbyteArray)env->CallObjectMethod((jobject)jstr, mid, strencode);
 jsize alen = env->GetArrayLength(barr);
 jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
 if (alen > 0)
 {
 desc = (char*)malloc(alen);
 memcpy(desc, ba, alen);
 }
 env->ReleaseByteArrayElements(barr, ba, JNI_COMMIT);
 return alen;
 }*/
/*
 * char* convert to jchar*
 */
void charToJchar(char* src, jchar* desc, int srcLen) {
	for (int i = 0; i < srcLen; i++) {
		desc[i] = (jchar) src[i];
	}
}
/*
 * jchar* convert to char*
 */
void jcharTochar(jchar* src, char* desc, int srcLen) {
	for (int i = 0; i < srcLen; i++) {
		desc[i] = (char) src[i];
	}
}

/*
 * From hex char to char
 * eg: 0xA  => "A"
 */
char char2Hex(unsigned char c) // 将字符以16进制表示
		{
	char ch = c / 0x10 + '0';
	if (ch > '9')
		ch += ('A' - '9' - 1);
	char cl = c % 0x10 + '0';
	if (cl > '9')
		cl += ('A' - '9' - 1);
	return cl;
}
/*
 * From byte to hex char
 * eg: {0xA0, 0xA4} => "A0A4"
 */
void byte2HexChar(char* desc, jbyte * src, int srcLen) {
//	char tmp[srcLen*2];
	for (int i = 0; i < srcLen; i++) {
		unsigned int t = src[i];
		desc[i * 2] = char2Hex(t >> 4);
		desc[i * 2 + 1] = char2Hex(t & 0x0F);
	}
}

/*
 * From ASCII char to hex
 * eg: "A0A4" => {0xA0, 0xA4}
 */
void ASCIIChar2Hex(char* src, int srcLen, char * result) {
//		char tmp[2];
	for (int i = 0; i < srcLen / 2; i++) {
		int hight = char2Int(src[i * 2]) << 4;
		int low = char2Int(src[(i * 2) + 1]);
//			tmp[0] = (const char)src[i*2];
//			tmp[1] = (const char)src[i*2+1];
//			LOGI("pos: %d, c1:%c c2:%c || c1:%d c2:%d;", i , src[i*2], src[i*2+1], hight ,low);
		result[i] = hight | low;
//			int num = 0;
	}
}

/*void byteTojbyteArray(char *src, jbyteArray desc, int srcLen){
 jbyteArray cmdByteArray = env->NewByteArray(cldLen);
 env->SetByteArrayRegion(cmdByteArray,0,cldLen,(jbyte*)cld);
 }*/

/*
 * From char to int
 */
int char2Int(char c) {
	int nDecNum = 0;
	switch (c) {
	case 'a':
	case 'A':
		nDecNum = 10;
		break;
	case 'b':
	case 'B':
		nDecNum = 11;
		break;
	case 'c':
	case 'C':
		nDecNum = 12;
		break;
	case 'd':
	case 'D':
		nDecNum = 13;
		break;
	case 'e':
	case 'E':
		nDecNum = 14;
		break;
	case 'f':
	case 'F':
		nDecNum = 15;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':

		nDecNum = (c - '0') % 48;
		break;
	default:
		return 0;
	}
	return nDecNum;
}
int PathExist(char *strPath) {
	return (access(strPath, 0) == 0);
}
int CreateMultDir(const char *dir) {
	char *tarDir; /* 路径缓存 */
	int dirLen; /* 路径长度 */
	char *szTemp; /* 建立路径用临时缓存 */
	char *pTmp; /* 临时指针变量 */

	dirLen = strlen(dir) + 1;
	tarDir = (char*) malloc(dirLen * sizeof(char));
	szTemp = (char*) malloc(dirLen * sizeof(char));
	sprintf(tarDir, dir);

	if (tarDir[dirLen - 1] == '/') {
		tarDir[dirLen - 1] = '\0';    // 先删除最后的“\”
	}
	pTmp = tarDir;
	pTmp++;
	while (1)            // 之后的位置(要能进入此循环, 至少是二级目录, 如: c:\一级目录\二级目录\...)
	{
		// 查找路径分隔符号'\'
		pTmp = strchr(pTmp, '/'); // 第一次的出现位置(地址)
		if (NULL == pTmp) {
			break;
		}
		memset(szTemp, '\0', dirLen);
		strncpy(szTemp, tarDir, (pTmp - tarDir) / sizeof(char));     // 如: e:\1

		if (!PathExist(szTemp)) // 如果不存在, 则一路创建到倒数第二个目录
				{
			if (mkdir(szTemp, S_IRWXU)) {
				free(tarDir);
				free(szTemp);
				return 0;
			}
		}
		pTmp++;
	}
	if (!PathExist(tarDir)) // 如果不存在, 则一路创建到倒数第二个目录
			{
		if (mkdir(tarDir, S_IRWXU)) {
			free(tarDir);
			free(szTemp);
			return 0;
		}
	}
	free(tarDir);
	free(szTemp);
	return 1;

}
jstring byte2jstring(JNIEnv* env, const char* byte) {
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(byte));
	env->SetByteArrayRegion(bytes, 0, strlen(byte),
			(jbyte*) byte);
	jstring encoding = env->NewStringUTF("UTF-16LE");
	jobject perString = env->NewObject(strClass, ctorID, bytes, encoding);
	env->DeleteLocalRef(strClass);
	env->DeleteLocalRef(bytes);
	env->DeleteLocalRef(encoding);
	return (jstring) perString;
}
jstring byte2jstring(JNIEnv* env, const char* byte, int byteLen) {
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(byteLen);
	env->SetByteArrayRegion(bytes, 0, byteLen, (jbyte*) byte);
	jstring encoding = env->NewStringUTF("UTF-16LE");
	jobject perString = env->NewObject(strClass, ctorID, bytes, encoding);
	env->DeleteLocalRef(strClass);
	env->DeleteLocalRef(bytes);
	env->DeleteLocalRef(encoding);
	return (jstring) perString;
}
jstring trim(JNIEnv* env,jstring string) {
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID trim = env->GetMethodID(strClass, "trim", "()Ljava/lang/String;");
	jstring s = (jstring) env->CallObjectMethod((jobject) string, trim);
	env->DeleteLocalRef(strClass);
	return s;
}

