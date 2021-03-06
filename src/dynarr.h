#ifndef dynarr_h__
#define dynarr_h__

#include <sys/types.h>

#define DYNARR_INITSIZE 16
#define DYNARR_BLOCKSIZE 1024
#define DYNARR_NEWSIZE(oldsize) ((oldsize) > DYNARR_BLOCKSIZE? \
                                 (oldsize) + DYNARR_BLOCKSIZE: \
                                 (oldsize) << 1)

struct dynarr {
	void **arr;
	size_t size;
	size_t len;
	void (*free)(void *);
};

void
dynarr_init(struct dynarr *);

void
dynarr_free(struct dynarr *);

void
dynarr_clean(struct dynarr *);

void
dynarr_set_empty(struct dynarr *);

void
dynarr_add(struct dynarr *, void *);

void
dynarr_add_array(struct dynarr *, void **);

ssize_t
dynarr_find(struct dynarr *, void *);

void
dynarr_rm(struct dynarr *, ssize_t);

void
dynarr_optsize(struct dynarr *);

void
dynarr_free_none(void *);

#endif /* dynarr_h__ */
