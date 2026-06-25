/* strlib.h -- v0.1 -- public domain single-header library -- https://github.com/loncike
 *
 * Usage:
 *   #define STRLIB_IMPLEMENTATION
 *   #include "strlib.h"
 *
 * Optional:
 *   #define STRLIB_NO_PREFIX           // removes the prefixes
 *   #define STRLIB_PREFIX customprefix // sets custom prefix (default: strlib)
 *   #define STRLIB_STATIC              // make functions static instead of extern
 */

#ifndef STRLIB_H
#define STRLIB_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(STRLIB_NO_PREFIX)
    #define STRLIB_NAME(n) n
#else
    #ifndef STRLIB_PREFIX
    #define STRLIB_PREFIX strlib
    #endif
    #define STRLIB__CONCAT(a, b)  a##_##b
    #define STRLIB__CONCAT2(a, b) STRLIB__CONCAT(a, b)
    #define STRLIB_NAME(n)        STRLIB__CONCAT2(STRLIB_PREFIX, n)
#endif

#ifdef STRLIB_STATIC
#define STRLIB_DEF static
#else
#define STRLIB_DEF extern
#endif


typedef struct {
    char *data;
    size_t len;
    size_t cap;   
}STRLIB_NAME(stringBuilder);

typedef struct{
    const char *data;
    size_t len;
}STRLIB_NAME(stringView);


STRLIB_DEF void STRLIB_NAME(sbAppendChar)(STRLIB_NAME(stringBuilder) *sb, char c);
STRLIB_DEF void STRLIB_NAME(sbAppendCstring)(STRLIB_NAME(stringBuilder) *sb, char *c);
STRLIB_DEF void STRLIB_NAME(sbAppendSb)(STRLIB_NAME(stringBuilder) *sb1, STRLIB_NAME(stringBuilder) *sb2);
STRLIB_DEF void STRLIB_NAME(sbAppendSlice)(STRLIB_NAME(stringBuilder) *sb, STRLIB_NAME(stringView) *sv);
STRLIB_DEF void STRLIB_NAME(sbSlice)(STRLIB_NAME(stringView) *sv, STRLIB_NAME(stringBuilder) *sb, size_t start, size_t end);
STRLIB_DEF int STRLIB_NAME(svcmp)(STRLIB_NAME(stringView) *sv1, STRLIB_NAME(stringView) *sv2);
STRLIB_DEF int STRLIB_NAME(svcmpToCstring)(STRLIB_NAME(stringView) *sv1, const char *s);

STRLIB_DEF char STRLIB_NAME(svToChar)(STRLIB_NAME(stringView) *sv, size_t idx);

STRLIB_DEF void sbFree(STRLIB_NAME(stringBuilder) *sb);
#endif 



#ifdef STRLIB_IMPLEMENTATION


STRLIB_DEF void STRLIB_NAME(sbAppendChar)(STRLIB_NAME(stringBuilder) *sb, char c){
  if (sb->len >= sb->cap){
    if (sb->cap == 0) sb->cap = 16;
    else sb->cap *= 2;
    sb->data = realloc(sb->data, sb->cap*sizeof(*sb->data));
  } 
  sb->data[sb->len++] = c;
}

STRLIB_DEF void STRLIB_NAME(sbAppendCstring)(STRLIB_NAME(stringBuilder) *sb, char *c){
  for (int i = 0; i < strlen(c)-1; ++i){
    STRLIB_NAME(sbAppendChar)(sb, c[i]);;
  }
}

STRLIB_DEF void STRLIB_NAME(sbAppendSb)(STRLIB_NAME(stringBuilder) *sb1, STRLIB_NAME(stringBuilder) *sb2) {
  for (int i = 0; i < sb2->len; ++i) {
    STRLIB_NAME(sbAppendChar)(sb1, sb2->data[i]);
  }
}

STRLIB_DEF void STRLIB_NAME(sbAppendSlice)(STRLIB_NAME(stringBuilder) *sb, STRLIB_NAME(stringView) *sv){
  for (int i = 0; i < sv->len; ++i){
    STRLIB_NAME(sbAppendChar)(sb, sv->data[i]);
  }
}


STRLIB_DEF void STRLIB_NAME(sbSlice)(STRLIB_NAME(stringView) *sv, STRLIB_NAME(stringBuilder) *sb, size_t start, size_t end){
  sv->data = sb->data;
  sv->data += start ;
  sv->len = end-start+1;
}

STRLIB_DEF char STRLIB_NAME(svToChar)(STRLIB_NAME(stringView) *sv, size_t idx){  
  return sv->data[idx];
}

//returns 0 if the same, -1 otherwise
STRLIB_DEF int STRLIB_NAME(svcmp)(STRLIB_NAME(stringView) *sv1, STRLIB_NAME(stringView) *sv2){
  if (sv2->len != sv1->len) return -1; 
  if (memcmp(sv1->data, sv2->data, sv1->len) == 0) return 0;
  else return -1;
}

STRLIB_DEF int STRLIB_NAME(svcmpToCstring)(STRLIB_NAME(stringView) *sv1, const char *s){
  if (sv1->len != strlen(s)) return -1; 
  if (memcmp(sv1->data, s, sv1->len) == 0) return 0;
  else return -1;
}

STRLIB_DEF void STRLIB_NAME(sbFree)(STRLIB_NAME(stringBuilder) *sb){
    free(sb->data);
    sb->data = NULL;
    sb->len = 0;
    sb->cap = 0;
}

#endif 
