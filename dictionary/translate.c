#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(void)
{
    char word[30] = {0};
    // 存放sql语句
    char buf[1024] = {0};
    sqlite3 *db = NULL;
    int ret = sqlite3_open("system.db", &db);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_open error: %s\n", sqlite3_errmsg(db));
        exit(-1);
    }
    printf("Enter the word you wanna search: ");
    fgets(word, sizeof(word), stdin);
    sprintf(buf, "select word,explain from dict_table where word = '%s'", word);
    
    char *sql = buf;

    //查找数据
    char **result = NULL;
    int row = -1;
    int column = -1;
    ret = sqlite3_get_table(db, sql, &result, &row, &column, NULL);
    printf("log: get table ok, row: %d column: %d\n", row, column);
    int index = column;
    for (int i = 0; i < row * column; ++i)
    {
        printf("%s", result[index++]);
        if (index % column == 0)
        {
            putchar('\n');
        }
    }
    sqlite3_free_table(result);
    sqlite3_close(db);


    return 0;
}