#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

struct students
{
    char name[20];
    int id;
    int score;
};

int main(void)
{
    sqlite3 *db = NULL; // 数据库句柄

    /*创建并打开数据库*/
    int ret = sqlite3_open("students_info.db", &db);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open error: %s\n", sqlite3_errmsg(db));// 只会记录最后一个记录
        exit(-1);
    }

    /*执行sql语句*/
    char *sql = "create table if not exists students_info(ID int primary key, NAME text, GOAL int)";// 通过API调用可以不带分号

    char *errmsg = NULL; //报错信息
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);//执行sql

    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec error: %s\n", errmsg);
        //sqlite3_free(errmsg);//释放错误信息
        exit(-1);
    }

    struct students st = {"rose", 2, 90};

    char name[1024] = {0};
    sprintf(name, "insert into students_info values(%d, '%s', %d)", st.id, st.name, st.score);//字符串拼接
    //sprintf(name, "insert into students_info values(%d, \"%s\", %d)", st.id, st.name, st.score); 也可以转义


    sql = "insert into students_info values(st.name, 'ykdeng', 80)";
    sql = name;
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK)
    {
        printf("exec: %s\n", errmsg);
        sqlite3_free(errmsg);
        //exit(-1);
    }

    //查找数据
    sql = "select ID from students_info where NAME = 'xiaogang'";
    char **result = NULL;// 字符串数组
    int row = -1;
    int column = -1;
    ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);

    printf("row: %d, col: %d\n", row, column);

    //int count = 0;
    /*
    //不打表头
    int count = column;
    for (int i = 0; i <= row; ++i) // 他会多存一个表头
    for (int i = 0; i < row; ++i) // 不打表头
    {
        for (int j = 0; j < column; ++j)
        {
            printf("%s", result[count++]);
        }
        putchar('\n');
    }

    */

    // 不打表头2
    int index = column;
    for (int i = 0; i < row * column; ++i)
    {
		/*
        printf("%s", result[index++]);
        if (index % column == 0)
        {
            putchar('\n');
        }
		*/
		if (strcmp(result[index++], "1") == 1)
		{
			printf("ok\n");
		}
    }
    //释放结果表
    sqlite3_free_table(result);

    //关闭数据库
    sqlite3_close(db);

    return 0;
}
