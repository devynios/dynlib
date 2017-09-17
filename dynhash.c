#include <stdlib.h>
#include <string.h>
#include "dynhash.h"


unsigned
dynhash_hash_jenkins(const char *key)
{
	unsigned h = 0;
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


unsigned
dynhash_hash_djb(const char *key)
{
	unsigned h = 5381;
	size_t i;

	for (i = 0; key[i] != '\0'; ++i)
		h = ((h << 5) + h) + (unsigned) key[i];
	return h;
}


unsigned
dynhash_hash_sdbm(const char *key)
{
	unsigned h = 0;
	size_t i;

	for (i = 0; key[i] != '\0'; ++i)
		h = ((h << 6) + (h << 16) + (unsigned) key[i]) - h;
	return h;
}


unsigned
dynhash_hash_kr(const char *key)
{
	unsigned h = 0;
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
dynhash_init(struct dynhash_t *dh)
{
	if (dh == NULL)
		return;
	dh->size = DYNHASH_INITSIZE;
	dh->arr = calloc(dh->size, sizeof(struct dynhash_elem_t));
	dh->len = 0;
#if 0
	dh->keyofft = 0;
	dh->maxcol = 0;
#endif
	dh->free = &dynhash_free_none;
	dh->hash = &dynhash_hash_djb;
	dh->cmp = &strcmp;
}


void
dynhash_free(struct dynhash_t *dh)
{
	size_t i;

	if (dh == NULL)
		return;
	for (i = 0; i < dh->size; ++i)
		if (dh->arr[i].key != NULL)
			dh->free(dh->arr[i].val); /* TODO: free(key) */
	free(dh->arr);
}


void
dynhash_resize(struct dynhash_t *dh, size_t size)
{
	struct dynhash_elem_t *arr = calloc(size, sizeof(struct dynhash_elem_t));
	size_t i, j;

	if (size <= dh->size)
		return;
	for (i = 0; i < dh->size; ++i) {
		if (dh->arr[i].key == NULL)
			continue;
		j = dh->arr[i].hash % size;
		while (arr[j].key != NULL)
			j = (j + DYNHASH_STEP) % size;
		arr[j].key = dh->arr[i].key;
		arr[j].val = dh->arr[i].val;
		arr[j].hash = dh->arr[i].hash;
	}
	free(dh->arr);
	dh->arr = arr;
	dh->size = size;
}


void
dynhash_add(struct dynhash_t *dh, void *key, void *val)
{
	unsigned h = dh->hash(key);
	size_t i;
	struct dynhash_elem_t *e;

	if (DYNHASH_RESIZE_NEEDED(dh))
		dynhash_resize(dh, dh->size << 1);
	i = h % dh->size;
	while ((e = &dh->arr[i])->key != NULL) {
		if ((e->hash == h) && (dh->cmp(e->key, key) == 0)) {
			dh->free(e->val);
			e->val = val;
			/* TODO: free(key) */
			return;
		}
		i = (i + DYNHASH_STEP) % dh->size;
	}
	e->key = key; /* TODO: free(e->key) */
	e->val = val;
	e->hash = h;
	++dh->len;
}


void *
dynhash_find(struct dynhash_t *dh, void *key)
{
	unsigned h = dh->hash(key);
	size_t i = h % dh->size;
	struct dynhash_elem_t *e = &dh->arr[i];

	/* FIXME: possible infinite loop */
	while (e->key != NULL) {
		if ((e->hash == h) && (dh->cmp(e->key, key) == 0))
			return e->val;
		i = (i + DYNHASH_STEP) % dh->size;
		e = &dh->arr[i];
	}
	return NULL;
}
