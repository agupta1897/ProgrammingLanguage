OBJD = lex.o scanner.o recognizer.o interpreter.o environment.o
OOPTS = -Wall -std=c99 -Wextra -g -c
LOPTS = -Wall -std=c99 -Wextra -g

all :	PInterpreter

PInterpreter :	$(OBJD)
	gcc $(LOPTS) -o PInterpreter $(OBJD)

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
	rm -f $(OBJD) PInterpreter *.o

test :
	./PInterpreter test.idc

error1:
	cat error1.sea

error1x:
	sea error1.sea

error2:
	cat error2.sea

error2x:
	sea error2.sea

error3:
	cat error3.sea

error3x:
	sea error3.sea

error4:
	cat error4.sea

error4x:
	sea error4.sea

error5:
	cat error5.sea

error5x:
	sea error5.sea

arrays:
	cat arrays.sea

arraysx:
	sea arrays.sea

conditionals:
	cat conditionals.sea

conditionalsx:
	sea conditionals.sea

recursion:
	cat recursion.sea

recursionx:
	sea recursion.sea

iteration:
	cat iteration.sea

iterationx:
	sea iteration.sea

functions:
	cat functions.sea

functionsx:
	sea functions.sea

lambdas:
	cat lambdas.sea

lambdasx:
	sea lambdas.sea