#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <pthread.h>

void get_timestamp(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}

void *handle_client(void *arg) {
    int c = *(int *)arg;
    free(arg);

    char buf[1024];
    char time_str[64];
    int n;

    while ((n = read(c, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        get_timestamp(time_str, sizeof(time_str));

        printf("[%s] client (fd %d): %s\n", time_str, c, buf);

        if (strncmp(buf, "exit", 4) == 0) {
            write(c, "bye", 3);
            break;
        }

        // Send back echo with server timestamp
        char reply[1200];
        snprintf(reply, sizeof(reply), "[%s] echo: %s", time_str, buf);
        write(c, reply, strlen(reply));
    }

    get_timestamp(time_str, sizeof(time_str));
    printf("[%s] client (fd %d) disconnected\n", time_str, c);
    close(c);
    return NULL;
}

int main() {
    int s;
    struct sockaddr_in addr;
    char time_str[64];

    s = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    get_timestamp(time_str, sizeof(time_str));
    printf("[%s] multi-client server running on port 8080...\n", time_str);

    while (1) {
        int *c = malloc(sizeof(int));
        *c = accept(s, NULL, NULL);

        if (*c < 0) {
            free(c);
            continue;
        }

        get_timestamp(time_str, sizeof(time_str));
        printf("[%s] new client connected! (fd %d)\n", time_str, *c);

        pthread_t t;
        pthread_create(&t, NULL, handle_client, c);
        pthread_detach(t);
    }

    close(s);
    return 0;
}
