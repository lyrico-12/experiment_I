#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fp = open(argv[1], O_RDONLY);
    if (fp == -1) {
        perror("open");
        exit(1);
    }
    short data;
    int count = 0;
    while (1) {
        int n = read(fp, &data, 2);
        if (n == -1) {
            perror("read");
            exit(1);
        }
        if (n == 0) {
            break;
        }
        printf("%d %d\n", count, data);
        count++;
    }
    
    close(fp);
    return 0;
}