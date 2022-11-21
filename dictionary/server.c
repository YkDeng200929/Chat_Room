#include "head.h"

int main(int argc, const char *argv[])
{
    /*非阻塞回收子进程*/
    signal(SIGCHLD, signal_handler);
    int ret = -1, count = 0, flag = -1;
    if (argc < 3)
    {
        printf("Please enter port and ip\n");
        exit(-1);
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    int u_sfd = socket(AF_INET, SOCK_DGRAM, 0);
    flag = fcntl(u_sfd, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(u_sfd, F_SETFL, flag);
    if (sfd == 0)
    {
        perror("socket");
        return 0;
    }
    if (u_sfd == 0)
    {
        perror("socket");
        return 0;
    }

    /*2. 创建父子进程共用的临时接受信息容器*/
    pack recv_info;
    bzero(&recv_info, sizeof(recv_info));

    /*3. 绑定*/
    struct sockaddr_in saddr, caddr;
    bzero(&saddr, sizeof(saddr));
    bzero(&caddr, sizeof(caddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = (htons(atoi(argv[2])));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t slen = sizeof(saddr);
    socklen_t clen = sizeof(caddr);

    bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
    bind(sfd, (struct sockaddr *)&caddr, sizeof(caddr));

    /*4. 创建无名管道用于交换数据*/
    int pipfd[2] = {0};
    pipe(pipfd);

    /*5. 监听*/
    ret = listen(sfd, 124);
    if (ret < 0)
    {
        perror("listen");
        return -1;
    }
    printf("log:[%d]:Listening\n", ++count);
    char recv_buf[1024] = {0};
    while (1)
    {
        int cfd = -1;
        count = 0;

        /*6. 阻塞等待接受客户端请求, 一旦有请求就 fork*/
        cfd = accept(sfd, (struct sockaddr *)&saddr, &slen);
        char ip[24] = {0};
        printf("client ip: %s port: %d\n",inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(caddr.sin_port));

        pid_t pid = fork();
        if (pid == 0)
        {
            //子进程与客户端通信
            close(sfd);
            while(1)
            {
                ret = child_do(cfd);
                if (ret <= 0)
                {
                    break;
                }
            }
            close(cfd);
            exit(0);
        }
        else if(pid > 0)
        {
            // 父进程不与客户端通信
            close(cfd);
        }

    }
    printf("log:[%d]: in end\n", ++count);
    return 0;
}