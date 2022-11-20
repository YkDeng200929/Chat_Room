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

struct test
{
    int data;
};

int main(void)
{
    int pipfd[2] = {0};
    pipe(pipfd);

    pid_t pid = fork();
    struct test temp;
    bzero(&temp, sizeof(temp));

    int buf[10] = {0};
    if (pid == 0)
    {
        int data;
        close(pipfd[0]);
        int pipfd[2] = {0};
        printf("enter: ");
        scanf("%d", &data);
        getchar();
        bzero(buf, sizeof(buf));
        temp.data = data;
        memcpy(buf, &temp, sizeof(temp));
        write(pipfd[1], (int *)&buf, sizeof(buf));
    }

    else
    {
        close(pipfd[1]);
        bzero(buf, sizeof(buf));
        read(pipfd[0], (int *)&buf, sizeof(buf));
        memcpy(&temp, buf, sizeof(temp));
        printf("buf: %d\n", temp.data);
    }

    return 0;
}