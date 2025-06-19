#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFSIZE 1000

int main(int argc, char *argv[]) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int send_count = 0;

    while (send_count < 50) {
        char buf[BUFSIZE];
        int n;
        if ((n = read(0, buf, BUFSIZE)) == -1) {
            perror("read");
            close(sock);
            exit(1);
        }

        if (n == BUFSIZE) {
            int is_eod = 1;
            for (int i = 0; i < BUFSIZE; i++) {
                if ((unsigned char)buf[i] != 1) {
                    is_eod = 0;
                    break;
                }
            }
            if (is_eod) {
                break;
            }
        }

        if (sendto(sock, buf, n, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("sendto");
            close(sock);
            exit(1);
        }
        send_count++;
    }

    while (1) {
        char buf[BUFSIZE];
        socklen_t len = sizeof(addr);
        ssize_t n = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *)&addr, &len);
        if (n < 0) {
            perror("recvfrom");
            close(sock);
            exit(1);
        }

        if (n == BUFSIZE) {
            int is_eod = 1;
            for (int i = 0; i < BUFSIZE; i++) {
                if ((unsigned char)buf[i] != 1) {
                    is_eod = 0;
                    break;
                }
            }
            if (is_eod) {
                break;
            }
        }

        if (write(1, buf, n) != n) {
            perror("write");
            close(sock);
            exit(1);
        }
    }
    close(sock);
    return 0;
}
