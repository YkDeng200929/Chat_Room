#include "head.h"

int main(int argc, const char *argv[])
{
    printf("client\n");
    int ret = -1;
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
    char name[10] = {0};
    int option = -1;

    /*5. 通信*/
    while (1)
    {
        if (option == LOG_IN)
        {
            do
            {
                printf("Do you wanna chat?");
                scanf("%d", &option);
                getchar();
            } while (option < 0 || option > 3);

            if (option == CHAT)
            {
                /*打包信息*/
                printf("Enter message: ");
                fgets(buf, sizeof(buf), stdin);
                msg.type = option;
                strcpy(msg.name, name);
                ret = send(cfd, (pack*)&msg, sizeof(pack), 0);
                if (ret < 0)
                {
                    perror("send");
                    return -1;
                }
            }
            if (option == LOG_IN)
            {
                option = CHAT;
            }
        }
        else if (option == LOG_OUT)
        {
            close(cfd);
            exit(EXIT_SUCCESS);
        }

        do
        {
            printf("1) log in\n");
            printf("2) chat\n");
            printf("3) log out\n");
            printf("Please enter options: ");
            scanf("%d", &option);
            getchar();
        }
        while (option < 0 || option > 3);

        if (option == 1)
        {
            printf("Enter your name: ");
            fgets(name, sizeof(name), stdin);
        }
    }

    return -1;
}