
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void divide(char b[]) {
    char g[] = "10011";
    int n = strlen(b);

    for (int i = 0; i <= n - 5; i++) {
        if (b[i] == '1') {
            for (int j = 0; j < 5; j++)
                b[i+j] = (b[i+j] == g[j]) ? '0' : '1';
        }
    }
}

int main() {
    int s, c, n;
    char frame[4096], check[4096];
    struct sockaddr_in a = {0};

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8081);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&a, sizeof(a));
    listen(s, 1);

    printf("Waiting for client...\n");
    c = accept(s, NULL, NULL);

    recv(c, &n, sizeof(n), MSG_WAITALL);
    recv(c, frame, n + 1, MSG_WAITALL);

    printf("Received frame: %s\n", frame);

    // Optional: flip one bit to create an error 
    // AQ.Ab8RN6LInPsJ0oaWoQw34PZImn02KNbJBMUddh9IH2Y__cgbMA
    int choice, pos;

    printf("Introduce error? (1=yes, 0=no): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter bit position (0 to %d): ", n-1);
        scanf("%d", &pos);

        if (pos >= 0 && pos < n)
            frame[pos] = (frame[pos] == '0') ? '1' : '0';
    }

    printf("Checking frame: %s\n", frame);

    strcpy(check, frame);
    divide(check);

    // Last four bits must be zero
    int error = 0;

    for (int i = n-4; i < n; i++) {
        if (check[i] != '0')
            error = 1;
    }

    if (error)
        printf("Error detected\n");
    else
        printf("No error detected\n");

    close(c);
    close(s);
    return 0;
}
