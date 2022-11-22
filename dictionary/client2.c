#include "head.h"

int main(int argc, const char *argv[])
{
    printf("client\n");
    int ret = -1, option = -1;
    pack msg;
    if (argc < 3)
    {
        printf("Please enter ip && port\n");
        return -1;
    }

    /*1. 创建套接字*/
    int cfd = socket(AF_INET, SOCK_STREAM, 0);   
    if (cfd == 0)
    {
        perror("socket");
        return 0;
    }

    /*2. 创建连接服务端信息*/
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    saddr.sin_port = (htons(atoi(argv[2])));

    /*3. 发送连接请求*/
    ret = connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret < 0)
    {
        perror("connect");
        return -1;
    }

    /*4. 通信缓存*/
    char buf[1024] = {0};
    char recv_buf[1024] = {0};

    /*5. 通信*/
    while (1)
    {
        printf("Enter type");
        scanf("%d", &msg.type);
        getchar();
        printf("Enter msg: ");
        fgets(msg.data, sizeof(msg.data), stdin);
        zip_pack(buf, &msg, sizeof(msg));
        send(cfd, buf, sizeof(buf), 0);
        c_login(cfd);
    }

    return -1;
}