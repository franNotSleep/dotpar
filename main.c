#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTIONS_SECTION 'o'
#define BLOCK_SECTION 'b'
#define TYPE_SECTION 't'

typedef enum { INT, STR, BOOL } Type;
typedef enum { CAMEL_CASE, PASCAL_CASE, SNAKE_CASE } CaseType;

typedef struct {
  CaseType case_type;
} Options;

typedef struct {
  Type type;
  char *identifier;
  char *value;
} Variable;

typedef struct {
  char *block_name;
  Variable *variables;
} Block;

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

void parse_options(FILE *stream, Options *options) {
  char *line = NULL;
  ssize_t len = 0;
  char *next;
  char option[BUFSIZ];
  char option_val[BUFSIZ];
  int i = 0;

  while (getline(&line, &len, stream) != -1) {
    next = line;
    next++;

    if (*line == '#' && *next == OPTIONS_SECTION) {
      break;
    }

    while (*line++ == '#' || isspace(*line++))
      ;

    --line;
    for (i = 0; !isspace(*line); line++, i++) {
      option[i] = *line;
    }
    option[i] = '\0';

    while (isspace(*line++))
      ;

    --line;
    for (i = 0; !isspace(*line); line++, i++) {
      option_val[i] = *line;
    }
    option_val[i] = '\0';

    if (strcmp("case", option) == 0) {
      set_case_option(option_val, options);
    }
  }
}

void parse_block(char *line) { printf("block\n"); }

void parse_type(char *line) { printf("type\n"); }

int main(int argc, char *argv[]) {
  FILE *stream;
  char *line = NULL;
  ssize_t len = 0;

  Options *options = (Options *)malloc(sizeof(Options));

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
    if (*line == '#' && next_c != ' ' && next_c != EOF) {
      switch (next_c) {
      case OPTIONS_SECTION:
        parse_options(stream, options);
        break;
      case BLOCK_SECTION:
        parse_block(line);
        break;
      case TYPE_SECTION:
        parse_type(line);
        break;
      default:
        fprintf(stderr, "Invalid section '%c'\n", *line);
        exit(EXIT_FAILURE);
      }
    }
  }
  printf("case %d\n", options->case_type);

  free(options);
  free(line);
  fclose(stream);
  exit(EXIT_SUCCESS);
}
