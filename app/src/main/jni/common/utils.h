/*
 * utils.h
 *
 *  Created on: 2013-12-10
 *      Author: liuzg
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned char hexToByte(char *str);
unsigned char * hexToBytes(char* str);
unsigned char * hexToBytesByLen(char* str,int len);
unsigned char *byteToHex(unsigned char ch);
unsigned char *bytesToHex(unsigned char *bytes);
unsigned char *bytesToHexByLen(unsigned char *bytes,int len);
#ifdef __cplusplus
}
#endif
#endif /* UTILS_H_ */
