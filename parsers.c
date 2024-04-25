int main(int argc, char *argv[]) {
  FILE *stream;
  char *line = NULL;
  ssize_t len = 0;
  int i, lineno, block_state;

  block_state = IN_BLOCK;
  lineno = 0;

  Options *options = (Options *)malloc(sizeof(Options));
  Block *main_block = (Block *)malloc(sizeof(Block));

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
          printf("open block!!\n");
          break;
        case CLOSE_BLOCK_LINE:
          printf("close block!!\n");
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
