LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:=libecc608

LOCAL_SRC_FILES:= \
	libecc608_util.c \
	libecc608_api.c \
	verify.c

LOCAL_MODULE_TAGS:=optional debug

LOCAL_SHARED_LIBRARIES:= \
	libutils \
	libcutils \
	libdl \

LOCAL_STATIC_LIBRARIES:= \
		libcrypto_igs \
		libssl_igs \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE:=libecc608

LOCAL_SRC_FILES:= \
	libecc608_util.c \
	libecc608_api.c \
	verify.c

LOCAL_MODULE_TAGS:=optional debug

LOCAL_SHARED_LIBRARIES:= \
	libutils \
	libcutils \
	libdl \

LOCAL_STATIC_LIBRARIES:= \
		libcrypto_igs \
		libssl_igs \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(BUILD_STATIC_LIBRARY)
