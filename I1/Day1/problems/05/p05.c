/* 
 * p05.c
 */

/* 
 * usage:
 *   ./a.out
 * 
 * Intended behavior
 *   It reads the contents of this file and print it.
 */

#include <assert.h>
#include <stdio.h>
int main()
{
  FILE * fp = fopen("p05.c", "rb"); // edited O to 0
  char buf[100];
  while (1) {
    size_t n = fread(buf, 1, 100, fp); // changed int to size_t
    if (n == 0) break;
    fwrite(buf, 1, n, stdout);
  }

  fclose(fp);
  return 0;
}

