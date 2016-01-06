#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <errno.h>

#ifndef _FILE_LISTENER_H
#define _FILE_LISTENER_H

#define loge(...) __android_log_print(ANDROID_LOG_ERROR,"HJT",__VA_ARGS__)

#define LOCAL_PORT_FILE "port.t"

int file_inotify_init();

int file_inotify_watch(int file_descriptor,int type,const char *path,void(*inotify_callback)(void *));

int file_inotify_rm(int file_descriptor);

int check_file(const char*);

int save_file(const char*,int);

#endif
