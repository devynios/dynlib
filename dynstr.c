#include <stdlib.h>
#include "dynstr.h"


void
dynstr_init(struct dynstr_t *dstr)
{
	if (dstr == NULL)
		return;
	dstr->size = DYNSTR_INITSIZE;
	dstr->str = malloc(dstr->size * sizeof(char));
	dstr->len = 0;
	dstr->str[0] = '\0';
}


void
dynstr_free(struct dynstr_t *dstr)
{
	if (dstr != NULL)
		free(dstr->str);
}


void
dynstr_set_empty(struct dynstr_t *dstr)
{
	dstr->len = 0;
	dstr->str[0] = '\0';
}


void
dynstr_add(struct dynstr_t *dstr, char c)
{
	dstr->str[(dstr->len)++] = c;
	if (dstr->len == dstr->size) {
		dstr->size = DYNSTR_NEWSIZE(dstr->size);
		dstr->str = realloc(dstr->str, dstr->size * sizeof(char));
	}
	dstr->str[dstr->len] = '\0';
}


void
dynstr_concat(struct dynstr_t *dstr, const char *s)
{
	size_t i;

	if (s == NULL)
		return;
	for (i = 0; s[i] != '\0'; ++i)
		dynstr_add(dstr, s[i]);
}


void
dynstr_optsize(struct dynstr_t *dstr)
{
	dstr->size = dstr->len + 1;
	dstr->str = realloc(dstr->str, dstr->size * sizeof(char));
}


char*
dynstr_str_new(struct dynstr_t *dstr)
{
	char *str;
	size_t i;

	str = malloc((dstr->len + 1) * sizeof(char));
	for (i = 0; i <= dstr->len; ++i)
		str[i] = dstr->str[i];
	return str;
}


char*
dynstr_substr_new(struct dynstr_t *dstr, size_t offt, size_t len)
{
	char *substr;
	size_t i;

	substr = malloc((len + 1) * sizeof(char));
	for (i = 0; i < len; ++i)
		substr[i] = dstr->str[offt + i];
	substr[len] = '\0';
	return substr;
}
