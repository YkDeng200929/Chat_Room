#include "head.h"

int main(int argc, const char *argv[])
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

    pack recv_info;
    bzero(&recv_info, sizeof(recv_info));

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

    /*将监听描述符挂在树上*/
    int epfd = epoll_create(100);
    if (epfd == -1)
    {
        perror("epoll_create");
        exit(-1);
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);

    struct epoll_event evs[100];
    int maxlen = sizeof(evs) / sizeof(evs[0]);

    int his_fd = -1;
    his_fd = open("history.txt", O_RDWR | O_CREAT, 0666);
    if (his_fd == -1)
    {
        perror("open");
        exit(-1);
    }

    while (1)
    {
        // printf("epoll wait\n");
        int num = epoll_wait(epfd, evs, maxlen, 0);
        if (num == -1)
        {
            perror("epoll wait");
            exit(-1);
        }

        for (int i = 0; i < num; ++i)
        {
            if (evs[i].events & EPOLLIN)
            {
                int getfd = evs[i].data.fd;
                if (getfd == sfd)
                {
                    int cfd = accept(sfd, (struct sockaddr *)&saddr, &slen);
                    printf("accept ok\n");
                    if (cfd == -1)
                    {
                        perror("accept");
                        continue;
                    }
                    struct epoll_event ev2;
                    ev2.data.fd = cfd;
                    ev2.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev2);
                    ++maxlen;
                }
                else
                {
                    char buf[1024] = {0};
                    memset(buf, 0, sizeof(buf));
                    int len = recv(getfd, buf, sizeof(buf), 0);
                    if (len > 0)
                    {
                        unzip_pack(&recv_info, buf, sizeof(buf));
                        printf("type %d msg: %s\n", recv_info.type, recv_info.data);
                        ret = s_login(getfd);
                        if (ret == false)
                            break;
                        /*switch (recv_info.type)
                        {
                            case LOGIN: login(recv_info.type, getfd); break;
                            case TRANSLATE:
                            case LOGOUT:
                            case HISTORY:
                        }*/
                    }
                    else if (len == 0)
                    {
                        close(getfd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, getfd, NULL);
                        --maxlen;
                    }
                    else
                    {
                        perror("recv");
                        continue;
                    }
                }
            }
        }
    }

    printf("log:[%d]: in end\n", ++count);
    return 0;
}