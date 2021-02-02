# Makefile

DYNLIB_DIR=../src

DYNSTR_SRC=$(DYNLIB_DIR)/dynstr.c test_dynstr.c
DYNSTR_HDR=$(DYNLIB_DIR)/dynstr.h
DYNSTR_TARGET=test_dynstr

DYNARR_SRC=$(DYNLIB_DIR)/dynarr.c test_dynarr.c
DYNARR_HDR=$(DYNLIB_DIR)/dynarr.h
DYNARR_TARGET=test_dynarr

DYNHASH_SRC=$(DYNLIB_DIR)/dynhash.c test_dynhash.c
DYNHASH_HDR=$(DYNLIB_DIR)/dynhash.h
DYNHASH_TARGET=test_dynhash

.PHONY: all

all: $(DYNSTR_TARGET) $(DYNARR_TARGET) $(DYNHASH_TARGET)

$(DYNSTR_TARGET): $(DYNSTR_SRC) $(DYNSTR_HDR)
	gcc -Wall -g -Og -I"$(DYNLIB_DIR)" $(DYNSTR_SRC) -o $(DYNSTR_TARGET)

$(DYNARR_TARGET): $(DYNARR_SRC) $(DYNARR_HDR)
	gcc -Wall -g -Og -I"$(DYNLIB_DIR)" $(DYNARR_SRC) -o $(DYNARR_TARGET)

$(DYNHASH_TARGET): $(DYNHASH_SRC) $(DYNHASH_HDR)
	gcc -Wall -g -Og -I"$(DYNLIB_DIR)" $(DYNHASH_SRC) -o $(DYNHASH_TARGET)
