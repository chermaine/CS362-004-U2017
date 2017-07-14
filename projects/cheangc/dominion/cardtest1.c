#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTCARD "smithy"

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
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0, discarded = 1;
	int testResult = 1, passed = 0, totalTest = 6;
	int i;

	//setup game state
	struct gameState G, testG;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, steward};
	int numPlayers = 2;
	int currentPlayer = 0;
	int randomSeed = 100;
	initializeGame(numPlayers, k, randomSeed, &G);

	printf("\n----------------- Testing Card: %s ----------------\n\n", TESTCARD);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//put smithy card into player's hand
	testG.hand[currentPlayer][handpos] = smithy;
	
	//play a smithy card
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);


	/*************** test 1: current player should receive exact 3 cards *************************/
	printf("--Test 1: Player should receive 3 cards\n");
	int newCardCount = 3;
	int gainedCardCount = 0;
	gainedCardCount = testG.handCount[currentPlayer] - G.handCount[currentPlayer] + discarded;
	assertTrue(gainedCardCount == newCardCount, "Player gained 3 cards", &passed, &testResult);


	/*************** test 2: cards gained should come from his own pile ************************/
	printf("\n--Test 2: Gained cards should come from player's own pile\n");
	assertTrue(testG.deckCount[currentPlayer] + gainedCardCount == G.deckCount[currentPlayer], "Gained cards came from player's own pile", &passed, &testResult);


	/**************** test 3: no state change should occur for other players *******************/
	printf("\n--Test 3: No state change for players\n");
	//whoseTurn is still currentPlayer
	assertTrue(testG.whoseTurn == G.whoseTurn, "It is still current player's turn", &passed, &testResult);


	/************* test 4: no state change should occur to supply piles *****/
	printf("\n--Test 4: No state change for supply piles\n");
	int stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change occured to supply piles", &passed, &testResult);


	/************* test 5: played card is discarded ******************/
	printf("\n--Test 5: Played smithy card is discarded\n");
	assertTrue(testG.playedCardCount == G.playedCardCount + 1, "Player's playedCardCount increased by 1", &passed, &testResult);
	assertTrue(testG.playedCards[testG.playedCardCount - 1] == smithy, "Last played card is smithy card", &passed, &testResult);
	

	//Overall test results:
	if (testResult == 1) {
		printf("\nALL TESTS PASSED!!!\n");
	}
	else {
		printf("\nPassed %d of %d tests\n", passed, totalTest);
	}
	
	printf("\n");
	return 0;
}
