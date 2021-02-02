#include <stdlib.h>
#include "dynstr.h"


void
dynstr_init(struct dynstr *dstr)
{
	if (dstr == NULL)
		return;
	dstr->size = DYNSTR_INITSIZE;
	dstr->str = malloc(dstr->size * sizeof(char));
	dstr->len = 0;
	dstr->str[0] = '\0';
}


void
dynstr_free(struct dynstr *dstr)
{
	if (dstr != NULL)
		free(dstr->str);
}


void
dynstr_set_empty(struct dynstr *dstr)
{
	dstr->len = 0;
	dstr->str[0] = '\0';
}


void
dynstr_add_char(struct dynstr *dstr, char c)
{
	dstr->str[(dstr->len)++] = c;
	if (dstr->len == dstr->size) {
		dstr->size = DYNSTR_NEWSIZE(dstr->size);
		dstr->str = realloc(dstr->str, dstr->size * sizeof(char));
	}
	dstr->str[dstr->len] = '\0';
}


void
dynstr_add_chararr(struct dynstr *dstr, const char *s, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		dynstr_add_char(dstr, s[i]);
}


void
dynstr_add(struct dynstr *dstr, const char *s)
{
	if (s == NULL)
		return;
	while (*s != '\0')
		dynstr_add_char(dstr, *s++);
}


static void
dynstr_swapchar(char *str, size_t n)
{
	size_t i = 0;
	char t;

	/* assert n > 0 */
	--n;
	while (i < n) {
		t = str[i];
		str[i++] = str[n];
		str[n--] = t;
	}
}


void
dynstr_add_int(struct dynstr *dstr, int n)
{
	size_t k = dstr->len;

	if (n < 0) {
		dynstr_add_char(dstr, '-');
		n = -n;
		++k;
	}
	do {
		dynstr_add_char(dstr, '0' + (char) (n % 10));
	} while ((n /= 10) != 0);

	dynstr_swapchar(dstr->str + k, dstr->len - k);
}


void
dynstr_add_date(struct dynstr *dstr, const struct tm *tm)
{
	char s[11];
	size_t i;

	if (tm == NULL)
		return;
	if (strftime(s, 11, "%Y-%m-%d", tm) == 0)
		return;
	for (i = 0; s[i] != '\0'; ++i)
		dynstr_add_char(dstr, s[i]);
}


void
dynstr_add_datetime(struct dynstr *dstr, const struct tm *tm)
{
	char s[20];
	size_t i;

	if (tm == NULL)
		return;
	if (strftime(s, 20, "%Y-%m-%d %H:%M:%S", tm) == 0)
		return;
	for (i = 0; s[i] != '\0'; ++i)
		dynstr_add_char(dstr, s[i]);
}


void
dynstr_add_char_xmlesc(struct dynstr *dstr, char c)
{
	switch (c) {
	case '<':
		dynstr_add(dstr, "&lt;");
		break;
	case '>':
		dynstr_add(dstr, "&gt;");
		break;
	case '&':
		dynstr_add(dstr, "&amp;");
		break;
	case '"':
		dynstr_add(dstr, "&quot;");
		break;
	case '\'':
		dynstr_add(dstr, "&apos;");
		break;
	default:
		dynstr_add_char(dstr, c);
	}
}


void
dynstr_add_xmlesc(struct dynstr *dstr, const char *s)
{
	size_t i;

	if (s == NULL)
		return;
	for (i = 0; s[i] != '\0'; ++i)
		dynstr_add_char_xmlesc(dstr, s[i]);
}


void
dynstr_optsize(struct dynstr *dstr)
{
	dstr->size = dstr->len + 1;
	dstr->str = realloc(dstr->str, dstr->size * sizeof(char));
}


char *
dynstr_str_new(struct dynstr *dstr)
{
	char *str;
	size_t i;

	str = malloc((dstr->len + 1) * sizeof(char));
	for (i = 0; i <= dstr->len; ++i)
		str[i] = dstr->str[i];
	return str;
}


char *
dynstr_substr_new(struct dynstr *dstr, size_t offt, size_t len)
{
	char *substr;
	size_t i;

	substr = malloc((len + 1) * sizeof(char));
	for (i = 0; i < len; ++i)
		substr[i] = dstr->str[offt + i];
	substr[len] = '\0';
	return substr;
}
