#include <ctype.h>

void camel_case(char *src, char **dst) {
  int i = 0;
  *dst = (char *)malloc(sizeof(char) * strlen(src) * 2);

  while (*src != '\0') {

    if (*src == '_') {
      *(*dst + i++) = toupper(*++src);
      ++src;
      continue;
    }

    *(*dst + i++) = tolower(*src++);
  }
  *(*dst + i) = '\0';
}
