#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TESTFUNCTION "gainCard()"

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

void getCardName(int cardNumber, char *card) {
	switch (cardNumber) {
		case 0: strcpy(card, "curse"); break;
		case 1: strcpy(card, "estate"); break;
		case 2: strcpy(card, "duchy"); break;
		case 3: strcpy(card, "province"); break;
		case 4: strcpy(card, "copper"); break;
		case 5: strcpy(card, "silver"); break;
		case 6: strcpy(card, "gold"); break;
		case 7: strcpy(card, "adventurer"); break;
		case 8: strcpy(card, "council_room"); break;
		case 9: strcpy(card, "feast"); break;
		case 10: strcpy(card, "gardens"); break;
		case 11: strcpy(card, "mine"); break;
		case 12: strcpy(card, "remodel"); break;
		case 13: strcpy(card, "smithy"); break;
		case 14: strcpy(card, "village"); break;
		case 15: strcpy(card, "baron"); break;
		case 16: strcpy(card, "great_hall"); break;
		case 17: strcpy(card, "minion"); break;
		case 18: strcpy(card, "steward"); break;
		case 19: strcpy(card, "tribute"); break;
		case 20: strcpy(card, "ambassador"); break;
		case 21: strcpy(card, "cutpurse"); break;
		case 22: strcpy(card, "embargo"); break;
		case 23: strcpy(card, "outpost"); break;
		case 24: strcpy(card, "salvager"); break;
		case 25: strcpy(card, "sea_hag"); break;
		case 26: strcpy(card, "treasure_map"); break;
	}
}

int main() {
	int allPassed = 1, passed = 0, totalTest = 0;
	int i;
	char card[50];
	char buffer[256];

	//setup game state
	struct gameState G, testG;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, steward};
	int numPlayers = 2;
	int currentPlayer = 0;
	int randomSeed = 100;
	initializeGame(numPlayers, k, randomSeed, &G);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n----------------- Testing Function: %s ----------------\n\n", TESTFUNCTION);

	/*************** test 1: gain a kingdowm card to player's hand *****************/
	printf("--Test 1: Gain one of each kingdom card from supply to player's hand\n");
	for (i = 0; i < 10; i++) {
		// reset test case
		memcpy(&testG, &G, sizeof(struct gameState));

		//test 1a: gain a kingdom card
		totalTest++;
		if (gainCard(k[i], &testG, 2, currentPlayer) == 0) {
			assertTrue(testG.handCount[currentPlayer] == G.handCount[currentPlayer] + 1, "Player's hand gained a card", &passed, &allPassed);

			//test 1b: gained a kingdom card from supply pile
			totalTest++;
			assertTrue(testG.supplyCount[k[i]] == G.supplyCount[k[i]] - 1, "Gained card from supply piles", &passed, &allPassed);

			//test 1c: check gained card is the card we specified
			totalTest++;
			memset(card, '\0', sizeof(card));
			getCardName(k[i], card);
			memset(buffer, '\0', sizeof(buffer));
			sprintf(buffer, "Gained the specified %s card", card);
			assertTrue(testG.hand[currentPlayer][testG.handCount[currentPlayer]-1] == k[i], buffer, &passed, &allPassed);
		}	
	}


	/************ test 2: gain a card that is not in supply ********************/
	printf("\n--Test 2: Gain a kingdom card that is not in supply\n");
	//reset test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//test 2a: check that gainCard() return -1
	totalTest++;
	assertTrue(gainCard(treasure_map, &testG, 2, currentPlayer) == -1, "gainCard() return -1: Did not gain card", &passed, &allPassed);
	
	//test 2b: check that supply piles remain unchanged
	totalTest++;
	int stateChange = 0;
	for (i = 0; i <= steward; i++) {
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			stateChange = 1;
			break;
		}
	}
	assertTrue(stateChange == 0, "No state change for supply cards", &passed, &allPassed);

	//test 2c: check that player did not gain a card
	totalTest++;
	assertTrue(testG.handCount[currentPlayer] == G.handCount[currentPlayer], "Player's hand count remain unchanged", &passed, &allPassed);


	/***************** test 3: gain a card that is empty in supply ****************/
	printf("\n--Test 3: Gain a kingdom card that is empty from supply\n");
	memcpy(&testG, &G, sizeof(struct gameState)); //reset test case
	testG.supplyCount[adventurer] = 0; //set adventurer in supply to 0

	//test 3a: gainCard() return -1
	totalTest++;
	assertTrue(gainCard(adventurer, &testG, 2, currentPlayer) == -1, "gainCard() return -1: Did not gain card", &passed, &allPassed);
	
	//test 3b: check that player did not gain a card
	totalTest++;
	assertTrue(testG.handCount[currentPlayer] == G.handCount[currentPlayer], "Player's hand count remain unchanged", &passed, &allPassed);

	
	/*************** test 4: gain a card to player's deck ***********************/
	printf("\n--Test 4: Gain a card from supply to player's deck\n");
	memcpy(&testG, &G, sizeof(struct gameState)); //reset test case

	//test 4a: gain a copper to player's deck
	totalTest++;
	assertTrue(gainCard(copper, &testG, 1, currentPlayer) == 0, "gainCard() return 0", &passed, &allPassed);

	totalTest++;
	assertTrue(testG.deckCount[currentPlayer] == G.deckCount[currentPlayer] + 1, "Player's deck count increased", &passed, &allPassed);

	//test 4b: check that card gained is a copper card
	totalTest++;
	assertTrue(testG.deck[currentPlayer][testG.deckCount[currentPlayer]-1] == copper, "Player gained a copper card", &passed, &allPassed);

	//test 4c: check that card gained came from supply pile
	totalTest++;
	assertTrue(testG.supplyCount[copper] == G.supplyCount[copper] - 1, "Gained card came from supply pile", &passed, &allPassed);

	/************* test 5: gain a card to player's played pile *******************/
	printf("\n--Test 5: Gain a card from supply to player's played pile\n");
	memcpy(&testG, &G, sizeof(struct gameState)); //reset test case

	//test 5a: gain a province to player's played pile
	totalTest++;
	assertTrue(gainCard(province, &testG, 0, currentPlayer) == 0, "gainCard() return 0", &passed, &allPassed);

	totalTest++;
	assertTrue(testG.playedCardCount == G.playedCardCount + 1, "Player's played pile count increased", &passed, &allPassed);
	
	//test 5b: check that card gained is a province card
	totalTest++;
	assertTrue(testG.playedCards[testG.playedCardCount - 1] == province, "Player gained a province card", &passed, &allPassed);

	//test 5c: check that card gained is from supply pile
	totalTest++;
	assertTrue(testG.supplyCount[province] == G.supplyCount[province] - 1, "Gained card came from supply pile", &passed, &allPassed);

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
	