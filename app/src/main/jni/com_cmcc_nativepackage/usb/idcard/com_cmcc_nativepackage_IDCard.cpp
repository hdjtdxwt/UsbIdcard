/*
 * com_cmcc_nativepackage.cpp
 *
 *  Created on: 2015-3-12
 *      Author: liuzg
 */

#include "com_cmcc_nativepackage_IDCard.h"
#include "IDCardScanner_usb.h"
#include "Android_Log.h"
#include "common.h"

#define VID 0x0400
#define PID 0xC35A
/*
 * Class:     com_cmcc_nativepackage_IDCard
 * Method:    openIDCard
 * Signature: (ILjava/lang/String;Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_openIDCard(
		JNIEnv * env, jclass cls, jint type, jstring deviceid,
		jstring password) {
	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_openIDCard");

	char deviceidBuf[128] = { 0, };
	char passwordBuf[128] = { 0, };
	env->GetStringUTFRegion(deviceid, 0, env->GetStringUTFLength(deviceid),
			deviceidBuf);
	env->GetStringUTFRegion(password, 0, env->GetStringUTFLength(password),
			passwordBuf);
	return openIDCard_usb(type, deviceidBuf, passwordBuf);
}

/*
 * Class:     com_cmcc_nativepackage_IDCard
 * Method:    closeIDCard
 * Signature: ()I
 */JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_closeIDCard(JNIEnv *,
		jclass) {
	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_closeIDCard");
	return closeIDCard_usb();
}
/*
 * Class:     com_cmcc_nativepackage_IDCard
 * Method:    getIDCardVersion
 * Signature: ([B)I
 */JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_getIDCardVersion(
		JNIEnv * env, jclass cls, jbyteArray version) {
	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_getIDCardVersion");
	int ret = -1;
	int len = env->GetArrayLength(version);
	char* ver = (char*) malloc(len);
	ret = getIDCardVersion_usb(ver);
	if (ret == 0) {
		env->SetByteArrayRegion(version, 0, len, (jbyte*) ver);
	}
	free(ver);
	ver = NULL;
	return ret;
}
/*
 * Class:     com_cmcc_nativepackage_IDCard
 * Method:    initialIDCard
 * Signature: ()I
 */JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_initialIDCard(
		JNIEnv *env, jclass cls) {
	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_initialIDCard");
	return initialIDCard_usb();
}
/*
 * Class:     com_cmcc_nativepackage_IDCard
 * Method:    getIdCardInfo
 * Signature: ([Ljava/lang/String;[B)I
 */JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_getIdCardInfo(
		JNIEnv *env, jclass cls, jobjectArray idCardInfo, jbyteArray image) {
	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_getIdCardInfo");
	char *img = (char *) malloc(65530);
	memset(img, 0, 65530);
	char cardInfo[9][128];
	int ret = getIdCardInfo_usb(idCardInfo, img);
	LOGD("imgLen = %d", strlen(img));
	env->SetByteArrayRegion(image, 0, strlen(img), (jbyte*) img);
	LOGD("Java_com_cmcc_nativepackage_IDCard_getIdCardInfo:end");
	return ret;
}

JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_isOTGDevice(
 		JNIEnv *env, jclass cls) {
 	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_isOTGDevice");
 	jclass UsbHelper = env->FindClass("com/android/api/UsbHelper");
 	int ret = -1;
 	if (UsbHelper != NULL) {
 		jmethodID isOTGDevice = env->GetStaticMethodID(UsbHelper, "isOTGDevice",
 					"(II)I");
 		if (isOTGDevice != NULL) {

 			ret = env->CallStaticIntMethod(UsbHelper, isOTGDevice,PID,VID);
 			return ret;
 		}
 	}
 	return ret;
}

JNIEXPORT jint JNICALL Java_com_cmcc_nativepackage_UsbIDCard_getDevicePermission(
 		JNIEnv *env, jclass cls) {
 	LOGD(">> call Java_com_cmcc_nativepackage_IDCard_getDevicePermission");
 	jclass UsbHelper = env->FindClass("com/android/api/UsbHelper");
 	int ret = -1;
 	if (UsbHelper != NULL) {
 		jmethodID getPer = env->GetStaticMethodID(UsbHelper, "getDevicePermission",
 					"(II)I");
 		if (getPer != NULL) {

 			ret = env->CallStaticIntMethod(UsbHelper, getPer,PID,VID);
 			return ret;
 		}
 	}
 	return ret;
}



