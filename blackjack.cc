#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "conio.h"

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
  kReplaying,
  kPlayersPlaying,
  kCrupierPlaying,
  kEnded
};
enum HandState {
  kHanding = 0,
  kEndedHanding,
  kWon,
  kLost,
  kTie
};

class Card {
public:

  char num;
  CardType type;
  bool revealed;
};

class Deck {
public:
  const static int kNSuits = 4;
  const static int kNCards = 13;
  const static int kTotalCards = kNSuits * kNCards;

  Card cards[kNSuits * kNCards];
};

class Hand {
public:
  int numCards;
  Card* cards;
  
  int gamble;

  HandState handstate;

  int TotalScore();
};

class Player {
public:
  int numHands;
  Hand* hand;

  int money;
};

class Crupier {
public:
  Deck deck;
  Hand hand;
};

void InitGame(Player** player, Crupier &crupier, int numPlayers);
void InitCards(Deck& deck);
void ShuffleCards(Deck& deck);
void PrepareGame(Player** player, Crupier &crupier, int numPlayers);

void GiveHand(Player* player, Crupier& crupier, int& currentCard, int numPlayers);

int PrintTotalCards(Hand hand);
void PrintfCardType(Card card);
void PrintGamble(Player* player, int whichHandIs);
void PrintCard(Card card);

void HitOrStand(Player* player, Crupier& crupier, int numPlayers, int& currentCard, GameState& gamestate);
void CrupierHitOrStand(Crupier& crupier, int& currentCard, GameState& gamestate);

bool StandHandWithAce(Hand hand);
bool CheckIfSplit(Player* player, Crupier& crupier, int& currentCard);
void GetBestScore(Hand& hand);

void WonOrLost(Player* player, int numPlayers, Crupier& crupier);
void ContinuePlaying(bool& end, GameState& gamestate);

void AddCard(Hand& hand, Crupier& crupier, int& currentCard);
void FreeMem(Player* player, int numPlayers, Crupier& crupier);

int main(int argc, char** argv){
  int numPlayers = 1;
  if(argc > 1){
    numPlayers = atoi(argv[1]);
    if(numPlayers > 6) numPlayers = 6;
  }

  Crupier crupier;
  Player* player;

  int currentCard = 0;

  GameState gamestate = GameState::kStarting;

  bool end = false;
  while(!end){
    switch(gamestate){
      case GameState::kStarting:{
        InitGame(&player, crupier, numPlayers);
        InitCards(crupier.deck);
        ShuffleCards(crupier.deck);

        GiveHand(player, crupier, currentCard, numPlayers);

        gamestate = GameState::kPlayersPlaying;
        break;
      }
      case GameState::kReplaying:{
        GiveHand(player, crupier, currentCard, numPlayers);
        gamestate = GameState::kPlayersPlaying;
        break;
      }
      case GameState::kPlayersPlaying:{
        HitOrStand(player, crupier, numPlayers, currentCard, gamestate);

        gamestate = GameState::kCrupierPlaying;
        break;
      }
      case GameState::kCrupierPlaying:{
        CrupierHitOrStand(crupier, currentCard, gamestate);
        break;
      }
      case GameState::kEnded:{
        WonOrLost(player, numPlayers, crupier);
        ContinuePlaying(end, gamestate);
        break;
      }
    }
  }
  FreeMem(player, numPlayers, crupier);
  return 0;
}

int Hand::TotalScore(){
  int score = 0;
  for (int i = 0; i < numCards; i++){
    switch (cards[i].num){
      case 'J':
      case 'Q':
      case 'K':
      case 'T':{
        score += 10;
        break;
      }
      case 'A':{
        score += 1;
        break;
      }
        
      case 11:{
        score += 11;
      }
      default:{
        score += atoi(&cards[i].num);
        break;
      }
    }
  }
  return score;
}

void InitGame(Player** player, Crupier &crupier, int numPlayers){
  srand(time(NULL));
  system("chcp 65001");
  system("cls");


  *player = (Player*) malloc(sizeof(Player) * numPlayers);
  for(int i = 0; i < numPlayers; i++){
    (*(player) + i)->numHands = 1;
    (*(player) + i)->hand = (Hand*) malloc(sizeof(Hand) * (*(player) + i)->numHands);
    (*(player) + i)->hand->handstate = HandState::kHanding;
    (*(player) + i)->hand->gamble = 0;


    (*(player) + i)->hand->numCards = 2;
    (*(player) + i)->hand->cards = (Card*) malloc(sizeof(Card) * 2);
    
    (*(player) + i)->money = 1000;
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
    shuffled[i] = true;
    //Swapping both cards
    Card card_1 = deck.cards[i];
    Card card_2 = deck.cards[number + suits * Deck::kNCards];

    deck.cards[i] = card_2;
    deck.cards[number + suits * Deck::kNCards] = card_1; 
  }
}

