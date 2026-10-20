#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
    int s,n;
    struct sockaddr_in server,client;
    socklen_t len = sizeof(client);

    char roll[20] = {0};
    char line[100];
    char result[200];

    s = socket(AF_INET,SOCK_DGRAM,0);

    memset(&server,0,sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(8081);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s,(struct sockaddr *)&server, sizeof(server));

    printf("udp started\n");

    while(1){
        memset(roll,0,sizeof(roll));

        n = recvfrom(s,roll,sizeof(roll) - 1 ,0 ,(struct sockaddr *)&client, &len);

        if(n<=0) continue;

        roll[n] = '\0';
        strcpy(result,"not found u fk");

        FILE *fp = fopen("students.csv","r");

        while(fp && fgets(line,sizeof(line),fp)){
            char *r = strtok(line,",");
            char *name = strtok(NULL,",");

            if (r && name && strcmp(r,roll)==0){
                snprintf(result,sizeof(result),"roll num: %s %s",r,name);
                break;
            }
        }
        
        if(fp) fclose(fp);

        sendto(s,result,strlen(result) + 1, 0, (struct sockaddr *)&client,len);

    }
    close(s);
    return 0;


}