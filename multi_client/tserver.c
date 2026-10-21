#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *handle_client(void *arg) {
    int c = *(int *)arg;
    free(arg);

    char buf[1024];
    int n;

    while ((n = read(c, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        printf("client (fd %d): %s\n", c, buf);

        if (strncmp(buf, "exit", 4) == 0) {
            write(c, "bye", 3);
            break;
        }

        // Echo message back with "echo: "
        char reply[1050];
        snprintf(reply, sizeof(reply), "echo: %s", buf);
        write(c, reply, strlen(reply));
    }

    printf("client (fd %d) disconnected\n", c);
    close(c);
    return NULL;
}

int main() {
    int s;
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    printf("multi-client server running on port 8080...\n");

    while (1) {
        int *c = malloc(sizeof(int));
        *c = accept(s, NULL, NULL);

        if (*c < 0) {
            free(c);
            continue;
        }

        printf("new client connected! (fd %d)\n", *c);

        pthread_t t;
        pthread_create(&t, NULL, handle_client, c);
        pthread_detach(t);
    }

    close(s);
    return 0;
}
