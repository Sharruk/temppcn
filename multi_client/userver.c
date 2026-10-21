#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int s, n;
    struct sockaddr_in addr, client;
    socklen_t len = sizeof(client);
    char buf[1024];

    s = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    printf("udp multi-client server waiting on port 8081...\n");

    while (1) {
        n = recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&client, &len);
        if (n > 0) {
            buf[n] = '\0';
            printf("from %s:%d -> %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buf);

            char reply[1050];
            snprintf(reply, sizeof(reply), "echo: %s", buf);
            sendto(s, reply, strlen(reply), 0, (struct sockaddr *)&client, len);
        }
    }

    close(s);
    return 0;
}
