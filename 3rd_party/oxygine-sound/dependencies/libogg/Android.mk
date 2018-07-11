LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libogg

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_SRC_FILES :=	src/framing.c \
					src/bitwise.c

include $(BUILD_STATIC_LIBRARY)