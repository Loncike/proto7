#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
  
#include "lexer.h"


#define STRLIB_NO_PREFIX
#include "strlib.h"

#include "stb_ds.h"


const char *readFile(char *filepath) {
  FILE *fp = fopen(filepath, "r");
  if (!fp) {
    printf("Couldnt open file at location %s", filepath);
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);

  char *buf = malloc(size + 1);
  if (!buf) {
    fclose(fp);
    exit(1);
  }

  size_t read = fread(buf, 1, size, fp);
  buf[read] = '\0';
  fclose(fp);
  return buf;

}

void lexer(const char *src, Token **tokens){
  int line = 1;

  while (*src){
    if (*src == '\n') line++;
    if (isspace(*src)) {src++; continue;}

    if (isalpha(*src)){
      stringView sv = {0};
      const char *start = src; 
      while (isalnum(*src) || *src == '_') src++;
      size_t len = src - start;
      sv.data = start;
      sv.len = len;

      arrput(*tokens, ((Token){.type = ID, .line = line, .as.sv = sv}));
      continue;
      
    }else if (isdigit(*src)){
      char *end;
      long num = strtol(src, &end, 10);
      src = end;
      arrput(*tokens, ((Token){.type = INT, .line = line, .as.value = (int)num}));
      continue;

    }else if (*src == ':'){
      arrput(*tokens, ((Token){.type = COLON, .line = line}));
      src++;
      continue;
    }else if (*src == '"'){
      src++;
      const char *start = src;
      for (int j = 0; j < 3; ++j){
        src++;
      }
      size_t len = src - start;
      stringView sv = {.data = start, .len = len};

      arrput(*tokens, ((Token){.type = STRING, .line = line, .as.sv = sv}));
    }else if (*src == '/'){
      src++;
      if (*src != '/'){printf("Expected another '/' for comment on line :%d", line); exit(1);}
      while (*src != '\n') src++;
      continue;
    }
    src++;
  }
}
