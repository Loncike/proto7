#include "lexer.h"
#include "parser.h"
#include "vm.h"
#include <stdio.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define STRLIB_NO_PREFIX
#define STRLIB_IMPLEMENTATION
#include "strlib.h"

int main(int argc, char *argv[]){

  if (argc != 2){
    printf("Usage : %s <file>\n", argv[0]);
    exit(1);
  }
  const char *program = readFile(argv[1]);
  
  VirtualMachine vm = {0};
  vmInit(&vm);

  Token *tokens = NULL;
  lexer(program, &tokens);

  Op *ops = NULL;
  Parse(&ops, tokens);

  // for (int i = 0; i < arrlen(ops); ++i){
  //   printf("[%d]> Op: %s| val: %ld\n", i, opName(ops[i].kind), ops[i].as.val);
  // }

  eval(&vm, ops);

  return 0;
}
