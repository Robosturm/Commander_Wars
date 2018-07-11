LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := tremor

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/tremor \
	$(LOCAL_PATH)/../libogg/include/
TREMOR_SRC := Tremor

LOCAL_SRC_FILES :=	\
		$(TREMOR_SRC)/block.c \
		$(TREMOR_SRC)/codebook.c \
		$(TREMOR_SRC)/floor0.c \
		$(TREMOR_SRC)/floor1.c \
		$(TREMOR_SRC)/info.c \
		$(TREMOR_SRC)/mapping0.c \
		$(TREMOR_SRC)/mdct.c \
		$(TREMOR_SRC)/registry.c \
		$(TREMOR_SRC)/res012.c \
		$(TREMOR_SRC)/sharedbook.c \
		$(TREMOR_SRC)/synthesis.c \
		$(TREMOR_SRC)/vorbisfile.c \
		$(TREMOR_SRC)/window.c \


include $(BUILD_STATIC_LIBRARY)