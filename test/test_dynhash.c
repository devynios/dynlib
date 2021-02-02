#include <stdlib.h>
#include <stdio.h>
#include <dynhash.h>

enum {
	TEST_DYNHASH_OK,
	TEST_DYNHASH_FAIL
};

static const char *test_dynhash_keys[] = {
	"nulis", "vienas", "du", "trys", "keturi",
	"penki", "šeši", "septyni", "aštuoni", "devyni",
	"dešimt", "vienuolika", "dvylika", "trylika", "keturiolika",
	"penkiolika", "šešiolika", "septyniolika", "aštuoniolika", "devyniolika",
	"dvidešimt", "21", "22", "23", "24",
	"25", "26", "27", "28", "29",
	"trisdešimt", "31", "32", "33", "34",
	"35", "36", "37", "38", "39",
	"keturiasdešimt", "41", "42", "43", "44",
	"45", "46", "47", "48", "49",
	NULL
};


static void
print_dynhash(struct dynhash *dh)
{
	struct dynhash_elem *e;
	size_t i;

	for (i = 0; i < dh->size; ++i) {
		if ((e = dh->arr[i]) == NULL)
			printf("[%lu]: NULL\n", i);
		else
			printf("[%lu]: hash=%016lx, key=%s\n", i, e->hash, (char *)e->key);
	}
}


static int
test_dynhash_get_val(struct dynhash *dh)
{
	static const char *notfound[] = {
		"penkiasdešimt", "51", "52", "53", "54",
		"šimtas", "viena", "vienass", "pienas", "2", "do", "2vylika", "rylika",
		NULL
	};
	const char *val;
	size_t i;
	int ret = TEST_DYNHASH_OK;

	for (i = 0; test_dynhash_keys[i] != NULL; ++i) {
		val = dynhash_get_val(dh, test_dynhash_keys[i]);
		if (val == NULL)
			printf("dynhash_get_val(\"%s\"): NOT FOUND\n", test_dynhash_keys[i]);
		else
			printf("dynhash_get_val(\"%s\"): FOUND: %s\n", test_dynhash_keys[i], val);
	}

	for (i = 0; notfound[i] != NULL; ++i) {
		val = dynhash_get_val(dh, notfound[i]);
		if (val == NULL) {
			printf("dynhash_get_val(\"%s\"): NOT FOUND\n", notfound[i]);
		} else {
			printf("dynhash_get_val(\"%s\"): FOUND: %s\n", notfound[i], val);
			ret = TEST_DYNHASH_FAIL;
		}
	}
	return ret;
}


static int
test_dynhash_rm(struct dynhash *dh, size_t n)
{
	const char *val;
	size_t i, j;
	int ret = TEST_DYNHASH_OK;

	for (i = 0; i < n; ++i) {
		dynhash_rm(dh, test_dynhash_keys[i]);
		if ((val = dynhash_get_val(dh, test_dynhash_keys[i])) == NULL) {
			printf("dynhash_rm(\"%s\"): dynhash_get_val: NOT FOUND\n", test_dynhash_keys[i]);
		} else {
			printf("dynhash_rm(\"%s\"): dynhash_get_val: FOUND: %s\n", test_dynhash_keys[i], val);
			ret = TEST_DYNHASH_FAIL;
		}
		for (j = i + 1; j < n; ++j) {
			if ((val = dynhash_get_val(dh, test_dynhash_keys[j])) == NULL) {
				printf("dynhash_rm: dynhash_get_val(\"%s\"): NOT FOUND\n", test_dynhash_keys[j]);
				ret = TEST_DYNHASH_FAIL;
			} else {
				printf("dynhash_rm: dynhash_get_val(\"%s\"): FOUND: %s\n", test_dynhash_keys[j], val);
			}
		}
	}
	return ret;
}


static int
test_dynhash(struct dynhash *dh)
{
	size_t i;
	int ret = TEST_DYNHASH_OK;

	for (i = 0; test_dynhash_keys[i] != NULL; ++i) {
		if (DYNHASH_RESIZE_NEEDED(dh)) {
			printf("full table:\n");
			print_dynhash(dh);
			printf("end table\n");
			if (test_dynhash_get_val(dh) != TEST_DYNHASH_OK)
				ret = TEST_DYNHASH_FAIL;
			if (test_dynhash_rm(dh, i) != TEST_DYNHASH_OK)
				ret = TEST_DYNHASH_FAIL;
		}
		dynhash_add(dh, test_dynhash_keys[i], test_dynhash_keys[i]);
		printf("[%lu]: size=%lu, key=%s\n", i, dh->size, test_dynhash_keys[i]);
	}

	printf("full table:\n");
	print_dynhash(dh);
	printf("end table\n");
	if (test_dynhash_get_val(dh) != TEST_DYNHASH_OK)
		ret = TEST_DYNHASH_FAIL;
	/*
	if (test_dynhash_rm(dh) != TEST_DYNHASH_OK)
		ret = TEST_DYNHASH_FAIL;
	*/
	return ret;
}


int
main(void)
{
	struct dynhash dh;
	int ret;

	dynhash_init(&dh);
	ret = test_dynhash(&dh);
	dynhash_free(&dh);
	return ret;
}
