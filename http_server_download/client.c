#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<time.h>


int main(){

    char url[300],host[300],path[200];
    char request[1024];
    char response[2000000];

    int port,s,n,total = 0;

    printf("enter url: ");
    scanf("%s",url);

    if(sscanf(url,"http://%99[^:]:%d/%199s",host,&port,path) != 3){
        printf("use http://localhost:8000/file.txt\n");
        return 1;
    }

    s = socket(AF_INET,SOCK_STREAM,0);

    struct hostent *h = gethostbyname(host);

    if(s<0 || h==NULL){
        printf("connection setup failed \n");
        return 1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr,h->h_addr,h->h_length);

    if(connect(s,(struct sockaddr *)&addr, sizeof(addr))<0){
        perror("connect");
        close(s);
        return 1;
    }

    snprintf(request,sizeof(request),"GET /%s HTTP/1.1\r\n" "Host: %s\r\n" "connection: close\r\n\r\n",path,host);

    send(s,request,strlen(request),0);

    clock_t start = clock();

    while ((n = recv(s,response + total, sizeof(response) - total - 1,0)) > 0){
        total += n;
        if(total >= (int)sizeof(response) - 1) break; 
    }

    clock_t end = clock();
    close(s);

    response[total] = '\0';

    char *body = strstr(response,"\r\n\r\n");

    if(body == NULL){
        printf("invalid HTTP response \n");
        return 1;
    }

    body +=4;
    int header_size = body - response;
    int body_size = total - header_size;

    if (strncmp(response,"HTTP/1.",7) != 0 || 
        strstr(response," 200 ") == NULL){
            printf("download failed: ");
            printf("%.40s\n",response);
            return 1;
        }
    
        char *name = strrchr(path,'/');
        name = name ? name + 1 :path;

        FILE *fp = fopen(name,"wb");

        if (fp == NULL){
            perror("file save failed");
            return 1;
        }

        fwrite(body,1,body_size,fp);
        fclose(fp);

        double seconds = (double)(end - start) / CLOCKS_PER_SEC;

        printf("\nDownload complete!\n");
        printf("Saved as: %s\n", name);
        printf("Data size: %d bytes\n", body_size);
        printf("Download time: %.3f seconds\n", seconds);

        if (seconds >0){
            printf("trhoughput: %f kb/s\n",body_size/1024.0/seconds);
        }

 
    return 0;
}
