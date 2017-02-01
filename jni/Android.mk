LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := fmod
LOCAL_SRC_FILES := libfmod.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := fmodL
LOCAL_SRC_FILES := libfmodL.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := sound
LOCAL_SRC_FILES := supersound.cpp
LOCAL_SHARED_LIBRARIES := fmod fmodL
LOCAL_LDLIBS := -llog
LOCAL_CPP_FEATURES :=exceptions
include $(BUILD_SHARED_LIBRARY)
