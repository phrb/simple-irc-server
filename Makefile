all: ep1

ep1: ep1.c
	gcc -Wall -o ep1 ep1.c

test: test/tests.c modules/user.c include/user.h
	cd test && gcc -Wall -o test ../include/user.h ../modules/user.c tests.c

clean: 
	rm -f ep1 test/test

