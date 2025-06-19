#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFSIZE 1024

int sockfd;

// 受信スレッド（標準出力に出す）
void *recv_thread(void *arg) {
    char buf[BUFSIZE];
    int n;
    while ((n = read(sockfd, buf, BUFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            perror("write to stdout");
            break;
        }
    }
    return NULL;
}

// 送信スレッド（recコマンドから音声を取得）
void *send_thread(void *arg) {
    char buf[BUFSIZE];
    FILE *rec_fp = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r");
    if (!rec_fp) {
        perror("popen");
        return NULL;
    }

    // バッファリングを無効化して遅延を減らす
    setvbuf(rec_fp, NULL, _IONBF, 0);

    int n;
    while ((n = fread(buf, 1, BUFSIZE, rec_fp)) > 0) {
        if (write(sockfd, buf, n) != n) {
            perror("write to socket");
            break;
        }
    }

    pclose(rec_fp);
    shutdown(sockfd, SHUT_WR);
    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in addr;

    if (argc == 2) {
        // サーバーモード
        int port = atoi(argv[1]);

        int ss = socket(AF_INET, SOCK_STREAM, 0);
        if (ss < 0) { perror("socket"); exit(1); }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(ss, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind"); exit(1);
        }
        if (listen(ss, 1) < 0) {
            perror("listen"); exit(1);
        }

        socklen_t len = sizeof(addr);
        sockfd = accept(ss, (struct sockaddr *)&addr, &len);
        if (sockfd < 0) { perror("accept"); exit(1); }

        close(ss);
    } else if (argc == 3) {
        // クライアントモード
        const char *ip = argv[1];
        int port = atoi(argv[2]);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) { perror("socket"); exit(1); }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("connect"); exit(1);
        }
    } else {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  server: %s <port>\n", argv[0]);
        fprintf(stderr, "  client: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    pthread_t th_recv, th_send;
    pthread_create(&th_recv, NULL, recv_thread, NULL);
    pthread_create(&th_send, NULL, send_thread, NULL);

    pthread_join(th_recv, NULL);
    pthread_join(th_send, NULL);

    close(sockfd);
    return 0;
}
