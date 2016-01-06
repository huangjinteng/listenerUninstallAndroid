LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -std=c99
LOCAL_SRC_FILES:= \
	com_jason_uninstall_Uninstall.c \
	listen_file.c \
	utils.c 
LOCAL_LDLIBS += -L$(SYSTROOT)/usr/lib -llog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:=uninstall
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:=inotify_test.c
LOCAL_LDLIBS += -llog
LOCAL_MODULE_TAGS:=optional
LOCAL_MODULE:=inotify
LOCAL_STATIC_LIBRARIES:=libc
#LOCAL_MODULE_PATH:=$(TAGET_OUT_OPTIONAL_EXECUTABLES)
#LOCAL_MODULE_TAGS:=debug
include $(BUILD_SHARED_LIBRARY)

