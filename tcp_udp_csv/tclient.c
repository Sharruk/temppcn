#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

int main(){
    int s,n;
    struct sockaddr_in addr;
    char roll[20],buf[300] ={0};

    s = socket(AF_INET,SOCK_STREAM,0);

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);

    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    connect(s,(struct sockaddr *)&addr,sizeof(addr));

    printf("enter roll: ");
    scanf("%s",roll);

    write(s,roll,strlen(roll)+1);

    n = read(s,buf,sizeof(buf)-1);

    if(n>0){
        buf[n] = '\0';
        printf("%s\n",buf);
    }

    close(s);
    return 0;
}