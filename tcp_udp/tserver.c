#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(){
    int s,c,n;
    struct sockaddr_in addr;
    char buf[1024];

    s = socket(AF_INET,SOCK_STREAM,0);

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s,(struct sockaddr *)&addr, sizeof(addr));
    listen(s,5);

    printf("waiting...\n");
    c = accept(s,NULL,NULL);

    n = read(c,buf,1023);
    if(n>0){
        buf[n] = '\0';
        printf("clident: %s\n",buf);
        write(c,"hello",5);
    }
    close(c);
    close(s);
    return 0;


}