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

    /*打开数据库*/
    sqlite3 *db = NULL;

    ret = sqlite3_open("../dictionary/system.db", &db);

    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open error\n");
        exit(-1);
    }

    char *sql = "create table if not exists history(WORD text)";

    ret = sqlite3_exec(db, sql, NULL, NULL, NULL);

    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec error\n");
        exit(-1);
    }

    while (1)
    {
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
                    char temp[1024];
                    memset(buf, 0, sizeof(buf));
                    memset(temp, 0, sizeof(temp));
                    /*接受客户端包*/
                    printf("server recving\n");//log
                    int len = recv(getfd, buf, sizeof(buf), 0);
                    if (len > 0)
                    {
                        unzip_pack(&recv_info, buf, sizeof(buf));
                        printf("type %d name: %s passwd: %s\n", recv_info.type, recv_info.name, recv_info.passwd);//log
                        switch (recv_info.type)
                        {
                        case 1:
                            sprintf(temp, "insert into user_table values('%s', '%s')", recv_info.name, recv_info.passwd);
                            sql = temp;
                            ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
                            if (ret != SQLITE_OK)
                            {
                                printf("exec\n");
                                break;
                            }
                            else
                            {
                                printf("insert ok\n"); // log
                                break;
                            }
                        case 2:
                            printf("in login\n");//log
                            sprintf(temp, "select PASSWD from user_table where NAME = '%s'", recv_info.name);
                            sql = temp;
                            char **result = NULL;
                            int row = -1;
                            int column = -1;
                            ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);
                            int index = column;
                            printf("row: %d, column: %d, index: %d\n", row, column, index);//log
                            for (int i = 0; i < row * column; ++i)
                            {
                                printf("int get table passwd: %s", result[index]);
                                ret = strcmp(result[index++], recv_info.passwd);
                                printf("return val: %d\n", ret);
                                if (ret == 0)
                                {
                                    printf("passwd right\n");
                                    recv_info.type = 1;
                                    memset(temp, 0, sizeof(temp));
                                    zip_pack(temp, &recv_info, sizeof(recv_info));
                                    send(getfd, temp, sizeof(temp), 0);
                                }
                                else
                                {
                                    printf("passwd error\n");
                                }
                            }
                            break;
                        case 3:
                            printf("in translate\n");//log
                            sprintf(temp, "select explain from dict_table where word = '%s'", recv_info.name);
                            sql = temp;
                            result = NULL;
                            row = -1;
                            column = -1;
                            ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);
                            index = column;
                            printf("row: %d, column: %d, index: %d\n", row, column, index);
                            for (int i = 0; i < row * column; ++i)
                            {
                                printf("translate: %s\n", result[index]);//log
                                memset(temp, 0, sizeof(temp));
                                strcpy(recv_info.name, result[index++]);
                                zip_pack(temp, &recv_info, sizeof(recv_info));
                                send(getfd, temp, sizeof(temp), 0);
                            }
                        }
                    }
                    else if (len == 0)
                    {
                        /*客户端断开连接*/
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