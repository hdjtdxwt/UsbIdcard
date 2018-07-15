LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
# define so output folder
#NDK_APP_DST_DIR := libs/$(TARGET_ARCH_ABI)/usb

LOCAL_MODULE    := usbidcard
LOCAL_SRC_FILES += \
IDCardScanner.cpp \
DecodePhoto.cpp \

LOCAL_C_INCLUDES := \
. \
$(TOP_LOCAL_PATH)/common/ \
$(TOP_LOCAL_PATH)/channel/ \
$(TOP_LOCAL_PATH)/components/usb/channel/ \
$(TOP_LOCAL_PATH)/components/usb/decode_wlt64/ \ 

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_STATIC_LIBRARIES	:= \
usbchannel \
wlttobmp \

include $(BUILD_STATIC_LIBRARY)


