#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(){
    int s,n;
    struct sockaddr_in addr,client;
    socklen_t len = sizeof(client);
    char buf[1024];

    s = socket(AF_INET,SOCK_DGRAM,0);

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(
        s,
        (struct sockaddr *)&addr,
        sizeof(addr)
    );

    printf("udp waiting \n");

    n = recvfrom(s,buf,sizeof(buf),0,(struct sockaddr *)&client,&len);

    if(n>0){
        buf[n] = '\0';
        printf("client: %s \n",buf);

        sendto(s,"hello",5,0,(struct sockaddr *)&client,len);
    }
    close(s);
    return 0;
}