void GiveHand(Player* player, Crupier& crupier, int& currentCard, int numPlayers){
  currentCard = 0;
  ShuffleCards(crupier.deck);

  crupier.hand.numCards = 2;
  crupier.hand.cards[0] = crupier.deck.cards[currentCard++];
  crupier.hand.cards[1] = crupier.deck.cards[currentCard++];
  

  for(int i = 0; i < numPlayers; i++){
    (player + i)->numHands = 1;
    (player + i)->hand->handstate = HandState::kHanding;

    (player + i)->hand->gamble = ((player + i)->money / 20) * (rand() % 10 + 1);
    (player + i)->money -= (player + i)->hand->gamble;

    (player + i)->hand->cards[0] = crupier.deck.cards[currentCard++];
    (player + i)->hand->cards[1] = crupier.deck.cards[currentCard++];
    (player + i)->hand->numCards = 2;
  }
}

int PrintTotalCards(Hand hand){
  int cardAnumber = 0;
  for(int e = 0; e < hand.numCards; e++){
    if(hand.cards[e].num == 'T'){
      printf("[10");
      PrintfCardType(hand.cards[e]);
    }else{
      printf("[%c", hand.cards[e].num);
      PrintfCardType(hand.cards[e]);
    }
    
    if(hand.cards[e].num == 'A' && cardAnumber == 0) cardAnumber++;
  }

  printf("   -------    Numero total: <<");
  printf("%d", hand.TotalScore());
  if(cardAnumber > 0){
    printf(" / ");
    printf("%d", hand.TotalScore() + 10);
  }
  printf(">>");
  return cardAnumber;
}

void PrintCard(Card card){
  if(card.num == 'T'){
    printf("[10");
    PrintfCardType(card);
  }else{
    printf("[%c", card.num);
    PrintfCardType(card);
  }
}

void PrintfCardType(Card card){
  switch(card.type){
    case CardType::kClub_Type:{
      printf(u8"\u2663]");    // ♣
      break;
    }
    case CardType::kDiamond_Type:{
      printf(u8"\u2666]"); // ♦
      break;
    }
    case CardType::kHeart_Type:{
      printf(u8"\u2665]");   // ♥
      break;
    }
    case CardType::kSpades_Type:{
      printf(u8"\u2660]");   // ♠
      break;
    }
  }
}

bool StandHandWithAce(Hand hand){
  int cardAnumber = 0;
  for(int e = 0; e < hand.numCards; e++){
    if(hand.cards[e].num == 'A' && cardAnumber == 0) cardAnumber++;
  }
  for(int p = 0; p < cardAnumber + 1; p++){
    if(hand.TotalScore() + p * 10 >= 17 && hand.TotalScore() + p * 10 <= 21){
      return true;
    }
  }

  return false;
}

void GetBestScore(Hand& hand){
  int cardAnumber = 0;
  for(int e = 0; e < hand.numCards; e++){
    if(hand.cards[e].num == 'A' && cardAnumber == 0) cardAnumber++;
  }
  if(cardAnumber > 0){
    int ScoreWithA = 21 - (hand.TotalScore() + 10);
    int ScoreWithoutA = 21 - hand.TotalScore();

    if(ScoreWithA <= 21){
      ScoreWithA = sqrt(ScoreWithA * ScoreWithA);
      ScoreWithoutA = sqrt(ScoreWithoutA * ScoreWithoutA);

      if(ScoreWithA < ScoreWithoutA){
        int i = 0;
        do{
          if(hand.cards[i].num == 'A'){
            hand.cards[i].num = 11;
          }
        }while(hand.cards[i++].num != 'A' && i <= hand.numCards);
      }
    }
  } 
}

