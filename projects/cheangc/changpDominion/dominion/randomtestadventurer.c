#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define TESTCARD "adventurer"
#define NET_GAINED 1
#define NET_DRAW 2
#define NET_DISCARD 1
#define NUM_TESTS 100

void assertTrue(int condition, char *test, int *allPassed) {
	if (condition) {
		printf("PASS: %s\n", test);
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

void checkAdventurerEffect(int player, struct gameState *post, int cardPos, int *allPassed, int iter) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int r, i, bonus = 0;
	adventurerCard(cardPos, post);

	printf("Iteration #%d:\n", iter + 1);

	//test 1: check adventurerEffect return 0
	//assertTrue(r == 0, "adventurerEffect(): return 0", allPassed);

	//test 2: check net gained of 1 card
	assertTrue(post->handCount[player] == pre.handCount[player] + NET_GAINED, "Player gained a net of 1 card", allPassed);

	int cardDrawn = post->handCount[player] - pre.handCount[player] + NET_DISCARD;

	//test 3: check gained card came from player's own pile
	assertTrue(post->deckCount[player] + cardDrawn == pre.deckCount[player], "Gained card came from player's own pile", allPassed);

	//test 4: check num coins increased
	r = updateCoins(player, post, bonus);
	assertTrue(r == 0, "updateCoins(): return 0", allPassed);
	assertTrue(post->coins > pre.coins, "Player gained more coins", allPassed);

	//test 5: last 2 cards in hand is treasure card
	assertTrue(isTreasureCard(post->hand[player][post->handCount[player] - 1]) && isTreasureCard(post->hand[player][post->handCount[player] - 2]) , "Last 2 cards added to player's hand were treasure cards", allPassed);

	//test 6: check played card count increased
	assertTrue(post->playedCardCount == pre.playedCardCount + NET_DISCARD, "Player's playedCardCount has a net gain of 1", allPassed);

	//test 7: check last played card is adventurer
	assertTrue(post->playedCards[post->playedCardCount - 1] == adventurer, "Last played card was adventurer", allPassed);

	//test 8: check no other changes to game state
	struct gameState copyPre;
	memcpy(&copyPre, &pre, sizeof(struct gameState));

	//copy changes from post to copyPre
	copyPre.handCount[player] = post->handCount[player];
	copyPre.deckCount[player] = post->deckCount[player];
	copyPre.playedCardCount = post->playedCardCount;
	copyPre.discardCount[player] = post->discardCount[player];
	copyPre.numActions = post->numActions;
	copyPre.coins = post->coins;

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

		//put random cards into player's deck
		for (i = 0; i < G.deckCount[p]; i++) {
			G.deck[p][i] = floor(Random() * (treasure_map + 1));
		}

		//random number of discard count (1 - MAX_DECK)
		G.discardCount[p] = floor(Random() * MAX_DECK);

		//put random cards into player's discard pile
		for (i = 0; i < G.discardCount[p]; i++) {
			G.discard[p][i] = floor(Random() * (treasure_map + 1));
		}

		//random number of hand count (1 - MAX_HAND)
		G.handCount[p] = floor(Random() * MAX_HAND);

		//put random cards into player's hand
		for (i = 0; i < G.handCount[p]; i++) {
			G.hand[p][i] = floor(Random() * (treasure_map + 1));
		}

		//random number of played card count (1 - MAX_DECK)
		G.playedCardCount = floor(Random() * MAX_DECK);

		//put random cards into player's hand
		for (i = 0; i < G.playedCardCount; i++) {
			G.playedCards[i] = floor(Random() * (treasure_map + 1));
		}

		//random adventurer card in hand position (1 - handCount)
		cardPos = floor(Random() * G.handCount[p]);
		G.hand[p][cardPos] = adventurer;

		//random numCoins (0 - 100)
		G.coins = floor(Random() * 100);

		//set whose turn
		G.whoseTurn = p;

		checkAdventurerEffect(p, &G, cardPos, &allPassed, n);
	}

	if (allPassed)
		printf("All tests passed\n");
	
	return 0;
}