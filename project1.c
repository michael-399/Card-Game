#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

// Global variable to track health points on the table
int hpOnTable = 0;

/**
 * @brief Structure to represent a card with a type and suit.
 */
struct card{
	char card_type; /**< The type of the card (e.g., '1', 'J', 'K'). */
	char suit[10]; /**< The suit of the card (e.g., "heart", "diamond"). */
};

/**
 * @brief Structure to represent a player in the game.
 * 
 * Each player has an ID, health points (hp), two cards (face-up and face-down),
 * and pointers to the next and previous players in the circular list.
 */
struct player{
	int id; /**< Player's unique ID. */
	int hp; /**< Player's health points. */
	int revealedCard; /**< Indicates whether the face-down card has been revealed (1 = true, 0 = false). */
	struct card faceUpCard; /**< The player's face-up card. */
	struct card faceDawnCard; /**< The player's face-down card. */
	struct player* nextPlayer; /**< Pointer to the next player in the game. */
	struct player* previousPlayer; /**< Pointer to the previous player in the game. */
};

/**
 * @brief Prints the entire deck of cards.
 * 
 * @param deck An array of card structures representing the deck.
 */
void printDeck(struct card deck[]) {
    for (int i = 0; i < 40; i++) {
        printf("%c %s\n", deck[i].card_type, deck[i].suit);
    }
    return;
}

/**
 * @brief Shuffles the deck of cards randomly.
 * 
 * @param deck The deck of cards to shuffle.
 */
void shuffleDeck(struct card deck[]){
	for(int i = 0; i < 10000; i++){
		int random1 = rand() % 40; // Randomly select two cards to swap
		int random2 = rand() % 40;
		struct card tmp = deck[random1];
		deck[random1] = deck[random2];
		deck[random2] = tmp;
	}
	printf("Deck shuffled\n");
	return;
}

/**
 * @brief Deals two cards (face-up and face-down) to each player.
 * 
 * @param firstPlayer Pointer to the first player in the circular list.
 * @param deck The deck of cards to distribute.
 */
void giveCards(struct player* firstPlayer, struct card deck[]){
	int j = 0; // Index to track cards in the deck
	firstPlayer->faceUpCard = deck[j]; // Assign face-up card
	firstPlayer->faceDawnCard = deck[j+1]; // Assign face-down card
	j+=2; // Move to the next set of cards
	struct player* currentPlayer = firstPlayer;
	currentPlayer = firstPlayer->nextPlayer; // Move to the next player
	while(currentPlayer != firstPlayer){
		currentPlayer->faceUpCard = deck[j];
		currentPlayer->faceDawnCard = deck[j+1];
		j+=2;
		currentPlayer = currentPlayer->nextPlayer; // Continue to next player
	}
	return;
}

/**
 * @brief Prints the properties (ID, hp, cards) of all players.
 * 
 * @param firstPlayer Pointer to the first player in the circular list.
 */
void printPlayersProperties(struct player* firstPlayer){
	struct player* currentPlayer = firstPlayer;
	printf("Current player id: %d\n", currentPlayer->id);
	printf("Health points: %d\n", currentPlayer->hp);
	printf("Was the face-dawn card revealed?: %d\n", currentPlayer->revealedCard);
	printf("face-up card: %c %s\nface dawn card:%c %s\n", currentPlayer->faceUpCard.card_type, currentPlayer->faceUpCard.suit, currentPlayer->faceDawnCard.card_type, currentPlayer->faceDawnCard.suit);
	currentPlayer = firstPlayer->nextPlayer;
	while(currentPlayer != firstPlayer){
		printf("Current player id: %d\n", currentPlayer->id);
		printf("face up card: %c %s face dawn card:%c %s\n", currentPlayer->faceUpCard.card_type, currentPlayer->faceUpCard.suit, currentPlayer->faceDawnCard.card_type, currentPlayer->faceDawnCard.suit);
		currentPlayer = currentPlayer->nextPlayer;
	}
	return;
}

/**
 * @brief Applies the effect of the player's card based on its type.
 * 
 * @param currentPlayer Pointer to the current player.
 * @param card_type The type of card being played.
 */
