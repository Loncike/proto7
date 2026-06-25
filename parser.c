#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "vm.h"

#define STRLIB_NO_PREFIX
#include "strlib.h"

#include "stb_ds.h"


const char *opName(OpKind kind);

static const struct { const char *word; OpKind kind; } keywords[] = {
  {"push", PUSH},

  {"add", ADD},
  {"sub", SUB},
  {"mul", MUL},
  {"div", DIV},

  {"and", AND},
  {"or", OR},
  {"xor", XOR},
  {"not", NOT},
  {"shl", SHL},
  {"shr", SHR},

  {"eq", EQ},
  {"gt", GT},
  {"lt", LT},

  { "dup", DUP},
  { "over", OVER},
  { "swap", SWAP},
  { "rot", ROT},
  { "drop", DROP},

  {"alloc", ALLOC},
  {"write", WRITE},
  {"read", READ},
  {"free", FREE},

  {"jmp", JMP},
  {"jt", JT},
  {"jf", JF},
  {"call", CALL},
  {"ret", RET},

  { "dump", DUMP},
  { "syscall", SYSCALL},

};

OpKind checkKeyword(stringView sv){
  for (int i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); ++i){
    if (memcmp(sv.data, keywords[i].word, sv.len) == 0 && keywords[i].word[sv.len] == '\0'){
      return keywords[i].kind;
    }
  }
  return NIL;
}


typedef struct{
  stringView name;
  int idx;
} Symbol;

//returns idx if symbol exists, -1 if doesnt
int checkSymbol(Symbol *symbols, stringView *sv){
  for (int i = 0; i < arrlen(symbols); ++i){
    if (svcmp(sv, &symbols[i].name) == 0){
      return symbols[i].idx;
    }
  }
  return -1;
}


void generateSymbols(Symbol **symbols, Token *tokens){
  int opsCount = 0;
  int i = 0;
  while (i < arrlen(tokens)-1){
    OpKind opkind = checkKeyword(tokens[i].as.sv);
    if (opkind == 0) { // Should be label, but what if it no? 
      ++i;
      if (tokens[i].type != COLON) continue;
      if (checkSymbol(*symbols, &tokens[i-1].as.sv) != -1){
        printf("symbol: '%.*s' already declared, redeclaration on line %d\n", (int)tokens[i-1].as.sv.len, tokens[i-1].as.sv.data, tokens[i-1].line);
        exit(1);
      }
      arrput(*symbols, ((Symbol){.name = tokens[i-1].as.sv, .idx = opsCount}));

    }
    switch (opkind){
        case PUSH: i+=2; opsCount++; continue;
        case ADD: ++i; opsCount++; continue;
        case SUB: ++i; opsCount++; continue;
        case MUL: ++i; opsCount++; continue;
        case DIV: ++i; opsCount++; continue;
        case AND: ++i; opsCount++; continue;
        case OR: ++i; opsCount++; continue;
        case XOR: ++i; opsCount++; continue;
        case NOT: ++i; opsCount++; continue;
        case SHL: ++i; opsCount++; continue;
        case SHR: ++i; opsCount++; continue;
        case EQ: ++i; opsCount++; continue;
        case GT: ++i; opsCount++; continue;
        case LT: ++i; opsCount++; continue;
        case DUP: ++i; opsCount++; continue;
        case OVER: ++i; opsCount++; continue;
        case SWAP: ++i; opsCount++; continue;
        case ROT: ++i; opsCount++; continue;
        case DROP: ++i; opsCount++; continue;
        case ALLOC: i+=2; opsCount++; continue;
        case WRITE: i+=2; opsCount++; continue;
        case READ: i+=2; opsCount++; continue;
        case FREE: ++i; opsCount++; continue;
        case JMP: i+=2; opsCount++; continue;
        case JT: i+=2; opsCount++; continue;
        case JF: i+=2; opsCount++; continue;
        case CALL: i+=2; opsCount++; continue;
        case RET: ++i; opsCount++; continue;
        case DUMP: ++i; opsCount++; continue;
        case SYSCALL: i+=2; opsCount++; continue;
        default:
          break;
      }
    ++i;
  }
}


