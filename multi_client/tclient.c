#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void get_timestamp(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}

int main() {
    int s, n;
    struct sockaddr_in addr;
    char msg[1024], buf[1200], time_str[64];

    s = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("connection failed\n");
        return 1;
    }

    get_timestamp(time_str, sizeof(time_str));
    printf("[%s] connected to server! type message ('exit' to quit):\n", time_str);

    while (1) {
        printf("> ");
        if (fgets(msg, sizeof(msg), stdin) == NULL) break;

        // remove trailing newline
        msg[strcspn(msg, "\n")] = '\0';
        if (strlen(msg) == 0) continue;

        get_timestamp(time_str, sizeof(time_str));
        printf("[%s] sent: %s\n", time_str, msg);

        write(s, msg, strlen(msg));

        n = read(s, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("server reply: %s\n", buf);
        }

        if (strcmp(msg, "exit") == 0) break;
    }

    close(s);
    return 0;
}
