LOCAL_PATH:= $(call my-dir)


#include $(CLEAR_VARS)

#LOCAL_MODULE_CLASS:=SHARED_LIBRARIES
#LOCAL_MODULE_SUFFIX:=.so
#LOCAL_MODULE_TAGS   := optional
#LOCAL_MODULE:=libio_mem
#LOCAL_SRC_FILES :=libio_mem.so
#LOCAL_MODULE_PATH   := $(PRODUCT_OUT)/system/lib64

#include $(BUILD_PREBUILT)

include $(CLEAR_VARS)

LOCAL_MODULE:=ecc608_sample

LOCAL_SRC_FILES:=ecc608_sample.c 

LOCAL_MODULE_TAGS:=optional debug

LOCAL_SHARED_LIBRARIES:= \
	libutils \
	libcutils \
	libecc608 \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/

include $(BUILD_EXECUTABLE)



