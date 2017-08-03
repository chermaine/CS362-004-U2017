#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTCARD "council_room"

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
	printf("--Current Player: Player %d\n", currentPlayer + 1);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//put council_room card into player's hand
	testG.hand[currentPlayer][handpos] = council_room;

	//play a council_room card
	cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

	//test 1: gain 4 cards
	totalTest++;
	int expectedGain = 4;
	int actualGain = testG.handCount[currentPlayer] - G.handCount[currentPlayer] + discarded;
	assertTrue(actualGain == expectedGain, "Gained 4 cards", &passed, &allPassed);
	printf("actualGain = %d\n", actualGain);
	//test 2: numBuy increase by 1
	totalTest++;
	assertTrue(testG.numBuys == G.numBuys + 1, "NumBuys increased by 1", &passed, &allPassed);

	//test 3: played card pile increase by 1
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount - discarded, "Played council_room card is discarded", &passed, &allPassed);

	//test 4: gained cards come from player's pile
	totalTest++;
	assertTrue (testG.deckCount[currentPlayer] + actualGain - discarded == G.deckCount[currentPlayer], "Gained cards came from player's own pile", &passed, &allPassed);

	//test 5: no state change for player
	totalTest++;
	assertTrue (testG.whoseTurn == G.whoseTurn, "No state change for players", &passed, &allPassed);

	printf("\n--All other players:\n");
	//test 6: all other player gain 1 card
	char buffer[256];

	for (i = 0; i < numPlayers; i++) {
		if (i != currentPlayer) {
			totalTest++;
			memset(buffer, '\0', sizeof(buffer));
			sprintf(buffer, "Player %d gained 1 card", i + 1);
			assertTrue (testG.handCount[i] == G.handCount[i] + 1, buffer, &passed, &allPassed);
		}
	}

	//test 7: gained cards come from player's pile
	for (i = 0; i < numPlayers; i++) {
		if (i != currentPlayer) {
			totalTest++;
			memset(buffer, '\0', sizeof(buffer));
			sprintf(buffer, "Player %d gained 1 card from player %d's own pile", i + 1, i + 1);
			assertTrue(testG.deckCount[i] + 1 == G.deckCount[i], buffer, &passed, &allPassed);
		}
	}

	printf("\n--Supply Cards:\n");
	//test 8: no state change to supply cards
	totalTest++;
	int stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue (stateChange == 0, "No state change to supply cards", &passed, &allPassed);

	//print all tests passed if all test passed
	if (allPassed) {
		printf("\nALL TESTS PASSED\n");
	}
	else {
		printf("\nPassed %d of %d tests\n", passed, totalTest);
	}

	printf("\n");

	return 0;
}