void applyCard(struct player* currentPlayer, char card_type){
	if(card_type >= '2' && card_type <= '6'){
		printf("Card has no effect\n");
		return;
	}
	if(card_type == 'K'){
		printf("Player no.%d gets %d health points, hp on table is 0\n", currentPlayer->id, hpOnTable);
		currentPlayer->hp+=hpOnTable; // Add hp points on the table to current player
		hpOnTable = 0; // Reset table hp
		return;
	}
	if(card_type == '7'){
		if(currentPlayer->nextPlayer->revealedCard){
			printf("The players no.%d face-dawn card was already flipped\n", currentPlayer->nextPlayer->id);
			return;
		}
		printf("The player no.%d is forced to flip the face-dawn card\nIt is a card of type %c\n", currentPlayer->nextPlayer->id,currentPlayer->nextPlayer->faceDawnCard.card_type);
		applyCard(currentPlayer->nextPlayer, currentPlayer->nextPlayer->faceDawnCard.card_type); // Apply effect of face-down card
		return;
	}
	if(currentPlayer->hp <= 0){
		printf("The player has no hp, the card can not be played\n");
		return;
	}
	if(card_type == 'J'){
		printf("Player no.%d gets 1 health points. Player no.%d loses 1 health point.\n", currentPlayer->previousPlayer->id, currentPlayer->id);
		currentPlayer->previousPlayer->hp+=1; // Previous player gains hp
		currentPlayer->hp-=1; // Current player loses hp
		return;
	}
	if(card_type == 'Q'){
		printf("Player no.%d gets 1 health points. Player no.%d loses 1 health point.\n", currentPlayer->nextPlayer->nextPlayer->id, currentPlayer->id);
		currentPlayer->nextPlayer->nextPlayer->hp+=1; // Second next player gains hp
		currentPlayer->hp-=1; // Current player loses hp
		return;
	}
	if(card_type == '1'){
		hpOnTable+=1;
		currentPlayer->hp-=1;
		printf("Player no.%d loses 1 health point. There are %d health points on the table\n", currentPlayer->id, hpOnTable);
	}
	return;
}

/**
 * @brief Asks the player if they want to flip their face-down card.
 * 
 * @param currentPlayer Pointer to the current player.
 * @return 'Y' or 'N' based on the player's input.
 */
char getInput(struct player* currentPlayer){
	char wantToFlip = 'S';
	printf("Do you want to flip the card?(%c %s)\n[Y/N]:", currentPlayer->faceDawnCard.card_type, currentPlayer->faceDawnCard.suit);
	while(wantToFlip != 'N' && wantToFlip != 'Y' && wantToFlip != 'n' && wantToFlip != 'y' ){
		scanf(" %c", &wantToFlip);
	}
	return wantToFlip;
}

/**
 * @brief Updates the game state for each player's turn.
 * 
 * This function applies the effect of the face-up card, asks the player if they want to flip the face-down card, and applies its effect if flipped.
 * 
 * @param firstPlayer Pointer to the first player in the circular list.
 */
void updateGame(struct player* firstPlayer){
	struct player* currentPlayer = firstPlayer;
	char wantToFlip = 'S';
	printf("Player no.%d turn(%d hp)\n", currentPlayer->id, currentPlayer->hp);
	printf("Effect of face-up card is applied(%c %s)\n", currentPlayer->faceUpCard.card_type, currentPlayer->faceUpCard.suit);
	applyCard(currentPlayer, currentPlayer->faceUpCard.card_type); // Apply face-up card effect
	if(currentPlayer->revealedCard){
		printf("Your face-down card was already flipped\n");
	}
	else{
		char wantToFlip = getInput(currentPlayer);
		if(wantToFlip == 'Y' || wantToFlip == 'y'){
			currentPlayer->revealedCard = 1;
			applyCard(currentPlayer, currentPlayer->faceDawnCard.card_type); // Apply face-down card effect
		}
	}
	currentPlayer = firstPlayer->nextPlayer;
	while(currentPlayer != firstPlayer){
		printf("Player no.%d turn(%d hp)\n", currentPlayer->id, currentPlayer->hp);
		printf("Effect of face-up card is applied(%c %s)\n", currentPlayer->faceUpCard.card_type, currentPlayer->faceUpCard.suit);
		applyCard(currentPlayer, currentPlayer->faceUpCard.card_type);
		if(currentPlayer->revealedCard){
			printf("Your face-down card was already flipped\n");
		}
		else{
			char wantToFlip = getInput(currentPlayer);
			if(wantToFlip == 'Y' || wantToFlip == 'y'){
				currentPlayer->revealedCard = 1;
				applyCard(currentPlayer, currentPlayer->faceDawnCard.card_type);
			}
		}
		currentPlayer = currentPlayer->nextPlayer;
	}
	return;
}

