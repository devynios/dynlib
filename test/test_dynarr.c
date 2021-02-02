#include <stdlib.h>
#include <stdio.h>
#include <dynarr.h>


static void
free_print(void *e)
{
	printf("free: %s\n", (char *)e);
}


static void
test_dynarr_size(struct dynarr *darr)
{
	size_t oldsize = darr->size;
	int i;
	char *arr[] = {
		"0", "1", "2", "3", "4", "5", "6", "7",
		"8", "9", "a", "b", "c", "d", "e", "f",
		NULL
	};

	printf("size=%zu\n", darr->size);
	for (i = 0; i < 4096; ++i) {
		dynarr_add_array(darr, (void **)arr);
		if (darr->size != oldsize) {
			oldsize = darr->size;
			printf("size=%zu\n", darr->size);
		}
	}
	dynarr_set_empty(darr);
}


static void
print_dynarr(struct dynarr *darr)
{
	int i;

	printf("darr:\n");
	for (i = 0; i < darr->len; ++i)
		printf("%s\n", (char *)darr->arr[i]);
	printf("--\n");
}


static void
test_dynarr(struct dynarr *darr)
{
	int i;
	char *arr[] = {
		"0123456789",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"abcdefghijklmnopqrstuvwxyz",
		NULL
	};

	print_dynarr(darr);
	printf("size=%zu, len=%zu\n\n", darr->size, darr->len);

	dynarr_add_array(darr, (void **)arr);
	dynarr_add(darr, "labas rytas");
	dynarr_add(darr, "viso gero");
	printf("add_array, add\n");
	print_dynarr(darr);
	printf("size=%zu, len=%zu\n\n", darr->size, darr->len);

	dynarr_set_empty(darr);
	printf("set_empty\n");
	print_dynarr(darr);
	printf("size=%zu, len=%zu\n\n", darr->size, darr->len);

	for (i = 0; i < 10; ++i) {
		dynarr_add(darr, "hello hello");
		dynarr_add_array(darr, (void **)arr);
	}
	printf("add, add_array\n");
	print_dynarr(darr);
	printf("size=%zu, len=%zu\n\n", darr->size, darr->len);

	dynarr_optsize(darr);
	printf("optsize\n");
	print_dynarr(darr);
	printf("size=%zu, len=%zu\n\n", darr->size, darr->len);

	darr->free = &free_print;
	dynarr_clean(darr);
	printf("clean\n");
	print_dynarr(darr);
	printf("size=%zu, len=%zu\n\n", darr->size, darr->len);
	darr->free = &dynarr_free_none;
}


int
main(void)
{
	struct dynarr darr;

	dynarr_init(&darr);
	test_dynarr(&darr);
	dynarr_set_empty(&darr);
	dynarr_optsize(&darr);
	test_dynarr_size(&darr);
	dynarr_free(&darr);
	return 0;
}
