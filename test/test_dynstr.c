#include <stdlib.h>
#include <stdio.h>
#include <dynstr.h>


static void
test_dynstr_size(struct dynstr *dstr)
{
	size_t oldsize = dstr->size;
	int i;

	printf("size=%zu\n", dstr->size);
	for (i = 0; i < 4096; ++i) {
		dynstr_add(dstr, "0123456789abcdef");
		if (dstr->size != oldsize) {
			oldsize = dstr->size;
			printf("size=%zu\n", dstr->size);
		}
	}
}


static void
test_dynstr(struct dynstr *dstr)
{
	char *str;
	int i;

	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);

	dynstr_add(dstr, "abcdefgh");
	dynstr_add_char(dstr, 'i');
	dynstr_add_char(dstr, 'j');
	dynstr_add_char(dstr, 'k');
	dynstr_add(dstr, "123456");
	printf("add, add_char\n");
	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);

	dynstr_set_empty(dstr);
	printf("set_empty\n");
	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);

	for (i = 0; i < 3; ++i) {
		dynstr_add_char(dstr, '\\');
		dynstr_add(dstr, "uvwXYz_0987+-*/");
	}
	printf("add_char, add\n");
	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);

	str = dynstr_str_new(dstr);
	printf("str_new\n");
	printf("str: %s\n", str);
	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);
	free(str);

	dynstr_optsize(dstr);
	printf("optsize\n");
	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);

	str = dynstr_substr_new(dstr, 8, 4);
	printf("substr_new\n");
	printf("substr: %s\n", str);
	printf("dstr: %s\n", dstr->str);
	printf("size=%zu, len=%zu\n\n", dstr->size, dstr->len);
	free(str);
}


int
main(void)
{
	struct dynstr dstr;

	dynstr_init(&dstr);
	test_dynstr(&dstr);
	dynstr_set_empty(&dstr);
	dynstr_optsize(&dstr);
	test_dynstr_size(&dstr);
	dynstr_free(&dstr);
	return 0;
}
