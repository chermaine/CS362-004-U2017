Skip to content
This repository
Search
Pull requests
Issues
Marketplace
Gist
 @chermaine
 Sign out
 Unwatch 1
  Star 0
  Fork 105 chermaine/CS362-004-U2017
forked from aburasa/CS362-004-U2017
 Code  Pull requests 0  Projects 0  Wiki  Settings Insights 
Branch: master Find file Copy pathCS362-004-U2017/projects/cheangc/dominion/Makefile
e785ddc  12 minutes ago
@chermaine chermaine Updated unit tests
1 contributor
RawBlameHistory     
97 lines (69 sloc)  3.41 KB
CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)

unittestresult.out: cardtest1 cardtest2 cardtest3 cardtest4 unittest1 unittest2 unittest3 unittest4
	echo "Result for running unit tests:\n" > unittestresult.out
	echo "unittest1.c" >> unittestresult.out
	./unittest1 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "unittest2.c" >> unittestresult.out
	./unittest2 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "unittest3.c" >> unittestresult.out
	./unittest3 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "unittest4.c" >> unittestresult.out
	./unittest4 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "cardtest1.c:" >> unittestresult.out
	./cardtest1 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "cardtest2.c:" >> unittestresult.out
	./cardtest2 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "cardtest3.c" >> unittestresult.out
	./cardtest3 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

	echo "cardtest4.c" >> unittestresult.out
	./cardtest4 >> unittestresult.out
	gcov -b -c dominion.c >> unittestresult.out

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out badTestDrawCard badTestDrawCard.exe testDrawCard testDrawCard.exe cardtest1 cardtest1.exe cardtest2 cardtest2.exe cardtest3 cardtest3.exe cardtest4 cardtest4.exe unittest1 unittest1.exe unittest2 unittest2.exe unittest3 unittest3.exe unittest4 unittest4.exe 
Contact GitHub API Training Shop Blog About
© 2017 GitHub, Inc. Terms Privacy Security Status Help
