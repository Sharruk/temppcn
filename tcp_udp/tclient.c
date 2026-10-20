#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(){
    int s,n;
    struct sockaddr_in addr;
    char buf[1024];

    s=socket(AF_INET,SOCK_STREAM,0);

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    connect(s,(struct sockaddr *)&addr,sizeof(addr));
    printf("got con \n");

    write(s,"hell",4);
    n = read(s,buf,1023);

    if(n>0){
        buf[n] = '\0';
        printf("server:%s",buf);
    }
    else{
        printf("fuh u\n");
    }

    close(s);
    return 0;
}