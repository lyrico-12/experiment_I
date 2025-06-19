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

// added header files
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
  int fd = open("p06.c", O_RDONLY); // modified O to 0

  char buf[100];
  while (1) {
    int n = read(fd, buf, 100);
    if (n == 0) break;
    write(1, buf, n);
  }

  close(fd); // added close
  return 0;
}
