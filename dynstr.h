#ifndef dynstr_h_
#define dynstr_h_

#define DYNSTR_INITLEN 1

struct dynstr_t
{
  char *elem;
  int size;
  int len;
};

void
dynstr_init(struct dynstr_t*);
void
dynstr_free(struct dynstr_t*);
void
dynstr_setEmpty(struct dynstr_t*);
void
dynstr_add(struct dynstr_t*, char);
void
dynstr_concat(struct dynstr_t*, char*);
void
dynstr_optSize(struct dynstr_t*);
char*
dynstr_getSubStr(struct dynstr_t*, int, int);

#endif /* dynstr_h_ */
