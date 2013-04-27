#ifndef dynarr_h_
#define dynarr_h_

#define DYNARR_INITLEN 1

struct dynarr_t
{
  void **elem;
  int size;
  int len;
  void (*free)(void*);
};

void
dynarr_init(struct dynarr_t*);
void
dynarr_free(struct dynarr_t*);
void
dynarr_add(struct dynarr_t*, void*);
void
dynarr_concat(struct dynarr_t*, void**);
int
dynarr_find(struct dynarr_t*, void*);
void
dynarr_clean(struct dynarr_t*);
void
dynarr_setEmpty(struct dynarr_t*);
void
dynarr_optSize(struct dynarr_t*);

#endif /* dynarr_h_ */
