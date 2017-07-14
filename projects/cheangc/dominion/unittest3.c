#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTFUNCTION "updateCoins()"

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

/******************************** Test 1: Add 5 copper cards ************************************************/
	printf("--Test 1: Add 5 copper cards\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//set up player's hand to have 5 copper cards
	int copperCount = 0;
	i = 0;

	//get current copper count
	while (copperCount < 5 && i < testG.handCount[currentPlayer]) {
		if (testG.hand[currentPlayer][i] == copper) {
			copperCount++;
		}
		i++;
	}

	//add more copper card to player's hand until player has a total of 5 copper cards
	while (copperCount < 5) {
		testG.hand[currentPlayer][testG.handCount[currentPlayer]] = copper;
		testG.handCount[currentPlayer]++;
		copperCount++;
	}

	//call updateCoins
	totalTest++;
	assertTrue(updateCoins(currentPlayer, &testG, 0) == 0, "updateCoins() return 0", &passed, &allPassed);

	//player's coin is 5
	totalTest++;
	assertTrue(testG.coins == 5, "Player's coins = 5", &passed, &allPassed);

	//no state change in supply 
	totalTest++;
	int stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change occured to supply piles", &passed, &allPassed);

	//no state change in players
	totalTest++;
	assertTrue(testG.whoseTurn == G.whoseTurn, "No turn changed", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numBuys == G.numBuys, "Player's numBuys remain unchanged", &passed, &allPassed);

/*********************************** Test 2: Add 5 silver cards ************************************************/
	printf("\n--Test 2: Add 5 silver cards\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case

	//set up player's hand to have only 5 silver cards
	int silverCount = 0;

	//discard all non silver treasure cards and add silver cards to hand until player has 5 silver cards
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] == silver) {
			silverCount++;
		}
		else if (testG.hand[currentPlayer][i] == copper || testG.hand[currentPlayer][i] == gold) {
			testG.hand[currentPlayer][i] = silver;
			silverCount++;
		}
	}
	
	while (silverCount < 5) {
		testG.hand[currentPlayer][testG.handCount[currentPlayer]] = silver;
		testG.handCount[currentPlayer]++;
		silverCount++;
	}

	//call updateCoins
	totalTest++;
	assertTrue(updateCoins(currentPlayer, &testG, 0) == 0, "updateCoins() return 0", &passed, &allPassed);

	//player's coin is 5 * 2 = 10
	totalTest++;
	assertTrue(testG.coins == 10, "Player's coins = 10", &passed, &allPassed);

	//no state change in supply 
	totalTest++;
	stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change occured to supply piles", &passed, &allPassed);

	//no state change in players
	totalTest++;
	assertTrue(testG.whoseTurn == G.whoseTurn, "No turn changed", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numBuys == G.numBuys, "Player's numBuys remain unchanged", &passed, &allPassed);


/***************************************** Test 3: Add 5 gold cards *******************************************/
	printf("\n--Test 3: Add 5 gold cards\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case
	int goldCount = 0;

	//discard all non gold treasure cards and add gold cards to hand until player has 5 gold cards
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		if (testG.hand[currentPlayer][i] == gold) {
			goldCount++;
		}
		else if (testG.hand[currentPlayer][i] == copper || testG.hand[currentPlayer][i] == silver) {
			testG.hand[currentPlayer][i] = gold;
			goldCount++;
		}
	}
	
	while (goldCount < 5) {
		testG.hand[currentPlayer][testG.handCount[currentPlayer]] = gold;
		testG.handCount[currentPlayer]++;
		goldCount++;
	}

	//call updateCoins
	totalTest++;
	assertTrue(updateCoins(currentPlayer, &testG, 0) == 0, "updateCoins() return 0", &passed, &allPassed);

	//player's coin is 5 * 3 = 15
	totalTest++;
	assertTrue(testG.coins == 15, "Player's coins = 15", &passed, &allPassed);

	//no state change in supply 
	totalTest++;
	stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change occured to supply piles", &passed, &allPassed);

	//no state change in players
	totalTest++;
	assertTrue(testG.whoseTurn == G.whoseTurn, "No turn changed", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numBuys == G.numBuys, "Player's numBuys remain unchanged", &passed, &allPassed);


/**************************** Test 4: Add 1 gold, 1 silver, 1 copper card ***********************************/
	printf("\n--Test 4: Add 1 gold, 1 silver, and 1 copper card\n");
	memcpy(&testG, &G, sizeof(struct gameState)); // copy the game state to a test case
	
	//setup hand to only have 1 copper, 1 silver and 1 gold card
	testG.hand[currentPlayer][0] = copper;
	testG.hand[currentPlayer][1] = silver;
	testG.hand[currentPlayer][2] = gold;
	testG.hand[currentPlayer][3] = estate;
	testG.hand[currentPlayer][4] = estate;

	//call updateCoins
	totalTest++;
	assertTrue(updateCoins(currentPlayer, &testG, 0) == 0, "updateCoins() return 0", &passed, &allPassed);

	//player's coin is 1 + 2 + 3 = 6
	totalTest++;
	assertTrue(testG.coins == 6, "Player's coins = 6", &passed, &allPassed);

	//no state change in supply 
	totalTest++;
	stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change occured to supply piles", &passed, &allPassed);

	//no state change in players
	totalTest++;
	assertTrue(testG.whoseTurn == G.whoseTurn, "No turn changed", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numBuys == G.numBuys, "Player's numBuys remain unchanged", &passed, &allPassed);


/********************** Test 5: Add 1 gold, 1 silver, 1 copper card, and +5 bonus ******************************/
	printf("\n--Test 5: Add 1 gold, 1 silver, 1 copper card, and +5 bonus\n");

	//call updateCoins
	totalTest++;
	assertTrue(updateCoins(currentPlayer, &testG, 5) == 0, "updateCoins() return 0", &passed, &allPassed);

	//player's coin is 1 + 2 + 3 + 5 = 11
	totalTest++;
	assertTrue(testG.coins == 11, "Player's coins = 11", &passed, &allPassed);

	//no state change in supply 
	totalTest++;
	stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change occured to supply piles", &passed, &allPassed);

	//no state change in players
	totalTest++;
	assertTrue(testG.whoseTurn == G.whoseTurn, "No turn changed", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numActions == G.numActions, "Player's numActions remain unchanged", &passed, &allPassed);
	totalTest++;
	assertTrue(testG.numBuys == G.numBuys, "Player's numBuys remain unchanged", &passed, &allPassed);


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