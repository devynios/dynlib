#include <stdlib.h>
#include "dynarr.h"

#define DYNARR_MIN(x, y) (x < y)? x: y

void
dynarr_freeNone(void *e)
{
  /* nieko */
}

void
dynarr_init(struct dynarr_t *arr)
{
  if (arr == NULL)
    return;
  arr->size = DYNARR_INITLEN+1;
  arr->elem = (void**) malloc((arr->size)*sizeof(void*));
  arr->len = 0;
  arr->elem[0] = NULL;
  arr->free = &dynarr_freeNone;
}

void
dynarr_free(struct dynarr_t *arr)
{
  int i;
  if (arr == NULL)
    return;
  if (arr->free != NULL)
    for (i=0; i<arr->len; ++i)
      arr->free(arr->elem[i]);
  free(arr->elem);
}

void
dynarr_add(struct dynarr_t *arr, void *e)
{
  arr->elem[(arr->len)++] = e;
  if (arr->len == arr->size)
  {
    arr->size = DYNARR_MIN((arr->size)<<1, (arr->size)+1024);
    arr->elem = (void**) realloc(arr->elem, (arr->size)*sizeof(void*));
  }
  arr->elem[arr->len] = NULL;
}

void
dynarr_concat(struct dynarr_t *arr, void **e)
{
  int i;
  if (e != NULL)
    for (i=0; e[i]!=NULL; ++i)
      dynarr_add(arr, e[i]);
}

int
dynarr_find(struct dynarr_t *arr, void *e)
{
  int i;
  for (i=0; i<arr->len; ++i)
    if (arr->elem[i] == e)
      return(i);
  return(-1);
}

void
dynarr_clean(struct dynarr_t *arr)
{
  if (arr->free != NULL)
    while (arr->len > 0)
      arr->free(arr->elem[--(arr->len)]);
  arr->elem[0] = NULL;
}

void
dynarr_setEmpty(struct dynarr_t *arr)
{
  arr->len = 0;
  arr->elem[0] = NULL;
}

void
dynarr_optSize(struct dynarr_t *arr)
{
  arr->size = arr->len+1;
  arr->elem = (void**) realloc(arr->elem, (arr->size)*sizeof(void*));
}
