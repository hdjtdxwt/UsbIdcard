#ifndef ALAN_BASE64_H
#define ALAN_BASE64_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************
 * RETURNMALLOC	: 1, 用动态开辟内存返回
 *                0, 使用参数接收返回值
 */
#define RETURNMALLOC 1

/********************************************************
 * base64_encode: 标准base64编码函数
 * 		  返回值：编码结果字符串指针
 * @s		: 待编码的字符串指针
 */
char *base64_encode(char *s, int len);

/********************************************************
 * base64_decode: 标准base64解码函数
 * 		  返回值：解码结果字符串指针
 * @s		: 待解码的字符串指针
 */
char *base64_decode(char *s, int len);

#endif//ALAN_BASE64_H
