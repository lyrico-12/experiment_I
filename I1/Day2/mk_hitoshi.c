#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("hitoshi", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    unsigned char hitoshi[6] = {228, 186, 186, 229, 191, 151};
    write(fd, hitoshi, 6);
    close(fd);
    return 0;
}