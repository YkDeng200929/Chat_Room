#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "head.h"

void init(c_list *info_book)
{
    bzero(info_book, sizeof(c_list));
    info_book->next = NULL;
}

bool s_login(int connect_fd, pack * client_pack, c_list * info_book, struct sockaddr_in * client_addr)
{
    /*登录用户数据包*/
    pack * temp = client_pack;
    c_list *new_info = NULL;
    new_info->addr = client_addr->sin_addr.s_addr;
    new_info->cfd = connect_fd;
    new_info->next = NULL;

    /*添加到链表*/
    new_info->next = info_book->next;
    info_book->next = new_info;
}

bool s_chat(int connect_fd, pack * client_pack, c_list * info_book, struct sockaddr_in * client_addr)
{
    c_list *curse = NULL;
    for (curse = info_book; curse != NULL; curse = curse->next)
    {
        send(curse->cfd, client_pack->text, strlen(client_pack->text), 0);
    }
}

bool s_logout(int connect_fd, pack * client_pack, c_list * info_book, struct sockaddr_in * client_addr)
{}