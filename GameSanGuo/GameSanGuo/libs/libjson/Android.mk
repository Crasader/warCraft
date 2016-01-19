LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := json_static

LOCAL_MODULE_FILENAME := libjson

APP_STL := gnustl_static  

APP_CPPFLAGS := -frtti

LOCAL_CPP_EXTENSION := .cpp

define walk
     $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef


ALL_FILES := $(call walk, $(LOCAL_PATH))
#$(info "ALL_FILES=" $(ALL_FILES))

SRC_LIST := $(filter %.cpp, $(ALL_FILES))
#$(info "SRC_LIST=" $(SRC_LIST))

HEAD_LIST := $(filter %.h, $(ALL_FILES))
#$(info "HEAD_LIST=" $(HEAD_LIST))

LOCAL_SRC_FILES :=  $(SRC_LIST:$(LOCAL_PATH)/%=%)
#$(info "LOCAL_SRC_FILES=" $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES := $(HEAD_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Source
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Source/JSONDefs
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Dependencies/mempool++
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Dependencies/libbase64++
#$(info "LOCAL_C_INCLUDES=" $(LOCAL_C_INCLUDES))

LOCAL_LDLIBS := -llog \  -lz
                                 
include $(BUILD_STATIC_LIBRARY)
