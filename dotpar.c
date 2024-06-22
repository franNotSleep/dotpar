#include "get_words.c"
#include <ctype.h>
#include <error.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dotpar.h"

Block *make_block() {
  Block *block = (Block *)malloc(sizeof(Block));
  block->name = '\0';
  block->childs = (Block **)malloc(sizeof(Block *) * MAX_CHILD_BLOCKS);
  block->variables = (Variable **)malloc(sizeof(Variable *) * MAX_VARIABLES);
  block->varno = 0;
  block->childno = 0;
  return block;
}

void set_case_option(char *option_val, Options *options) {
  if (strcmp("pascal", option_val) == 0) {
    options->case_type = PASCAL_CASE;
  } else if (strcmp("camel", option_val) == 0) {
    options->case_type = CAMEL_CASE;
  } else if (strcmp("snake", option_val) == 0) {
    options->case_type = SNAKE_CASE;
  } else {
    fprintf(stderr, "Unknown case. Available: case | pascal | camel\n");
    exit(EXIT_FAILURE);
  }
}

void parse_option(const char *line, Options *options) {
  int i, j;
  char key[BUFSIZ];
  char val[BUFSIZ];
  WORDS *words = get_words(line);

  if (get_next_word(words, key) == '\0') {
    fprintf(stderr, "No key was found after '#o'.\n");
    exit(EXIT_FAILURE);
  }

  if (get_next_word(words, val) == '\0') {
    fprintf(stderr, "No value was found after '%s'.\n", key);
    exit(EXIT_FAILURE);
  }

  if (strcmp(key, "case") == 0) {
    set_case_option(val, options);
  } else {
    fprintf(stderr, "Invalid '#o' value\n");
    exit(EXIT_FAILURE);
  }
}

Variable *parse_var(char *ltype, FILE *stdin) {
  Variable *var = (Variable *)malloc(sizeof(Variable));
  WORDS *words = get_words(ltype);
  char word[BUFSIZ];
  int i = 0;

  ssize_t len = 0;
  ssize_t nread = 0;
  char *line = NULL;

  get_next_word(words, word); // skip '#t'

  if (get_next_word(words, word) == 0) {
    fprintf(stderr, "Missing type after '#t'\n");
    exit(EXIT_FAILURE);
  }

  if (getline(&line, &len, stdin) == -1) {
    fprintf(stderr, "Missing variable identifier after new line\n");
    exit(EXIT_FAILURE);
  }

  words = get_words(line);

  if (strcmp(word, "str") == 0) {
    var->type = STR;
  } else if (strcmp(word, "int") == 0) {
    var->type = INT;
  } else if (strcmp(word, "bool") == 0) {
    var->type = BOOL;
  } else {
    fprintf(stderr, "Unexpected type '%s'\n", word);
    exit(EXIT_FAILURE);
  }

  if (get_next_word(words, word) == 0) {
    fprintf(stderr, "Missing variable identifier after new line\n");
    exit(EXIT_FAILURE);
  }

  while (word[i] != '=' && word[i] != '\0' && word[i] != ' ') {
    i++;
  }

  if (word[i] == '\0') {
    fprintf(stderr, "Missing '=' after identifier\n");
    exit(EXIT_FAILURE);
  }

  var->identifier = strndup(word, i);
  return var;
}

Block *parse_block(char *name, FILE *stdin) {
  Block *block = make_block();
  char *line = NULL;
  ssize_t len = 0;
  ssize_t nread = 0;
  char word[BUFSIZ];
  WORDS *words;

  trim(name, name, strlen(name));
  block->name = strdup(name);

  while (getline(&line, &len, stdin) != -1) {
    words = get_words(line);

    while ((nread = get_next_word(words, word)) != '\0') {
      if (strncmp(word, CLOSE_BLOCK, 2) == 0) {
        return block;
      } else if (strncmp(word, VAR_SECTION, 2) == 0) {
        block->variables[block->varno] = parse_var(line, stdin);
        block->varno++;
      } else if (strncmp(word, OPEN_BLOCK, 2) == 0) {
        block->childs[block->childno] = parse_block(line + nread + 1, stdin);
        block->childno++;
      }
    }
  }
  return block;
}

int main(int argc, char *argv[]) {
  char *line = NULL;
  ssize_t len = 0;
  int i, lineno, block_state;

  block_state = IN_BLOCK;
  lineno = 0;

  Options *options = (Options *)malloc(sizeof(Options));
  Block *main_block = make_block();
  main_block->name = "Configuration";

  while (getline(&line, &len, stdin) != -1) {
    char next_c = *(line + 1);
    lineno++;
    for (i = 0; i < len; i++) {
      skip_whitespace(line, (ssize_t *)&i);

      if (line[i] == '#' && line[i + 1] != EOF) {
        switch (line[i + 1]) {
        case OPTION_LINE:
          parse_option(line + 2, options);
          break;
        case OPEN_BLOCK_LINE:
          main_block->childs[main_block->childno++] =
              parse_block(line + 2, stdin);
          break;
        default:
          fprintf(stderr, "Invalid section '%c'\n", line[i + 1]);
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  char *content = NULL;
  format_to_ts(main_block, 0, &content);

  fputs(content, stdout);

  free(content);
  free(options);
  free(line);
  fclose(stdin);
  exit(EXIT_SUCCESS);
}
