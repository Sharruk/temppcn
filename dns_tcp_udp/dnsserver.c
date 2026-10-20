
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int s, n;
    char domain[100], ip[100];

    struct sockaddr_in server = {0}, client;
    socklen_t len = sizeof(client);

    // DNS table: domain -> IP
    char names[5][30] = {
        "google.com", "youtube.com", "github.com",
        "example.com", "ssn.edu.in"
    };

    char ips[5][30] = {
        "142.250.72.14", "142.250.72.206",
        "140.82.112.4", "93.184.216.34",
        "192.168.1.10"
    };

    // Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address and port
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // Attach socket to port
    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("DNS server waiting...\n");

    while (1) {
        // Receive domain name
        n = recvfrom(s, domain, 99, 0,
                     (struct sockaddr *)&client, &len);

        if (n <= 0) continue;

        domain[n] = '\0';
        printf("Received: %s\n", domain);

        // Special domain to test timeout
        if (strcmp(domain, "timeout.com") == 0)
            continue;

        strcpy(ip, "NXDOMAIN"); // Default: not found

        // Search DNS table
        for (int i = 0; i < 5; i++) {
            if (strcmp(domain, names[i]) == 0) {
                strcpy(ip, ips[i]);
                break;
            }
        }

        // Send IP or NXDOMAIN to client
        sendto(s, ip, strlen(ip), 0,
               (struct sockaddr *)&client, len);
    }

    close(s);
    return 0;
}
