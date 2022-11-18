#include "head.h"

int main(int argc, const char *argv[])
{
    int ret;
    if (argc < 3)
    {
        printf("Please enter port and ip");
        exit(-1);
    }
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == 0)
    {
        perror("socket");
        return 0;
    }
    /*1. 用户信息表准备*/
    c_list c_info_book;
    init(&c_info_book);
    /*2. 创建父子进程共用的临时接受信息容器*/
    pack *temp = NULL;

    struct sockaddr_in saddr, caddr;
    bzero(&saddr, sizeof(saddr));
    bzero(&caddr, sizeof(caddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = (htons(atoi(argv[2])));
    saddr.sin_addr.s_addr = inaddr(argv[1]);
    bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
    socklen_t clen = sizeof(caddr);
    ret = listen(sfd, 100);
    if (ret < 0)
    {
        perror("listen");
        return -1;
    }
    printf("Listen ok-------------------------\n");
    while (1)
    {
        pid_t pid;
        int cfd = accept(sfd, (struct sockaddr *)&caddr, &clen);
        if (cfd < 0)
        {
            perror("accept");
            return -1;
        }
        printf("Accept ok, cfd: %d\n", cfd);
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return -1;
        }
        while (1)
        {
            if (pid == 0)
            {
                close(sfd);
                printf("In child, pid: %d----------------------------\n", pid);
                recvfrom(sfd, (pack*)temp, sizeof(pack), 0, (struct sockaddr *)&caddr, sizeof(caddr));
                exit(EXIT_SUCCESS);
            }
            else if (pid > 0)
            {
                printf("In father, pid: %d----------------------------\n", pid);
                while (1)
                {
                    switch (temp->type)
                    {
                        case LOG_IN: s_login(cfd, temp, &c_info_book, &caddr); break;
                        case CHAT: s_chat();
                        case LOG_OUT: s_logout();
                    }
                }
            }
        }
    }
    return 0;
}