/*
 * Initialize.cpp
 *
 *  Created on: 2015-3-12
 *      Author: liuzg
 */
#include "Initialize.h"
//#include "IDCardScanner.h"
#include "com_cmcc_nativepackage_IDCard.h"
JavaVM * mVM = NULL;
const char *idcard_class_path = "com/cmcc/nativepackage/IDCard";

JNINativeMethod idcalrd_nm[1] = { { "openIDCard", "(ILjava/lang/String;Ljava/lang/String;)I", (void*) Java_com_cmcc_nativepackage_IDCard_openIDCard } };

jint JNICALL JNI_OnLoad(JavaVM * vm, void * vp) {
	LOGI("===== JNI_OnLoad ======");
	LOGD("JNI_OnLoad vm=%x",vm);
	mVM = vm;
	JNIEnv* env = NULL;

	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {

		LOGE("ERROR: GetEnv failed/n");
		return -1;
	}
	// Find the class calling native function
	jclass NativeUsb = env->FindClass(idcard_class_path);
	if (NativeUsb == NULL) {
		LOGE("FindClass failed : No class found.");
		return -1;
	}
	// Register native method for getUsbPermission

	if (env->RegisterNatives(NativeUsb, idcalrd_nm, sizeof(idcalrd_nm) / sizeof(JNINativeMethod))) {
		LOGE("RegisterNatives Failed.");
		return -1;
	}

	return JNI_VERSION_1_4;
}
void JNICALL JNI_OnUnload(JavaVM * vm, void * vp) {
	LOGI("===== JNI_OnUnload ======");
	JNIEnv *env = NULL;
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4)) {
		return;
	}
	jclass cls = env->FindClass(idcard_class_path);
	if (cls == NULL) {
		return;
	}
	jint nRes = env->UnregisterNatives(cls);
	return;
}
