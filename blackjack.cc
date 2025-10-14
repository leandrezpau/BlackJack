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

struct Card {
  int num;
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
  Card* cards;
  int TotalScore(Card* cards);
};

struct Player {
  Hand* hand;
};

struct Crupier {
  Deck deck;
  Hand hand;
};

/*
void InitCards(Deck& deck);
void Shuffle(Deck& deck);

int main(){
  srand(time(NULL));
  Deck deck;
  InitCards(deck);
  Shuffle(deck);
  for(int i = 0; i < Deck::kTotalCards; i++){
    printf("%d\n",deck.cards[i].num);
  }
  return 0;
}

void InitCards(Deck& deck){
  for(int e = 0; e < Deck::kNSuits; e++){
    for(int i = 0; i < Deck::kNCards; i++){

      deck.cards[i + e * Deck::kNCards].type = CardType(i);
      deck.cards[i + e * Deck::kNCards].revealed = false;
      
      switch(i){
        case 10:{
          deck.cards[i + e * Deck::kNCards].num = J;
          break;
        }
        case 11:{
          deck.cards[i + e * Deck::kNCards].num = Q;
          break;
        }
        case 12:{
          deck.cards[i + e * Deck::kNCards].num = K;
          break;
        }
        default:{
          deck.cards[i + e * Deck::kNCards].num = i + 1;
          break;
        }
      }
    }
  }
}
void Shuffle(Deck& deck){
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
*/