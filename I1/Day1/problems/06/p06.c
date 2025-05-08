/* 
 * p06.c
 */

/* 
 * usage:
 * 
 *   ./a.out
 *
 * Intented behavior:
 * It should print the content of this file.
 *
 */

// ヘッダーファイルを追加
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
  int fd = open("p06.c", O_RDONLY); // 数字の0を編集

  char buf[100];
  while (1) {
    int n = read(fd, buf, 100);
    if (n == 0) break;
    write(1, buf, n);
  }

  close(fd); // closeを追加
  return 0;
}
