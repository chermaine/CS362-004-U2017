#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define TESTCARD "village"
#define NET_GAINED 0
#define NET_DRAW 1
#define NET_DISCARD 1
#define NUM_TESTS 100
#define NET_NUM_ACTION 2

void assertTrue(int condition, char *test, int *allPassed) {
	if (condition) {
		printf("PASS: %s\n", test);
	}
	else {
		printf("FAIL: %s\n", test);
		*allPassed = 0;
	}
}

void checkVillageEffect(int player, struct gameState *post, int cardPos, int *allPassed, int iter) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int r, i;

	villageCard(cardPos, post);

	printf("Iteration #%d:\n", iter + 1);

	//test 1: check smithyEffect return 0
	//assertTrue(r == 0, "villageEffect(): return 0", allPassed);

	//test 2: check net gain of 0 card
	assertTrue(post->handCount[player] == pre.handCount[player], "Player gained a net of 0 card", allPassed);

	int cardDrawn = post->handCount[player] - pre.handCount[player] + NET_DISCARD;

	//test 3: check gained card came from player's deck
	assertTrue(post->deckCount[player] + cardDrawn == pre.deckCount[player], "Drawn cards came from player's own pile", allPassed);

	//test 4: numActions increased by 2
	assertTrue(post->numActions == pre.numActions + NET_NUM_ACTION, "Player gained a net of 2 actions", allPassed);

	//test 5: check playedCardCount has a net gained of 1
	assertTrue(post->playedCardCount == pre.playedCardCount + NET_DISCARD, "Player's playedCardCount has a net gained of 1", allPassed);

	//test 6: check played village card is discarded
	assertTrue(post->playedCards[post->playedCardCount - 1] == village, "Last played card is village", allPassed);

	//test 7: check no other changes occured
	struct gameState copyPre;
	memcpy(&copyPre, &pre, sizeof(struct gameState));

	//copy all changes from post to copyPre
	copyPre.handCount[player] = post->handCount[player];
	copyPre.deckCount[player] = post->deckCount[player];
	copyPre.playedCardCount = post->playedCardCount;
	copyPre.discardCount[player] = post->discardCount[player];
	copyPre.numActions = post->numActions;
	
	//copy all cards piles (deck, hand, discard, played) 
	memcpy(copyPre.deck[player], post->deck[player], sizeof(int) * copyPre.deckCount[player]);
	memcpy(copyPre.discard[player], post->discard[player], sizeof(int) * copyPre.discardCount[player]);
	memcpy(copyPre.playedCards, post->playedCards, sizeof(int) * copyPre.playedCardCount);

	for (i = 0; i < MAX_HAND; i++) {
		copyPre.hand[player][i] = post->hand[player][i];
	}

	//check no other changes occurred
	assertTrue(memcmp(&copyPre, post, sizeof(struct gameState)) == 0, "No other changes to game state", allPassed);

	printf("\n");

}
int main() {
	int i, n, p, cardPos;
	int allPassed = 1;

	struct gameState G;

	printf("Random Testing: %s\n\n", TESTCARD);
	SelectStream(2);
	PutSeed(3);

	for (n = 0; n < NUM_TESTS; n++) {
		//random byte for gamestate
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*) &G)[i] = floor(Random() * 256);
		}
		//random number of player (1 - 2)
		p = floor(Random() * 2);

		//random number of deck count (1 - MAX_DECK)
		G.deckCount[p] = floor(Random() * MAX_DECK);

		//random number of discard count (1 - MAX_DECK)
		G.discardCount[p] = floor(Random() * MAX_DECK);

		//random number of hand count (1 - MAX_HAND)
		G.handCount[p] = floor(Random() * MAX_HAND);

		//random number of played card count (1 - MAX_DECK)
		G.playedCardCount = floor(Random() * MAX_DECK);

		//random number of played cards (1 - MAX_DECK)
		G.playedCards[p] = floor(Random() * MAX_DECK);

		//random village card in hand position (1 - handCount)
		cardPos = floor(Random() * G.handCount[p]);
		G.hand[p][cardPos] = village;

		//random number actions (1 - 5)
		G.numActions = floor(Random() * 5);

		//set whose turn
		G.whoseTurn = p;

		checkVillageEffect(p, &G, cardPos, &allPassed, n);
	}

	if (allPassed)
		printf("All tests passed\n");
	
	return 0;
}