bool CheckIfSplit(Player* player, Crupier& crupier, int& currentCard){
  if(player->hand->cards[0].num == player->hand->cards[1].num && player->numHands < 2 && (player->hand->gamble * 2) <= player->money){
    printf("\nAPUESTA en Mano numero {1}: || %d$ ||", player->hand->gamble);
    printf("\nMano numero {%d}: ", 1);
    PrintTotalCards((player)->hand[0]);
    printf("\nSplit");

    player->numHands++;
    player->hand = (Hand*) realloc(player->hand, sizeof(Hand) * player->numHands);
    player->hand[1].numCards = 2;
    player->hand[1].handstate = HandState::kHanding;

    player->hand[1].gamble = player->hand[0].gamble;
    player->money -= player->hand[1].gamble;

    player->hand[1].cards = (Card*) malloc(sizeof(Card) * player->hand[1].numCards);
    player->hand[1].cards[0] = player->hand->cards[1];
    player->hand[1].cards[1] = crupier.deck.cards[currentCard++];

    player->hand[0].cards[1] = crupier.deck.cards[currentCard++];
    return true;
  }
  return false;
}

void PrintGamble(Player* player, int whichHandIs){
  switch((player)->numHands){
    case 1:{
      printf("\nAPUESTA en Mano numero {%d}: || %d$ ||", whichHandIs + 1, player->hand->gamble);
      break;
    }
    case 2:{
      if(whichHandIs == 1){
        printf("\nAPUESTA en Mano numero {%d}: || %d$ ||", whichHandIs + 1, player->hand->gamble);
      }
      break;
    }
    
  }
}

