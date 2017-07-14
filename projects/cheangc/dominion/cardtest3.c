#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTCARD "village"

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
	int allPassed = 1, passed = 0, totalTest = 0;
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

	//put a village card to player's hand
	testG.hand[currentPlayer][handpos] = village;

	//play a village card
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

	//test 1: gain 1 card
	totalTest++;
	int expectedGain = 1;
	int actualGain;
	actualGain = testG.handCount[currentPlayer] - G.handCount[currentPlayer] + discarded;
	assertTrue (actualGain == expectedGain, "Gained 1 card", &passed, &allPassed);

	//test 2: gain card came from player's pile
	totalTest++;
	assertTrue (testG.deckCount[currentPlayer] + actualGain == G.deckCount[currentPlayer], "Gained card came from player's own pile", &passed, &allPassed);

	//test 3: +2 actions
	totalTest++;
	assertTrue (testG.numActions == G.numActions + 2, "Gained 2 actions", &passed, &allPassed);

	//test 4: discarded played card
	totalTest++;
	assertTrue (testG.playedCardCount == G.playedCardCount + 1, "Discarded played card", &passed, &allPassed);

	//test 5: discarded card is village card
	totalTest++;
	assertTrue(testG.playedCards[testG.playedCardCount - 1] == village, "Last discarded played card is village", &passed, &allPassed);

	//test 6: no state change for player
	totalTest++;
	assertTrue (testG.whoseTurn == G.whoseTurn, "No state change for players", &passed, &allPassed);

	//test 7: no state change for supply count
	totalTest++;
	int stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue (stateChange == 0, "No state change occured for supply cards", &passed, &allPassed);


	//all tests passed
	if (allPassed) {
		printf("\nALL TESTS PASSED\n");
	}
	else {
		printf("\nPassed %d of %d tests\n", passed, totalTest);
	}
	
	printf("\n");
	return 0;
}