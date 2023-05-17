CC = /bin/gcc
FLAG = -std=c99 -Wall

NAM = exfc

OBJECTS = obj/exfc.o

all : $(OBJECTS)
	$(CC) $(FLAG) -shared -so $(OBJECTS)

.PHONY : clean
clean:
    rm -fv $(OBJECTS)
