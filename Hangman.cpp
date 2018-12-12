/*
    @author: Samuel Teague
    @date: 03 Dec 2018 (most recent)
    @file: Hangman.cpp
    @description: This file contains a basic Hangman command line game which draws from a dictionary, 
        words-alpha.txt, in the same directory. 

    @other: This was originally written in 2016, and has been refactored a few times since then, buts its main design 
        and functionality has not changed since then. Its biggest change was when it went from raw C arrays and C strings
        to C++ STL objects in early 2018
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include "./GetItemsFromFile/getItemsFromFile.hpp"


void drawHangman(int);
std::string genSecretWord(const std::vector<std::string> & );
void showStatus(const std::string & );
char getGuess();
void recordGuess(const std::string & , std::string &, char);
bool isWin(std::string);
bool fillDictionaryFromFile(const std::string & fileName, std::vector<std::string> & dictionary);
bool letterIsInWord(char letter, const std::string & word);


int main()
{
    //constants
    const std::string INFILE_NAME = "words_alpha.txt";
    const int NUM_WRONG_ALLOWED = 7;

    //vector
    std::vector<std::string> dictionary;

    if (!fillDictionaryFromFile(INFILE_NAME, dictionary))   //if an error occurred reading from file
    {
        std::cerr << "Error reading from file\n";
        return 1;
    }

    //variables
    int numWrong = 0;
    char letter;
    std::string word;
    std::string guessWord;

    do {
        system("cls");
        word = genSecretWord(dictionary);

        guessWord = "";
        for (int i = 0; i < word.length(); ++i)
            guessWord += " ";

        numWrong = 0;
        do {

            drawHangman(numWrong);
            showStatus(guessWord);
            letter = getGuess();

            if (letterIsInWord(letter, word))
                recordGuess(word, guessWord, letter);
            else
                numWrong++;


        } while (!isWin(guessWord) && numWrong < NUM_WRONG_ALLOWED);

        if (!isWin(guessWord))
        {
            for (int i = 0; i < word.length(); ++i)
                std::cout << ' ' << word[i] << ' ';

            std::cout << '\n';
            std::cout << "--------------------------\n";
            std::cout << "Game over. You lost.\n";
            std::cout << "--------------------------\n";
        }
        else
        {
            std::cout << "--------------------------\n";
            std::cout << "You Win!!!! :)\n";
            std::cout << "--------------------------\n";
        }
        std::cout << "Do you want to play again? [Y or N]";
        std::cin >> word;
    } while (toupper(word[0]) == 'Y');


    return 0;
}

bool fillDictionaryFromFile(const std::string & fileName, std::vector<std::string> & dictionary)
{
	/*
		@name: fillDictionaryFromFile()
		@author: Samuel Teague
		@date: 12 December 2018 (mostrecent)
		@description: Fills a vector with the contents of a dictionary file
		@param: string fileName: the name of the file
				vector of strings: reference variable of the vector to fill
		@return: true if successful, false if not
	*/
	try 
	{
		dictionary = getItemsFromFile(fileName);
	}
	catch (file_not_found)
	{
		return false;
	}
	return true;
}

std::string genSecretWord(const std::vector<std::string> & d )
{
    /*
    Function : genSecretWord()
    Description : Randomly select an index to the dictionary array (number between 0 and number of words-1.)
    Parameters : the dictionary
    Returns : the secret word
    */


    //get random index number
    std::srand(std::time(NULL));
    int index = std::rand() % d.size();


    return d[index];
}

char getGuess()
{
    /*
    Function : getGuess()
    Description : Get next guess from the player via the console window
    Parameters : none
    Returns : The guessed letter
    */

    //variable
    char l;
    bool isValid = false;   //added in update

    //get input
    do {    //loop added in update
        std::cout << "Enter your guess: ";
        std::cin >> l;

        //below code added in update
        isValid = isalpha(l);
        if (!isValid)
            std::cout << "Invalid, Please try again.\n";
    } while (!isValid);


    return tolower(l);
}

void showStatus(const std::string & guess)
{
    /*
    Function : showStatus()
    Description : Show current game status, i.e., each guessed letter is shown in its correct position within the secret word.
    Letters that have not been guessed yet are shown as " _ " to indicate missing letters.
    Parameters : Guess array of char containing currently guessed letters in their correct positions.
    Returns : none

    Update Note: Adjusted to use std::string instead of c-syle string. 
    */

    for (int i = 0; i < guess.length(); i++)
    {
        std::cout << ' ';
        if (guess[i] == ' ')
            std::cout << '_';
        else
            std::cout << guess[i];
        std::cout << ' ';
    }
    std::cout << '\n';
}

bool letterIsInWord(char letter, const std::string & word)
{
    /*
        @name: letterIsInWord()
        @author: Samuel Teague
        @date: 07 Dec 2018
        @description: checks if a letter is in a word. Really just a nicer way to call std::find()
        @param: letter, word
        @return: true if letter is in word, false otherwise
    */

    return std::find(word.begin(), word.end(), letter) != word.end();
}

void recordGuess(const std::string & word, std::string & guess, char guessedLetter)
{
    /*
        @name: recordGuess()
        @author: Samuel Teague
        @date: 07 Dec 2018 (most recent)
        @description: Place correctly guessed letter in array guess in its correct spot.
        @param : the correct word, the guess word so far, and the letter guessed
        @return: changes the second parameter, the guess string, by reference
    */

    for (int i = 0; i < word.length(); i++)
        if (guessedLetter == word[i])
            guess[i] = guessedLetter;
}

void drawHangman(int x)
{
    /*
    Function : drawHangman()
    Description : Draw hangman figure. Number of body parts shown corresponds to the current wrong turn. 
        Wrong turn 1 -> head, 2 -> body, etc.
    Parameters : Current amt of wrong answers (1 -> 6)
    Returns : none

    NOTE: Modify this function with extreme care. Output formatting on the console is hard to set up properly.
    */

    std::cout << "-------\n";
    std::cout << "|/    |\n";
    std::cout << "|     ";

    if (x > 0)
        std::cout << 'O';
    else
        std::cout << ' ';

    std::cout << '\n';
    std::cout << "|     ";

    if (x > 1)
        std::cout << '|';
    else
        std::cout << ' ';

    std::cout << '\n';
    std::cout << "|    ";

    if (x > 3)
        std::cout << '/';
    else
        std::cout << ' ';

    if (x > 2)
        std::cout << 'O';

    if (x > 4)
        std::cout << '\\';
    else
        std::cout << ' ';

    std::cout << '\n';
    std::cout << "|    ";

    if (x > 5)
        std::cout << '/';
    else
        std::cout << ' ';

    std::cout << ' ';

    if (x > 6)
        std::cout << '\\';
    else
        std::cout << ' ';

    std::cout << '\n';
    std::cout << '|' << '\n';
    std::cout << "----------------------------------\n";
}

bool isWin(std::string guess)
{
    /*
    @name: isWin()
    @author: Samuel Teague
    @date: 07 Dec 2018 (most recent)
    @description: Has the player won, i.e., are all characters in guess array not ' ' at this time?
    @param: 
        guess: a string, holding the current status of the game thus far
    @return: true if there are no ' ' in the string, false otherwise
        In the context of the program: returns true if the game is won, false if not yet.
    */

    for(int i = 0; i < guess.length(); i++)
        if (guess[i] == ' ')
            return false;
    return true;
}


