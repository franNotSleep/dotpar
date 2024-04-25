#include <ctype.h>
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
