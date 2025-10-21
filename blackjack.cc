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
  int TotalScore(Card* cards, int numCards);
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

void GiveHand(Player* player, Crupier& crupier, int& currentCard, int numPlayers);
void HitOrStand(Player* player, Crupier& crupier, int numPlayers, int& currentCard, GameState& gamestate);


void FreeMem(Player* player, int numPlayers);

int main(int argc, char** argv){
  srand(time(NULL));
  int numPlayers = argc;

  Crupier crupier;
  Player* player;
  int currentCard = 0;
  GameState gamestate = GameState::kStarting;
  bool end = false;
  while(!end){
    switch(gamestate){
      case GameState::kStarting:{
        InitCards(crupier.deck);
        InitGame(&player, crupier, numPlayers);
        ShuffleCards(crupier.deck);

        GiveHand(player, crupier, currentCard, numPlayers);

        gamestate = GameState::kPlaying;
        break;
      }
      case GameState::kPlaying:{
        static int currentCard = 0;
        while(gamestate == GameState::kPlaying){
          HitOrStand(player, crupier, numPlayers, currentCard, gamestate);
        }
        end = true;
        break;
      }
      case GameState::kWinned:{
        
        break;
      }
      case GameState::kLost:{
        
        break;
      }
    }
  }
  FreeMem(player, numPlayers);
  return 0;
}
int Hand::TotalScore(Card* cards, int numCards){
  int score = 0;
  for(int i = 0; i < numCards; i++){
    switch((cards + i)->num){
      case 'J':{
        score += 10;
        break;
      }
      case 'Q':{
        score += 10;
        break;
      }
      case 'K':{
        score += 10;
        break;
      }
      case 'A':{
        score += 1;
        break;
      }
      default:{
        score += atoi(&(cards + i)->num);
        break;
      }
    }
  }
  return score;
}

void InitGame(Player** player, Crupier &crupier, int numPlayers){
  *player = (Player*) malloc(sizeof(Player) * numPlayers);
  for(int i = 0; i < numPlayers; i++){
    (*(player) + i)->hand = (Hand*) malloc(sizeof(Hand));
    (*(player) + i)->hand->cards = (Card*) malloc(sizeof(Card) * 2);
    (*(player) + i)->hand->numCards = 0;
    (*(player) + i)->numHands = 1;
  }
  crupier.hand.cards = (Card*) malloc(sizeof(Card) * 2);
}

void InitCards(Deck& deck){
  for(int e = 0; e < Deck::kNSuits; e++){
    for(int i = 0; i < Deck::kNCards; i++){

      deck.cards[i + e * Deck::kNCards].type = CardType(e);
      deck.cards[i + e * Deck::kNCards].revealed = false;
      
      switch(i){
        case 0:{
          deck.cards[i + e * Deck::kNCards].num = 'A';
          break;
        }
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
          itoa(i + 1, &deck.cards[i + e * Deck::kNCards].num, 10);
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

void GiveHand(Player* player, Crupier& crupier, int& currentCard, int numPlayers){
  ShuffleCards(crupier.deck);

  crupier.hand.cards[0] = crupier.deck.cards[currentCard++];
  crupier.hand.cards[1] = crupier.deck.cards[currentCard++];
  crupier.hand.numCards = 2;

  for(int i = 0; i < numPlayers; i++){
    (player + i)->hand->cards[0] = crupier.deck.cards[currentCard++];
    (player + i)->hand->cards[1] = crupier.deck.cards[currentCard++];
    (player + i)->hand->numCards = 2;
  }
}
void PrintTotalCards(Hand hand){
  int cardAnumber = 0;
  for(int e = 0; e < hand.numCards; e++){
    printf("%c, ", hand.cards[e].num);
    if(hand.cards[e].num == 'A') cardAnumber++;
  }

  printf("\nNumero total: <<");
  for(int p = 0; p < cardAnumber + 1; p++){
    printf("%d", hand.TotalScore(hand.cards, hand.numCards) + p * 10);
    if(p + 1 <= cardAnumber) printf(" / ");
  }
  printf(">>");
}

void HitOrStand(Player* player, Crupier& crupier, int numPlayers, int& currentCard, GameState& gamestate){
  for(int i = 0; i < numPlayers; i++){
    for(int j = 0; j < (player + i)->numHands; j++){
      printf("\nMano numero %d: ", j + 1);
      PrintTotalCards((player + i)->hand[j]);
    
      char hitorstand;
      do{
        printf("\nHit (Press 1) or Stand (Press 2): ");
        getchar();
        hitorstand = getchar();
        if(hitorstand == '1'){
          (player + i)->hand[j].cards = (Card*) realloc((player + i)->hand[j].cards, sizeof(Card) * (player + i)->hand[j].numCards++);

          (player + i)->hand[j].cards[(player + i)->hand[j].numCards - 1] = crupier.deck.cards[currentCard++];
          printf("\nNueva carta: %c", (player + i)->hand[j].cards[(player + i)->hand[j].numCards - 1].num);

          printf("\nMano numero %d: ", j + 1);
          PrintTotalCards((player + i)->hand[j]);

        }


        if(hitorstand == '3'){
          gamestate = GameState::kWinned;
          hitorstand = '2';
        }
      }while(hitorstand != '2' && (player + i)->hand[j].TotalScore((player + i)->hand[j].cards, (player + i)->hand[j].numCards) <= 21);
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
a