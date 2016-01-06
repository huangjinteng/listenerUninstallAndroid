#include "utils.h"
#include <errno.h>
#include "listen_file.h"

/******************************	SOCKET FUNC***********************************************************/
#define loge(...) __android_log_print(ANDROID_LOG_ERROR,"HJT",__VA_ARGS__)
int init_server_socket_listen(int port,int type,struct sockaddr *add)
{
	struct sockaddr_in s_add;
	int sfp = 0;

	sfp = socket(AF_INET,type,0);
	if(sfp == -1)
	{
		printf("socket fail!\n");
		loge("socket fail!");
		return -1;
	}
	memset(&s_add,0,sizeof(s_add));
	s_add.sin_family = AF_INET;
	s_add.sin_addr.s_addr = htonl(INADDR_ANY);
	s_add.sin_port = htons(port);

	if(-1 == bind(sfp,(struct sockaddr *)(&s_add),sizeof(struct sockaddr)))
	{
		printf("bind fail!\n");
		loge("bind fail!,bind = %s",strerror(errno));
		return -1;
	}
	if(-1 == listen(sfp,5))
	{
		printf("listen fail!\n");
		loge("listen fail!");
		return -1;
	}

	return sfp;
}

int server_socket_accept(int sfp,struct sockaddr *c_add)
{
	int nfp = 0;
	unsigned int sin_size = 0;

	sin_size = sizeof(struct sockaddr_in);

	nfp = accept(sfp,(struct sockaddr*)(c_add),&sin_size);

	if(-1 == nfp)
	{
		printf("accept fail!\n");
		return -1;
	}
	return nfp;
}

int init_client_socket_connect(const char *ip,int port,int type)
{
	int cfp = 0,sin_size = 0,connect_np;
	struct sockaddr_in c_add;

	cfp = socket(AF_INET,type,0);
	if(cfp == -1)
	{
		printf("socket fail!\n");
		return cfp;
	}

	memset(&c_add,0,sizeof(c_add));

	c_add.sin_family = AF_INET;
	c_add.sin_addr.s_addr = inet_addr(ip);
	c_add.sin_port = htons(port);

	sin_size = sizeof(struct sockaddr);

	if(-1 == (connect_np = connect(cfp,(struct sockaddr*)(&c_add),sin_size)))
	{
		printf("connect fail!\n");
		return -1;
	}
	printf("connect succeed!\n");
	return cfp;
}

int close_socket(int fd)
{
	int ret = 0;
	close(fd);
	return ret;
}

/****************************************************************************************************/

/******************************STRING FUNC***********************************************************/

int strfind(const char *src,const char *match)
{
	int find = 0;
	if(src == NULL || match == NULL )
		return -1;
	int len1 = strlen(src),len2 = strlen(match);
	printf("len1 = %d,len1 = %d\n",len1,len2);
	if(len1 < len2)
		return -1;
	for(int i = 0;i < len2;){
		for(int j = 0;j < len1;j++){
			if((char)*(src + j) == (char)*(match + i)){
				if(i == (len2 - 1)){
					return j - (len2 - 1);
				}
				i++;
			}else if(j == (len1 - 1)){
				return -1;
			}
		}
	}
}

void strsubstring(char *destin,const char *source,int start,int end)
{
	if(destin == NULL || source == NULL || strlen(source) == 0 || start > end)
		return;
	memset(destin,0,sizeof destin);
	int len = end - start + 1;
	char temp[len + 1];
	memset(temp,0,len + 1);
	for(int i = 0;i < end;i++){
		temp[i] = (char)*(source + (start + i));
	}
	temp[len + 1] = '\0';
	strcpy(destin,temp);
}

long getcurenttime()
{
  	struct timeval tv;
   	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000;
}

/*****************************************************************************************/

int file_function(void *buf,size_t size,const char *path,const char *type)
{
	if(buf == NULL || path == NULL || type == NULL)
		return -1;
	FILE *fd = fopen(path,type);
	if(fd == NULL)
		return -1;
	if(!strcmp(type,"r")){
		fseek(fd,0,SEEK_SET);
		int status = fread(buf,size,1,fd);
		fclose(fd);
		return status;
	}else if(!strcmp(type,"w")){
		fseek(fd,0,SEEK_SET);
		int status = fwrite(buf,size,1,fd);
		fflush(fd);
		fclose(fd);
		return status;
	}else if(!strcmp(type,"a")){
		fseek(fd,0,SEEK_SET);
		int status = fwrite(buf,size,1,fd);
		fflush(fd);
		fclose(fd);
		return status;
	}
	return -1;
}
