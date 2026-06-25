#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "stb_ds.h"

#include "vm.h"

void vmInit(VirtualMachine *vm){
  vm->sp = 0;
  vm->callStackPointer = 0;
}

void vmPushCallStackPointer(VirtualMachine *vm, int v){
  vm->callStack[vm->callStackPointer++] = v;
}

int vmPopCallStackPointer(VirtualMachine *vm){
  return vm->callStack[--vm->callStackPointer];
}

void vmPush(VirtualMachine *vm, uint64_t v){
  vm->stack[vm->sp++] = v;
}

uint64_t vmPop(VirtualMachine *vm){
  return vm->stack[--vm->sp];
}

uint64_t vmAlloc(VirtualMachine *vm, size_t size){
  return (uint64_t)malloc(size);
}

void vmFree(VirtualMachine *vm, uint64_t addr){
  free((void*)addr);
}

void vmWrite8(VirtualMachine *vm, uint64_t addr, uint8_t v){
  *(uint8_t*)addr = v;
}

uint8_t vmRead8(VirtualMachine *vm, uint64_t addr){
  return *(uint8_t*)addr;
}

void vmWrite16(VirtualMachine *vm, uint64_t addr, uint16_t v){
  *(uint16_t*)addr = v;
}

uint16_t vmRead16(VirtualMachine *vm, uint64_t addr){
  return *(uint16_t*)addr;
}

void vmWrite32(VirtualMachine *vm, uint64_t addr, uint32_t v){
  *(uint32_t*)addr = v;
}

uint32_t vmRead32(VirtualMachine *vm, uint64_t addr){
  return *(uint32_t*)addr;
}

void vmWrite64(VirtualMachine *vm, uint64_t addr, uint64_t v){
  *(uint64_t*)addr = v;
}

uint64_t vmRead64(VirtualMachine *vm, uint64_t addr){
  return *(uint64_t*)addr;
}

