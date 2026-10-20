int sockfd;
sockfd = socket(AF_INET,SOCK_DGRAM,0); //UDP
sockfd = socket(AF_INET,SOCKSTREAM,0); //TCP

bind(
    sockfd,
    (struct sockaddr *)&server_addr,
    sizeof(server_addr)
);

listen(sockfd,5)

client_fd = accept(
    sockfd,
    (sctruct sockaddr *)&client_addr,
    &client_len
)

connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));

read(sockfd,buffer,sizeof(buffer))
write(sockfd,"fuhu",4)

send(sockfd,"hello",5,0);
recv(sockfd,buffer,sizeof(buffer,0))


close(sockfd);

struct sock_addr_in server_addr;

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr)