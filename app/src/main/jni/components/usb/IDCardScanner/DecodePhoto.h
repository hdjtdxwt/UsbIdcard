/*
 * DecodePhoto.h
 *
 *  Created on: 2015-3-12
 *      Author: liuzg
 */

#ifndef DECODEPHOTO_H_
#define DECODEPHOTO_H_
#include <jni.h>
#include <string.h>
#include <dlfcn.h>
typedef int * HMODULE;
typedef int (*Unpack)(const char*, const char*, int);
int Loaddl();
int CallWlt2Bmp(char* wltPath, char* bmpPath);
int CallWlt2BmpBylib(char* wltPath, char* bmpPath);
#endif /* DECODEPHOTO_H_ */
