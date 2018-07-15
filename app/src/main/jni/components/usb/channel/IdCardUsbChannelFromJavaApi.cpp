/*
 * IdCardUsbChannelFromJavaApi.cpp
 *
 *  Created on: 2015-3-16
 *      Author: liuzg
 */
#include "IdCardUsbChannelFromJavaApi.h"
#include "Android_Log.h"
#include <unistd.h>
#define VID 0x0400
#define PID 0xC35A
#define ENDPOINT_OUTPUT_1 2
#define ENDPOINT_OUTPUT_2 6
#define ENDPOINT_INPUT_1 1
#define ENDPOINT_INPUT_2 5
UsbChannel::UsbChannel() {
	LOGD("UsbChannel::UsbChannel");
	this->mChannelType = type_usb;
}
UsbChannel::~UsbChannel() {
	LOGD("UsbChannel::~UsbChannel");
	CloseChannel(true);

}

/**
 * 打开usb连接
 * 打开成功，返回1
 * 打开失败，返回0
 */
int UsbChannel::OpenConnect(int deviceid, char* addr) {
	LOGD(">> call OpenConnect()");
	CloseConnect();
	JNIEnv *env = this->GetJNIEnv(true);
	jclass UsbHelper = env->FindClass("com/android/api/UsbHelper");
	if (UsbHelper != NULL) {
		jmethodID openDevice = env->GetStaticMethodID(UsbHelper, "openDevice",
				"(II)I");
		if (openDevice != NULL) {
			int ret = 0;
			ret = env->CallStaticIntMethod(UsbHelper, openDevice, PID, VID);
			LOGD("UsbChannel::OpenConnect() return %s",
					ret == 1 ? "true" : "false");
			if (ret == 1) {
				mUsbHelper = (jclass) env->NewGlobalRef(UsbHelper);
				isConnected = true;
			}

		}
	}



	env = this->GetJNIEnv(true);
	UsbHelper = env->FindClass("com/android/api/UsbHelper");
	if(UsbHelper == NULL){
		LOGE("UsbHelper == NULL");
	}
	jmethodID isgetpermisonID = env->GetStaticMethodID(UsbHelper,
			"getIsGetPermision", "()I");
	if (NULL == isgetpermisonID) {
		LOGE("isgetpermisonID == NULL");
	}

	int times_ret = 1000;
	int tt = 0;
	while (times_ret > 0) {
		int ret_isgetPermision = 0;

		if (NULL != UsbHelper) {
//			LOGD("获取权限：times_ret = %d", times_ret);
			ret_isgetPermision = env->CallStaticIntMethod(
					UsbHelper, isgetpermisonID);

			times_ret = times_ret - 1;
//			LOGD("获取权限：ret_isgetPermision = %d", ret_isgetPermision);
			tt = ret_isgetPermision;
			if (tt == 1) {
				break;
			}
			usleep(10000);
		} else {
			LOGE("NULL == UsbHelper");
		}
	}
	if (tt == 0) {
		LOGE("ret_isgetPermision == %d,权限获取超时", tt);
		return 0;
	}

	jmethodID isopenedCardReaderID = env->GetStaticMethodID(
			UsbHelper, "getIsCardOpened","()I");
	if (NULL == isopenedCardReaderID) {
		LOGE("isopenedCardReaderID == NULL");
	}

	int times_ret_open = 1000;
	int tt2 = 0;
	while (times_ret_open > 0) {
		int ret_isopenedReader = 0;
		if (NULL != UsbHelper) {
			LOGD("检测是否打开：times_ret_open = %d", times_ret_open);
			ret_isopenedReader = env->CallStaticIntMethod(
					UsbHelper, isopenedCardReaderID);

			times_ret_open = times_ret_open - 1;
			LOGD("检测是否打开：ret_isopenedReader = %d", ret_isopenedReader);
			tt2 = ret_isopenedReader;
			if (tt2 == 1) {
				break;
			}
			usleep(10000);
		} else {
			LOGE("NULL == simCardCls->simCardBuss.ret_isopenedReader");
		}
	}

	if (tt2 == 0) {
		LOGE("ret_isopenedReader == %d,获取打开状态超时", tt2);
		return -1;
	}

	if (tt2 == -1) {
		LOGE("ret_isopenedReader == %d,读卡器未打开", tt2);
		return -1;
	}

	LOGD("UsbChannel::OpenConnect() return false");
	return 1;

}
void UsbChannel::CloseConnect() {
	if (isConnected) {
		isConnected = false;
		this->GetJNIEnv(true)->DeleteGlobalRef(mUsbHelper);
	}

}
void UsbChannel::Write(jbyteArray data) {
	if (!isConnected)
		return;
	LOGD(">> call UsbChannel::Write() ");
	JNIEnv *env = this->GetJNIEnv(true);
	jmethodID writeData = env->GetStaticMethodID(mUsbHelper, "writeData",
			"(II[BI)I");
	if (writeData != NULL) {
		int ret = 0;
		int data_len = env->GetArrayLength(data);
		jbyte*buf = (jbyte*) malloc(data_len);
		env->GetByteArrayRegion(data, 0, data_len, buf);
		ret = env->CallStaticIntMethod(mUsbHelper, writeData, PID, VID, data,
				data_len);
		LOGD("UsbChannel::Write() writeData return %d", ret);
		free(buf);
	} else {
		LOGD("UsbChannel::Write() failed, writeData=NULL");
	}

}

void UsbChannel::ReadResp() {
	memset(this->buffer,0,sizeof(this->buffer));
	this->totalLen=0;
	if (!isConnected)
		return;
	JNIEnv *env = this->GetJNIEnv(true);
	jmethodID readData = env->GetStaticMethodID(mUsbHelper, "readData",
			"(II[BII)I");
	if (readData != NULL) {
		char buf[2048] = { 0, };
		jbyteArray data=env->NewByteArray(2048);
		int ret = env->CallStaticIntMethod(mUsbHelper, readData, PID, VID, data,
				sizeof(buf),3000);
		if (ret != -1) {
			env->GetByteArrayRegion(data,0,ret,(jbyte*)buf);
			this->totalLen = ret;
			memcpy(buffer, buf, ret);
			LOGD("UsbChannel::ReadResp() ok,totalLen=%d", totalLen);
			return;
		}
	}
	LOGD("UsbChannel::ReadResp() readData failed");
}

void UsbChannel::ReadResp(int timeout) {
	memset(this->buffer,0,sizeof(this->buffer));
	this->totalLen=0;
	if (!isConnected)
		return;
	JNIEnv *env = this->GetJNIEnv(true);
	jmethodID readData = env->GetStaticMethodID(mUsbHelper, "readData",
			"(II[BII)I");
	if (readData != NULL) {
		char buf[2048] = { 0, };
		jbyteArray data=env->NewByteArray(2048);
		int ret = env->CallStaticIntMethod(mUsbHelper, readData, PID, VID, data,
				sizeof(buf),timeout);
		if (ret != -1) {
			env->GetByteArrayRegion(data,0,ret,(jbyte*)buf);
			this->totalLen = ret;
			memcpy(buffer, buf, ret);
			LOGD("UsbChannel::ReadResp() ok,totalLen=%d", totalLen);
			return;
		}
	}
	LOGD("UsbChannel::ReadResp() readData failed");
}
int UsbChannel::CheckResponse(int timeout) {
	return 1; //永远默认有数据返回
}
