#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
int clientfd2;//�ͻ���socket
char* IP = "127.0.0.1";//��������IP
short PORT = 6666;//����������˿�
typedef struct sockaddr meng;
char name[30];//����֧�ֵ��û�������
time_t nowtime;

void init(){

    clientfd2 = socket(PF_INET,SOCK_STREAM,0);//�����׽���
    struct sockaddr_in addr;//���׽��ִ���sockaddr_in�ṹ����
    addr.sin_family = PF_INET;//��ַ��
    addr.sin_port = htons(PORT);//�˿ں� ���������ã��������ɳ����涨�ķ�Χ
    addr.sin_addr.s_addr = inet_addr(IP);//inet_addr()���������ʮ���Ƶ��ַ���ת��Ϊ32λ�������ֽ�˳���ip��Ϣ
//��������

    if (connect(clientfd2,(meng*)&addr,sizeof(addr)) == -1){
        perror("�޷����ӵ�������");
        exit(-1);
    }
    printf("�ͻ��������ɹ�\n");
}

void start(){
    pthread_t id;
    void* recv_thread(void*);
    //����һ���߳��������ݵĽ��գ�һ���������ݵķ���
    pthread_create(&id,0,recv_thread,0);
    char buf2[100] = {};
    sprintf(buf2,"%s������Ⱥ��",name);
    time(&nowtime);
    printf("�����ʱ����: %s\n",ctime(&nowtime));
    send(clientfd2,buf2,strlen(buf2),0);

    while(1){
        char buf[100] = {};
        scanf("%s",buf);
        char msg[100] = {};
        sprintf(msg,"%s���͵���Ϣ��:%s",name,buf);
        send(clientfd2,msg,strlen(msg),0);

        if (strcmp(buf,"quit") == 0){
            memset(buf2,0,sizeof(buf2));//��ʼ��
            sprintf(buf2,"%s�˳���Ⱥ��",name);
            send(clientfd2,buf2,strlen(buf2),0);
            break;
        }
    }

    close(clientfd2);
}

void* recv_thread(void* p){

    while(1){
        char buf[100] = {};
        if (recv(clientfd2,buf,sizeof(buf),0) <= 0){
            break;
        }
        printf("%s\n",buf);
    }

}

int main(){
    init();
    printf("�������û�����");
    scanf("%s",name);
    printf("\n\n*****************************\n");
    printf("��ӭ%s ����Ⱥ��\n",name);
    printf("  ����quit �˳�\n");
    printf("\n*****************************\n\n");
    start();

    return 0;

}
