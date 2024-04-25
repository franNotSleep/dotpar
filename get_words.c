#include "common.c"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *line;
  ssize_t next_word_start;
} WORDS;

WORDS *get_words(const char *line) {
  WORDS *l = (WORDS *)malloc(sizeof(WORDS));

  l->line = (char *)malloc(sizeof(char) * strlen(line));
  strcpy(l->line, line);
  skip_whitespace(l->line, &l->next_word_start);

  return l;
}

int get_next_word(WORDS *words, char *word) {
  ssize_t i = words->next_word_start;
  ssize_t j = 0;

  while (!isspace(words->line[i]) && words->line[i] != '\0') {
    word[j] = words->line[i];
    i++;
    j++;
  }

  word[j] = '\0';
  words->next_word_start = i;
  skip_whitespace(&(words->line[i]), &words->next_word_start);

  return j == 0 ? '\0' : i;
}
