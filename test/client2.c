#include "head.h"

int main(int argc, const char *argv[])
{
    printf("client\n");
    int ret = -1, option = -1, key = -1;
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

    /*5. 通信*/
    while (1)
    {
        printf("key: %d\n", key);//log
        memset(buf, 0, sizeof(buf));
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();
        if (option == 3 && key == 1)
        {
            // translateprintf("Enter word: ");
            msg.type = option;
            fgets(msg.name, sizeof(msg.name), stdin);
            zip_pack(buf, &msg, sizeof(msg));
            send(cfd, buf, sizeof(buf), 0);
            memset(buf, 0, sizeof(buf));
            recv(cfd, buf, sizeof(buf), 0);
            unzip_pack(&msg, buf, sizeof(buf));
            printf("explain: %s\n", msg.name); // log
            printf("translate ok\n");
        }
        else if (option >= 1 && option < 3 && key != 1)
        {
            switch (option)
            {
            case 1: // 注册
                printf("Enter name: ");
                fgets(msg.name, sizeof(msg.name), stdin);
                printf("Enter passwd: ");
                fgets(msg.passwd, sizeof(msg.passwd), stdin);
                msg.type = option;
                zip_pack(buf, &msg, sizeof(msg));
                send(cfd, buf, sizeof(buf), 0);
                break;

            case 2:
                printf("Enter name: ");
                fgets(msg.name, sizeof(msg.name), stdin);
                printf("Enter passwd: ");
                fgets(msg.passwd, sizeof(msg.passwd), stdin);
                msg.type = option;
                zip_pack(buf, &msg, sizeof(msg));
                send(cfd, buf, sizeof(buf), 0);
                memset(buf, 0, sizeof(buf));
                recv(cfd, buf, sizeof(buf), 0);
                unzip_pack(&msg, buf, sizeof(buf));
                printf("login type: %d\n", msg.type); // log
                if (msg.type == 1)
                {
                    key = 1;
                    printf("You are login\n");
                }
                break; // 登录
            }
        }
        else
        {
            printf("You are not login\n");
            continue;
        }
    }

    return -1;
}