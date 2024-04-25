#include "get_words.c"
#include <ctype.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTION_LINE 'o'
#define OPEN_BLOCK_LINE 'b'

#define VAR_SECTION "#t"

#define CLOSE_BLOCK "#q"
#define OPEN_BLOCK "#b"

#define IN_BLOCK 1
#define OUT_BLOCK 0

#define MAX_CHILD_BLOCKS 10
#define MAX_VARIABLES 100

typedef enum { INT, STR, BOOL } Type;
typedef enum { CAMEL_CASE, PASCAL_CASE, SNAKE_CASE } CaseType;

typedef struct {
  CaseType case_type;
} Options;

typedef struct Variable {
  Type type;
  char *identifier;
} Variable;

typedef struct Block {
  char *name;
  int childno;
  int varno;
  struct Variable **variables;
  struct Block **childs;
} Block;

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

void camel_case(char *src, char **dst) {
  int i = 0;
  *dst = (char *)malloc(sizeof(char) * strlen(src) + 1);

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

Variable *parse_var(char *ltype, FILE *stream) {
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

  if (getline(&line, &len, stream) == -1) {
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

void parse_block(char *name, FILE *stream, Block *block) {
  char *line = NULL;
  ssize_t len = 0;
  ssize_t nread = 0;
  char word[BUFSIZ];
  WORDS *words;

  char *first_pname = name;

  skip_pwhitespace(name);

  block->name = strdup(name);

  while (getline(&line, &len, stream) != -1) {
    words = get_words(line);

    while ((nread = get_next_word(words, word)) != '\0') {
      if (strncmp(word, CLOSE_BLOCK, 2) == 0) {
        return;
      } else if (strncmp(word, VAR_SECTION, 2) == 0) {
        parse_var(line, stream);
        block->varno++;
      } else if (strncmp(word, OPEN_BLOCK, 2) == 0) {
        Block *child_block = make_block();
        block->childs[block->childno] = child_block;
        block->childno++;
        parse_block(line + nread + 1, stream, child_block);
      }
    }
  }

  free(first_pname);
}

int main(int argc, char *argv[]) {
  FILE *stream;
  char *line = NULL;
  ssize_t len = 0;
  int i, lineno, block_state;

  block_state = IN_BLOCK;
  lineno = 0;

  Options *options = (Options *)malloc(sizeof(Options));
  Block *main_block = make_block();

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  stream = fopen(argv[1], "r");

  if (stream == NULL) {
    fprintf(stderr, "fopen\n");
    exit(EXIT_FAILURE);
  }

  while (getline(&line, &len, stream) != -1) {
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
          parse_block(line + 2, stream, main_block);
          break;
        default:
          fprintf(stderr, "Invalid section '%c'\n", line[i + 1]);
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  free(options);
  free(line);
  fclose(stream);
  exit(EXIT_SUCCESS);
}
