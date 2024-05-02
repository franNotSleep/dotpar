#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dotpar.h"


char *to_ts_format_block(Block *block, int depth) {
  char *interface = (char *)malloc(sizeof(char) * BUFSIZ);
  char tabs[20];
  int nspace;
  int next_depth;
  int i;

  if (depth != 0) {
    next_depth = depth + 1;
    nspace = pow(2, depth);
  } else {
    nspace = 0;
    next_depth = 1;
  }

  for (i = 0; i < nspace; i++) {
    tabs[i] = ' ';
  }

  tabs[i] = '\0';
  if (depth == 0) {
    char format[BUFSIZ];
    sprintf(format, "interface %s {\n", block->name);
    strcat(interface, format);
  } else {
    char format[BUFSIZ];
    sprintf(format, "%s%s: {\n", tabs, block->name);
    strcat(interface, format);
  }

  for (int i = 0; i < block->varno; i++) {
    char *identifier = block->variables[i]->identifier;
    char *type;

    if (block->variables[i]->type == INT) {
      type = "number";
    } else if (block->variables[i]->type == STR) {
      type = "string";
    } else if (block->variables[i]->type == BOOL) {
      type = "boolean";
    }
    char format[BUFSIZ];
    sprintf(format, "%s  %s: %s;\n", tabs, identifier, type);
    strcat(interface, format);
  }

  for (int i = 0; i < block->childno; i++) {
    to_ts_format_block(block->childs[i], next_depth);
  }

  if (depth == 0) {
    strcat(interface, "};\n");
  } else {
    char format[BUFSIZ];
    sprintf(format, "%s};\n", tabs);
    strcat(interface, format);
  }
}
