LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)  
LOCAL_MODULE := wlttobmp
#LOCAL_SRC_FILES := wlttobmp.a
LOCAL_SRC_FILES := libidcard.a
include $(PREBUILT_STATIC_LIBRARY)