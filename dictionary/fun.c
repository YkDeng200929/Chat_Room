#include "head.h"

/*显示面板*/
void display(int *option, pack *datapack)
{
    printf("Personal dictionary\n");
    printf("1) Login\n");
    printf("2) Translate\n");
    printf("3) Get History\n");
    printf("4) Log out\n");
    scanf("%d", option);
    getchar();
    datapack->type = *option;
}

/*登录*/
char *login(int fd, char *buf, int bufsize, pack *data, int size)
{
    memcpy(buf, data, size);
    send(fd, buf, bufsize, 0);
    recv(fd, buf, bufsize, 0);
}

/*回收子进程*/
void signal_handler(int signum)
{
    waitpid(-1, NULL, WNOHANG);
}

/*子进程通信*/
int child_do(int cfd)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int len = recv(cfd, buf, sizeof(buf), 0);
    if (len > 0)
    {
        send(cfd, buf, len, 0);
    }
    else if(len == 0)
    {
        printf("client out");
    }
    else
    {
        perror("read");
    }
    
    return len;

}