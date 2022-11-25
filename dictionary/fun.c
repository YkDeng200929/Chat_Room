#include "head.h"

/***********************************************************客户端功能************************************************************************************/

/*用户注册(客户端)*/
extern bool c_reg(int fd, int option, pack *msg, int msg_size, char *buf, int buf_size)
{
    printf("Enter name: ");
    fgets(msg->name, sizeof(msg->name), stdin);
    printf("Enter passwd: ");
    fgets(msg->passwd, sizeof(msg->passwd), stdin);
    msg->type = option;
    memcpy(buf, (char *)msg, msg_size);
    send(fd, buf, buf_size, 0); // 将用户注册信息传送给服务端

    memset(buf, 0, buf_size);
    recv(fd, buf, buf_size, 0);
    memcpy((char *)msg, buf, buf_size);
    if (msg->type == OK)
    {
        printf("Register ok\n");
        return true;
    }
    else
    {
        printf("Register error\n");
    }
}

/*用户登录(客户端)*/
extern bool c_login(int *key, int fd, int option, pack *msg, int msg_size, char *buf, int buf_size)
{
    int count = 0;
    do
    {
        printf("Enter name: ");
        fgets(msg->name, sizeof(msg->name), stdin);
        printf("Enter passwd: ");
        fgets(msg->passwd, sizeof(msg->passwd), stdin);
        msg->type = option;
        memcpy(buf, (char *)msg, msg_size);
        send(fd, buf, buf_size, 0);
        memset(buf, 0, buf_size);

        /*接受登录请求结果*/
        recv(fd, buf, buf_size, 0);
        memcpy((char *)msg, buf, buf_size);
        if (msg->type == 1)
        {
            *key = 1;
            printf("Login success\n");
            return true;
        }
        printf("Login error\n");
        count++;
    } while (count < 3);
    printf("Too many trys\n");
    return false;
}

/*用户翻译(客户端)*/
extern bool c_trans(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size)
{
    bzero(msg, msg_size);
    memset(buf, 0, sizeof(buf));
    msg->type = option;
    printf("Please enter the words you wanna translate:");
    fgets(msg->name, sizeof(msg->name), stdin);
    msg->name[strlen(msg->name) - 1] = '\0'; //!!!输出结果有回车, 所以搜索不到结果
    memcpy(buf, (char *)msg, msg_size);
    send(fd, buf, buf_size, 0);

    /*接受翻译结果*/
    memset(buf, 0, buf_size);
    recv(fd, buf, buf_size, 0);
    unzip_pack(msg, buf, buf_size);
    msg->type = option;
    printf("---------------------------------------------------\n");
    printf("explain: %s\n", msg->name); // log
    printf("---------------------------------------------------\n");
    printf("translate ok\n");

    return true;
}

/*用户获取历史记录(客户端)*/
extern bool c_hist(int fd, int option, pack* msg, int msg_size, char *buf, int buf_size)
{
    memset(buf, 0, sizeof(buf));
    msg->type = option;
    zip_pack(buf, msg, msg_size);
    send(fd, buf, buf_size, 0);
    memset(buf, 0, sizeof(buf));
    int hist_len = 999;
    printf("history------------------\n");
    for (int i = 0; i < hist_len; i++)
    {
        recv(fd, buf, buf_size, 0);
        unzip_pack(msg, buf, buf_size);
        printf("%s\n", msg->name);
        hist_len = msg->type;
    }
    printf("--------------------------\n");
}

/**********************************************************服务端**************************************************************************/

/*用户注册(服务端)*/
extern bool s_reg(int fd, sqlite3 *db, pack *recv_info, int info_size)
{
    char buf[1024] = {0};
    sprintf(buf, "insert into user_table values('%s', '%s')", recv_info->name, recv_info->passwd);
    char *sql = buf;
    int ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        recv_info->type = OK;
        memcpy(buf, (char *)recv_info, info_size);
        send(fd, buf, sizeof(buf), 0);
        printf("server: Client register error\n");// log
        return false;
    }
    else
    {
        recv_info->type = OK;
        memcpy(buf, (char *)recv_info, info_size);
        send(fd, buf, sizeof(buf), 0);
        printf("server: Register ok\n"); // log
        return true;
    }
}

/*用户在线功能(服务端)*/
extern bool s_online(sqlite3 *db, int type, int fd, pack *recv_info, int info_size)
{
    char **result = NULL;
    int row = -1;
    int column = -1;
    int index = -1;

    char temp[1024] = {0};
    memset(temp, 0, sizeof(temp));
    if (type == LOGIN)// 登录处理
    {
        sprintf(temp, "select PASSWD from user_table where NAME = '%s'", recv_info->name);
    }
    else if (type == TRANS)// 翻译处理
    {
        sprintf(temp, "insert into history values('%s')", recv_info->name);
        char *sql_exc = temp;
        sqlite3_exec(db, sql_exc, NULL, NULL, NULL);
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "select explain from dict_table where word = '%s'", recv_info->name);
    }
    else if (type == HISTORY)// 历史记录处理
    {
        strcpy(temp, "select * from history");
        printf("temp: %s\n", temp);//log
    }
    else
    {
        printf("type error\n"); // log
        return false;
    }

    char *sql = temp;
    int ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);
    index = column;
    if (type == LOGIN && column == 0)
    {
        printf("No such name or passwd\n");
        recv_info->type = ERR;
        memset(temp, 0, sizeof(temp));
        zip_pack(temp, recv_info, info_size);
        send(fd, temp, sizeof(temp), 0);
        sqlite3_free_table(result);
        return false;
    }
    else if (column == 0)
    {
        char err[] = "No such infomation";
        send(fd, err, strlen(err), 0);
    }
    printf("row: %d, column: %d, index: %d, result: %s\n", row, column, index, result[index]); // log
    for (int i = 0; i < row * column; ++i)
    {
        /*用户登录处理*/
        if (type == LOGIN)
        {
            ret = strcmp(result[index++], recv_info->passwd);
            if (ret == OK)
            {
                printf("passwd right\n");
                recv_info->type = 1;
                memset(temp, 0, sizeof(temp));
                memset(recv_info->name, 0, sizeof(recv_info->name));
                zip_pack(temp, recv_info, info_size);
                send(fd, temp, sizeof(temp), 0);
                sqlite3_free_table(result);

                return true;
            }
            else
            {
                printf("passwd error\n");

                return false;
            }
        }
        /*用户翻译处理*/
        else if (type == TRANS)
        {
            memset(temp, 0, sizeof(temp));
            strcpy(recv_info->name, result[index++]);
            zip_pack(temp, recv_info, info_size);
            send(fd, temp, sizeof(temp), 0);
        }
        /*用户查询历史记录*/
        else if (type == HISTORY)
        {
            printf("client get history, column: %d\n", column);
            memset(temp, 0, sizeof(temp));
            strcpy(recv_info->name, result[index++]);
            recv_info->type = row;
            zip_pack(temp, recv_info, info_size);
            send(fd, temp, sizeof(temp), 0);
        }
        else
        {
            printf("error in get table, type: %d\n", type);
            return false;
        }
    }
}
