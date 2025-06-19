#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define BUFSIZE 1024

void read_and_print(int s) {
    char buf[BUFSIZE];
    int n;
    while((n = read(s, buf, BUFSIZE)) > 0) {
        if (write(1, buf, n) != n) {
            perror("write");
            exit(1);
        }
    }
    if (n == -1) {
        perror("read");
        exit(1);
    }
}

int write_from_stdin(int s) {
    char buf[BUFSIZE];
    int n;
    while((n = read(0, buf, BUFSIZE)) > 0) {
        if (write(s, buf, n) != n) {
            perror("write");
            exit(1);
            return -1;
        }
    }

    if (n < 0) {
        perror("read from stdin");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect");
        exit(1);
    }

    write_from_stdin(s);
    shutdown(s, SHUT_WR);
    read_and_print(s);

    close(s);
    return 0;
}