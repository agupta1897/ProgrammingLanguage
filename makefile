OBJD = lex.o scanner.o interpreter.o
OOPTS = -Wall -std=c99 -Wextra -g -c
LOPTS = -Wall -std=c99 -Wextra -g

all :	PORNInterpreter

PORNInterpreter :	$(OBJD)
	gcc $(LOPTS) -o PORNInterpreter $(OBJD)

interpreter.o :	interpreter.c lex.h
	gcc $(OOPTS) interpreter.c

lex.o :	lex.c lex.h scanner.h
	gcc $(OOPTS) lex.c

scanner.o:	scanner.c scanner.h
	gcc $(OOPTS) scanner.c

clean :
	rm -f $(OBJD) PORNInterpreter *.o

test :
	./PORNInterpreter test.idc