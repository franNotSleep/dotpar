#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "case.c"
#include "dotpar.h"

void format_to_ts(Block *block, int depth, char **dst) {
  char buf[BUFSIZ];
  char tabs[20];
  int nspace;
  int next_depth;
  int i;

  if (*dst == NULL) {
    *dst = (char *)malloc(sizeof(char) * BUFSIZ);
    (*dst)[0] = '\0';
  }

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
    sprintf(buf, "interface %s {\n", block->name);
    strcat(*dst, buf);
  } else {
    sprintf(buf, "%s%s: {\n", tabs, block->name);
    strcat(*dst, buf);
  }

  for (int i = 0; i < block->varno; i++) {
    char *identifier = block->variables[i]->identifier;
    char *camel_case_identifier;
    camel_case(identifier, &camel_case_identifier);
    char *type;

    if (block->variables[i]->type == INT) {
      type = "number";
    } else if (block->variables[i]->type == STR) {
      type = "string";
    } else if (block->variables[i]->type == BOOL) {
      type = "boolean";
    }
    sprintf(buf, "%s  %s: %s;\n", tabs, camel_case_identifier, type);
    strcat(*dst, buf);
  }

  for (int i = 0; i < block->childno; i++) {
    format_to_ts(block->childs[i], next_depth, dst);
  }

  if (depth == 0) {
    sprintf(buf, "};\n");
    strcat(*dst, buf);
  } else {
    sprintf(buf, "%s};\n", tabs);
    strcat(*dst, buf);
  }
}
