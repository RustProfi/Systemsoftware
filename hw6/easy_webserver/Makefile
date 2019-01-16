CC=gcc
CFLAGS=-static

.PHONY: easy

easy: easy.o
	$(CC) $(CFLAGS) $< -o easy

easymake: easy.c
	$(CC) -c $<

clean:
	rm -f easy.o easy
