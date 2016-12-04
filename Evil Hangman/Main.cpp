
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

string secret(char guess,int guessesLeft);
void setUp(vector<string> words,int length,int difficulty);

int game(vector<string> list,int length,int difficulty)
{
   int guessesLeft;

   cout<<"Enter amount of guesses :";
   cin>>guessesLeft;

   while(cin.fail())
    {

        cout<<"Please enter a number"<<endl;
        cin.clear();
        cin.sync();
        cin>>guessesLeft;
    }
   //vector to store letters guessed
   vector<char> usedLetters = vector<char>();;

   int correct=0;
   string display,word;

    //display current state of game
   for(int i=0; i<length; i++)
   {
       display+='-';
   }


    setUp(list,length,difficulty);

    //while the user has not guessed the entire word keep looping unless guesses run out
   while(correct <length)
   {

       cout<<"Enter a letter "<<": ";

       char guess;
       cin>>guess;
       while(cin.fail())
       {
           cin.clear();
            cin.sync();
           cout<<"Please enter a letter"<<endl;
          cin>>guess;
       }
        //call method to make sure letter is lower case
       guess=tolower(guess);

        word=secret(guess,guessesLeft);

        //check if letter entered was already guessed if so return to start of loop and ask for input again
        if(find(usedLetters.begin(), usedLetters.end(), guess)!=usedLetters.end()){
          cout<<"Letter already entered "<<endl;
           continue;
       }

       bool correctGuess=false;
       //add guessed letter to vector
       usedLetters.push_back (guess);
       //for loop to check if the guessed letter is in the word
       for(int i=0; i<length; i++)
       {

           if(guess==word[i])
           {

               display[i] =word[i];
               correctGuess=true;

               correct++;
           }
       }
       if(!correctGuess)
       {

           guessesLeft--;
            //if user has ran out of guessed end game
           if(guessesLeft==0)
           {

               cout<<"You have ran out of guesses."<<endl;
               cout<<"The word was "<<word<<endl;
               return guessesLeft;
           }


           cout<<guess<<" : is not in the word."<<endl;

       }
       //organise the vector in alphabetical order
       sort( usedLetters.begin(), usedLetters.end() );
       cout<<"Letters already used :";
       //for loop to print out letters guesssed

       for(int i=0; i<usedLetters.size(); i++)
       {
           cout<<" "<<usedLetters[i];
       }
           cout<<endl;
           cout<<"Word : "<<display<<endl;
           cout<<"Guesses left "<<guessesLeft<< ": "<<endl;
       }

   cout<<"You got it! ,"<<display<<endl;
   return guessesLeft;
}


int main ()
{
   string word;


   ifstream dictionary("Dictionary.txt");

   //check if text file was read
  if(dictionary){
    cout<<"Enter word length :";
    int length,difficulty;;
    cin>>length;

    while(cin.fail())
    {

        cout<<"Please enter a number"<<endl;
        cin.clear();
        cin.sync();
        cin>>length;
    }

    cout<<"Pick difficulty level 1-3: ";
    cin>>difficulty;
    while(difficulty!=1 && difficulty>3 ||cin.fail())
    {
        cout<<"Please enter a number between 1-3"<<endl;
        cin.clear();
        cin.sync();
        cin>>difficulty;
    }

    vector<string> list = vector<string>();

  while (getline(dictionary, word))
   {
       if(word.size()==length)
       {
            list.push_back(word);
       }

   }

        //check if there is words of that length in the dictionary
       if(list.size()==0)
        {
            cout<<"There are no words of that length "<<endl;
            main();
        }
        else
        {

            game(list,length,difficulty);
        }
    }
    else
    {
        cout<<"No dictionary inputed"<<endl;
        main();
    }

}
