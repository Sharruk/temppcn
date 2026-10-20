
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, n;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);
    char roll[20], buf[200] = {0};

    // Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    printf("Enter Roll Number: ");
    scanf("%19s", roll);

    // Send roll number to server
    sendto(s, roll, strlen(roll) + 1, 0,
           (struct sockaddr *)&server, len);

    // Receive server's response
    n = recvfrom(s, buf, sizeof(buf) - 1, 0,
                 (struct sockaddr *)&server, &len);

    if (n > 0) {
        buf[n] = '\0';
        printf("%s\n", buf);
    }

    close(s);
    return 0;
}
