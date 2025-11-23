#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Grid<char> board(4, 4); 
/* Function prototypes */

void welcome();
void giveInstructions();
void shuffleCubes(Vector<string>&);
void drawLetters(Vector<string>&);
void fillVec(Vector<string>&);
void fillVecFromStr(const string(&cubes)[16], Vector<string>& result);
void playerTurn(Lexicon& dict,int&, Set<string>& alrUsed);
void computerTurn(Lexicon& dict,int&,Set<string>);
bool findWordForHuman(string word, int i, int j, Grid<bool>& visited, int index, Vector<pair<int, int>>& path);
void findWordsForComp(Lexicon& dict, int i, int j, string currWord,
     Vector<string>& words, Grid <bool>& visited);
bool isValidPosition(int, int);
char lower(char);
void fillTheGrid(Grid<bool>&);
void updateScore(string,int&);
bool contains(string,Vector<string>);

/* Main program */

int main() {
    Lexicon dictionary("EnglishWords.dat");
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();

    bool playAgain = true;

    // you can play again. 
    while (playAgain) {
        string input = getLine("Do you want to generate a random board? (y/n) ");
        Vector<string> cubes;
        if (input == "n") {
            fillVec(cubes);
        }
        else {
            fillVecFromStr(STANDARD_CUBES, cubes);
        }
        giveInstructions();
        drawBoard(4,4);
        // draws letters inside cubes.
        drawLetters(cubes);

        Set<string> alrUsed;
        int playerScore = 0;
        int computerScore = 0;

        playerTurn(dictionary, playerScore,alrUsed);
        cout << "Now it's my turn to destroy you :)" << endl;
        computerTurn(dictionary, computerScore,alrUsed);

        // final results.
        cout << "Player: " << playerScore << endl;
        cout << "Computer: " << computerScore << endl;

        // if you want to repeat the game again.
        string repeating = getLine("Do you want to play again? (y/n)");
        if (repeating == "n") {
            cout << "Better luck next time ;)." << endl;
            playAgain = false;
        }
    }
    return 0;
}

// it fills vector
void fillVec(Vector<string>& cubes) {
    for (int i = 0; i < 16; i++) {
        string s = getLine(to_string(i + 1) + ")");
        cubes.add(s);
    }
}
// it fills vector from string arr.
void fillVecFromStr(const string(&cubes)[16], Vector<string>& result) {
    for (int i = 0; i < 16; i++) {
        result.add(cubes[i]);
    }
}

// cubes randomizer.
void shuffleCubes(Vector<string>& cubes) {
    for (int i = 0; i < cubes.size(); i++) {
        int x = randomInteger(i, cubes.size() - 1);
        swap(cubes[i], cubes[x]);
    }
}

// it drawsLetters randomly.
void drawLetters(Vector<string>& cubes) {
    shuffleCubes(cubes);
    for (int i = 0; i < board.numCols(); i++) {
        for (int j = 0; j < board.numRows(); j++) {
            int face = randomInteger(0, 5);
            char letter = cubes[i * 4 + j][face];
            board[i][j] = letter;
            labelCube(i, j, letter);
        }
    }
}

// if i and j are inBounds
bool isValidPosition(int i, int j) {
    if (i >= 0 && j >= 0 && i < 4 && j < 4) return true;
    return false;
}

// this method return lower letter
char lower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + 32;
    }
    return ch;
}

// At first each element in grid must be false. That's what this method does.
void fillTheGrid(Grid<bool>& isVisited) {
    for (int i = 0; i < isVisited.numCols(); i++) {
        for (int j = 0; j < isVisited.numRows(); j++) {
            isVisited[i][j] = false;
        }
    }
}

