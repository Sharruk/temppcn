
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <ctype.h>

int main() {
    int s, n;
    char domain[100], ip[100];

    // Simple cache
    char cache_name[20][100];
    char cache_ip[20][100];
    int count = 0;

    struct sockaddr_in server = {0};

    // Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // Wait at most 3 seconds for reply
    struct timeval t = {3, 0};
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    while (1) {
        printf("\nEnter domain (or exit): ");
        scanf("%99s", domain);

        if (strcmp(domain, "exit") == 0)
            break;

        // Basic domain format check
        if (strchr(domain, '.') == NULL ||
            domain[0] == '.' ||
            domain[strlen(domain)-1] == '.') {
            printf("Invalid domain format\n");
            continue;
        }

        // Search cache first
        int found = 0;

        for (int i = 0; i < count; i++) {
            if (strcmp(domain, cache_name[i]) == 0) {
                strcpy(ip, cache_ip[i]);
                found = 1;
                printf("Cache hit!\n");
                break;
            }
        }

        // If not cached, ask server
        if (!found) {
            sendto(s, domain, strlen(domain), 0,
                   (struct sockaddr *)&server, sizeof(server));

            n = recvfrom(s, ip, 99, 0, NULL, NULL);

            if (n < 0) {
                printf("Request timed out\n");
                continue;
            }

            ip[n] = '\0';

            if (strcmp(ip, "NXDOMAIN") == 0) {
                printf("Domain not found\n");
                continue;
            }

            // Store successful lookup in cache
            if (count < 20) {
                strcpy(cache_name[count], domain);
                strcpy(cache_ip[count], ip);
                count++;
            }
        }

        printf("IP address: %s\n", ip);
    }

    close(s);
    return 0;
}
