#ifndef LEXER_H_
#define LEXER_H_

#define STRLIB_NO_PREFIX
#include "strlib.h"

typedef enum{
  INT,
  ID,
  STRING,
  COLON,
}TokenType;

typedef struct {
  TokenType type;
  int line;
  union{
    int value;
    stringView sv;
  }as;
}Token;

const char *readFile(char *filepath);
void lexer(const char *src, Token **tokens);

#endif // LEXER_H_
