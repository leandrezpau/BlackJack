#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

enum CardType {
  kHeart_Type = 0,
  kDiamond_Type,
  kClub_Type,
  kSpades_Type
};

struct Card {
  char num;
  CardType type;
}

struct Deck {
  const static int kNSuits = 4;
  const static int kNCards = 13;

  Card cards[kNSuits][kNCards];
  Card shuffled_Deck[kNSuits * kNCards];
}

void InitCards(Deck& deck);
void Shuffle(Deck& deck);

int main(){
  Deck deck;
  
  InitCards(deck);
  Shuffle(deck);
}

void InitCards(Deck& deck){
  for(int i = 0; i < Deck::kNSuits; i++){
    for(int e = 0; e < Deck::kNCards; e++){
      deck.cards[i][e].num = itoa(e);
      deck.cards[i][e].type = CardType(i);
      switch(deck.cards[i][e].num){
        case 0:{
          deck.cards[i][e].num = 'A';
          break;
        }
        case 10:{
          deck.cards[i][e].num = 'J';
          break;
        }
        case 11:{
          deck.cards[i][e].num = 'Q';
          break;
        }
        case 12:{
          deck.cards[i][e].num = 'K';
          break;
        }
      }
    }
  }
  for(int i = 0; i < Deck::kNCards * Deck::kNSuits){
    deck.shuffled_Deck[i * e]
  }
}

void Shuffle(Deck& deck){
  for(int i = 0; i < Deck::kNCards * Deck::kNSuits){
    
  }
}