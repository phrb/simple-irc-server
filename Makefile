FILES  = server.c modules/*.c include/*.h
INPUT  = server.c modules/*.c
CFLAGS = -Wall -g -pthread
OUTPUT = server

all: server

server: $(FILES)
	gcc -o $(OUTPUT) $(CFLAGS) $(INPUT)

test: test/tests.c modules/user.c include/user.h
	cd test && gcc -Wall -o test ../include/user.h ../modules/user.c tests.c

clean: 
	rm -f server test/test

