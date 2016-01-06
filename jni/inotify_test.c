#include <stdio.h>
#include <android/log.h>
#include <unistd.h>

#define loge(...) __android_log_print(ANDROID_LOG_ERROR,"HJT",__VA_ARGS__)

int main(int arg,char **args)
{
	loge("hello");
	sleep(100);
	return 0;
}
