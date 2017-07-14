#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTCARD "adventurer"

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

int isTreasureCard(int card) {
	if (card == copper || card == silver || card == gold) {
		return 1;
	}
	else {
		return 0;
	}
}

int treasureValue(int card) {
	if (card == copper)
		return 1;
	else if (card == silver)
		return 2;
	else 
		return 3;
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

	//put adventurer card to player's hand
	testG.hand[currentPlayer][handpos] = adventurer;

	//play an adventurer card
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	//test 1: gain 2 cards
	totalTest++;
	int expectedGain = 2;
	int actualGain;
	actualGain = testG.handCount[currentPlayer] - G.handCount[currentPlayer] + discarded;
	assertTrue (actualGain == expectedGain, "Gain 2 cards", &passed, &allPassed);

	//test 2: gain extra coins
	totalTest++;
	int originalCoins = 0;
	
	for (i = 0; i < G.handCount[currentPlayer]; i++) {
		if (isTreasureCard(G.hand[currentPlayer][i])) {
			originalCoins += treasureValue(G.hand[currentPlayer][i]);
		}
		if (isTreasureCard(G.hand[currentPlayer][i])) {
			originalCoins += treasureValue(G.hand[currentPlayer][i]);
		}
		if (isTreasureCard(G.hand[currentPlayer][i])) {
			originalCoins += treasureValue(G.hand[currentPlayer][i]);
		}	
	}

	int newCoins = 0;
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (isTreasureCard(testG.hand[currentPlayer][i])) {
			newCoins += treasureValue(G.hand[currentPlayer][i]);
		}
		if (isTreasureCard(testG.hand[currentPlayer][i])) {
			newCoins += treasureValue(G.hand[currentPlayer][i]);
		}
		if (isTreasureCard(testG.hand[currentPlayer][i])) {
			newCoins += treasureValue(G.hand[currentPlayer][i]);
		}
	}
	assertTrue (newCoins > originalCoins, "Gain more coins", &passed, &allPassed);
	

	//test 3: gain cards come from player's pile
	totalTest++;
	assertTrue (testG.deckCount[currentPlayer] + actualGain - discarded == G.deckCount[currentPlayer], "Gained cards came from player's pile", &passed, &allPassed);


	//test 4: top 2 cards on hand is treasure card
	totalTest++;
	int topCard = testG.handCount[currentPlayer] - 1;
	assertTrue (isTreasureCard(testG.hand[currentPlayer][topCard]) && isTreasureCard(testG.hand[currentPlayer][topCard-1]), "Top 2 cards on hand is treasure card", &passed, &allPassed);


	//test 5: discard played card
	totalTest++;
	assertTrue (testG.playedCardCount == G.playedCardCount + 1, "Discarded played card", &passed, &allPassed);

	//test 6: last played card is adventurer
	totalTest++;
	assertTrue(testG.playedCards[testG.playedCardCount - 1] == adventurer, "Last discarded played card is adventurer", &passed, &allPassed);

	//test 6: no state change for players and supply cards
	totalTest++;
	assertTrue (testG.whoseTurn == G.whoseTurn, "No state change for players", &passed, &allPassed);

	totalTest++;
	int stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue (stateChange == 0, "No state change for supply cards", &passed, &allPassed);


	//check if all tests passed
	if (allPassed) {
		printf("\nALL TESTS PASSED\n");
	}
	else {
		printf("\nPassed %d of %d tests\n", passed, totalTest);
	}

	printf("\n");
	return 0;
}