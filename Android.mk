LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie
# give module name
LOCAL_MODULE    := lights
# list your C files to compile
LOCAL_SRC_FILES := lights.c
# this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)
