#ifndef dynarr_h_
#define dynarr_h_

#include <sys/types.h>

#define DYNARR_INITSIZE 16
#define DYNARR_BLOCKSIZE 1024
#define DYNARR_NEWSIZE(oldsize) ((oldsize) > DYNARR_BLOCKSIZE)? \
                                ((oldsize) + DYNARR_BLOCKSIZE): \
                                ((oldsize) << 1)

struct dynarr_t {
	void **arr;
	size_t size;
	size_t len;
	void (*free)(void *);
};

void
dynarr_init(struct dynarr_t *);

void
dynarr_free(struct dynarr_t *);

void
dynarr_clean(struct dynarr_t *);

void
dynarr_set_empty(struct dynarr_t *);

void
dynarr_add(struct dynarr_t *, void *);

void
dynarr_concat(struct dynarr_t *, void **);

ssize_t
dynarr_find(struct dynarr_t *, void *);

void
dynarr_optsize(struct dynarr_t *);

void
dynarr_free_none(void *);

#endif /* dynarr_h_ */
