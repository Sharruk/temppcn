
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, n, sum = 0;
    unsigned char data[100], cs;
    struct sockaddr_in a = {0};

    printf("Number of bytes: ");
    scanf("%d", &n);

    printf("Enter %d hexadecimal values (00-FF):\n", n);

    for (int i = 0; i < n; i++) {
        unsigned int x;
        scanf("%x", &x);
        data[i] = x;
    }

    // Calculate checksum
    for (int i = 0; i < n; i++) {
        sum += data[i];

        if (sum > 255)
            sum = (sum & 255) + 1;
    }

    cs = ~sum;

    printf("Checksum = %02X\n", cs);

    // Connect to server
    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr *)&a, sizeof(a)) < 0) {
        printf("Connection failed\n");
        close(s);
        return 1;
    }

    send(s, &n, sizeof(n), 0);
    send(s, data, n, 0);
    send(s, &cs, 1, 0);

    printf("Data sent successfully\n");

    close(s);
    return 0;
}
