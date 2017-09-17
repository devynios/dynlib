#ifndef dynhash_h_
#define dynhash_h_

#define DYNHASH_INITSIZE 64
#define DYNHASH_STEP 1
#define DYNHASH_RESIZE_NEEDED(dh) ((dh)->size / ((dh)->size - (dh)->len) >= 3)

struct dynhash_elem_t {
	void *key;
	void *val;
	unsigned hash;
};

struct dynhash_t {
	struct dynhash_elem_t *arr;
	size_t size;
	size_t len;
	long (*hash)(void *);
	void (*free)(void *);
	int (*cmp)(void *, void *);
#if 0
	void *keyofft;
	size_t maxcol;
#endif
};

void
dynhash_init(struct dynhash_t *);

void
dynhash_free(struct dynhash_t *);

void
dynhash_resize(struct dynhash_t *, size_t);

void
dynhash_add(struct dynhash_t *, void *, void *);

void *
dynhash_find(struct dynhash_t *, void *);

#endif /* dynhash_h_ */
