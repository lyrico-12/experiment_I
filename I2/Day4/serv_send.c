// サーバー側 rec -t raw -b 16 -c 1 -e s -r 44100 - | ./serv_send 50001
// クライアント側 ./client_recv 10.100.180.251 50001 | play -t raw -b 16 -c 1 -e s -r 44100 - 

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

void read_and_send(int cs) {
    char buf[BUFSIZE];
    int n;
    while((n = read(0, buf, BUFSIZE)) > 0) {
        if (write(cs, buf, n) != n) {
            perror("write");
            exit(1);
        }
    }
    if (n == -1) {
        perror("read");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int ss = socket(PF_INET, SOCK_STREAM, 0);
    if (ss == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ss, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }
    
    if (listen(ss, 10) < 0) {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int cs = accept(ss, (struct sockaddr *)&client_addr, &len);
    if (cs < 0) {
        perror("accept");
        exit(1);
    }

    read_and_send(cs);

    close(cs);
    close(ss);
    return 0;
}