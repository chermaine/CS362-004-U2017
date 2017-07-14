#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTFUNCTION "playCard()"

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
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;
	int allPassed = 1, passed = 0, totalTest = 0;

	//setup game state
	struct gameState G, testG;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, steward};
	int numPlayers = 2;
	int currentPlayer = 0;
	int randomSeed = 100;
	initializeGame(numPlayers, k, randomSeed, &G);

	printf("\n----------------- Testing Function: %s ----------------\n\n", TESTFUNCTION);

	/*************************** Test 1: Play an action card ***************************/
	printf("--Test 1: Play an action card in player's hand\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//put an action card to player's hand
	testG.hand[currentPlayer][0] = smithy;

	//call playCard
	totalTest++;
	assertTrue(playCard(handPos, choice1, choice2, choice3, &testG) == 0, "playCard() return 0", &passed, &allPassed);

	//player's numActions decreased by 1
	totalTest++;
	assertTrue(testG.numActions == G.numActions - 1, "Player's numActions decreased by 1", &passed, &allPassed);

	//played card is now in played pile
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount + 1, "Player's playedCardCount increased by 1", &passed, &allPassed);

	//player's phase remain unchanged
	totalTest++;
	assertTrue(testG.phase == G.phase, "Player's phase remain unchanged", &passed, &allPassed);


	/***************** Test 2: Play an action card not in player's hand ******************/
	printf("\n--Test 2: Play an action card not in player's hand\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//put an action card to end of player's hand, but do not increase player's handCount
	testG.hand[currentPlayer][testG.handCount[currentPlayer]] = smithy;
	handPos = testG.handCount[currentPlayer];

	//call playCard
	totalTest++;
	assertTrue(playCard(handPos, choice1, choice2, choice3, &testG) == -1, "playCard() return -1", &passed, &allPassed);

	//player's numActions remain unchanged
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);

	//played card is not in played pile
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount, "Player's playedCardCount remain unchanged", &passed, &allPassed);

	//player's phase remain unchanged
	totalTest++;
	assertTrue(testG.phase == G.phase, "Player's phase remain unchanged", &passed, &allPassed);

	/****************** Test 3: Play an action card not in action phase ******************/
	printf("\n--Test 3: Play an action card during the wrong phase\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//change phase to buy phase
	testG.phase = 1;

	//play an action card
	handPos = 0;
	testG.hand[currentPlayer][0] = smithy;
	totalTest++;
	assertTrue(playCard(handPos, choice1, choice2, choice3, &testG) == -1, "playCard() return -1", &passed, &allPassed);

	//player's numActions remain unchanged
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);

	//played card is not in played pile
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount, "Player's playedCardCount remain unchanged", &passed, &allPassed);

	//player's phase remain unchanged
	totalTest++;
	assertTrue(testG.phase == 1, "Player's phase remain unchanged", &passed, &allPassed);


	/****************** Test 4: Play an action card when numAction = 0 ******************/
	printf("\n--Test 4: Play an action card when player's numActions is 0\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//set numActions to 0
	testG.numActions = 0;

	//play an action card
	handPos = 0;
	testG.hand[currentPlayer][0] = smithy;
	totalTest++;
	assertTrue(playCard(handPos, choice1, choice2, choice3, &testG) == -1, "playCard() return -1", &passed, &allPassed);

	//player's numActions remain unchanged
	totalTest++;
	assertTrue(testG.numActions == 0, "Player's numActions remain unchanged", &passed, &allPassed);

	//played card is not in played pile
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount, "Player's playedCardCount remain unchanged", &passed, &allPassed);

	//player's phase remain unchanged
	totalTest++;
	assertTrue(testG.phase == G.phase, "Player's phase remain unchanged", &passed, &allPassed);


	/************************** Test 5: Play a non action card  *************************/
	printf("\n--Test 5: Play a non action card\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//play an estate card
	testG.hand[currentPlayer][handPos] = estate;
	totalTest++;
	assertTrue(playCard(handPos, choice1, choice2, choice3, &testG) == -1, "playCard() return -1", &passed, &allPassed);

	//player's numActions remain unchanged
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);

	//played card is not in played pile
	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount, "Player's playedCardCount remain unchanged", &passed, &allPassed);

	//player's phase remain unchanged
	totalTest++;
	assertTrue(testG.phase == G.phase, "Player's phase remain unchanged", &passed, &allPassed);

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