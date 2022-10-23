// Games, Template Method example
// Mikhail Nesterenko
// 2/4/2014

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using std::cout; using std::endl; using std::vector; using std::cin;

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game{
public:
   Game():playersCount_(0), movesCount_(0), playerWon_(noWinner){}

   // template method
   void playGame(const int playersCount = 0) {
      playersCount_ = playersCount;
      movesCount_=0;

      initializeGame();

      for(int i=0; !endOfGame(); i = (i+1) % playersCount_ ){
	 makeMove(i);
	 if (i==playersCount_-1) 
	    ++movesCount_; 
      }
      printWinner();
   }

   virtual ~Game(){}

protected:
   // primitive operations
   virtual void initializeGame() = 0;
   virtual void makeMove(int player) = 0;
   virtual void printWinner() = 0;
   virtual bool endOfGame() { return playerWon_ != noWinner;} // this is a hook
                   // returns true if winner is decided
   static const int noWinner=-1;

   int playersCount_;
   int movesCount_;
   int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly: public Game {
public:  
   // implementing concrete methods
   void initializeGame(){
      playersCount_ = rand() % numPlayers_ + 1 ; // initialize players
   }

   void makeMove(int player) {
      if (movesCount_ > minMoves_){ 
	 const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
	 if (chance < movesCount_) playerWon_= player;
      }
   }

   void printWinner(){
      cout << "Monopoly, player "<< playerWon_<< " won in "
	   << movesCount_<< " moves." << endl;
   }

private:
   static const int numPlayers_ = 8; // max number of players
   static const int minMoves_ = 20; // nobody wins before minMoves_
   static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess: public Game {
public:
   void initializeGame(){
      playersCount_ = numPlayers_; // initalize players
      for(int i=0; i < numPlayers_; ++i) 
	     experience_[i] = rand() % maxExperience_ + 1 ; 
   }

   void makeMove(int player){
      if (movesCount_ > minMoves_){
	 const int chance = (rand() % maxMoves_) / experience_[player];
	 if (chance < movesCount_) playerWon_= player;
      }
   }

   void printWinner(){
      cout << "Chess, player " << playerWon_ 
	   << " with experience " << experience_[playerWon_]
	   << " won in "<< movesCount_ << " moves over"
	   << " player with experience " << experience_[playerWon_== 0 ? 1:0] 
           << endl;
   }

private:
   static const int numPlayers_ = 2;
   static const int minMoves_ = 2; // nobody wins before minMoves_
   static const int maxMoves_ = 100; // somebody wins before maxMoves_
   static const int maxExperience_ = 5; // player's experience
                              // the higher, the greater probability of winning
   int experience_[numPlayers_]; 
};

class Dice: public Game {
public:
	void initializeGame() {
		playersCount_ = numPlayers_; //initialize players
	}

	void makeMove(int player) {
		if (movesCount_ < maxMoves_ && (computerRollAgain || playerRollAgain)) { //play a round
			vector<int> rolls(5);
			generate(rolls.begin(), rolls.end(), [](){ return rand()%6+1; });
		       	if (player == 0) { //computer's turn
				cout << "Round " << movesCount_+1 << ": ";
				cout << "Computer rolled:";
				if (computerRollAgain) {
					int score = 0;	
					for (int r : rolls) {
						cout << " " << r;
						score += r;
					}
					cout << ", ";
					computerScore_ = score;
				} else {
					cout << " (passed), ";
				}
				cout << "computer's score = " << computerScore_ << endl;
				if (movesCount_ < maxMoves_ - 1) { //don't ask computer to roll again after last round
					if (rand()%2 == 0) computerRollAgain = true;
					else computerRollAgain = false;
				}
		       	} else if (player == 1) { //your turn
				cout << "You rolled:";
				if (playerRollAgain) {
					int score = 0;
					for (int r : rolls) {
						cout << " " << r;
						score += r;
					}
					cout << ", ";
					playerScore_ = score;
				} else {
					cout << " (passed), ";
				}
				cout << "your score = " << playerScore_ << endl;
				if (movesCount_ < maxMoves_ - 1) { //don't prompt to roll again after last round
					cout << "Roll again? [y/n] ";
					char letter;
					cin >> letter;
					if (letter == 'n') playerRollAgain = false;
					else playerRollAgain = true;
				}
			}
		} else { //end the game
			if (playerScore_ > computerScore_)
				playerWon_ = 1;
			else playerWon_ = 0;
		}
	}

	void printWinner() {
		if (playerWon_ == 0)
			cout << "You lose!";
		else cout << "You win!";
		cout << endl;
	}
private:
	static const int numPlayers_ = 2;
	static const int minMoves_ = 1;
	static const int maxMoves_ = 3;
	int playerScore_ = 0;
	int computerScore_ = 0;
	bool playerRollAgain = true;
	bool computerRollAgain = true;
};

int main() {
   srand(time(nullptr));

   Game* gp = nullptr;
	
   /*
   // play chess 10 times
   for (int i = 0; i < 10; ++i){ 
      gp = new Chess;
      gp->playGame(); 
      delete gp;
   }
      

   // play monopoly 8 times
   for (int i = 0; i < 8; ++i){
      gp = new Monopoly;
      gp->playGame(); 
      delete gp;
   }
   */

   // play dice
   gp = new Dice;
   gp->playGame();
   delete gp;
}
