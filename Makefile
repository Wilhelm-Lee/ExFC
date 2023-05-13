CC = /bin/gcc
FLAG = -std=c99 -Wall

NAM = exfc

EXFC = obj/exfc.o

all : $(EXFC)
	$(CC) $(FLAG) -shared -so $(EXFC)

.PHONY : clean
clean:
    rm -fv $(EXFC)
