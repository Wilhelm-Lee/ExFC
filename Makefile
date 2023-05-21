CC = /bin/gcc
FLAG = -std=c99 -Wall -g

NAM = exfc

OBJECTS = build/src/test.o \
		  build/exfc.o \
		  build/src/exception.o

TARGETS = bin/test \
		  bin/exfc.so

all : $(OBJECTS)
	$(CC) $(FLAG) -shared -so $(OBJECTS)

build/src/exception.o: src/exception.c
	$(CC) $(FLAG) -c src/exception.c -o build/src/exception.o

build/src/test.o : src/test.c
	$(CC) $(FLAG) -c src/test.c -o build/src/test.o

.PHONY : test
test: build/src/test.o build/src/exception.o
	$(CC) $(FLAG) build/src/test.o build/src/exception.o -o bin/test

.PHONY : clean
clean:
	rm -fv $(OBJECTS)
