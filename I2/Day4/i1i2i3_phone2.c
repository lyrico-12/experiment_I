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

// サーバー
void server(char *port) {
    int ss = socket(PF_INET, SOCK_STREAM, 0);
    if (ss == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
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

    FILE *rec_fp = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r");
    if (rec_fp == NULL) {
        perror("popen");
        exit(1);
    }

    char buf[BUFSIZE];
    int n;

    while(1) {
        int recv = read(cs, buf, BUFSIZE);
        if (recv == -1) {
            perror("read");
            exit(1);
        }
        if (recv == 0) {
            break;
        }
        fwrite(buf, 1, recv, stdout);
        fflush(stdout);

        int n = fread(buf, 1, BUFSIZE, rec_fp);
        if (n == -1) {
            perror("read");
            exit(1);
        }
        if (write(cs, buf, n) != n) {
            perror("write");
            exit(1);
        }
        if (n == 0) {
            break;
        }
    }

    
    pclose(rec_fp);
    close(cs);
    close(ss);
}



// クライアント
void client(char* ip, char* port) {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect");
        exit(1);
    }

    FILE *rec_fp = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r");
    if (rec_fp == NULL) {
        perror("popen");
        exit(1);
    }

    char buf[BUFSIZE];
    int n;
    while(1) {
        n = fread(buf, 1, BUFSIZE, rec_fp);
        if (n == -1) {
            perror("fread");
            break;
        }
        if (n == 0) {
            break;
        }
        if (write(s, buf, n) == -1) {
            perror("send");
            break;
        }

        int recv = read(s, buf, BUFSIZE);
        if (recv == -1) {
            perror("recv");
        }
        if (recv == 0) {
            break;
        }
        fwrite(buf, 1, recv, stdout);
        fflush(stdout);
    }

    close(s);
    pclose(rec_fp);
}


int main(int argc, char *argv[]) {
    if (argc == 2) {
        server(argv[1]);
    } else if (argc == 3) {
        client(argv[1], argv[2]);
    } else {
        printf("Usage: %s <server_ip> <port>\nor\n", argv[0]);
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
}