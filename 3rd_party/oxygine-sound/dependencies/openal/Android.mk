LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := openal
# LOCAL_ARM_MODE   := arm
#LOCAL_PATH       := $(ROOT_PATH)
LOCAL_C_INCLUDES := \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/OpenAL32/Include \
                    $(LOCAL_PATH)/android_config
LOCAL_SRC_FILES  := \
                    OpenAL32/alAuxEffectSlot.c \
                    OpenAL32/alBuffer.c        \
                    OpenAL32/alEffect.c        \
                    OpenAL32/alError.c         \
                    OpenAL32/alExtension.c     \
                    OpenAL32/alFilter.c        \
                    OpenAL32/alListener.c      \
                    OpenAL32/alSource.c        \
                    OpenAL32/alState.c         \
                    OpenAL32/alThunk.c         \
                    Alc/ALc.c                  \
                    Alc/alcConfig.c            \
                    Alc/alcDedicated.c         \
                    Alc/alcEcho.c              \
                    Alc/alcModulator.c         \
                    Alc/alcReverb.c            \
                    Alc/alcRing.c              \
                    Alc/alcThread.c            \
                    Alc/ALu.c                  \
                    Alc/bs2b.c                 \
                    Alc/helpers.c              \
                    Alc/hrtf.c                 \
                    Alc/mixer.c                \
                    Alc/panning.c              \
                    Alc/backends/android.c     \
                    Alc/backends/opensl.c      \
                    Alc/backends/null.c      \
                    Alc/backends/loopback.c      \

LOCAL_CFLAGS     := -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_EXPORT_LDLIBS     := -lOpenSLES -llog -Wl,-s

include $(BUILD_STATIC_LIBRARY)