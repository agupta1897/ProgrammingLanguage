OBJD = lex.o scanner.o recognizer.o interpreter.o environment.o
OOPTS = -Wall -std=c99 -Wextra -g -c
LOPTS = -Wall -std=c99 -Wextra -g

all :	PORNInterpreter

PORNInterpreter :	$(OBJD)
	gcc $(LOPTS) -o PORNInterpreter $(OBJD)

interpreter.o :	interpreter.c lex.h recognizer.h environment.h
	gcc $(OOPTS) interpreter.c

recognizer.o : recognizer.c recognizer.h environment.h lex.h
	gcc $(OOPTS) recognizer.c

environment.o :	environment.c environment.h lex.h
	gcc $(OOPTS) environment.c

lex.o :	lex.c lex.h scanner.h
	gcc $(OOPTS) lex.c

scanner.o:	scanner.c scanner.h
	gcc $(OOPTS) scanner.c

clean :
	rm -f $(OBJD) PORNInterpreter *.o

test :
	./PORNInterpreter test.idc