/**
 * @brief Eliminates players with 0 or fewer health points and updates the circular list of players.
 * 
 * @param currentPlayer Pointer to the current player in the list.
 * @param numOfPlayersLeft Pointer to the number of players left in the game.
 * @return Pointer to the next player in the circular list after the elimination process.
 */
struct player* deletePlayers(struct player* currentPlayer, int* numOfPlayersLeft){
	int i = 0;
	while(i < 20){
		if(currentPlayer->hp <= 0){
			currentPlayer->previousPlayer->nextPlayer = currentPlayer->nextPlayer;
			currentPlayer->nextPlayer->previousPlayer = currentPlayer->previousPlayer;
			*numOfPlayersLeft -= 1;
			printf("Player no,%d was eliminated\n", currentPlayer->id);
		}
		currentPlayer = currentPlayer->nextPlayer;
		i++;
	}
	printf("%d players left\n", *numOfPlayersLeft);
	return currentPlayer;
}

/**
 * @brief Resets the 'revealedCard' ability of all players to 0 (not revealed).
 * 
 * @param firstPlayer Pointer to the first player in the circular list.
 */
void resetAbility(struct player* firstPlayer){
	firstPlayer->revealedCard = 0;
	struct player* currentPlayer = firstPlayer->nextPlayer;
	while(currentPlayer != firstPlayer){
		currentPlayer->revealedCard = 0;
		currentPlayer = currentPlayer->nextPlayer;
	}
	return;
}

/**
 * @brief Selects a random player to start the next turn.
 * 
 * @param firstPlayer Pointer to the first player in the circular list.
 * @return Pointer to the selected player.
 */
struct player* pickPlayer(struct player* firstPlayer){
	int i = rand() % 20;
	while(i > 0){
		firstPlayer = firstPlayer->nextPlayer;
		i--;
	}
	printf("First player is player no.%d\n", firstPlayer->id);
	return firstPlayer;
}

int main(){
	char suits[4][10] = {"heart", "diamond", "club", "spade"};
	char cards_types[10] = {'1','2','3','4','5','6','7','J','Q', 'K'};
	int game = 1;
	struct card deck[40];
	srand(time(NULL));
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 10; j++){
			deck[i*10+j].card_type = cards_types[j];
			strcpy(deck[i*10+j].suit, suits[i]);
		}
	}
	while(game){
		int numOfPlayers = 0;
		while(numOfPlayers < 2 || numOfPlayers > 20){
			printf("Introduce the number of players(2-20): ");
			scanf(" %d", &numOfPlayers);
		}
		int numOfPlayersLeft = numOfPlayers;
		struct player players[numOfPlayers];

		for(int i = 0; i < numOfPlayers; i++){
			players[i].id = i;
			players[i].hp = 2;
			players[i].revealedCard = 0;
			if(i + 1 != numOfPlayers){
				players[i].nextPlayer = &players[i+1];
	
			} else{
				players[i].nextPlayer = &players[0];				
			}
			if(i != 0){
				players[i].previousPlayer = &players[i-1];
			} else{
				players[i].previousPlayer = &players[numOfPlayers-1];
			}
		}
		struct player* firstPlayer = &players[rand() % numOfPlayers];
		while(numOfPlayersLeft > 1){
			shuffleDeck(deck);
			firstPlayer = pickPlayer(firstPlayer);
			giveCards(firstPlayer, deck);
			updateGame(firstPlayer);
			resetAbility(firstPlayer);
			firstPlayer = deletePlayers(firstPlayer, &numOfPlayersLeft);
		}
		printf("Player no. %d is the winner!!!\n", firstPlayer->id);
		printf("If you want to exit the game press Ctrl+C\n");
	}
	return 0;
}
