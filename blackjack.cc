#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define A 1
#define J 10
#define Q 10
#define K 10
#define T 10

enum CardType {
  kHeart_Type = 0,
  kDiamond_Type,
  kClub_Type,
  kSpades_Type
};
enum GameState {
  kStarting = 0,
  kPlayersPlaying,
  kCrupierPlaying,
  kEnded
};
enum HandState {
  kHanding = 0,
  kEndedHanding,
  kWon,
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

  HandState handstate;
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

bool CalcHandWithAce(Hand hand);
int PrintTotalCards(Hand hand);

void HitOrStand(Player* player, Crupier& crupier, int numPlayers, int& currentCard, GameState& gamestate);
void CrupierHitOrStand(Crupier& crupier, int& currentCard, GameState& gamestate);

HandState WonOrLost(Player* player, int numPlayers);

void FreeMem(Player* player, int numPlayers);

int main(int argc, char** argv){
  srand(time(NULL));
  system("cls");
  int numPlayers = argc;
  if(argc == 0) numPlayers = 1;

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

        gamestate = GameState::kPlayersPlaying;
        break;
      }
      case GameState::kPlayersPlaying:{
        static int currentCard = 0;
        HitOrStand(player, crupier, numPlayers, currentCard, gamestate);

        gamestate = GameState::kCrupierPlaying;
        break;
      }
      case GameState::kCrupierPlaying:{
        CrupierHitOrStand(crupier, currentCard, gamestate);
        end = true;
      }
      case GameState::kEnded:{
        
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
      case 'T':{
        score += 10;
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
    (*(player) + i)->hand->handstate = HandState::kHanding;
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
        case 9:{
          deck.cards[i + e * Deck::kNCards].num = 'T';
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

int PrintTotalCards(Hand hand){
  int cardAnumber = 0;
  for(int e = 0; e < hand.numCards; e++){
    if(hand.cards[e].num == 'T'){
      printf("10, ");
    }else{
      printf("%c, ", hand.cards[e].num);
    }
    
    if(hand.cards[e].num == 'A' && cardAnumber == 0) cardAnumber++;
  }

  printf("\nNumero total: <<");
  printf("%d", hand.TotalScore(hand.cards, hand.numCards));
  if(cardAnumber > 0){
    printf(" / ");
    printf("%d", hand.TotalScore(hand.cards, hand.numCards) + 10);
  }
  printf(">>");
  return cardAnumber;
}

bool CalcHandWithAce(Hand hand){
  int cardAnumber = 0;
  for(int e = 0; e < hand.numCards; e++){
    if(hand.cards[e].num == 'A' && cardAnumber == 0) cardAnumber++;
  }
  for(int p = 0; p < cardAnumber + 1; p++){
    if(hand.TotalScore(hand.cards, hand.numCards) + p * 10 >= 17){
      return true;
    }
  }

  return false;
}
void HitOrStand(Player* player, Crupier& crupier, int numPlayers, int& currentCard, GameState& gamestate){
  int everyPlayerEnded = 0;
  for(int i = 0; i < numPlayers; i++){
    for(int j = 0; j < (player + i)->numHands; j++){
      if((player + i)->hand->handstate == HandState::kHanding){
        printf("\n\nMano numero %d: ", j + 1);
        PrintTotalCards((player + i)->hand[j]);
        int hitorstand = 0;
        while(hitorstand != 2 && (player + i)->hand[j].TotalScore((player + i)->hand[j].cards, (player + i)->hand[j].numCards) < 17){
          printf("\nHit or Stand: ");
          if(CalcHandWithAce((player + i)->hand[j])){
            hitorstand = 2;
            (player + i)->hand->handstate = HandState::kEndedHanding;
            printf("\nStand");
          }else{
            if((player + i)->hand[j].TotalScore((player + i)->hand[j].cards, (player + i)->hand[j].numCards) < 17){
              (player + i)->hand[j].numCards++;
              (player + i)->hand[j].cards = (Card*) realloc((player + i)->hand[j].cards, sizeof(Card) * (player + i)->hand[j].numCards);

              (player + i)->hand[j].cards[(player + i)->hand[j].numCards - 1] = crupier.deck.cards[currentCard++];
              printf("\nHit: %c", (player + i)->hand[j].cards[(player + i)->hand[j].numCards - 1].num);

              printf("\nMano numero %d: ", j + 1);
              PrintTotalCards((player + i)->hand[j]);
            }else{
              printf("\nStand");
              hitorstand = 2;
              (player + i)->hand->handstate = HandState::kEndedHanding;
            }
          }
        }
      }
    }
  }
}
void CrupierHitOrStand(Crupier& crupier, int& currentCard, GameState& gamestate){
  //CRUPIER HIT OR STAND
  printf("\n\nMano de crupier: ");
  PrintTotalCards(crupier.hand);
  int crupierHitOrStand = 0;
  while(crupierHitOrStand != 2 && crupier.hand.TotalScore(crupier.hand.cards, crupier.hand.numCards) < 17){
    printf("\nHit or Stand: ");
    if(CalcHandWithAce(crupier.hand)){
      crupierHitOrStand = 2;
      printf("\nStand");
    }else{
      if(crupier.hand.TotalScore(crupier.hand.cards, crupier.hand.numCards) < 17){
        crupier.hand.numCards++;
        crupier.hand.cards = (Card*) realloc(crupier.hand.cards, sizeof(Card) * crupier.hand.numCards);

        crupier.hand.cards[crupier.hand.numCards - 1] = crupier.deck.cards[currentCard++];
        printf("\nHit: %c", crupier.hand.cards[crupier.hand.numCards - 1].num);

        printf("\n\nMano de crupier: ");
        PrintTotalCards(crupier.hand);
      }else{
        printf("\nStand");
        crupierHitOrStand = 2;
      }
    }
  }
} 
HandState WonOrLost(Player* player, int numPlayers){
  for(int i = 0; i < numPlayers; i++){
    for(int j = 0; j < (player + i)->numHands; j++){
    }
  }
  return HandState::kEndedHanding;
}
void FreeMem(Player* player, int numPlayers){
  for(int i = 0; i < numPlayers; i++){
    for(int e = 0; e < (player + i)->numHands; e++){
      free(((player + i)->hand + e)->cards);
    }
    free((player + i)->hand);
  }
  free(player);
  printf("\nMemoria Liberada");
}