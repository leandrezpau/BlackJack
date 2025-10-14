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
  bool shuffled;
};

struct Deck {
  const static int kNSuits = 4;
  const static int kNCards = 13;
  const static int KTotalCards = kNSuits * kNCards;

  Card cards[kNSuits][kNCards];
  Card shuffled_Deck[kNSuits * kNCards];
};

void InitCards(Deck& deck);
void Shuffle(Deck& deck);

int main(){
  Deck deck;
  srand(time(NULL));
  
  InitCards(deck);
  Shuffle(deck);
  
}

void InitCards(Deck& deck){
  for(int i = 0; i < Deck::kNSuits; i++){
    for(int e = 0; e < Deck::kNCards; e++){

      deck.cards[i][e].type = CardType(i);
      deck.cards[i][e].shuffled = false;
      
      switch(e){
        case 10:{
          deck.cards[i][e].num = J;
          break;
        }
        case 11:{
          deck.cards[i][e].num = Q;
          break;
        }
        case 12:{
          deck.cards[i][e].num = K;
          break;
        }
        default:{
          deck.cards[i][e].num = e + 1;
          break;
        }
      }
    }
  }
}

void Shuffle(Deck& deck){
  for(int i = 0; i < Deck::KTotalCards; i++){
    int suits, number;
    do{
      suits = rand() % Deck::kNSuits;
      number = rand() % Deck::kNCards;
    }while(deck.cards[suits][number].shuffled == true);

    deck.shuffled_Deck[i] = deck.cards[suits][number];
    deck.cards[suits][number].shuffled = true;
  }
}