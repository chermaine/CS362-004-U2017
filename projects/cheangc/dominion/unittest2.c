#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTFUNCTION "discardCard"

void assertTrue(int condition, char *test, int * passed, int * allPassed) {
	if (condition) {
		printf("PASS: %s\n", test);
		*passed = *passed + 1;
	}
	else {
		printf("FAIL: %s\n", test);
		*allPassed = 0;
	}
}

int main() {
	int allPassed = 1, passed = 0, totalTest = 0;
	int i;

	//setup game state
	struct gameState G, testG;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, steward};
	int numPlayers = 2;
	int currentPlayer = 0;
	int randomSeed = 100;
	initializeGame(numPlayers, k, randomSeed, &G);

	printf("\n----------------- Testing Function: %s ----------------\n\n", TESTFUNCTION);

	/******************* Test 1: Discard first card from player's hand ********************/
	printf("--Test 1: Discard the first card from player's hand \n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//discard the first card in hand
	int discardedCard = testG.hand[currentPlayer][0];

	//function return 0
	totalTest++;
	assertTrue(discardCard(0, currentPlayer, &testG, 0) == 0, "discardCard() return 0", &passed, &allPassed);

	//player's hand count decreased
	totalTest++;
	assertTrue(testG.handCount[currentPlayer] == G.handCount[currentPlayer] - 1, "Player's hand count decreased by 1", &passed, &allPassed);

	//player's playedCardCount increased
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount + 1, "Player's playedCardCount increased by 1", &passed, &allPassed);

	//last played card in player's played pile is the card we discarded
	totalTest++;
	assertTrue(testG.playedCards[testG.playedCardCount - 1] == discardedCard, "Last card in played pile is the card we discarded", &passed, &allPassed);

	//first card is now the last card in hand before discarding
	totalTest++;
	assertTrue(testG.hand[currentPlayer][0] == G.hand[currentPlayer][G.handCount[currentPlayer]-1], "First card is now the last card in player's hand before calling dicardCard()", &passed, &allPassed);


	/******************* Test 2: Trash last card ****************************************/
	printf("\n--Test 2: Discard last card from player's hand to trash\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//trash last card in hand
	discardedCard = testG.hand[currentPlayer][testG.handCount[currentPlayer] - 1];

	//function return 0
	totalTest++;
	assertTrue(discardCard(testG.handCount[currentPlayer] - 1, currentPlayer, &testG, 1) == 0, "discardCard() return 0", &passed, &allPassed);

	//player's hand count decreased
	totalTest++;
	assertTrue(testG.handCount[currentPlayer] == G.handCount[currentPlayer] - 1, "Player's hand count decreased by 1", &passed, &allPassed);

	//player's discard count increased
	totalTest++;
	assertTrue(testG.discardCount[currentPlayer] == G.discardCount[currentPlayer] - 1, "Player's discard count increased by 1", &passed, &allPassed);

	//last card in player's discard pile is the card we discarded
	totalTest++;
	assertTrue(testG.discard[currentPlayer][testG.discardCount[currentPlayer] - 1] == discardedCard, "Last discarded card in discard pile is the card we discarded", &passed, &allPassed);

	//last card in hand is now -1
	totalTest++;
	assertTrue(testG.hand[currentPlayer][testG.handCount[currentPlayer]] == -1, "Last card in hand is -1", &passed, &allPassed);

	//all remaining cards in hand remain unchanged
	totalTest++;
	int changed = 0;
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i]) {
			changed = 1;
			break;
		}
	}
	assertTrue(changed == 0, "Remaining cards in hand are unchanged", &passed, &allPassed);


	/*************** Test 3: Discard the only card in player's hand *********************/
	printf("\n--Test 3: Discard the only card in player's hand\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//reduce player's hand count to 1
	testG.handCount[currentPlayer] = 1;

	//get card we discarding
	discardedCard = testG.hand[currentPlayer][testG.handCount[currentPlayer] - 1];

	//call discardCard
	totalTest++;
	assertTrue(discardCard(testG.handCount[currentPlayer] - 1, currentPlayer, &testG, 0) == 0, "discardCard() return 0", &passed, &allPassed);

	//player's hand count is 0
	totalTest++;
	assertTrue(testG.handCount[currentPlayer] == 0, "Player's hand count is 0", &passed, &allPassed);

	//player's playedCardCount increased
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount + 1, "Player's playedCardCount increased by 1", &passed, &allPassed);

	//last played card in player's played pile is the card we discarded
	totalTest++;
	assertTrue(testG.playedCards[testG.playedCardCount - 1] == discardedCard, "Last card in played pile is the card we discarded", &passed, &allPassed);


	/********** Test 4: Discard a card that is not in player's hand *********************/
	printf("\n--Test 4: Discard a card not in player's hand\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//set card position to some arbituary number
	int handPos = 499;

	//call discardCard
	totalTest++;
	assertTrue(discardCard(handPos, currentPlayer, &testG, 0) == -1, "discardCard() return -1", &passed, &allPassed);

	//player's hand count remain unchanged
	totalTest++;
	assertTrue(testG.handCount[currentPlayer] == G.handCount[currentPlayer], "Player's hand count remain unchanged", &passed, &allPassed);

	//player's playedCardCount remain unchanged
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount + 1, "Player's playedCardCount remain unchanged", &passed, &allPassed);

	//all cards in hand remain unchanged
	totalTest++;
	changed = 0;
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i]) {
			changed = 1;
			break;
		}
	}
	assertTrue(changed == 0, "Cards in hand remain unchanged", &passed, &allPassed);

	//check if all tests passed
	if (allPassed) {
		printf("\nALL TESTS PASSED\n");
	}
	else {
		//print number of tests passed
		printf("\nPassed %d of %d tests\n", passed, totalTest);
	}
		
	printf("\n");
	return 0;
}