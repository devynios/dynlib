#ifndef dynstr_h__
#define dynstr_h__

#include <sys/types.h>
#include <time.h>

#define DYNSTR_INITSIZE 32
#define DYNSTR_BLOCKSIZE 4096
#define DYNSTR_NEWSIZE(oldsize) ((oldsize) > DYNSTR_BLOCKSIZE? \
                                 (oldsize) + DYNSTR_BLOCKSIZE: \
                                 (oldsize) << 1)

struct dynstr {
	char *str;
	size_t size;
	size_t len;
};

void
dynstr_init(struct dynstr *);

void
dynstr_free(struct dynstr *);

void
dynstr_set_empty(struct dynstr *);

void
dynstr_add_char(struct dynstr *, char);

void
dynstr_add_chararr(struct dynstr *, const char *, size_t);

void
dynstr_add(struct dynstr *, const char *);

void
dynstr_add_int(struct dynstr *, int);

void
dynstr_add_date(struct dynstr *, const struct tm *);

void
dynstr_add_datetime(struct dynstr *, const struct tm *);

void
dynstr_add_char_xmlesc(struct dynstr *, char);

void
dynstr_add_xmlesc(struct dynstr *, const char *);

void
dynstr_optsize(struct dynstr *);

char *
dynstr_str_new(struct dynstr *);

char *
dynstr_substr_new(struct dynstr *, size_t, size_t);

#endif /* dynstr_h__ */
