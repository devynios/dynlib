#include <stdlib.h>
#include <string.h>
#include "dynhash.h"


unsigned long
dynhash_hash_jenkins(const char *key)
{
	unsigned long h = 0;
	size_t i;

	for (i = 0; key[i] != '\0'; ++i) {
		h += (unsigned char) key[i];
		h += h << 10;
		h ^= h >> 6;
	}
	h += h << 3;
	h ^= h >> 11;
	h += h << 15;
	return h;
}


unsigned long
dynhash_hash_djb(const char *key)
{
	unsigned long h = 5381;
	size_t i;

	for (i = 0; key[i] != '\0'; ++i)
		h = ((h << 5) + h) + (unsigned char) key[i];
	return h;
}


unsigned long
dynhash_hash_sdbm(const char *key)
{
	unsigned long h = 0;
	size_t i;

	for (i = 0; key[i] != '\0'; ++i)
		h = ((h << 6) + (h << 16) + (unsigned char) key[i]) - h;
	return h;
}


unsigned long
dynhash_hash_kr(const char *key)
{
	unsigned long h = 0;
	size_t i;

	for (i = 0; key[i] != '\0'; ++i)
		h += (unsigned char) key[i];
	return h;
}


void
dynhash_free_none(void *e)
{
	/* do not free anything */
}


void
dynhash_init(struct dynhash *dh)
{
	if (dh == NULL)
		return;
	dh->size = DYNHASH_INITSIZE;
	dh->arr = calloc(dh->size, sizeof(struct dynhash_elem *));
	dh->len = 0;
	dh->hash = &dynhash_hash_djb;
	dh->cmp = &strcmp;
	dh->freekey = &dynhash_free_none;
	dh->freeval = &dynhash_free_none;
}


void
dynhash_free(struct dynhash *dh)
{
	struct dynhash_elem *e;
	size_t i;

	if (dh == NULL)
		return;
	for (i = 0; i < dh->size; ++i) {
		if ((e = dh->arr[i]) == NULL)
			continue;
		dh->freekey(e->key);
		dh->freeval(e->val);
		free(e);
	}
	free(dh->arr);
}


void
dynhash_resize(struct dynhash *dh, size_t size)
{
	struct dynhash_elem *e, **arr = calloc(size, sizeof(struct dynhash_elem *));
	size_t i, j;

	if (size <= dh->size)
		return;
	for (i = 0; i < dh->size; ++i) {
		if ((e = dh->arr[i]) == NULL)
			continue;
		j = e->hash % size;
		while (arr[j] != NULL)
			j = (j + DYNHASH_STEP) % size;
		arr[j] = e;
	}
	free(dh->arr);
	dh->arr = arr;
	dh->size = size;
}


void
dynhash_add(struct dynhash *dh, void *key, void *val)
{
	unsigned long h = dh->hash(key);
	size_t i;
	struct dynhash_elem *e;

	if (DYNHASH_RESIZE_NEEDED(dh))
		dynhash_resize(dh, dh->size << 1);
	i = h % dh->size;
	while ((e = dh->arr[i]) != NULL) {
		if ((e->hash == h) && (dh->cmp(e->key, key) == 0)) {
			dh->freekey(e->key);
			dh->freeval(e->val);
			e->key = key;
			e->val = val;
			return;
		}
		i = (i + DYNHASH_STEP) % dh->size;
	}
	e = malloc(sizeof(struct dynhash_elem *));
	e->key = key;
	e->val = val;
	e->hash = h;
	dh->arr[i] = e;
	++dh->len;
}


static int
dynhash_between(size_t i, size_t ifirst, size_t ilast)
{
	/* FIXME: do not assume that DYNHASH_STEP == 1 */
	if (ifirst < ilast)
		return (i > ifirst && i < ilast);
	return (i > ifirst || i < ilast);
}


static void
dynhash_fill_gap(struct dynhash *dh, size_t igap)
{
	size_t iend = (igap + DYNHASH_STEP) % dh->size;
	size_t i;

	/* assert(dh->arr[igap] == NULL) */
	while (dh->arr[iend] != NULL)
		iend = (iend + DYNHASH_STEP) % dh->size;
	i = iend;
	while ((i = (i - DYNHASH_STEP) % dh->size) != igap) {
		if (dynhash_between(dh->arr[i]->hash % dh->size, igap, iend))
			continue;
		dh->arr[igap] = dh->arr[i];
		dh->arr[i] = NULL;
		igap = i;
		i = iend;
	}
}


void
dynhash_rm(struct dynhash *dh, const void *key)
{
	unsigned long h;
	size_t i;
	struct dynhash_elem *e;

	h = dh->hash(key);
	i = h % dh->size;
	/* FIXME: possible infinite loop */
	while ((e = dh->arr[i]) != NULL) {
		if ((e->hash == h) && (dh->cmp(e->key, key) == 0)) {
			dh->freekey(e->key);
			dh->freeval(e->val);
			free(e);
			dh->arr[i] = NULL;
			--dh->len;
			dynhash_fill_gap(dh, i);
			return;
		}
		i = (i + DYNHASH_STEP) % dh->size;
	}
}


void *
dynhash_get_val(struct dynhash *dh, const void *key)
{
	unsigned long h = dh->hash(key);
	size_t i = h % dh->size;
	struct dynhash_elem *e;

	/* FIXME: possible infinite loop */
	while ((e = dh->arr[i]) != NULL) {
		if ((e->hash == h) && (dh->cmp(e->key, key) == 0))
			return e->val;
		i = (i + DYNHASH_STEP) % dh->size;
	}
	return NULL;
}