// recursion for a player. 
// it tells if the word is valid.
bool findWordForHuman(string word, int i , int j,Grid<bool>& isVisited , int index,
    Vector<pair<int,int>>& path) {
    if (index == word.length()) return true;

    // make sure it is not outside bounds
    if (!isValidPosition(i, j)) return false;

    if(lower(board[i][j]) != lower(word[index])) return false;

    // visited locations.
    if (isVisited[i][j]) return false;

    isVisited[i][j] = true;
    // saving path for highlighting
    path.add({ i, j });

    if (findWordForHuman(word, i + 1, j + 1, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i - 1, j, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i + 1, j, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i, j - 1, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i, j + 1, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i - 1, j - 1, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i - 1, j + 1, isVisited, index + 1,path)) {
        return true;
    }
    if (findWordForHuman(word, i + 1, j - 1, isVisited, index + 1,path)) {
        return true;
    }

    // backtracking
    isVisited[i][j] = false;
    path.remove(path.size() - 1);
    return false;
}

// recursion for every word.
void findWordsForComp(Lexicon& dict, int i, int j, string currWord,
    Vector<string>& words, Grid <bool>& visited) {
    // make sure it is a valid location.
    if (!isValidPosition(i, j)) return;

    currWord += board[i][j];

    // if its length is at least 4 and dictionary contains this word we add it to the words vector.
    if (currWord.size() >= 4 && dict.contains(currWord) && !contains(currWord,words)) {
        words.push_back(currWord);
    }

    if (!dict.containsPrefix(currWord)) return;

    if (visited[i][j]==true) return;

    visited[i][j] = true;
    // looking for each position.
    for (int deltaX = -1; deltaX <= 1; deltaX++) {
        for (int deltaY = -1; deltaY <= 1; deltaY++) {
            if ((deltaX == 0 && deltaY == 0)) continue;
            else {
                findWordsForComp(dict, i + deltaX, j + deltaY, currWord, words, visited);
            }
        }
    }
    //backtracking
    visited[i][j] = false;
}

// this method updates screen , points and words
void playerTurn(Lexicon& dict,int& playerScore, Set<string>& alrUsed) {
    while (true) {
        string currWord = getLine("Enter a word(or return to finish): ");
        if (currWord.size() == 0) break;
        Vector<pair<int, int>> path;
        bool found = false;
        // trying from each position
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4 ; j++) {
                if (!found) {
                    Grid<bool> isVisited(4, 4);
                    fillTheGrid(isVisited);
                    path.clear();
                    if (findWordForHuman(currWord, i, j, isVisited, 0, path)) {
                        found = true;
                    }
                }
            }
        }

        // if the word is found 
        if (found == true && !alrUsed.contains(currWord) &&
            currWord.length() >= 4 && dict.contains(currWord)) {
            // highlights letters.
            for (pair<int,int>& p : path) {
                highlightCube(p.first, p.second, true);
            }
            pause(600); 
            for (pair<int, int>& p : path) {
                highlightCube(p.first, p.second, false);
            }

            alrUsed.add(currWord);
            // updates screen
            updateScore(currWord, playerScore);
            recordWordForPlayer(currWord, HUMAN);

        }
        else if (!found) {
            cout << "That word is not on the board." << endl;
        }
    }

}

void updateScore(string currWord,int& playerScore) {
   playerScore += currWord.size() - 3;
}

// after the player, it's computer's turn to defeat him.
void computerTurn(Lexicon& dict,int& computerScore,Set<string> alrUsed) {
    Vector<string> words;
    Grid<bool> visited(4, 4);
    fillTheGrid(visited);
    // trying each position.
    for (int i = 0; i < board.numCols(); i++) {
        for (int j = 0; j < board.numRows(); j++) {
            findWordsForComp(dict, i, j, "", words, visited);
        }
    }

    for (string s : words) {
        // if player could not write it.
        if (!alrUsed.contains(s)) {
            updateScore(s, computerScore);
            recordWordForPlayer(s, COMPUTER);
        }
    }
}

bool contains(string s, Vector<string> words){
    for (int i = 0; i < words.size(); i++) {
        if (s == words[i])
            return true;
    }
    return false;
}

 // Print out a cheery welcome message. 
void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

 // Print out the instructions for the user.
void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}