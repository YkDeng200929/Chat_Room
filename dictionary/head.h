#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sqlite3.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#define LOGIN 1
#define TRANSLATE 2
#define HISTORY 3
#define LOGOUT 4
#define OK 5

/*包*/
typedef struct pack
{
    int type;
    char data[1024];
}pack;

/*显示面板*/
extern void display(int *option, pack *datapack);

/*密码查询*/
extern bool checkpasswd(char *name, char *passwd);

/*服务器处理登录信息*/
extern bool s_login(int fd);

/*客户端处理登录信息*/
extern bool c_login(int fd);

/*翻译*/
extern void translate(int fd, char *word);

/*打包数据*/
extern bool zip_pack(char *buf, pack *msg_data, int msg_size);

/*解压数据*/
extern bool unzip_pack(pack *msg_data, char *buf, int buf_size);


#endif