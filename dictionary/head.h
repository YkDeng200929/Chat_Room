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

#define OK 0
#define REG 1
#define LOGIN 2
#define TRANS 3
#define LOGOUT 4
#define HISTORY 5
#define ERR 6



/*包*/
typedef struct pack
{
    int type;
    char name[50];
    char passwd[50];
}pack;

/******************************************客户端功能************************************************/

/*用户注册(客户端)*/
extern bool c_reg(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/*用户登录(客户端)*/
extern bool c_login(int *key, int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/*用户翻译(客户端)*/
extern bool c_trans(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/*用户获取历史记录(客户端)*/
extern bool c_hist(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/******************************************服务端功能************************************************/

/*用户注册(服务端)*/
extern bool s_reg(int fd, sqlite3* db, pack* recv_info, int info_size);

/*用户在线功能(服务端): 含登录确认翻译与翻译功能*/
extern bool s_online(sqlite3* db, int type, int fd, pack* recv_info, int info_size);

#endif