LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := freetype2_static
LOCAL_MODULE_FILENAME := freetype2
LOCAL_SRC_FILES := freetype/android/libs/$(TARGET_ARCH_ABI)/libfreetype.a
include $(PREBUILT_STATIC_LIBRARY) 


include $(CLEAR_VARS)

LOCAL_MODULE := oxygine-freetype_static
LOCAL_MODULE_FILENAME := liboxygine-freetype

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../oxygine-framework/oxygine/src/ \
					$(LOCAL_PATH)/../SDL/include \
					$(LOCAL_PATH)/freetype/android/include


LOCAL_SRC_FILES :=  src/ResFontFT.cpp


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src/

include $(BUILD_STATIC_LIBRARY)