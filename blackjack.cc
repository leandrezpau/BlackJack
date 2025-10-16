#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define A 1
#define J 10
#define Q 10
#define K 10

enum CardType {
  kHeart_Type = 0,
  kDiamond_Type,
  kClub_Type,
  kSpades_Type
};
enum GameState {
  kStarting = 0,
  kPlaying,
  kWinned,
  kLost
};

struct Card {
  char num;
  CardType type;
  bool revealed;
};

struct Deck {
  const static int kNSuits = 4;
  const static int kNCards = 13;
  const static int kTotalCards = kNSuits * kNCards;

  Card cards[kNSuits * kNCards];
};

struct Hand {
  int numCards;

  Card* cards;

  int TotalScore(Card* cards);
};

struct Player {
  int numHands;

  Hand* hand;
};

struct Crupier {
  Deck deck;
  Hand hand;
};

void InitGame(Player** player, Crupier &crupier, int numPlayers);
void InitCards(Deck& deck);
void ShuffleCards(Deck& deck);

void GiveHand(Player* player, Crupier& crupier, bool started, int& currentCard, int numPlayers);

void FreeMem(Player* player, int numPlayers);

int main(){
  srand(time(NULL));
  int numPlayers = 1;

  Crupier crupier;
  Player* player;
  GameState gamestate = GameState::kStarting;

  switch(gamestate){
    case GameState::kStarting:{
      InitCards(crupier.deck);
      InitGame(&player, crupier, numPlayers);
      ShuffleCards(crupier.deck);
      break;
    }
    case GameState::kPlaying:{
      static int currentCard = 0;
      static bool started = false;
      while(gamestate == GameState::kPlaying){
        GiveHand(player, crupier, started, currentCard, numPlayers);
      }
      break;
    }
    case GameState::kWinned:{
      
      break;
    }
    case GameState::kLost:{
      
      break;
    }
  }
  
  FreeMem(player, numPlayers);
  return 0;
}

void InitGame(Player** player, Crupier &crupier, int numPlayers){
  *player = (Player*) malloc(sizeof(Player) * numPlayers);
  for(int i = 0; i < numPlayers; i++){
    (*(player) + i)->hand = (Hand*) malloc(sizeof(Hand));
    (*(player) + i)->hand->cards = (Card*) malloc(sizeof(Card) * 2);
    (*(player) + i)->numHands = 1;
  }
  crupier.hand.cards = (Card*) malloc(sizeof(Card) * 2);
}

void InitCards(Deck& deck){
  for(int e = 0; e < Deck::kNSuits; e++){
    for(int i = 0; i < Deck::kNCards; i++){

      deck.cards[i + e * Deck::kNCards].type = CardType(i);
      deck.cards[i + e * Deck::kNCards].revealed = false;
      
      switch(i){
        case 10:{
          deck.cards[i + e * Deck::kNCards].num = 'J';
          break;
        }
        case 11:{
          deck.cards[i + e * Deck::kNCards].num = 'Q';
          break;
        }
        case 12:{
          deck.cards[i + e * Deck::kNCards].num = 'K';
          break;
        }
        default:{
          deck.cards[i + e * Deck::kNCards].num = (i + 1) * 48;
          break;
        }
      }
    }
  }
}

void ShuffleCards(Deck& deck){
  //Getting a record to check if that card is shuffled
  bool shuffled[Deck::kTotalCards];
  for(int i = 0; i < Deck::kTotalCards; i++){
    shuffled[i] = false;
  }

  for(int i = 0; i < Deck::kTotalCards; i++){
    int suits, number;
    do{
      suits = rand() % Deck::kNSuits;
      number = rand() % Deck::kNCards;
    }while(shuffled[i] == true);

    //Swapping both cards
    Card card_1 = deck.cards[i];
    Card card_2 = deck.cards[number + suits * Deck::kNSuits];

    deck.cards[i] = card_2;
    deck.cards[number + suits * Deck::kNSuits] = card_1; 
  }
}

void GiveHand(Player* player, Crupier& crupier, bool started, int& currentCard, int numPlayers){
  if(!started){
    ShuffleCards(crupier.deck);

    //*crupier.hand.(cards + currentCard) = crupier.deck.cards[currentCard++];

    for(int i = 0; i < numPlayers; i++){
      (player + i)->hand->cards[currentCard] = crupier.deck.cards[currentCard++];
    }
    
  }
}


void FreeMem(Player* player, int numPlayers){
  for(int i = 0; i < numPlayers; i++){
    for(int e = 0; e < (player + i)->numHands; e++){
      free(((player + i)->hand + e)->cards);
    }
    free((player + i)->hand);
  }
  free(player);
  printf("Memoria Liberada");
}