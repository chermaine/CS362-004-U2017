#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define TESTCARD "smithy"
#define NET_GAINED 2
#define NET_DRAW 3
#define NET_DISCARD 1
#define NUM_TESTS 25

void assertTrue(int condition, char *test, int *allPassed) {
	if (condition) {
		printf("PASS: %s\n", test);
	}
	else {
		printf("FAIL: %s\n", test);
		*allPassed = 0;
	}
}

void checkSmithyEffect(int player, struct gameState *post, int cardPos, int *allPassed, int iter) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int r, i;

	smithyCard(cardPos, post);
	printf("Iteration #%d:\n", iter + 1);

	//test 1: check smithyEffect return 0
	//assertTrue(r == 0, "smithyEffect(): return 0", allPassed);

	//test 2: check handCount has a net gained of 2 cards
	assertTrue(post->handCount[player] == pre.handCount[player] + NET_GAINED, "Played gained a net of 2 cards", allPassed);

	int cardDrawn = post->handCount[player] - pre.handCount[player] + NET_DISCARD;

	//test 3: check deckCount decreased by number of cards drawn
	assertTrue(post->deckCount[player] + cardDrawn == pre.deckCount[player], "Cards gained came from player's own deck", allPassed);

	//test 4: check playedCardCount increased by NET_DISCARD
	assertTrue(post->playedCardCount == pre.playedCardCount + NET_DISCARD, "Player's played card count has a net gained of 1", allPassed);

	//test 5: check last played card is smithy
	assertTrue(post->playedCards[post->playedCardCount - 1] == smithy, "Last played card is smithy", allPassed);

	//test 6: check no other changes occured
	struct gameState copyPre;
	memcpy(&copyPre, &pre, sizeof(struct gameState));
	//copy all changes from post to copyPre
	copyPre.handCount[player] = post->handCount[player];
	copyPre.deckCount[player] = post->deckCount[player];
	copyPre.playedCardCount = post->playedCardCount;
	copyPre.discardCount[player] = post->discardCount[player];

	//copy cards in deck, discard, playedCards, hand pile
	memcpy(copyPre.deck[player], post->deck[player], sizeof(int) * copyPre.deckCount[player]);
	memcpy(copyPre.discard[player], post->discard[player], sizeof(int) * copyPre.discardCount[player]);
	memcpy(copyPre.playedCards, post->playedCards, sizeof(int) * copyPre.playedCardCount);
	for (i = 0; i < MAX_HAND; i++) {
		copyPre.hand[player][i] = post->hand[player][i];
	}

	assertTrue(memcmp(&copyPre, post, sizeof(struct gameState)) == 0, "No other changes to game state were made", allPassed);

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

		//random smithy card in hand position (1 - handCount)
		cardPos = floor(Random() * G.handCount[p]);

		//put smithy card into hand
		G.hand[p][cardPos] = smithy;

		//set whose turn
		G.whoseTurn = p;

		checkSmithyEffect(p, &G, cardPos, &allPassed, n);
	}

	if (allPassed)
		printf("All tests passed\n");

	return 0;
}