#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(){
    int s,n;
    struct sockaddr_in addr;
    char buf[1024];

    s = socket(AF_INET,SOCK_DGRAM,0);

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    sendto(s,"hell",4,0,(struct sockaddr *)&addr,sizeof(addr));

    n = recvfrom(s,buf,sizeof(buf),0,NULL,NULL);

    if(n>0){
        buf[n] = '\0';
        printf("server: %s \n",buf);
    }

    close(s);
    return 0;
}