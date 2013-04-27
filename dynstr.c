#include <stdlib.h>
#include "dynstr.h"

#define DYNSTR_MIN(x, y) (x < y)? x: y

void
dynstr_init(struct dynstr_t *str)
{
  if (str == NULL)
    return;
  str->size = DYNSTR_INITLEN+1;
  str->elem = (char*) malloc((str->size)*sizeof(char));
  str->len = 0;
  str->elem[0] = '\0';
}

void
dynstr_free(struct dynstr_t *str)
{
  if (str != NULL)
    free(str->elem);
}

void
dynstr_setEmpty(struct dynstr_t *str)
{
  str->len = 0;
  str->elem[0] = '\0';
}

void
dynstr_add(struct dynstr_t *str, char c)
{
  str->elem[(str->len)++] = c;
  if (str->len == str->size)
  {
    str->size = DYNSTR_MIN((str->size)<<1, (str->size)+4096);
    str->elem = (char*) realloc(str->elem, (str->size)*sizeof(char));
  }
  str->elem[str->len] = '\0';
}

void
dynstr_concat(struct dynstr_t *str, char *s)
{
  int i;
  if (s != NULL)
    for (i=0; s[i]!='\0'; ++i)
      dynstr_add(str, s[i]);
}

void
dynstr_optSize(struct dynstr_t *str)
{
  str->size = str->len+1;
  str->elem = (char*) realloc(str->elem, (str->size)*sizeof(char));
}

char*
dynstr_getSubStr(struct dynstr_t *str, int offt, int len)
{
  char *ss;
  int i;
  ss = (char*) malloc((len+1)*sizeof(char));
  for (i=0; i<len; ++i)
    ss[i] = str->elem[offt+i];
  ss[len] = '\0';
  return(ss);
}
