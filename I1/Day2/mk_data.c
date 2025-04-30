#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fp = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fp == -1) {
        perror("open");
        exit(1);
    }
    for (int i = 0; i <= 255; i++) {
        unsigned char byte = (unsigned char)i;
        int n = write(fp, &byte, 1);
        if (n == -1) {
            perror("write");
            exit(1);
        }
    }

    close(fp);
    return 0;
}