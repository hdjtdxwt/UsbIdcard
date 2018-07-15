/*
 * BaseChannel.cpp
 *
 *  Created on: 2015-3-11
 *      Author: liuzg
 */
#include <stdlib.h>
#include "unistd.h"
#include "BaseChannel.h"
#include "Android_Log.h"
extern JavaVM *mVM;
BaseChannel::BaseChannel() {
	LOGD("BaseChannel::BaseChannel mVM=%x", mVM);
	Init();
}
BaseChannel::~BaseChannel() {
	LOGD("BaseChannel::~BaseChannel");
	CloseChannel(true);

}
void BaseChannel::Init() {
	mJNIEnv = GetJNIEnv(true);
	isConnected = false;
	isEnterDev = false;
	hasMCU = true;
	totalLen = 0;
	memset(buffer, 0, sizeof(buffer));

}
void BaseChannel::SetJNIEnv(JNIEnv * env) {
	mJNIEnv = env;
}
JNIEnv *BaseChannel::GetJNIEnv(bool IsBindThread) {

	if (IsBindThread) {
		JNIEnv *ThreadEnv = NULL;
		if (mVM != NULL)
			mVM->AttachCurrentThread(&ThreadEnv, NULL);
		return ThreadEnv;

	}
	return mJNIEnv;
}
/**
 *打开设备通道
 *打开成功，返回1
 *打开失败，返回0
 */
int BaseChannel::OpenChannel(int device, char * Addr, bool hasMCU) {
	LOGD(">> call BaseChannel::OpenChannel()");
	this->hasMCU = hasMCU;
	int ret = -1;
	if (isConnected || OpenConnect(device, Addr) > 0) {

		if (hasMCU) {
			ret = EnterDev(device);
			isEnterDev = (ret >= 0 ? true : false);

		} else {
			isEnterDev = true;
			ret = 1;

		}

	}
	LOGD("BaseChannel::OpenChannel(%d) %s", device,
			(ret == 1) ? "ok" : "failed");
	return ret;
}
void BaseChannel::CloseChannel(bool exitMCU) {
	if (isConnected) {
		if (isEnterDev && hasMCU && exitMCU)
			EnterDev(-1);
		CloseConnect();
		isEnterDev = false;
	}

}
/**
 * 设备切换，用于MCU进行串口切换用
 */
