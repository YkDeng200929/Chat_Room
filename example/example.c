#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

int serverfd;//������socket
int clientfd[100];//�ͻ��˵�socketfd,100��Ԫ�أ�clientfd[0]~clientfd[99]
int size =50;//�������ƽ��������ҵ�����Ϊ50����
char* IP = "127.0.0.1";//����ip��ַ
short PORT = 6666;//�˿ں�
typedef struct sockaddr meng;
time_t nowtime;

void init(){
    serverfd = socket(PF_INET,SOCK_STREAM,0);
    if (serverfd == -1){
        perror("����socketʧ��");
        exit(-1);
    }

//Ϊ�׽�������ipЭ�� ���ö˿ں�  ���Զ���ȡ����ipת��Ϊ����ip
    struct sockaddr_in addr;//�洢�׽��ֵ���Ϣ
    addr.sin_family = PF_INET;//��ַ��
    addr.sin_port = htons(PORT);//����server�˶˿ںţ�������������,��sin_port = 0ʱ��ϵͳ���ѡ��һ��δ��ʹ�õĶ˿ں�
    addr.sin_addr.s_addr = inet_addr(IP);//��127.0.0.1��Ϊ�Լ���server�˵�ip��ַ,��sin_addr = INADDR_ANYʱ����ʾ�ӱ�������һ������������
//���׽���

    if (bind(serverfd,(meng*)&addr,sizeof(addr)) == -1){
        perror("��ʧ��");
        exit(-1);
    }

    if (listen(serverfd,100) == -1){//�������������
        perror("���ü���ʧ��");
        exit(-1);
    }
}

void SendAll(char* msg){

    int i;
    for (i = 0;i < size;i++){
        if (clientfd[i] != 0){
            printf("���͸�%d\n",clientfd[i]);
    printf("���͵���Ϣ��: %s\n",msg);
//д���ļ�

char buf[1024];
FILE *logs = fopen("log.txt", "a+");
if(logs== NULL)
{
    printf("open file erroe: \n");
}
else
{
sprintf(buf, "����ʱ�䣺%s\tIP��ַ��%s\n",ctime(&nowtime),IP);
fputs(buf,logs);
sprintf(buf, "������Ϣ��%s\n",msg);
fputs(buf,logs);
fclose(logs);
}
            send(clientfd[i],msg,strlen(msg),0);
        }
    }
}

void* server_thread(void* p){

    int fd = *(int*)p;
    printf("pthread = %d\n",fd);
    while(1){
        char buf[100] = {};
        if (recv(fd,buf,sizeof(buf),0) <= 0){
            int i;
            for (i = 0;i < size;i++){
                if (fd == clientfd[i]){
                    clientfd[i] = 0;
                    break;
                }
            }
            printf("�˳���fd = %d �˳��ˡ�\n",fd);

char buf[1024];

FILE *logs = fopen("log.txt", "a");

if(logs== NULL)

{

printf("open file erroe: \n");

}else{

sprintf(buf, "�˳�ʱ�䣺%s\tIP��ַ��%s\n",ctime(&nowtime),IP);

fputs(buf,logs);

fclose(logs);

}

                pthread_exit(0);

        }

        //�ѷ��������ܵ�����Ϣ�������еĿͻ���

        SendAll(buf);

    }

}

void server(){

    printf("����������\n");

    while(1){

        struct sockaddr_in fromaddr;

        socklen_t len = sizeof(fromaddr);

        int fd = accept(serverfd,(meng*)&fromaddr,&len);

//����accept�������״̬���ȴ��ͻ��˵�����

        if (fd == -1){

            printf("�ͻ������ӳ���...\n");

            continue;

        }

        int i = 0;

        for (i = 0;i < size;i++){

            if (clientfd[i] == 0){

                //��¼�ͻ��˵�socket

                clientfd[i] = fd;

                printf("�̺߳�= %d\n",fd);//

                //�пͻ�������֮�������̸߳��˿ͻ�����

                pthread_t tid;

                pthread_create(&tid,0,server_thread,&fd);

                break;

            }

        if (size == i){

            //���͸��ͻ���˵����������

            char* str = "�Բ����������Ѿ�����!";

            send(fd,str,strlen(str),0);

            close(fd);

        }
        }
    }

}

int main(){

    init();

    server();

}