void Parse(Op **ops, Token *tokens){
  int idx = -1; // idx for jumps
  int i = 0;

  Symbol *symbols = NULL;
  generateSymbols(&symbols, tokens);

  while (i < arrlen(tokens)){
    if (tokens[i].type == ID){
      OpKind opkind = checkKeyword(tokens[i].as.sv);
      if (opkind == 0) { // Should be label, but what if it no? 
        ++i;
        if (tokens[i].type != COLON){
          printf("Expected ':' after a label declaration on line: %d\n", i-1);
          exit(1);
        }

        ++i;
        continue;
      }
      switch (opkind){
        case PUSH:
          ++i;
          if (tokens[i].type != INT) {
            printf("Expected number after Push instruction on line: %d", tokens[i-1].line);
            exit(1);
          }; 
          arrput(*ops, ((Op){.kind = PUSH, .as.val = tokens[i].as.value}));
          ++i;
          continue;
        case ADD:
          arrput(*ops, ((Op){.kind = ADD}));
          ++i;
          continue;
        case SUB:
          arrput(*ops, ((Op){.kind = SUB}));
          ++i;
          continue;
        case MUL:
          arrput(*ops, ((Op){.kind = MUL}));
          ++i;
          continue;
        case DIV:
          arrput(*ops, ((Op){.kind = DIV}));
          ++i;
          continue;
        case AND:
          arrput(*ops, ((Op){.kind = AND}));
          ++i;
          continue;
        case OR:
          arrput(*ops, ((Op){.kind = OR}));
          ++i;
          continue;
        case XOR:
          arrput(*ops, ((Op){.kind = XOR}));
          ++i;
          continue;
        case NOT:
          arrput(*ops, ((Op){.kind = NOT}));
          ++i;
          continue;
        case SHL:
          arrput(*ops, ((Op){.kind = SHL}));
          ++i;
          continue;
        case SHR:
          arrput(*ops, ((Op){.kind = SHR}));
          ++i;
          continue;
        case EQ:
          arrput(*ops, ((Op){.kind = EQ}));
          ++i;
          continue;
        case GT:
          arrput(*ops, ((Op){.kind = GT}));
          ++i;
          continue;
        case LT:
          arrput(*ops, ((Op){.kind = LT}));
          ++i;
          continue;
        case DUP:
          arrput(*ops, ((Op){.kind = DUP}));
          ++i;
          continue;
        case OVER:
          arrput(*ops, ((Op){.kind = OVER}));
          ++i;
          continue;
        case SWAP:
          arrput(*ops, ((Op){.kind = SWAP}));
          ++i;
          continue;
        case ROT:
          arrput(*ops, ((Op){.kind = ROT}));
          ++i;
          continue;
        case DROP:
          arrput(*ops, ((Op){.kind = DROP}));
          ++i;
          continue;
        case ALLOC:
          ++i;
          if (tokens[i].type == ID){
            if (svcmpToCstring(&tokens[i].as.sv, "stack") == 0){
              arrput(*ops, ((Op){.kind = ALLOC, .as.val = ALLOC_FROM_STACK})); // -1 for value from stack
            }else{
              printf("Unexpected token %.*s on line: %d\n", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i].line);
              exit(1);
            }
          }else if (tokens[i].type == INT){  
            arrput(*ops, ((Op){.kind = ALLOC, .as.val = tokens[i].as.value}));
          }else{            
            printf("Expected number/'stack' keyword after Alloc instruction on line: %d", tokens[i-1].line);
            exit(1);
          }
          ++i;
          continue;
        case WRITE:
          ++i;
          if (tokens[i].type != INT) {
            printf("Expected number after Write instruction on line: %d", tokens[i-1].line);
            exit(1);
          }; 
          arrput(*ops, ((Op){.kind = WRITE, .as.val = tokens[i].as.value}));
          ++i;
          continue;
        case READ:
          ++i;
          if (tokens[i].type != INT) {
            printf("Expected number after Read instruction on line: %d", tokens[i-1].line);
            exit(1);
          }; 
          arrput(*ops, ((Op){.kind = READ, .as.val = tokens[i].as.value}));
          ++i;
          continue;
        case FREE:
          arrput(*ops, ((Op){.kind = FREE}));
          ++i;
          continue;
        case JMP:
          ++i;
          if (tokens[i].type != ID) {
            printf("Expected label after Jmp instruction on line: %d", tokens[i-1].line);
            exit(1);
          }
          idx = checkSymbol(symbols, &tokens[i].as.sv);
          if (idx == -1) {
            if (checkKeyword(tokens[i].as.sv) != NIL) {
              printf("Expected label but got keyword:'%.*s' after Jmp instruction on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i-1].line);
            }else{
              printf("Symbol '%.*s' doesnt exists on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i-1].line);
            }
            exit(1);
          };

          arrput(*ops, ((Op){.kind = JMP, .as.val = idx}));

          ++i;
          continue;
        case JT:
          ++i;
          if (tokens[i].type != ID) {
            printf("Expected label after Jt instruction on line: %d", tokens[i-1].line);
            exit(1);
          }
          idx = checkSymbol(symbols, &tokens[i].as.sv);
          if (idx == -1) {
            if (checkKeyword(tokens[i].as.sv) != NIL) {
              printf("Expected label but got keyword:'%.*s' after Jt instruction on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i - 1].line);
            } else {
              printf("Symbol '%.*s' doesnt exists on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i - 1].line);
            }
            exit(1);
          };

          arrput(*ops, ((Op){.kind = JT, .as.val = idx}));

          ++i;
          continue;
        case JF:
          ++i;
          if (tokens[i].type != ID) {
            printf("Expected label after Jf instruction on line: %d", tokens[i-1].line);
            exit(1);
          }
          idx = checkSymbol(symbols, &tokens[i].as.sv);
          if (idx == -1) {
            if (checkKeyword(tokens[i].as.sv) != NIL) {
              printf("Expected label but got keyword:'%.*s' after Jf instruction on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i - 1].line);
            } else {
              printf("Symbol '%.*s' doesnt exists on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i - 1].line);
            }
            exit(1);
          };
          arrput(*ops, ((Op){.kind = JF, .as.val = idx}));

          ++i;
          continue;
        case CALL:
          ++i;
          if (tokens[i].type != ID) {
            printf("Expected label after Call instruction on line: %d", tokens[i-1].line);
            exit(1);
          }
          idx = checkSymbol(symbols, &tokens[i].as.sv);
          if (idx == -1) {
            if (checkKeyword(tokens[i].as.sv) != NIL) {
              printf("Expected label but got keyword:'%.*s' after Jf instruction on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i - 1].line);
            } else {
              printf("Symbol '%.*s' doesnt exists on line: %d", (int)tokens[i].as.sv.len, tokens[i].as.sv.data, tokens[i - 1].line);
            }
            exit(1);
          };
          arrput(*ops, ((Op){.kind = CALL, .as.val = idx}));

          ++i;
          continue;
        case RET:
          arrput(*ops, ((Op){.kind = RET}));
          ++i;
          continue;
        case DUMP:
          arrput(*ops, ((Op){.kind = DUMP}));
          ++i;
          continue;
        case SYSCALL:
          i++;
          if (tokens[i].type != INT) {
            printf("Expected Number after Syscall instruction on line: %d", tokens[i-1].line);
            exit(1);
          }
          arrput(*ops, ((Op){.kind = SYSCALL, .as.val = tokens[i].as.value}));
          continue;
        default:
          printf("unexpected '%d' token on: %d\n", tokens[i].type, i);
          break;
      }
    }else if (tokens[i].type == STRING){
      arrput(*ops, ((Op){.kind = WRITESTRING, .as.sv = tokens[i].as.sv}));
    }
    ++i;
  }
}