int doSyscall(int num, int64_t *args, int64_t argc) {
    switch (argc) {
        case 0: return syscall(num);
        case 1: return syscall(num, args[0]);
        case 2: return syscall(num, args[0], args[1]);
        case 3: return syscall(num, args[0], args[1], args[2]);
        case 4: return syscall(num, args[0], args[1], args[2], args[3]);
        case 5: return syscall(num, args[0], args[1], args[2], args[3], args[4]);
        case 6: return syscall(num, args[0], args[1], args[2], args[3], args[4], args[5]);
        default:
            return -1;
    }
}
void eval(VirtualMachine *vm, Op *ops){
  uint64_t v1,v2,v3;
  for (int i = 0; i < arrlen(ops); ++i){
    switch (ops[i].kind){
      case PUSH:
        vmPush(vm, ops[i].as.val);
        break;
      case ADD:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 + v2);
        break;
      case SUB:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 - v2);
        break;
      case MUL:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 * v2);
        break;
      case DIV:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 / v2);
        break;
      case AND:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1&v2);
        break;
      case OR:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1|v2);
        break;
      case XOR:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1^v2);
        break;
      case NOT:
        v1 = vmPop(vm);
        vmPush(vm, ~v1);
        break;
      case SHL:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1<<v2);
        break;
      case SHR:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1>>v2);
        break;
      case EQ:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 == v2);
        break;
      case GT:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 > v2);
        break;
      case LT:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1 < v2);
        break;
      case DUP:
        v1 = vmPop(vm);
        vmPush(vm, v1);
        vmPush(vm, v1);
        break;
      case OVER:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v2);
        vmPush(vm, v1);
        vmPush(vm, v2);
        break;
      case SWAP:
        v1 = vmPop(vm);
        v2 = vmPop(vm);
        vmPush(vm, v1);
        vmPush(vm, v2);
        break;
      case ROT:
        v1 = vmPop(vm); 
        v2 = vmPop(vm);
        v3 = vmPop(vm);
        vmPush(vm, v2); 
        vmPush(vm, v1);
        vmPush(vm, v3);
        break;
      case DROP:
        vmPop(vm);
        break;
      case ALLOC:
        if (ops[i].as.val == -1){
          v1 = vmPop(vm);
          v2 = vmAlloc(vm, v1);
        }else{
          v2 = vmAlloc(vm, ops[i].as.val);
        }
        vmPush(vm, v2);
        break;
      case WRITE:
        switch (ops[i].as.val){
          case 1:
            v1 = vmPop(vm);
            v2 = vmPop(vm);
            vmWrite8(vm, v2, v1);
            break;
          case 2:
            v1 = vmPop(vm);
            v2 = vmPop(vm);
            vmWrite16(vm, v2, v1);
            break;
          case 4:
            v1 = vmPop(vm);
            v2 = vmPop(vm);
            vmWrite32(vm, v2, v1);
            break;
          case 8:
            v1 = vmPop(vm);
            v2 = vmPop(vm);
            vmWrite64(vm, v2, v1);
            break;
          default:
            //TODO: check for unvalid size should be moved to lexer, so the line number is printable
            printf("Unvalid size:'%ld' for write", ops[i].as.val);
            exit(1);
            break;
        }
        break;
      case READ:
        switch (ops[i].as.val){
          case 1:
            v1 = vmPop(vm);
            vmPush(vm, vmRead8(vm, v1));
            break;
          case 2:
            v1 = vmPop(vm);
            vmPush(vm, vmRead16(vm, v1));
            break;
          case 4:
            v1 = vmPop(vm);
            vmPush(vm, vmRead32(vm, v1));
            break;
          case 8:
            v1 = vmPop(vm);
            vmPush(vm, vmRead64(vm, v1));
            break;
          default:
            //TODO: check for unvalid size should be moved to lexer, so the line number is printable
            printf("Unvalid size: '%ld' for write", ops[i].as.val);
            exit(1);
            break;
        }
        break;
      case FREE:
        v1 = vmPop(vm);
        vmFree(vm, v1);
        break;
      case JMP:
        i = ops[i].as.val-1;
        break;
      case JT:
        v1 = vmPop(vm);
        if (v1 == 1) i = ops[i].as.val-1;
        break;
      case JF:
        v1 = vmPop(vm);
        if (v1 == 0) i = ops[i].as.val-1;
        break;
      case CALL:
        vmPushCallStackPointer(vm, i);
        i = ops[i].as.val-1;
        break;
      case RET:
        i = vmPopCallStackPointer(vm);
        if (i > arrlen(ops)) continue;
        break;
      case DUMP:
        printf("top of the stack: %ld\n", vmPop(vm));
        break;
      case SYSCALL:
        int64_t args[6];
        for (int j = ops[i].as.val-1; j >= 0; --j){
          args[j] = vmPop(vm);
        }
        int num = vmPop(vm);
        vmPush(vm, doSyscall(num, args, ops[i].as.val));
        break;
      case WRITESTRING:
        v1 = vmPop(vm); //ptr
        memcpy((void*)v1, ops[i].as.sv.data, ops[i].as.sv.len);
        vmPush(vm, ops[i].as.sv.len);
        break;
      default: break;
    }
#if 0
    printf("sp: %d\n", vm->sp);
    printf("Stack: ");
    for(int j = 0; j < 4; ++j){
      printf("%ld ", vm->stack[j]);
    }
    printf("\n");
#endif
  }
}


const char *opName(OpKind kind){
  switch (kind){
    case NIL:     return "NIL";
    case PUSH:    return "PUSH";
    case ADD:     return "ADD";
    case SUB:     return "SUB";
    case MUL:     return "MUL";
    case DIV:     return "DIV";
    case AND:     return "AND";
    case OR:      return "OR";
    case XOR:     return "XOR";
    case NOT:     return "NOT";
    case SHL:     return "SHL";
    case SHR:     return "SHR";
    case EQ:      return "EQ";
    case GT:      return "GT";
    case LT:      return "LT";
    case DUP:     return "DUP";
    case OVER:    return "OVER";
    case SWAP:    return "SWAP";
    case ROT:     return "ROT";
    case DROP:    return "DROP";
    case ALLOC:   return "ALLOC";
    case WRITE:   return "WRITE";
    case READ:    return "READ";
    case FREE:    return "FREE";
    case JMP:     return "JMP";
    case JT:      return "JT";
    case JF:      return "JF";
    case CALL:    return "CALL";
    case RET:     return "RET";
    case DUMP:    return "DUMP";
    case SYSCALL:    return "SYSCALL";
    case WRITESTRING: return "WRITESTRING";
  }
  return "";
}
