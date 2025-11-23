/*
 * File: WordLadder.cpp
 * --------------------
 */

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "queue.h"                                                                                  
#include "set.h"
#include "vector.h"
#include <unordered_set>


using namespace std;

Vector<string> findTheShortestWay(Lexicon&, string, string);
Vector<string> findEveryWord(string,Lexicon&);
string generateWord(char, string,int);
string printVec(Vector<string>);

int main() {
    // [TODO: fill with your code]
    Lexicon english("EnglishWords.dat");

    while (true) {
        // if you do not write anything the program will end.
        cout << "Enter start word (RETURN to quit) : ";
        string startWord = getLine();
        if (startWord == "") {
            cout << "You stopped the program, Bye!" << endl;
            break;
        }
        cout << "Enter the ending word : ";
        string destWord = getLine();

        // if the lexicon does not contain the words , There will be ERROR.
        if (!english.contains(startWord) || !english.contains(destWord)) {
            cout << "ERROR: word can't be found." << endl;
            continue;
        }
        else {
            Vector<string> wordsPath = findTheShortestWay(english, startWord, destWord);
            // if the vector is empty return 'No ladder'.
            if (wordsPath.isEmpty()) {
                cout << "No ladder found." << endl;
            }
            else {
                cout << "Found ladder : " << printVec(wordsPath) << endl;
            }
        }
    }
    cout << endl;
    return 0;
}

// BFS algorithm. 
Vector<string> findTheShortestWay(Lexicon& english, string startWord, string destWord) {

    // queue for ladders.
    Queue<Vector<string>> wordsQueue;
    // set for already visited words.
    Set<string> visitedWords;
    // ladder.
    Vector<string> path;


    path.push_back(startWord);  
    wordsQueue.enqueue(path);
    visitedWords.add(startWord);

    while (!wordsQueue.isEmpty()) {
        Vector<string> newPath = wordsQueue.dequeue();
        // if the last word of the ladder is destination word, return.
        if (newPath[newPath.size() - 1] == destWord) {
            return newPath;
        }
        
        Vector<string> oneCharDifference = findEveryWord(newPath[newPath.size() - 1],english);
        for (int i = 0; i < oneCharDifference.size(); i++) {
            if (find(newPath.begin(), newPath.end(), oneCharDifference[i]) == newPath.end() 
                && !visitedWords.contains(oneCharDifference[i])) {
                Vector<string> lastPath = newPath;
                lastPath.push_back(oneCharDifference[i]);
                wordsQueue.enqueue(lastPath);
                visitedWords.add(oneCharDifference[i]);
            }
        }
    }

    // if there is no ladder.
    Vector<string> emptyVec;
    return emptyVec;
}

// it finds words which are different by one char.
Vector<string> findEveryWord(string word, Lexicon& english) {
    Vector<string> words;
    for (int i = 0; i < word.size(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            string newStr = generateWord(ch, word,i);
            if (ch == word[i]) {
                continue;
            }
            if (english.contains(newStr)) {
                words.push_back(newStr);
            }
        }
    }
    return words;
}

string generateWord(char ch, string mainWord,int i) {
    mainWord[i] = ch;
    return mainWord;
}

// it prints the vector.
string printVec(Vector<string> vec) {
    string s = "";
    for (int i = 0; i < vec.size(); i++) {
        if (i != vec.size() - 1) {
            s += vec[i] + " -> ";
        }
        else {
            s += vec[i];
        }
    }
    return s;
}