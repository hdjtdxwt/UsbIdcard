LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#适用于外挂式USB二代证(新中新USB二代证)
LOCAL_MODULE    := CMCC_USB_IDCARD
LOCAL_SRC_FILES += \
com_cmcc_nativepackage_IDCard.cpp \

LOCAL_C_INCLUDES := \
. \
${TOP_LOCAL_PATH}/components/usb/IDCardScanner/ \
${TOP_LOCAL_PATH}/common/ \

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_STATIC_LIBRARIES	:= \
usbidcard \

include $(BUILD_SHARED_LIBRARY)