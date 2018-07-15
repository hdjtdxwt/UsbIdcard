/*
 * IDCardScanner.h
 *
 *  Created on: 2015-3-11
 *      Author: liuzg
 */

#ifndef IDCARDSCANNER_H_USB
#define IDCARDSCANNER_H_USB
#include <jni.h>

int FindIDCard();
int SelectIDCard();
int ReadIDCard();
int decodeIDCardData(jobjectArray idCardInfo, char * img, char * data);

/** 连接二代证扫描设备 */
extern int openIDCard_usb(int idCardType,char* deviceId,char* password);
/** 关闭与 读卡器设备的连接 */
extern int closeIDCard_usb();
/** 获取各厂商 读卡器组件的版本信息 */
extern int getIDCardVersion_usb(char* version);
/** 初始化二代证扫描设备,清除缓存数据 */
extern int initialIDCard_usb ();
/** 获得二代证扫描设备数据。 */
//extern int getIdCardInfo(char idCardInfo[][128], char* img);
extern int getIdCardInfo_usb(jobjectArray cardInfo, char* img);
#endif /* IDCARDSCANNER_H_USB */
