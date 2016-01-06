#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#ifndef _UTILS_H

int init_server_socket_listen(int port,int type,struct sockaddr*);
int init_client_socket_connect(const char *ip,int port,int type);
int close_socket(int);
int server_socket_accept(int,struct sockaddr*);

int strfind(const char *src,const char *match);
void strsubstring(char *destin,const char *source,int start,int end);
long getcurenttime();
int file_function(void *buf,size_t size,const char *path,const char *type);

#define _UTILS_H
#endif
