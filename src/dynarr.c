#include <stdlib.h>
#include "dynarr.h"


void
dynarr_free_none(void *e)
{
	/* do not free anything */
}


void
dynarr_init(struct dynarr *darr)
{
	if (darr == NULL)
		return;
	darr->size = DYNARR_INITSIZE;
	darr->arr = malloc(darr->size * sizeof(void *));
	darr->len = 0;
	darr->arr[0] = NULL;
	darr->free = &dynarr_free_none;
}


void
dynarr_free(struct dynarr *darr)
{
	size_t i;

	if (darr == NULL)
		return;
	for (i = 0; i < darr->len; ++i)
		darr->free(darr->arr[i]);
	free(darr->arr);
}


void
dynarr_clean(struct dynarr *darr)
{
	while (darr->len > 0)
		darr->free(darr->arr[--(darr->len)]);
	darr->arr[0] = NULL;
}


void
dynarr_set_empty(struct dynarr *darr)
{
	darr->len = 0;
	darr->arr[0] = NULL;
}


void
dynarr_add(struct dynarr *darr, void *e)
{
	darr->arr[(darr->len)++] = e;
	if (darr->len == darr->size) {
		darr->size = DYNARR_NEWSIZE(darr->size);
		darr->arr = realloc(darr->arr, darr->size * sizeof(void *));
	}
	darr->arr[darr->len] = NULL;
}


void
dynarr_add_array(struct dynarr *darr, void **e)
{
	size_t i;

	if (e == NULL)
		return;
	for (i = 0; e[i] != NULL; ++i)
		dynarr_add(darr, e[i]);
}


ssize_t
dynarr_find(struct dynarr *darr, void *e)
{
	size_t i;

	for (i = 0; i < darr->len; ++i)
		if (darr->arr[i] == e)
			return i;
	return -1;
}


void
dynarr_rm(struct dynarr *darr, ssize_t k)
{
	size_t i;

	if (k < 0)
		return;
	darr->free(darr->arr[k]);
	for (i = k; i < darr->len; ++i)
		darr->arr[i] = darr->arr[i + 1];
	--(darr->len);
}


void
dynarr_optsize(struct dynarr *darr)
{
	darr->size = darr->len + 1;
	darr->arr = realloc(darr->arr, darr->size * sizeof(void *));
}
