#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "使用法: %s ダウンサンプル率\n", argv[0]);
        exit(1);
    }
    int downsample_rate = atoi(argv[1]);
    if (downsample_rate <= 0) {
        fprintf(stderr, "ダウンサンプル率は正の整数である必要があります\n");
        exit(1);
    }
    short data;
    int count = 0;
    
    while (1) {
        int n = read(0, &data, sizeof(short));
        if (n == -1) {
            perror("read");
            exit(1);
        }
        if (n == 0) {
            break;
        }
        if (count % downsample_rate == 0) {
            write(1, &data, sizeof(short));
        }
        count++;
    }
    return 0;
}