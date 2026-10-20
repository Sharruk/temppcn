
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c, n;
    unsigned char data[100], cs;
    struct sockaddr_in a = {0};

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&a, sizeof(a));
    listen(s, 1);

    printf("Waiting for client...\n");
    c = accept(s, NULL, NULL);

    recv(c, &n, sizeof(n), MSG_WAITALL);
    recv(c, data, n, MSG_WAITALL);
    recv(c, &cs, 1, MSG_WAITALL);

    // Add all data bytes and checksum
    int sum = cs;

    for (int i = 0; i < n; i++) {
        sum += data[i];

        // End-around carry
        if (sum > 255)
            sum = (sum & 255) + 1;

        printf("Word %d = %02X, Sum = %02X\n",
               i + 1, data[i], sum);
    }

    printf("Checksum = %02X\n", cs);

    if (sum == 255)
        printf("No error detected\n");
    else
        printf("Error detected\n");

    close(c);
    close(s);
    return 0;
}
