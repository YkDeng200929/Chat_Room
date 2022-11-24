#ifndef _HEAD_H
#define _HEAD_H

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
#include <sys/poll.h>
#include <sys/epoll.h>

#define OK 0
#define REG 1
#define LOGIN 2
#define TRANS 3
#define LOGOUT 4
#define HISTORY 5
#define ERR 6



/*鍖�*/
typedef struct pack
{
    int type;
    char name[50];
    char passwd[50];
}pack;

/******************************************鍏朵粬鍔熻兘**************************************************/

/*鎵撳寘鏁版嵁*/
extern bool zip_pack(char *buf, pack *msg_data, int msg_size);

/*瑙ｅ帇鏁版嵁*/
extern bool unzip_pack(pack *msg_data, char *buf, int buf_size);

/******************************************瀹㈡埛绔姛鑳�************************************************/

/*鐢ㄦ埛娉ㄥ唽(瀹㈡埛绔�)*/
extern bool c_reg(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/*鐢ㄦ埛鐧诲綍(瀹㈡埛绔�)*/
extern bool c_login(int *key, int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/*鐢ㄦ埛缈昏瘧(瀹㈡埛绔�)*/
extern bool c_trans(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/*鐢ㄦ埛鑾峰彇鍘嗗彶璁板綍(瀹㈡埛绔�)*/
extern bool c_hist(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size);

/******************************************鏈嶅姟绔姛鑳�************************************************/

/*鐢ㄦ埛娉ㄥ唽(鏈嶅姟绔�)*/
extern bool s_reg(sqlite3* db, pack* recv_info);

/*鐢ㄦ埛鍦ㄧ嚎鍔熻兘(鏈嶅姟绔�): 鍚櫥褰曠‘璁ょ炕璇戜笌缈昏瘧鍔熻兘*/
extern bool s_online(sqlite3* db, int type, int fd, pack* recv_info, int info_size);

#endif    }

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
                    char **result = NULL;
                    int row = -1;
                    int column = -1;
                    int index = -1;
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
                            case REG:
                                s_reg(db, &recv_info);
                            case LOGIN:
                                ret = s_online(db, recv_info.type, getfd, &recv_info, sizeof(recv_info));
                                if (ret == true)
                                {
                                    count = 0;
                                    break;
                                }
                                else if (ret == false && count >= 3)
                                {
                                    epoll_ctl(epfd, EPOLL_CTL_DEL, getfd, NULL);
                                    printf("Too many try\n");//log
                                    count = 0;
                                    break;
                                }
                                else
                                {
                                    count++;
                                    break;
                                }
                            case TRANS:
                                s_online(db, recv_info.type, getfd, &recv_info, sizeof(recv_info));
                                break;
                            case LOGOUT:
                                epoll_ctl(epfd, EPOLL_CTL_DEL, getfd, NULL);
                                printf("Del Connection\n");//log
                                break;
                            case HISTORY:
                                s_online(db, recv_info.type, getfd, &recv_info, sizeof(recv_info));
                                break;
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