void AddCard(Hand& hand, Crupier& crupier, int& currentCard){
  hand.numCards++;
  hand.cards = (Card*) realloc(hand.cards, sizeof(Card) * hand.numCards);

  hand.cards[hand.numCards - 1] = crupier.deck.cards[currentCard++];
}
//Function that makes all AI Player interactions...
void HitOrStand(Player* player, Crupier& crupier, int numPlayers, int& currentCard, GameState& gamestate){
  //First it prints first crupier card
  printf(" --------------  Crupier  --------------");
  printf("\nMano de crupier: ");
  PrintCard(*crupier.hand.cards);;
  printf("[Hidden]\n\n");

  //Then for every player checks if player can split. If he can, hand splits and he gets 2 hands with old hand 2 first cards
  for(int i = 0; i < numPlayers; i++){
    printf("\n  ------------- Jugador %d - Balance %d$ -------------", i + 1, (player + i)->money + (player + i)->hand->gamble);
    //Chekking if player can split
    CheckIfSplit((player + i), crupier, currentCard);
    //For every player's hand
    for(int j = 0; j < (player + i)->numHands; j++){
      //If player is still handing
      if((player + i)->hand->handstate == HandState::kHanding){
        //Print player info
        PrintGamble((player + i), j);
        printf("\nMano numero {%d}: ", j + 1);
        PrintTotalCards((player + i)->hand[j]);

        //Here Hit or Stand (or Double) loop is initiated
        int hitorstand = 0;
        //While variable != 2 or player score is under 17, he will continue to get cards
        while(hitorstand != 2 && (player + i)->hand[j].TotalScore() < 17){
          //This function checks every loop if player has 'A' and that score can get him to 17
          if(StandHandWithAce((player + i)->hand[j])){
            hitorstand = 2;
            (player + i)->hand->handstate = HandState::kEndedHanding;
          }else{
            //This part checks best cases to split gamble, if score is under 11 and crupier first card is under 6. 
            //Also checks if player has enought money to double gamble
            if((player + i)->hand[j].TotalScore() <= 11 && atoi(&crupier.hand.cards->num) < 6 && (player + i)->money >= (player + i)->hand[j].gamble){
              //This part adds cards to player hand directly from the crupier's deck
              AddCard((player + i)->hand[j], crupier, currentCard);

              //Adding double money to that hand
              (player + i)->money -= (player + i)->hand[j].gamble;
              (player + i)->hand[j].gamble *= 2;

              //Printing info
              printf("\nDouble: ");
              PrintCard((player + i)->hand[j].cards[(player + i)->hand[j].numCards - 1]);
              printf("\nMano numero {%d}: ", j + 1);
              PrintTotalCards((player + i)->hand[j]);
            }else if((player + i)->hand[j].TotalScore() < 17){  //If player score is under 17 he will always Hit
              //This part adds cards to player hand directly from the crupier's deck
              AddCard((player + i)->hand[j], crupier, currentCard);

              //Printing info
              printf("\nHit: ");
              PrintCard((player + i)->hand[j].cards[(player + i)->hand[j].numCards - 1]);
              printf("\nMano numero {%d}: ", j + 1);
              PrintTotalCards((player + i)->hand[j]);
            }else{
              //If player score is over 17 his hand ends gambling
              hitorstand = 2;
              (player + i)->hand->handstate = HandState::kEndedHanding;
            }
          }
        }
        //This function works if player has A and gets the better score to him to win
        GetBestScore((player + i)->hand[j]);
        printf("\nStand [<< %d >>]\n", (player + i)->hand[j].TotalScore());
      }
    }
  }
}
//Function that calcs crupier interactions
void CrupierHitOrStand(Crupier& crupier, int& currentCard, GameState& gamestate){
  //Printing crupier info
  printf("\n\n  --------------  Crupier  --------------");
  printf("\nMano de crupier: ");
  PrintTotalCards(crupier.hand);
  //Starting Hit or Stand loop on crupier, until he has 17
  int crupierHitOrStand = 0;
  while(crupierHitOrStand != 2 && crupier.hand.TotalScore() < 17){
    //If crupier gets to 17 with an A loop stops
    if(StandHandWithAce(crupier.hand)){
      crupierHitOrStand = 2;
      printf("\nStand");
    }else{
      //While his hand is under 17 he will always play Hit
      if(crupier.hand.TotalScore() < 17){
        //This function add another card to hand
        AddCard(crupier.hand, crupier, currentCard);

        printf("\nHit: ");
        PrintCard(crupier.hand.cards[crupier.hand.numCards - 1]);

        printf("\nMano de crupier: ");
        PrintTotalCards(crupier.hand);
      }else{
        crupierHitOrStand = 2;
      }
    }
  }
  GetBestScore(crupier.hand);
  printf("\nStand [<< %d >>]", crupier.hand.TotalScore());
  gamestate = GameState::kEnded;
} 
void WonOrLost(Player* player, int numPlayers, Crupier& crupier){
  printf("\n\n\n");
  for(int i = 0; i < numPlayers; i++){
    for(int j = 0; j < (player + i)->numHands; j++){
      if(crupier.hand.TotalScore() <= 21){
        if((player + i)->hand[j].TotalScore() > crupier.hand.TotalScore() && (player + i)->hand[j].TotalScore() <= 21){
          (player + i)->hand[j].handstate = HandState::kWon;

          (player + i)->money += ((player + i)->hand[j].gamble * 2);
          printf("\nJugador %d con mano %d ha << GANADO >> - <<Balance : %d>>", i + 1, j + 1, (player + i)->money);
        }else if((player + i)->hand[j].TotalScore() == crupier.hand.TotalScore()){
          (player + i)->hand[j].handstate = HandState::kTie;
          (player + i)->money += ((player + i)->hand[j].gamble);
          printf("\nJugador %d con mano %d ha << EMPATADO >> - <<Balance : %d>>", i + 1, j + 1, (player + i)->money);
        }else{
          (player + i)->hand[j].handstate = HandState::kLost;
          printf("\nJugador %d con mano %d ha << PERDIDO >> - <<Balance : %d>>", i + 1, j + 1, (player + i)->money);
        }
      }else{
        if((player + i)->hand[j].TotalScore() <= 21){
          (player + i)->hand[j].handstate = HandState::kWon;
          (player + i)->money += ((player + i)->hand[j].gamble * 2);
          printf("\nJugador %d con mano %d ha << GANADO >> - <<Balance : %d>>", i + 1, j + 1, (player + i)->money);
        }else{
          (player + i)->hand[j].handstate = HandState::kLost;
          printf("\nJugador %d con mano %d ha << PERDIDO >> - <<Balance : %d>>", i + 1, j + 1, (player + i)->money);
        }
      }
    }
    if((player + i)->money < 20){
      (player + i)->money += (500 - (player + i)->money);
      printf("\nJugador %d sin saldo, se te han añadido 500$ de compensacion <<Balance : %d>>", i + 1, (player + i)->money);
    }
  }
}

void ContinuePlaying(bool& end, GameState& gamestate){
  printf("\n\n\nContinue Gambling?");
  printf("\nTo CONTINUE press 'Space'");
  printf("\nTo END press 'Enter'");

  char keepgambling = getch();
  switch(keepgambling){
    case ' ':{
      gamestate = GameState::kReplaying;
      system("cls");
      break;
    }
    case '\r':{
      end = true;
      break;
    }
  }
}

void FreeMem(Player* player, int numPlayers, Crupier& crupier){
  for(int i = 0; i < numPlayers; i++){
    for(int e = 0; e < (player + i)->numHands; e++){
      free(((player + i)->hand + e)->cards);
    }
    free((player + i)->hand);
  }
  free(player);

  free(crupier.hand.cards);

  printf("\n\n\n\n\n\nEnded Correctly");
  
}