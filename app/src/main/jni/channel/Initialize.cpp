/*
 * Initialize.cpp
 *
 *  Created on: 2015-3-12
 *      Author: liuzg
 */
#include "Initialize.h"
JavaVM * mVM = NULL;

jint JNICALL JNI_OnLoad(JavaVM * vm, void * vp) {
	LOGI("===== JNI_OnLoad ======");
	mVM = vm;
	JNIEnv* env = NULL;

	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {

		LOGE("ERROR: GetEnv failed/n");
		return -1;
	}

	return JNI_VERSION_1_4;
}
void JNICALL JNI_OnUnload(JavaVM * vm, void * vp) {
	LOGI("===== JNI_OnUnload ======");
}
