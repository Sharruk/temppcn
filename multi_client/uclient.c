#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int s, n;
    struct sockaddr_in addr;
    char msg[1024], buf[1024];

    s = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    printf("udp client ready! type message ('exit' to quit):\n");

    while (1) {
        printf("> ");
        if (fgets(msg, sizeof(msg), stdin) == NULL) break;

        msg[strcspn(msg, "\n")] = '\0';
        if (strlen(msg) == 0) continue;

        sendto(s, msg, strlen(msg), 0, (struct sockaddr *)&addr, sizeof(addr));

        if (strcmp(msg, "exit") == 0) break;

        n = recvfrom(s, buf, sizeof(buf) - 1, 0, NULL, NULL);
        if (n > 0) {
            buf[n] = '\0';
            printf("server: %s\n", buf);
        }
    }

    close(s);
    return 0;
}
