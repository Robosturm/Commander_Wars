LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := oxygine-flow_static
LOCAL_MODULE_FILENAME := liboxygine-flow

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../oxygine-framework/oxygine/src/ \
					$(LOCAL_PATH)/../SDL/include \


LOCAL_SRC_FILES :=  src/flow/flow.cpp \
					src/flow/Transition.cpp \
					src/flow/Scene.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src/

include $(BUILD_STATIC_LIBRARY)
