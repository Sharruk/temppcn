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
    if (argc < 2) {
        printf("Usage: %s <Node_ID>\n", argv[0]);
        return 1;
    }

    int node_id = atoi(argv[1]);
    char main_data[] = "10101";
    char polynomials[3][10] = {"1011", "1101", "1001"};

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

    printf("\n=======================================================\n");
    printf("   Node %d Transmission Started | Data: %s\n", node_id, main_data);
    printf("=======================================================\n");

    for (int p = 0; p < 3; p++) {
        char *poly = polynomials[p];
        double total_latency = 0;
        int frames_sent = 3;

        printf("\n>>> Testing with Polynomial: %s <<<\n", poly);

        for (int i = 0; i < frames_sent; i++) {
            char crc[10];
            compute_crc(main_data, poly, crc);

            char frame[200];
            sprintf(frame, "%d,%d,%s,%s,%s", node_id, seq_num, main_data, crc, poly);

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

            printf("Seq %d | Frame: %s | Latency: %.4f ms\n", seq_num, frame, latency);
            seq_num++;
            sleep(1);
        }

        printf("--- Summary for %s: Avg Latency = %.4f ms ---\n", poly, total_latency / frames_sent);
    }

    printf("\n=======================================================\n");
    printf("   Node %d Completed All Polynomial Tests!\n", node_id);
    printf("=======================================================\n\n");

    close(sock);
    return 0;
}
