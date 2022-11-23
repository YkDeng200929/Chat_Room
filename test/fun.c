#include "head.h"


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
