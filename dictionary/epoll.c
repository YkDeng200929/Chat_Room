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
#include <sys/epoll.h>

int main(int argc, const char **argv)
{

    int ret = -1, count = 0, flag = -1;
    if (argc < 3)
    {
        printf("Please enter port and ip\n");
        exit(-1);
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == 0)
    {
        perror("socket");
        return 0;
    }
    
    struct sockaddr_in saddr, caddr;
    bzero(&saddr, sizeof(saddr));
    bzero(&caddr, sizeof(caddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = (htons(atoi(argv[2])));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t slen = sizeof(saddr);
    socklen_t clen = sizeof(caddr);

    bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));

    ret = listen(sfd, 124);
    if (ret < 0)
    {
        perror("listen");
        return -1;
    }

    /*创建epoll树*/
    int epfd = epoll_create(100);
    if (epfd == -1)
    {
        perror("epoll_create");
        exit(-1);
    }

    /*往树中添加监听套接字描述符*/
    struct epoll_event ev;
    ev.events = EPOLLIN; // 检测是否有来自客户端的传入请求
    ev.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);// 将监听套接字挂在树上
    if (ret == -1)
    {
        perror("epoll_ctl");
        exit(-1);
    }

    // 用于收集连接套接字
    struct epoll_event evs[1024];
    int size = sizeof(evs)/sizeof(struct epoll_event);

    printf("log:[%d]:Listening\n", ++count);
    while (1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        for (int i = 0; i < num; ++i)
        {
            
        }
    }

    return 0;
}