int BaseChannel::EnterDev(int device) {
	LOGD(">> call BaseChannel::EnterDev(%d)", device);
	int ret = -1;
	char cmd[10] =
			{ 0xDD, 0x4E, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	switch (device) {
	case 1:  //密码键盘  57600  DD 4E 4C 00 00 01 00 E1 00 E2
//		cmd = {0xDD,0x4E,0x4C,0x00,0x00,0x01,0x00,0xE1,0x00,0xE2};
		cmd[5] = 1;
		cmd[6] = 0;
		cmd[7] = 0xE1;
		cmd[8] = 0;
		cmd[9] = 0xE2;
		break;
	case 2: //打印机
//		cmd = { 0xDD,0x4E,0x4C,0x00,0x00,0x02,0x01,0xC2,0x00,0xC5};
		cmd[5] = 2;
		cmd[6] = 0;
		cmd[7] = 0x25;
		cmd[8] = 0x80;
		cmd[9] = 0xA7;
//		cmd[6] = 0x01;
//		cmd[7] = 0xC2;
//		cmd[8] = 0x00;
//		cmd[9] = 0xC5;
		break;
	case 3: //IC卡和银联刷卡
//		cmd = {0xDD,0x4E,0x4C,0x00,0x00,0x04,0x01,0xC2,0x00,0xC7};
		cmd[5] = 4;
		cmd[6] = 1;
		cmd[7] = 0xC2;
		cmd[8] = 0;
		cmd[9] = 0xC7;
		break;
	case 0: // 二代证扫描
	case Channel_ID_Scanner_CTS: //外接华视二代证，不通过MCU
//		cmd = {0xDD,0x4E,0x4C,0x00,0x00,0x00,0x01,0xC2,0x00,0xC3};
		cmd[5] = 0;
		cmd[6] = 1;
		cmd[7] = 0xC2;
		cmd[8] = 0;
		cmd[9] = 0xC3;
		break;
	case 111: //密码键盘 115200  DD 4E 4C 00 00 01 01 C2 00 C4
//		cmd = {0xDD,0x4E,0x4C,0x00,0x00,0x01,0x01,0xC2,0x00,0xC4};
		cmd[5] = 1;
		cmd[6] = 1;
		cmd[7] = 0xC2;
		cmd[8] = 0;
		cmd[9] = 0xC4;
		break;
	case -1:
	default: //退出
//		cmd = {0xDD,0x4E,0x4C,0x00,0x00,0xFF,0x01,0xC2,0x00,0xC2};
		cmd[5] = 0xFF;
		cmd[6] = 1;
		cmd[7] = 0xC2;
		cmd[8] = 0;
		cmd[9] = 0xC2;
		break;
	}
	JNIEnv *m_Env = this->GetJNIEnv(true);
	jbyteArray cmdByteArray = m_Env->NewByteArray(10);
	m_Env->SetByteArrayRegion(cmdByteArray, 0, 10, (jbyte*) cmd);
	Write(cmdByteArray);
	m_Env->ReleaseByteArrayElements(cmdByteArray, (jbyte*) cmd, JNI_COMMIT);
	m_Env->DeleteLocalRef((jobject) cmdByteArray);
	if (CheckResponse(400) > 0) {
		ReadResp();
		LOGI("BaseChannel::EnterDev(),ReadResp() return data, totalLen : %d",
				totalLen);
		if (device != Channel_ID_Scanner_CTS) {
			if (totalLen >= 10 && (buffer[3] & 0xFF) == 0xFF
					&& (buffer[4] & 0xFF) == 0x00) {
				LOGD("BaseChannel::EnterDev() buffer[3]:%d buffer[4]:%d",
						buffer[3], buffer[4]);
				ret = 1;
			} else {
				LOGE("BaseChannel::EnterDev()  buffer[3]:%d buffer[4]:%d",
						buffer[3], buffer[4]);
				ret = -1;
			}
			//ret=0;
		} else {
			if (totalLen >= 10 && (buffer[0] & 0xFF) == 0x4E
					&& (buffer[1] & 0xFF) == 0x4C &&(buffer[2] & 0xFF)==0xFF ) {
				LOGD("BaseChannel::EnterDev() buffer[0]:%d buffer[1]:%d",
						buffer[0], buffer[1]);
				ret = 1;
			} else {
				LOGE("BaseChannel::EnterDev()  buffer[0]:%d buffer[1]:%d",
						buffer[0], buffer[1]);
				ret = -1;
			}
			//ret=0;
		}

	} else {
		LOGE("BaseChannel::EnterDev(),CheckResponse()<=0,  no response");
	}
	LOGI("BaseChannel::EnterDev(%d)=%s", device, (ret == 1 ? "TRUE" : "FALSE"));
//	usleep(400000);//做延时，否则会影响后面发给模块的数据
	return ret;

}
void BaseChannel::Buzzer(int time, int period) {
	if (!hasMCU)
		return;
	int hight = period / 256 / 256;
	int middle = period / 256 & 0x00FF;
	int low = period % 256;
	char cmd[10] = { 0xDD, 0x4E, 0x4C, 0x55, 0x00, time, hight, middle, low,
			0x00 };
	cmd[9] = time + hight + middle + low;
	JNIEnv * m_Env = this->GetJNIEnv(true);
	jbyteArray cmdByteArray = m_Env->NewByteArray(10);
	m_Env->SetByteArrayRegion(cmdByteArray, 0, 10, (jbyte*) cmd);
	Write(cmdByteArray);
	m_Env->ReleaseByteArrayElements(cmdByteArray, (jbyte*) cmd, JNI_COMMIT);
	m_Env->DeleteLocalRef((jobject) cmdByteArray);
}
