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
