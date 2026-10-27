#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

void compute_crc(char *data, char *poly, char *crc) {
    int data_len = strlen(data);
    int poly_len = strlen(poly);
    char temp[100];
    strcpy(temp, data);
    
    for (int i = 0; i < poly_len - 1; i++) {
        strcat(temp, "0");
    }
    
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
    
    strncpy(crc, temp + data_len, poly_len - 1);
    crc[poly_len - 1] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <Node_ID> <Poly_Choice: 1, 2, or 3>\n", argv[0]);
        return 1;
    }

    int node_id = atoi(argv[1]);
    int choice = atoi(argv[2]);
    char poly[10];
    char poly_name[20];

    if (choice == 1) {
        strcpy(poly, "1011");
        strcpy(poly_name, "CRC-4 (1011)");
    } else if (choice == 2) {
        strcpy(poly, "1101");
        strcpy(poly_name, "CRC-4 (1101)");
    } else {
        strcpy(poly, "1001");
        strcpy(poly_name, "CRC-3 (1001)");
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connect failed");
        return 1;
    }

    srand(time(NULL) + node_id);
    int seq_num = 1;
    double total_latency = 0;

    printf("\n--- Starting Transmission for Node %d using %s ---\n", node_id, poly_name);

    for (int i = 0; i < 5; i++) {
        int reading = rand() % 100;
        char data_bits[20] = "";
        for (int b = 7; b >= 0; b--) {
            if ((reading >> b) & 1) strcat(data_bits, "1");
            else strcat(data_bits, "0");
        }

        char crc[10];
        compute_crc(data_bits, poly, crc);

        char frame[200];
        sprintf(frame, "%d,%d,%s,%s,%s", node_id, seq_num, data_bits, crc, poly);

        if (rand() % 100 < 30) {
            int err_pos = rand() % strlen(frame);
            if (frame[err_pos] == '0') frame[err_pos] = '1';
            else if (frame[err_pos] == '1') frame[err_pos] = '0';
        }

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        send(sock, frame, strlen(frame), 0);

        char ack;
        recv(sock, &ack, 1, 0);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double latency = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
        total_latency += latency;

        printf("Frame %d sent: %s | Latency: %.4f ms\n", seq_num, frame, latency);
        seq_num++;
        sleep(1);
    }

    printf("\n>>> Node %d Average Latency with %s: %.4f ms <<<\n\n", node_id, poly_name, total_latency / 5.0);
    close(sock);
    return 0;
}
