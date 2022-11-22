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

/*服务端处理登录信息*/
bool s_login(int fd)
{
    int count = 0, ret = -1;
    char recv_buf[1024] = {0};
    char name[30] = {0};
    pack recv_pack;

    /*接受用户名*/
    memset(recv_buf, 0, sizeof(recv_buf));
    int len = recv(fd, recv_buf, sizeof(recv_buf), 0);
    printf("log: server recv name: %s\n", recv_buf); //log
    if (len > 0)
    {
        strcpy(name, recv_buf);
    }
    else if (len == 0)
    {
        printf("client out\n");
        return false;
    }
    else
    {
        perror("recv");
        return false;
    }

    /*接受密码*/
    memset(recv_buf, 0, sizeof(recv_buf));
    len = recv(fd, recv_buf, sizeof(recv_buf), 0);
    if (len > 0)
    {
        strcpy(name, recv_buf);
        printf("log: server copy recv name: %s buf: %s\n", name, recv_buf);
    }
    else if (len == 0)
    {
        printf("client out\n");
        return false;
    }
    else
    {
        perror("recv");
        return false;
    }
    do
    {
        ret = checkpasswd(name, recv_buf);
    } while (ret != true && count < 3);

    if (ret == true)
    {
        bzero(&recv_pack, sizeof(recv_pack));
        bzero(recv_buf, sizeof(recv_buf));
        recv_pack.type = OK;
        zip_pack(recv_buf, &recv_pack, sizeof(recv_pack));
        send(fd, recv_buf, sizeof(recv_buf), 0);
        return true;
    }
    else
    {
        return false;
    }
}

/*客户端处理登录信息*/
extern bool c_login(int fd)
{
    int count = 0;
    char recv_buf[1024] = {0};
    pack recv_pack;

    printf("Enter user name: ");
    fgets(recv_buf, sizeof(recv_buf), stdin);
    send(fd, recv_buf, sizeof(recv_buf), 0);
    do 
    {
        bzero(recv_buf, sizeof(recv_buf));

        printf("Enter user passwd: ");
        fgets(recv_buf, sizeof(recv_buf), stdin);
        send(fd, recv_buf, sizeof(recv_buf), 0);

        recv(fd, recv_buf, sizeof(recv_buf), 0);
        unzip_pack(&recv_pack, recv_buf, sizeof(recv_buf));
        printf("log: client recv type: %d msg: %s\n", recv_pack.type, recv_pack.data);//log
    } while (count < 3 && recv_pack.type != OK);

    if (count >= 3)
    {
        return false;
    }
    else
    {
        return true;
    }

}

/*打包数据*/
bool zip_pack(char *buf, pack *msg_data, int msg_size)
{
    memcpy(buf, (char *)msg_data, msg_size);
}

/*解压数据*/
bool unzip_pack(pack *msg_data, char *buf, int buf_size)
{
    memcpy((char *)msg_data, buf, buf_size);
}

/*密码查询*/
extern bool checkpasswd(char *name, char *passwd)
{
    int ret = -1;

    sqlite3 *db = NULL;
    ret = sqlite3_open("system.db", &db);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open error\n");
        return false;
    }

    char buf[1024] = {0};
    sprintf(buf, "select PASSWD from user_table where NAME = '%s';", name);
    char *sql = buf;

    char **result = NULL;
    int row = -1;
    int column = -1;
    ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);

    int index = column;
    for (int i = 0; i < row * column; ++i)
    {
        if (strcmp(passwd, result[index++]) == 0)
        {
            sqlite3_free_table(result);
            sqlite3_close(db);
            printf("log: passwd right\n");//log
            return true;
        }
        else
        {
            sqlite3_free_table(result);
            sqlite3_close(db);
            return false;
        }
    }
}

/*翻译*/
extern void translate(int fd, char *word)
{
    int ret = -1;

    sqlite3 *db = NULL;
    ret = sqlite3_open("system.db", &db);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open error\n");
        return false;
    }

    char buf[1024] = {0};
    sprintf(buf, "select word,explain from dict_table where word = '%s';", word);
    char *sql = buf;

    char **result = NULL;
    int row = -1;
    int column = -1;
    ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);

    int index = column;
    for (int i = 0; i < row * column; ++i)
    {
        if (strcmp(word, result[index]) == 0)
        {
            send(fd, result, strlen(result[index]), 0);
        }
        else
        {
            sqlite3_free_table(result);
            sqlite3_close(db);
            return false;
        }
    }

}