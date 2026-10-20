
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *handleClient(void *arg) {
    int c = *(int *)arg;
    free(arg);

    char roll[20] = {0}, line[100];
    char result[300] = "Student Record Not Found";

    int n = read(c, roll, sizeof(roll)-1);

    if (n > 0) {
        roll[n] = '\0';

        FILE *fp = fopen("students.csv", "r");

        while (fp && fgets(line, sizeof(line), fp)) {
            char r[20], name[30], dept[20], sem[10], cgpa[10];

            sscanf(line, "%19[^,],%29[^,],%19[^,],%9[^,],%9[^\n]",
                   r, name, dept, sem, cgpa);

            if (strcmp(r, roll) == 0) {
                snprintf(result, sizeof(result),
                         "Roll: %s\nName: %s\nDept: %s\nSemester: %s\nCGPA: %s",
                         r, name, dept, sem, cgpa);
                break;
            }
        }

        if (fp) fclose(fp);
        write(c, result, strlen(result)+1);
    }

    close(c);
    return NULL;
}

int main() {
    int s;
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    printf("TCP Server Started...\n");

    while (1) {
        int *c = malloc(sizeof(int));
        *c = accept(s, NULL, NULL);

        if (*c < 0) {
            free(c);
            continue;
        }

        pthread_t t;
        pthread_create(&t, NULL, handleClient, c);
        pthread_detach(t);
    }

    close(s);
    return 0;
}
