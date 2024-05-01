#include "hangman.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set> 

using namespace std;

Hangman::Hangman(){
    //initializes all private variables to 0 or empty
    attempts = gamesWon  = gamesLost = points = hintsUsed = perfectGames = 0;
    word = category = "";
    difficulty = ' ';
}

/* Apart from setting the difficulty this function also sets the attempts and fills the word bank based on the difficulty chosen */
void Hangman::setDifficulty( string d ){
   difficulty = d;
   if( difficulty == "E" ){
      attempts = 7;
      fillBank( "animalsEasy.txt", "Animals" );
      fillBank( "foodAndDrinksEasy.txt", "Food & Drinks" );
      fillBank( "gamesEasy.txt", "Games" );

   }
   else if( difficulty == "M" ){
      attempts = 6;
      fillBank( "animalsMedium.txt", "Animals" );
      fillBank( "foodAndDrinksMedium.txt", "Food & Drinks" );
      fillBank( "gamesMedium.txt", "Games" );

   }
   else if(difficulty == "H"){
      attempts = 5;
      fillBank( "animalsHard.txt", "Animals" );
      fillBank( "foodAndDrinksHard.txt", "Food & Drinks" );
      fillBank( "gamesHard.txt", "Games" );
   }else{
      cout << "invalid input, retry\n";
   }
}

/* This function will open the file and insert each word in that file into the unordered map under the correct key */
void Hangman::fillBank( const string filename, const string category ){

   fstream in;
   string words;
   in.open( filename );

   if( in.fail() ){
      cerr << "Could not open file." << endl;
      exit(1);
   }

   while( getline( in, words ) ){
      wordBank[category].push_back(words);
   }

   in.close();

}

/* This function will randomly choose one of the 3 categories */
string Hangman::chooseCat(){
   vector<string> tmpCat { "Animals", "Food & Drinks", "Games" };
   int i;
   srand( time( NULL ) );

   i = rand() % tmpCat.size(); // just chooses a category from the tmp vector above

   return tmpCat[i];
}

/* Using the category chosen in chooseCat(), this function will randomly choose a word from that category and pop it from the vector so it isnt chosen again */
string Hangman::chooseWord( string c ){

   vector<string>& words = wordBank[c]; //temporary vector to help with returning the random word

   int i = rand() % words.size(); // this will randomly choose a number, we will use that to access the position of the random word

   string randomWord = words[i]; // storing it in a variable so we can remove it from the wordBank without losing the current word

   words.erase(words.begin()+i); // this will go to the position randomly chosen and erase the word

   return randomWord;
}

/*
 displayMan - Will print out the current art of the man given the number of attempts
   Parameters: the current number of attempts
   Returns: Nothing
*/
void Hangman::displayMan( int n ){

   if( difficulty == "E" ){
      vector<std::string> HANGMANPICS = {
         "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
         "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",

      };
      cout << HANGMANPICS[n] << endl;
   }
   else if( difficulty == "M" ){
      vector<std::string> HANGMANPICS = {
         "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
          "  +---+\n  |   |\n      |\n      |\n      |\n      |\n========="
      };
      cout << HANGMANPICS[n] << endl;

   }
   else{
      vector<std::string> HANGMANPICS = {
         "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
         "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
         "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
      };
      cout << HANGMANPICS[n] << endl;
   }
}

void Hangman::startGame(){
   //intro message
   cout << "=======================\n" << "| Welcome to Hangman! |\n" << "=======================" << endl;

   string guess; 
   bool gameOver;
   int option = 0;
   string difficulty, category, wordGuess;
   //Menu call

   while(option != 4){

      gameOver = false;
      option = menu();
      
      switch(option){
         case 1: 
            //first, the user has to choose a difficulty
            cout << "Choose Difficulty (E | M | H):\n";
            cin >> difficulty;
            setDifficulty(difficulty);

            //chooses word and category based on difficulty
            category = chooseCat();
            
            word = chooseWord(category);

            cout << word << endl;

            cout << word.size() << " " << endl;


            //this for loop makes the string that holds the guesses the same size as the actually word string, including spaces
            for(int i = 0; i < word.size(); i++){
               if(word[i] != ' '){
                  wordGuess.push_back('_');
               }else{
                  wordGuess.push_back(' ');
               }
            }

            while(gameOver == false){
               //ends game if too many wrong attempts have been made
               if(attempts == 0){
                  gamesLost++;
                  cout << "You're out of attempts. Better luck next time!" << endl;
                  gameOver = true;
                  break;
               } 

               //displays the man every time the user guesses the letter, regardless of the guess being right or wrong
               if(gameOver != true) displayMan(attempts-1);

               //user input to get the guess
               cout << "Guess a letter: ";
               cin >> guess;

               attempts--;

               //function call to checkWord to see if the guess was correct

               //this checks for if the player has won the hangman game
               if(wordGuess == word){
                  gamesWon++;
                  cout << "\nCongratulations, you won with " << attempts << " remaining. Give yourself a pat on the back!\n";
                  gameOver == true;
               }
            }

            cout << endl << word << endl << wordGuess << endl;

            break;
         case 2: 
            cout << "STATS\n";
            break;
         case 3: 
            cout << "TUTORIAL\n";
            break;
         case 4: 
            cout << "Thank You For Playing!" << endl << "Here are your session stats:\n";
            printStats();
            return;
      }

      wordGuess.clear();
   }
}

//displays menu and gets the choice of the player
int Hangman::menu(){
   int choice;
   cout << "1 | Play Game \n2 | Session Stats \n3 | Game Tutorial \n4 | Quit \nYour Pick: ";
   cin >> choice;
   
   return choice; 
}

void Hangman::hints(){

}

void Hangman::printStats(){

}

void Hangman::checkWord(string &guessedWord, char guess){



   for(int i = 0; i < word.size(); i++ ){
      if(word[i] == guess){
         guessedWord[i] = word[i];
          
      }
   }

}