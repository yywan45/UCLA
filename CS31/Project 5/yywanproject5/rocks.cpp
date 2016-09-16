//
//  rocks.cpp
//  project5
//
//  Created by Yuen Yee Wan on 11/9/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <cctype>
#include "utilities.h"
using namespace std;


bool probeWord4to6lower(const char pw[MAXWORDLEN+1])
// check if probe word is valid length and only lower case
{
    if (strlen(pw) < 4 || strlen(pw) > 6)
        return false;
    
    for (int k = 0; k < strlen(pw); k++)
    {
        if (!islower(pw[k]))
            return false;
    }
    
    return true;
    
}

bool wordPresent(const char pw[MAXWORDLEN+1], const char words[][MAXWORDLEN+1], int nWords)
// check if probe word is present in word list
{
    for (int k = 0; k < nWords; k++)
    {
        if (strcmp(words[k], pw) == 0)
            return true;
    }
    
    return false;
}


int manageOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    if (nWords <  0 || wordnum < 0 || wordnum >= nWords)
    {
        return -1;
    }
    
    
    char probeWord[98];
    cout << "Probe word: ";
    cin.getline(probeWord, 100);
    
    int score = 1; // start score at 1 because if first probe word entered is true then score is 1
    
    while (strcmp(words[wordnum], probeWord) != 0)
    {
        if (!probeWord4to6lower(probeWord))
            cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
        else if (!wordPresent(probeWord, words, nWords))
                 cout << "I don't know that word." << endl;
        else
        {
            score++; // increase score with each valid probe word input
            
            int rock = 0;
            int pebble = 0;
            
            for (int k = 0; k < strlen(words[wordnum]) && k < strlen(probeWord); k++)
                // compare characters one by one in each c string and increase rock if characters are the same
                // only compare until loop reaches the end of the shorter word
            {
                if (words[wordnum][k] == probeWord[k])
                    rock++;
            }
            
            for (int k = 0; k < strlen(words[wordnum]); k++)
                // go through each letter in secret word
            {
                int secretLetter = 0;
                int probeLetter = 0;
                bool multipleSecretLetter = false;
                
                for (int i = k - 1; i >= 0; i--)
                    // compare character k in secret word to characters before k
                    // if there is the same character appearing before position k, return true
                {
                    if (words[wordnum][k] == words[wordnum][i])
                        multipleSecretLetter = true;
                }
                
                if (!multipleSecretLetter)
                    // if character k is appearing for the first time in the word
                    // prevents repeating the counting of pebbles if letters appear multiple times in secret word
                {
                    for (int a = 0; a < strlen(words[wordnum]); a++)
                        // count how many same letters as character k appear in secret word
                    {
                        if (words[wordnum][k] == words[wordnum][a])
                            secretLetter++;
                    }
                    
                    for (int b = 0; b < strlen(probeWord); b++)
                        // count how many same letters as character k appear in probe word
                    {
                        if (words[wordnum][k] == probeWord[b])
                            probeLetter++;
                    }
                    
                    // the lesser number between probeLetter and secretLetter count is added to pebbles
                    // if both equal, add value to pebbles
                    // at this point, pebble count includes rocks as well
                    if (secretLetter > probeLetter)
                        pebble += probeLetter;
                    if (secretLetter < probeLetter)
                        pebble += secretLetter;
                    if (secretLetter == probeLetter)
                        pebble += secretLetter;

                }
            }

            pebble -= rock; // deduct pebbles that are rocks from pebble count
            
            cout << "Rocks: " << rock << ", Pebbles: " << pebble << endl;
        }
        cout << "Probe word: ";
        cin.getline(probeWord, 100);
        
    }
    
    return score;
 
}

int main()
{
    
    const int MAXWORDS = 9000;
    char wordList[MAXWORDS][MAXWORDLEN+1];
    int nWords = loadWords(wordList, 10000);
    
    
    // Along with the array, your main routine must declare an int that will contain the actual number of words in the array (i.e., elements 0 through one less than that number are the elements that contain the C strings of interest). The number may well be smaller than the declared size of the array, because for test purposes you may not want to fill the entire array.
    
    
    if (nWords < 1)
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 0;
    }
    
   
    int rounds;
    cout << "How many rounds do you want to play? ";
    cin >> rounds;
    cin.ignore(10000, '\n');
    
    if (rounds <= 0)
    {
        cout << "The number of rounds must be positive." << endl;
        return 0;
    }
    
    cout << endl;

    for (int k = 0; k < rounds; k++)
    {
        cout << "Round " << k + 1 << endl;
        int wordnum = randInt(0, nWords-1);
        cout << "The secret word is " << strlen(wordList[wordnum]) << " letters long." << endl;
        cerr << "The secret word is " << wordList[wordnum] << endl;
        
        int score;
        score = manageOneRound(wordList, nWords, wordnum);
        
        if (score == 1)
            cout << "You got it in 1 try." << endl;
        
        if (score > 1)
            cout << "You got it in " << score << " tries." << endl;
        
        double average;
        int minimum, maximum;
        
        if (k == 0) // because min and max are not defined in round 1
        {
            minimum = score;
            maximum = score;
        }
        
        average = ((average * k) + score)/ (k + 1);
        if (score < minimum)
            minimum = score;
        if (score > maximum)
            maximum = score;
        
        cout.setf(ios::fixed);
        cout.setf(ios::showpoint);
        cout.precision(2);
        
        cout << "Average: " << average << ", minimum: " << minimum << ", maximum: " << maximum << endl;
        cout << endl;
    }
    
    
    
}