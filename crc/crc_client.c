
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void divide(char b[]) {
    char g[] = "10011";
    int n = strlen(b);

    for (int i = 0; i <= n-5; i++) {
        if (b[i] == '1') {
            for (int j = 0; j < 5; j++)
                b[i+j] = (b[i+j] == g[j]) ? '0' : '1';
        }
    }
}

int main() {
    int s;
    char text[100], binary[4096];
    char temp[4096], frame[4096];
    struct sockaddr_in a = {0};

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    // Convert every character into 8 bits
    int k = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        for (int j = 7; j >= 0; j--)
            binary[k++] = ((text[i] >> j) & 1) + '0';
    }

    binary[k] = '\0';

    printf("Binary data: %s\n", binary);

    // Append four zeros
    strcpy(temp, binary);
    strcat(temp, "0000");

    // Calculate CRC
    divide(temp);

    char crc[5];

    for (int i = 0; i < 4; i++)
        crc[i] = temp[k+i];

    crc[4] = '\0';

    printf("CRC: %s\n", crc);

    // Append CRC to original data
    strcpy(frame, binary);
    strcat(frame, crc);

    printf("Frame: %s\n", frame);

    // Connect to server
    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8081);
    a.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr *)&a, sizeof(a)) < 0) {
        printf("Connection failed\n");
        close(s);
        return 1;
    }

    int n = strlen(frame);

    send(s, &n, sizeof(n), 0);
    send(s, frame, n+1, 0);

    printf("Frame sent successfully\n");

    close(s);
    return 0;
}
