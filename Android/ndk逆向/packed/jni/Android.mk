LOCAL_PATH := $(call my-dir)

# 编译 C 代码
include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_SRC_FILES := main.cpp
include $(BUILD_EXECUTABLE)