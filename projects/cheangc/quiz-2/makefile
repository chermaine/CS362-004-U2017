CFLAGS = -Wall -fpic -coverage -lm

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS)

clean:
	rm -f *.o testme testme.exe *.gcov *.gcda *.gcno 