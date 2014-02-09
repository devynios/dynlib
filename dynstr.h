#ifndef dynstr_h_
#define dynstr_h_

#define DYNSTR_INITSIZE 32
#define DYNSTR_BLOCKSIZE 4096
#define DYNSTR_NEWSIZE(oldsize) ((oldsize) > DYNSTR_BLOCKSIZE)? \
                                ((oldsize) + DYNSTR_BLOCKSIZE): \
                                ((oldsize) << 1)

struct dynstr_t {
	char *str;
	size_t size;
	size_t len;
};

void
dynstr_init(struct dynstr_t *);

void
dynstr_free(struct dynstr_t *);

void
dynstr_set_empty(struct dynstr_t *);

void
dynstr_add(struct dynstr_t *, char);

void
dynstr_concat(struct dynstr_t *, const char *);

void
dynstr_optsize(struct dynstr_t *);

char*
dynstr_str_new(struct dynstr_t *);

char*
dynstr_substr_new(struct dynstr_t *, size_t, size_t);

#endif /* dynstr_h_ */
