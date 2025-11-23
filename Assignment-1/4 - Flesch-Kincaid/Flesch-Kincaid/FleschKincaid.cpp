/*
 * File: FleschKincaid.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Flesch-Kincaid problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "tokenscanner.h"
#include "console.h"
#include <fstream>
#include "simpio.h"
using namespace std;

//xmovnebi
string const vowels = "aeiouy";
//operatorebi sentencis dasatvlelad
string const ops = ".!?";

//constantebi
double const C0 = -15.59;
double const C1 = 0.39;
double const C2 = 11.8;


bool isVowel(char ch);
void countWords(TokenScanner& scanner, int& wordsCnt, int& sentencesCnt,int& syllablesCnt);
int  countSyllables(string&);


int main() {
    // [TODO: fill in the code]
    int wordsCnt = 0;
    int sentencesCnt = 0;
    int syllablesCnt = 0;
    ifstream inputFile;
    //tu araswor files saxeli sheyavs tavidan cdis.
    while (true) {
        string fileName = getLine("Enter the file name: ");
        inputFile.open(fileName);
        if (!inputFile.is_open()) {
            cout << "Name is incorrect,try again." << endl;;
        }
        else break; 
    }

    //tokenscannerit vaignoreb spacebs da aseve minda rom ert sityvad chatvalots mag: isn't.
    TokenScanner scanner;
    scanner.setInput(inputFile);
    scanner.ignoreWhitespace();
    scanner.addWordCharacters("'");

    //vatvlevineb methodit da shemdeg gamomyavs shesabamisi grade
    countWords(scanner, wordsCnt, sentencesCnt, syllablesCnt);
    double grade = C0 + C1 * (wordsCnt *1.0/ sentencesCnt) + C2 * (syllablesCnt*1.0 / wordsCnt);
    cout << "Grade is: " << grade << endl;

    inputFile.close();

    return 0;
}

//itvlis sityvebs sentencebs da chashenebuli methodit marcvlebs.
void countWords(TokenScanner& scanner,int& wordsCnt,int& sentencesCnt,int& syllablesCnt) {
    while (scanner.hasMoreTokens()) {
        string word = scanner.nextToken();
        //tu sityvaa wordsCnt izrdeba.
        if (scanner.getTokenType(word) == WORD) {
            wordsCnt++;
            syllablesCnt += countSyllables(word);
        }
 
        //tu romelime sasurveli operatoria sentences raodenoba izrdeba
        else if (scanner.getTokenType(word) == OPERATOR) {
            for (int i = 0; i < ops.size(); i++) {
                string ch = "";
                ch += ops[i];
                if (word == ch) {
                    sentencesCnt++;
                }
            }
        }
    }

    if (wordsCnt == 0 && sentencesCnt==0) {
        wordsCnt=1;
        sentencesCnt=1;
    }
    else if (sentencesCnt == 0) {
        sentencesCnt=1;
    }
    else if (wordsCnt == 0) {
        wordsCnt=1;
    }
  
    cout << "words: " << wordsCnt << endl;
    cout << "sentences: " << sentencesCnt << endl;
    cout << "syllabels: " << syllablesCnt << endl;
}

//itvlis marcvlebs.gatvaliswinebulia ramdenime shemtxveva romelic pirobashi ewera.
int countSyllables(string& word) {
   

    int syllableCnt = 0;
    string token = "";
    bool lastVowel = false;
    //yvela lowercase asos vxdi rata gavamartivo
    for (char c : word) {
        token += tolower(c);
    }

    for (int i = 0; i < token.length(); i++) {

        char ch = token[i];
        if (isVowel(ch)) {
//arminda chaitvalos zedized xmovnebi
            if (!lastVowel) {
                syllableCnt++;
            }
            lastVowel = true;
        }
        else {
            lastVowel = false;
        }
    }

    //tu boloshi maqvs e.
    if (token.length() > 1 && token[token.size() - 1] == 'e') {
        syllableCnt--;
    }

    //magalitistvis me. gamodis rom marcvali argvaqvs , magram ideashi gvsurs rom marcvali chaitvalos,
    //shesabamisad nebismier sityvashi tu marcvali ar aris ert marcvlad mainc vtvli. 
    if (syllableCnt <= 0) {
        syllableCnt = 1;
    }
    
    return syllableCnt;
}

//method romelic amowmebs aso aris tuara xmovani. 
bool isVowel(char ch) {
    for (int i = 0; i < vowels.size(); i++) {
        if (vowels[i] == ch) {
            return true;
        }
    }
    return false;
}
