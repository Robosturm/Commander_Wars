LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := dependencies/openal-android/obj/local/$(TARGET_ARCH_ABI)/libopenal.a
include $(PREBUILT_STATIC_LIBRARY) 



include $(CLEAR_VARS)

LOCAL_MODULE    := oxygine-sound_static
LOCAL_MODULE_FILENAME := liboxygine-sound


SRC := src/sound

LOCAL_SRC_FILES := \
		$(SRC)/MemoryStream.cpp \
		$(SRC)/OggStream.cpp \
		$(SRC)/ResSound.cpp \
		$(SRC)/SoundHandle.cpp \
		$(SRC)/SoundInstance.cpp \
		$(SRC)/SoundPlayer.cpp \
		$(SRC)/SoundSystem.cpp \
		$(SRC)/WavStream.cpp \
		$(SRC)/oal/SoundHandleOAL.cpp \
		$(SRC)/oal/SoundOAL.cpp \
		$(SRC)/oal/SoundSystemOAL.cpp \
		$(SRC)/oal/StaticSoundHandleOAL.cpp \
		$(SRC)/oal/StaticStreamOAL.cpp \
		$(SRC)/oal/StreamingSoundHandleOAL.cpp \
		$(SRC)/null/SoundSystemNull.cpp \
		$(SRC)/null/SoundNull.cpp \
		$(SRC)/null/SoundHandleNull.cpp


LOCAL_CFLAGS := -DOXYGINE_SOUND=1

OXYGINE_SRC := $(LOCAL_PATH)/../oxygine-framework/oxygine/src

LOCAL_C_INCLUDES := $(OXYGINE_SRC)/ \
					$(LOCAL_PATH)/src \
					$(LOCAL_PATH)/dependencies/openal-android/jni/OpenAL/include \
					$(LOCAL_PATH)/dependencies/tremor/Tremor \
					$(LOCAL_PATH)/dependencies/libogg/include

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_EXPORT_CFLAGS += $(LOCAL_CFLAGS)
LOCAL_WHOLE_STATIC_LIBRARIES := libogg openal tremor
LOCAL_EXPORT_LDLIBS     := -lOpenSLES -ldl -llog

include $(BUILD_STATIC_LIBRARY)

$(call import-module, SDL)
$(call import-module, oxygine-sound/dependencies/libogg)
$(call import-module, oxygine-sound/dependencies/tremor)