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
        memset(buf, 0, sizeof(buf));

        printf("------------------------------------------\n");
        printf("1) Register\n");
        printf("2) Login\n");
        printf("3) Translate\n");
        printf("4) Logout && quit\n");
        printf("5) Get history\n");
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();
        printf("--------------------------------------------\n");

        /*成功登录后翻译*/
        if (option == TRANS && key == 1)
        {
            ret = c_trans(cfd, option, &msg, sizeof(msg), buf, sizeof(buf));
            if (ret == true)
                continue;
        }
        else if (option > 0 && option < 3 && key != 1) // 用户登录或注册
        {
            switch (option)
            {
            case REG: // 注册
                if (c_reg(cfd, option, &msg, sizeof(msg), buf, sizeof(buf)))
                {
                    printf("Register ok\n");
                }
                else
                {
                    printf("Register error\n");
                }
                break;
            case LOGIN: // 登录
                if (c_login(&key, cfd, option, &msg, sizeof(msg), buf, sizeof(buf)))
                {
                    printf("Login success\n");
                }
                else
                {
                    printf("Login error\n");
                }
                break;
            }
        }
        else if (option == LOGOUT)// 用户退出
        {
            memset(buf, 0, sizeof(buf));
            msg.type = option;
            memcpy(buf, (char *)&msg, sizeof(msg));
            send(cfd, buf, sizeof(buf), 0);
            close(cfd);
            printf("Log out success\n");
            return 0;
        }
        else if (option < 0 || option > 5)
        {
            printf("Enter error\n");
            continue;
        }
        else if (option == HISTORY)
        {
            c_hist(cfd, option, &msg, sizeof(msg), buf, sizeof(buf));
        }
        else if (option == LOGIN || option == REG && key == 1)
        {
            printf("You are online\n");
            continue;
        }
        else
        {
            printf("Error you are not login\n");
            break;
        }
    }
    close(cfd);

    return 0;
}