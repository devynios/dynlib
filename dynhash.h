#ifndef dynhash_h__
#define dynhash_h__

#define DYNHASH_INITSIZE 64
#define DYNHASH_STEP 1
#define DYNHASH_RESIZE_NEEDED(dh) ((dh)->size / ((dh)->size - (dh)->len) >= 3)


unsigned long
dynhash_hash_jenkins(const char *);

unsigned long
dynhash_hash_djb(const char *);

unsigned long
dynhash_hash_sdbm(const char *);


struct dynhash_elem {
	void *key;
	void *val;
	unsigned long hash;
};

struct dynhash {
	struct dynhash_elem **arr;
	size_t size;
	size_t len;
	long (*hash)(const void *);
	int (*cmp)(const void *, const void *);
	void (*freekey)(void *);
	void (*freeval)(void *);
};

void
dynhash_init(struct dynhash *);

void
dynhash_free(struct dynhash *);

void
dynhash_resize(struct dynhash *, size_t);

void
dynhash_add(struct dynhash *, void *, void *);

void
dynhash_rm(struct dynhash *, const void *);

void *
dynhash_get_val(struct dynhash *, const void *);

#endif /* dynhash_h__ */
