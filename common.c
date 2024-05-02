#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

void skip_whitespace(const char *src, ssize_t *i) {
  int j = 0;
  while (isspace(src[j++])) {
    (*i)++;
  }
}

void skip_pwhitespace(char *src) {
  while (isspace(*src)) {
    src++;
  }
}

void trim(char from[], char to[], int lim) {
  int start, i, len;

  len = strlen(from);
  i = 0;
  start = 0;

  skip_whitespace(from, (ssize_t *)&start);

  while (!isspace(from[start]) && from[start] != '\0') {
    to[i++] = from[start++];
  }

  to[i] = '\0';
}
