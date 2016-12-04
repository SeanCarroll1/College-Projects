#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <map>


using namespace std;

string wordFamilies(string word, char letter);
map<string, vector<string> > groups;
vector<string> list;
char guess;
int guessesLeft,difficulty;
string check;

void setUp(vector<string> words,int length,int diff)
{

    difficulty=diff;

    if(difficulty==1)
    {
        cout<<"Difficulty level : Easy"<<endl;

    }
    else if(difficulty==2)
    {
        cout<<"Difficulty level : Medium"<<endl;
    }else if(difficulty==3)
    {
        cout<<"Difficulty level : Hard"<<endl;
    }else
    {
        cout<<"Difficulty level : Very Hard"<<endl;
    }


    list=words;
    for(int i=0; i<length; i++)
    {
        check+='-';

    }
}

void createGroups(){


	for(int i = 0; i < list.size(); i++){
		groups[wordFamilies(list.at(i), guess)].push_back(list.at(i));
	}

}


void pickGroups(){
	map<string, vector<string> >::iterator pos = groups.begin();
    map<string, vector<string> >::iterator search = groups.find(check);
    //use pointer
	vector<string> *largestGroup=&(pos->second);

        if(difficulty==1)
        {
            //for this difficulty the smallist list will always be picked to make it easier
            for(pos = groups.begin(); pos != groups.end(); pos++){


                        if(pos->second.size() < largestGroup->size()){

                            largestGroup=&(pos->second);
                            }

                    }
        }else if(difficulty==2)
        {

            if(guessesLeft==1)
            {
                  //if one guess left pick group with all unkown letters so user has no chance to get one right
                if ( search != groups.end() )
                {
                     largestGroup=&(search->second);
                }
            }else
            {
                if(search!=groups.end())
                    {
                        if(search->second.size()>largestGroup->size()/.50)
                        {
                            largestGroup=&(search->second);
                        }
                    }else{
                         for(pos = groups.begin(); pos != groups.end(); pos++)
                         {

                                    if(pos->second.size() >largestGroup->size()){
                                        largestGroup=&(pos->second);
                                 }
                        }
                    }

            }
        }else
        {   //hardest difficulty
            if(search!=groups.end())
            {
                if(search->second.size()>guessesLeft)
                {
                    largestGroup=&(search->second);
                }
            }else
            {
                for(pos = groups.begin(); pos != groups.end(); pos++)
                     {

                                if(pos->second.size() >largestGroup->size()){
                                    largestGroup=&(pos->second);
                             }
                    }
            }
        }

	list.clear();
    list = *largestGroup;
	cout<<"words remaining :"<<list.size()<<endl;

	groups.clear();
}


string wordFamilies(string word, char letter){
	string temp = "";

	for(int i = 0; i < word.length(); i++){

        if(word.at(i)==letter)
        {
            temp+= word.at(i);
        }
        else{
            temp+='-';
        }


	}

	return temp;
}

string secret(char userGuess,int guessesLeft)
{
    guess=userGuess;
    createGroups();
    pickGroups();
    guessesLeft=guessesLeft;
    return  list[0];

}

