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
#define REG 5
#define OK 6

/*包*/
typedef struct pack
{
    int type;
    char name[50];
    char passwd[50];
}pack;

/*打包数据*/
extern bool zip_pack(char *buf, pack *msg_data, int msg_size);

/*解压数据*/
extern bool unzip_pack(pack *msg_data, char *buf, int buf_size);

/*服务端用户注册信息*/
extern s_reg(int fd, sqlite3 *db, pack *recv_info);


#endif