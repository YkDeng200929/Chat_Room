#include "head.h"

struct test
{
    int data[5];
};

int main(int argc, const char *argv[])
{
    /*非阻塞回收子进程*/
    signal(SIGCHLD, signal_handler);
    int ret = -1, client_num = -1, fork_num = -1, count = 0;
    if (argc < 3)
    {
        printf("Please enter port and ip\n");
        exit(-1);
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    int u_sfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    /*1. 用户信息表准备*/
    c_list c_info_book;
    init(&c_info_book);

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
    ret = listen(sfd, 2);
    if (ret < 0)
    {
        perror("listen");
        return -1;
    }
    printf("log:[%d]:Listening\n", ++count);
    char recv_buf[1024] = {0};
    struct test *temp = NULL;
    while (1)
    {
        int cfd = -1;
        count = 0;
        pid_t pid;

        /*6. 阻塞等待接受客户端请求, 一旦有请求就 fork*/
        cfd = accept(sfd, (struct sockaddr *)&saddr, &slen);
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return -1;
        }

        /*7. child 子进程的意义在于与多个客户端通信*/
        if (pid == 0)
        {
            close(pipfd[0]);
            int data = cfd;
            //send(cfd, (int *)&data, sizeof(int), 0);
            recv(cfd, recv_buf, sizeof(recv_buf), 0);
            write(pipfd[1], recv_buf, sizeof(recv_buf));
        }

        /*8. father*/
        else if (pid > 0)
        {
            close(pipfd[1]);
            read(pipfd[0], recv_buf, sizeof(recv_buf));
            send(cfd, recv_buf, sizeof(recv_buf), 0);
            printf("cfd: %d : I recv buf %s\n", cfd, recv_buf);
        }
        /*close(cfd);扫描完整个main后再执行其他的内容, 所以不能在这close*/
    }
    printf("log:[%d]: in end\n", ++count);
    return 0;
}