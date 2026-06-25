#ifndef VM_H_
#define VM_H_

#include <stdint.h>
#include <stdlib.h>

#define STRLIB_NO_PREFIX
#include "strlib.h"

typedef struct{
  uint64_t stack[1024];
  int sp;

  int callStack[1024];
  int callStackPointer;
  
}VirtualMachine;


typedef enum {
  NIL,

  PUSH,

  ADD,
  SUB,
  MUL,
  DIV,

  AND,
  OR,
  XOR,
  NOT,
  SHL,
  SHR,

  EQ,
  GT,
  LT,

  DUP,
  OVER,
  SWAP,
  ROT,
  DROP,

  ALLOC,
  WRITE,
  READ,
  FREE,

  JMP,
  JT,
  JF,
  CALL,
  RET,

  DUMP,
  SYSCALL,

  WRITESTRING,
}OpKind;

typedef struct{
  OpKind kind;  
  union{
    uint64_t val;
    stringView sv;
  }as;
}Op;

void vmInit(VirtualMachine *vm);
void eval(VirtualMachine *vm, Op *ops);
const char *opName(OpKind kind);


#endif // VM_H_
