#include "com_jason_uninstall_Uninstall.h"

/*  */
static char *browser_path;
static char *port_file;
static int server_no;

JNIEXPORT jint JNICALL Java_com_jason_uninstall_Uninstall_listenerUninstall(
		JNIEnv *env, jclass c, jstring usr_, jstring name_, jstring url_)
{

	const char *usr = (*env)->GetStringUTFChars(env, usr_, JNI_FALSE);
	const char *path = (*env)->GetStringUTFChars(env, name_, JNI_FALSE);
	const char *url = (*env)->GetStringUTFChars(env, url_, JNI_FALSE);

	//loge("path = %s,url = %s", path, url);

	port_file = (char*)malloc(sizeof(char) * 1024);
	memset(port_file,0,sizeof(char) * 1024);
	if(port_file == NULL){
		free(port_file);
		return -1;
	}
	strcat(port_file,path);
	strcat(port_file,"/");
	strcat(port_file,LOCAL_PORT_FILE);

	browser_path = (char*)malloc(sizeof(char) * 1024);
	memset(browser_path,0,sizeof(char) * 1024);
	if(usr == NULL || !strcmp(usr,"")){
		strcat(browser_path,"am start -a android.intent.action.VIEW -d ");
		strcat(browser_path,url);
	}else{
		strcat(browser_path,"am start --user ");
		strcat(browser_path,usr);
		//strcat(browser," -n com.android.browser/com.android.browser.BrowserActivity -d ");
		strcat(browser_path," -a android.intent.action.VIEW -d ");
		strcat(browser_path,url);
	}

	int sock = 0;
	if (0 > check_server_port()) { // check the file (save portNumber)
		ForkChild:;
		pid_t pid;
		int fd[2];
		pipe(fd);
		pid = fork();
		if (pid == 0) {
			pthread_t pthread_port;

			pthread_create(&pthread_port,NULL,interaction_port_file,port_file);// listen port file

			// process communication
			start_server(fd[1],path);
			free(port_file);
		} else {
			start_client(fd[0]);
		}
		waitpid(pid, NULL, 0);
	}else{
		// nothing to do
		//start_client(NO_CHECK_PIPE);
		return sock;
	}
}

int check_server_port()
{
	int client =  init_client_socket_connect("127.0.0.1", PORT,SOCK_STREAM);
	if(client > 0)
	{
		close(client);
		return client;
	}else
	{
		return -1;
	}
}

//	int ret = system("am start --user 0 -a android.intent.action.VIEW -d http://www.baidu.com");
//	loge("ret = %d", ret);

JNIEXPORT void JNICALL Java_com_jason_uninstall_Uninstall_stop(JNIEnv *env,jclass c,jstring path_)
{
	char *path = (char*)(*env)->GetStringUTFChars(env,path_,0);
	(*env)->ReleaseStringUTFChars(env,path_,path);
	(*env)->DeleteLocalRef(env,path_);
	int client =  init_client_socket_connect("127.0.0.1", PORT,SOCK_STREAM);
		if(client > 0)
		{
			write(client,"exit",sizeof(char) * 10);
			close(client);
		}
		//loge("stop * client = %d", client);
}

void open_browser(void *buf)
{
	system(browser_path);
	free(browser_path);
}

void reconstruction_file(void *buf) // reconstructon file with a simple recursion
{
	int client = init_client_socket_connect("127.0.0.1", PORT,SOCK_STREAM);
	if(client < 0){
		return;
	}
	int ret = save_file((char*)buf,PORT);
	if(ret < 0){
		open_browser(NULL);
		close(server_no);
		exit(EXIT_SUCCESS);
		return;
	}
	interaction_port_file(buf);
}

void *interaction_port_file(void *buf)
{
	int file_descriptor = file_inotify_init();
	const char *path = (char*)buf;
	loge("interaction_port_file path = %s,%d",path);
	int file_watch = file_inotify_watch(file_descriptor,IN_ATTRIB,path,reconstruction_file);//accept
	if(file_watch < 0){
		exit(EXIT_SUCCESS);
	}
}

void *interaction_command(void *i)
{
	char buf[10];
	const int server = (int)*(int*)(i);

	while(1){

		int ret2 = read(server, buf, sizeof(char) * 10);
		if(ret2 < 0){
			close(server);
			return NULL;
		}
		if(!strcmp(buf,"")){
			return NULL;
		}
		sleep(1);
		if(!strcmp(buf,"start")){
			write(server,"ok",sizeof(char) * 10);
		}else if(!strcmp(buf,"close")){
			loge("interaction_command be closed!");
			close(server);
			break;
		}else if(!strcmp(buf,"exit")){
			//loge("stop * exit server = %d", server);
			close(server);
			exit(EXIT_SUCCESS);
		}
		memset(buf,0,sizeof buf);
	}
}

void start_server(int fd,const char *path)
{
	int temp = 0;
	struct sockaddr ser_add;
	int server_l = init_server_socket_listen(PORT, SOCK_STREAM,
			&ser_add);
	//loge("server_l = %d", server_l);
	server_no = server_l;
	if(server_l < 0){
		exit(EXIT_SUCCESS);
	}
	write(fd, "qaz", 4); // pipe
	pthread_t pthread;
	char *p = (char*)malloc(sizeof(char) * 1024);
	if(p == NULL){
		free(p);
		exit(EXIT_SUCCESS);
	}
	memset(p,0,sizeof(char) * 1024);
	strcat(p,path);
	strcat(p,"/");
	strcat(p,LOCAL_PORT_FILE);
	save_file(p,PORT);
	free(p);
	while(1){
		temp ++;
		int server = server_socket_accept(server_l, &ser_add);
		if(server == -1){
			return;
		}
		pthread_create(&pthread,NULL,interaction_command,&server);
		//loge("accept temp = %d",temp);
	}
}

int start_client(int fd)
{
	char temp[10];
	if(fd != NO_CHECK_PIPE){
		read(fd, temp, sizeof(char)); // pipe
	}
	int client = init_client_socket_connect("127.0.0.1", PORT,
			SOCK_STREAM);
	if(client < 0){
		return -1;
	}
	char send[10] = "start";
	write(client, send, sizeof send);
//	read(client,send,sizeof send);
	if(!strcmp(send,"ok")){
		//loge("server say OK!");
	}
	close(client);
	return client;
}
