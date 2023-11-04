CC = /bin/gcc
FLAG = -std=c99 -Wall -g2

NAM = exfc

OBJECTS = build/src/test.o \
		      build/src/exfc.o

TARGETS = bin/test \
	bin/exfc.so

all : $(OBJECTS)
	$(CC) $(FLAG) -shared -o $(OBJECTS)

build/src/exfc.o: src/exfc.c
	$(CC) $(FLAG) -c src/exfc.c -o build/src/exfc.o

build/src/test.o : src/test.c
	$(CC) $(FLAG) -c src/test.c -o build/src/test.o

.PHONY : test
test: build/src/test.o build/src/exfc.o
	$(CC) $(FLAG) build/src/test.o build/src/exfc.o -o bin/test

.PHONY : clean
clean:
	rm -fv $(OBJECTS)
