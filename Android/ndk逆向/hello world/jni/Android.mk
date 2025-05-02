LOCAL_PATH := $(call my-dir)

# 编译 C 代码
include $(CLEAR_VARS)
LOCAL_MODULE := helloworld
LOCAL_SRC_FILES := helloworld.s
include $(BUILD_EXECUTABLE)