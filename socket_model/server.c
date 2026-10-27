#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

int verify_crc(char *data, char *crc, char *poly) {
    int data_len = strlen(data);
    int poly_len = strlen(poly);
    char temp[100];
    strcpy(temp, data);
    strcat(temp, crc);

    for (int i = 0; i < data_len; i++) {
        if (temp[i] == '1') {
            for (int j = 0; j < poly_len; j++) {
                if (temp[i + j] == poly[j]) {
                    temp[i + j] = '0';
                } else {
                    temp[i + j] = '1';
                }
            }
        }
    }

    for (int i = data_len; i < data_len + poly_len - 1; i++) {
        if (temp[i] != '0') {
            return 0;
        }
    }
    return 1;
}

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    free(socket_desc);
    char buffer[256];
    int read_size;

    while ((read_size = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[read_size] = '\0';
        int node_id, seq_num;
        char data_bits[50], crc_bits[20], poly[20];

        if (sscanf(buffer, "%d,%d,%[^,],%[^,],%s", &node_id, &seq_num, data_bits, crc_bits, poly) == 5) {
            int is_valid = verify_crc(data_bits, crc_bits, poly);
            FILE *log_file = fopen("transmission_log.txt", "a");

            if (is_valid) {
                printf("[SUCCESS] Node %d | Frame %d | CRC match using %s\n", node_id, seq_num, poly);
                if (log_file) {
                    fprintf(log_file, "Node: %d, Seq: %d, Poly: %s, Status: GOOD\n", node_id, seq_num, poly);
                }
            } else {
                printf("[ERROR] Node %d | Frame %d | Corrupted frame detected using %s!\n", node_id, seq_num, poly);
                if (log_file) {
                    fprintf(log_file, "Node: %d, Seq: %d, Poly: %s, Status: CORRUPTED\n", node_id, seq_num, poly);
                }
            }

            if (log_file) fclose(log_file);
            send(sock, "A", 1, 0);
        }
    }

    close(sock);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int opt = 1;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 5);
    printf("Server listening on port 8080...\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        
        if (client_sock < 0) {
            continue;
        }

        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        pthread_create(&thread_id, NULL, handle_client, (void*)new_sock);
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
