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
#define LOGIN 1
#define TRANSLATE 2
#define HISTORY 3
#define LOGOUT 4

/*包*/
typedef struct pack
{
    int type;
    char data[1024];
}pack;

/*显示面板*/
extern void display(int *option, pack *datapack);

/*登录*/
extern char *login(int fd, char *buf, int bufsize, pack *data, int size);

/*翻译*/
extern void translate(char *word);

/*回收子进程*/
extern void signal_handler(int signum);

/*子进程通信*/
extern int child_do(int cfd);

#endif