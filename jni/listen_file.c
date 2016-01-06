#include "listen_file.h"

int file_inotify_init()
{

	int fileDescriptor = inotify_init();
	if(fileDescriptor < 0){
		return -1;
	}
	printf("fileDescriptor = %d\n",fileDescriptor);
	//loge("file_inotify_init fileDescriptor = %d",fileDescriptor);
	return fileDescriptor;
}

int file_inotify_watch(int file_descriptor,int type,const char *path,void(*inotify_callback)(void *))
{
	void *p_buf = malloc(sizeof(struct inotify_event));
	if(p_buf == NULL){
		free(p_buf);
		return -1;
	}
	int maskStrLength = 7 + 10 + 1;
	char *p_markStr = (char*)malloc(maskStrLength);
	if(p_markStr == NULL){
		free(p_markStr);
		return -1;
	}
	int watchDescriptor = inotify_add_watch(file_descriptor,path,IN_ALL_EVENTS);
	if(watchDescriptor < 0){
		return watchDescriptor;
	}
	while(1){
		size_t readByte = read(file_descriptor,p_buf,sizeof(struct inotify_event));///((struct inotify_event*)p_buf)->mask
		snprintf(p_markStr,maskStrLength,"%x",(int*)((struct inotify_event*)p_buf)->mask);
		if((int)strtol(p_markStr,NULL,16) == type){
				file_inotify_rm(file_descriptor);
			if(inotify_callback != NULL){
				(*inotify_callback)((void*)path);
			}
			loge("haved been delete!");
			return watchDescriptor;
		}
	}
}

int file_inotify_rm(int file_descriptor)
{
	if(file_descriptor < 0)return -1;
	return inotify_rm_watch(file_descriptor,IN_ALL_EVENTS);
}

int check_file(const char *path)
{
	int status = -1;
	char *p = (char*)malloc(sizeof(char) * 1024);
	memset(p,0,sizeof(char) * 1024);
	strcat(p,path);
	strcat(p,"/");
	strcat(p,LOCAL_PORT_FILE);
	FILE *fd = fopen(p,"r");
	if(fd == NULL){
		return -2;
	}

	fread(p,sizeof(char) * 1024,1,fd);
	fflush(fd);
	fclose(fd);
	status = atoi(p);
	free(p);
	return status;
}

int save_file(const char *path,int port)
{
	FILE *fd = fopen(path,"w");
	if(fd <= 0){
		return -1;
	}
	char string[25];
	sprintf(string,"%d",port);
	fwrite(string,sizeof(int),1,fd);
	fflush(fd);
	fclose(fd);
	return 1;
}
