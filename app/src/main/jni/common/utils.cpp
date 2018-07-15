/*
 * utils.cpp
 *
 *  Created on: 2013-12-10
 *      Author: liuzg
 */
#include "utils.h"
#include <ctype.h>
#include "android_log.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * ʮ�����ASCII�ַ������ֽڣ�ת��һ���������ֽ�
 */
unsigned char hexToByte(char *str) {
	if (strlen(str) < 2)
		return 0;

	unsigned char h = *str;
	h = toupper(h);
	unsigned char l = *(str + 1);
	l = toupper(l);
	if (l >= '0' && l <= '9')
		l -= '0';
	else if (l >= 'A' && l <= 'F') {
		l -= 'A';
		l += 10;
	} else
		l = 0;
	if (h >= '0' && h <= '9')
		h -= '0';
	else if (h >= 'A' && h <= 'F') {
		h -= 'A';
		h += 10;
	} else
		h = 0;
	h = (unsigned char) (h << 4);
	return (unsigned char) (h | l);
}
/*
 * ʮ�����ASCII�ַ�����ֽڣ�ת�ɶ������ֽ�
 */
unsigned char * hexToBytes(char* str) {
	int bytesLen = strlen(str) / 2;
	if (bytesLen < 1)
		return NULL;
	unsigned char * ret = (unsigned char *) malloc(bytesLen);
	char singleHex[] = { 0, 0 };
	for (int i = 0; i < bytesLen; i++) {
		memcpy(singleHex, str + 2 * i, 2);
		ret[i] = hexToByte(singleHex);
	}
	return ret;
}
/*
 * ��ָ�����ȵ�ʮ�����ASCII�ַ�����ֽڣ�ת�ɶ������ֽ�
 */
unsigned char * hexToBytesByLen(char* str, int len) {
	char * temp = (char*) malloc(len + 1);
	strncpy(temp, str, len);
	temp[len] = 0;
	unsigned char * ret = hexToBytes(temp);
	free((void*) temp);
	return ret;
}
/*
 * һ���������ֽ�ת���ɶ����ֽڳ��ȵ�ʮ����Ƶ�ASCII�ַ�
 */
unsigned char *byteToHex(unsigned char ch) {
	unsigned char str[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'A', 'B', 'C', 'D', 'E', 'F' };
	unsigned char *hex = (unsigned char*) malloc(3);
	hex[0] = str[ch >> 4 & 0xF];
	hex[1] = str[ch & 0xF];
	hex[2] = 0;
	return hex;
}
/*
 * ����������ֽ�ת������ֽڳ��ȵ�ʮ����Ƶ�ASCII�ַ�
 */

unsigned char *bytesToHex(unsigned char *bytes) {
	unsigned char *ret = (unsigned char*) malloc(
			strlen((const char*) bytes) * 2 + 1);
	ret[strlen((const char*) bytes) * 2] = 0;
	for (int i = 0; i < strlen((const char*) bytes); i++) {
		unsigned char *hex = byteToHex(*(bytes + i));
		ret[2 * i] = hex[0];
		ret[2 * i + 1] = hex[1];
		free(hex);
	}
	return ret;
}
/*
 * ָ�����ȵĶ���������ֽ�ת������ֽڳ��ȵ�ʮ����Ƶ�ASCII�ַ�
 */

unsigned char *bytesToHexByLen(unsigned char *bytes, int len) {
	unsigned char *ret = (unsigned char*) malloc(len * 2 + 1);
	ret[len * 2] = 0;
	for (int i = 0; i < len; i++) {
		unsigned char *hex = byteToHex(*(bytes + i));
		ret[2 * i] = hex[0];
		ret[2 * i + 1] = hex[1];
		free(hex);
	}
	return ret;
}

#ifdef __cplusplus
}
#endif

