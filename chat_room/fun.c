#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include "head.h"

/*初始化*/
void init(c_list *info_book)
{
    info_book->cfd = -1;
    
    info_book->next = NULL;
}

/*登录*/
bool s_login(int connect_fd, pack * client_pack, c_list * info_book, struct sockaddr_in * client_addr)
{
    /*登录用户数据包*/
    pack * temp = client_pack;
    c_list *new_info = NULL;
    new_info->addr.sin_addr.s_addr = client_addr->sin_addr.s_addr;
    new_info->cfd = connect_fd;
    new_info->next = NULL;

    /*添加到链表*/
    new_info->next = info_book->next;
    info_book->next = new_info;
}

/*聊天*/
bool s_chat(int connect_fd, pack * client_pack, c_list * info_book, struct sockaddr_in * client_addr)
{
    c_list *curse = NULL;
    for (curse = info_book; curse != NULL; curse = curse->next)
    {
        send(curse->cfd, client_pack->text, strlen(client_pack->text), 0);
    }
}

/*登出*/
bool s_logout(int closefd, pack * client_pack, c_list * info_book, struct sockaddr_in * client_addr)
{
    c_list *find = NULL;
    c_list *prev = NULL;
    for (prev = info_book, find = prev->next; find != NULL || find->cfd != closefd; prev = prev->next, find = prev->next)
    {}
    if (find == NULL)
        return false;
    prev->next = find->next;
    free(find);
    return true;
}

/*回收子进程*/
void signal_handler(int signum)
{
    waitpid(-1, NULL, WNOHANG);
}