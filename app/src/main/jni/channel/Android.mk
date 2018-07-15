#
# Copyright 2009 Cedric Priscal
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. 
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := basechannel

LOCAL_SRC_FILES := \
BaseChannel.cpp \
Initialize.cpp \
 
LOCAL_C_INCLUDES := \
. \
${TOP_LOCAL_PATH}/common/ \

LOCAL_STATIC_LIBRARIES	:= common 
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
include $(BUILD_STATIC_LIBRARY)
