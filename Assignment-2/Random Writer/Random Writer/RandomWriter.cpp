/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Random Writer problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "console.h"
#include "map.h"
#include "random.h"
#include <fstream>
#include <istream>
#include "vector.h"
#include "simpio.h"

using namespace std;
int whichModel();
void generator(int, ifstream&, Map<string,Map<char,int>>&);
string readText(ifstream& file);
string generateChars(int, int,string);
string textGenerator(Map<string,Map<char,int>>&);
string mostfrequent(Map <string, Map<char, int>>& occMap);
int calculateTotal(Map<char, int>);

int main() {
    // [TODO: fill with your code]
    ifstream file;

    // Enter file name and if it's not correct , try again. 
    while (true) {
        cout << "Enter the filename: ";
        string fileName = getLine();
        file.open(fileName);
        if (!file.is_open()) {
            cout << "Invalid name ! Try again. " << endl;
        }else{
            break;
        }
    }

    // which order the costumer want.
    int k = whichModel();

    Map <string,Map<char, int>> occMap;
    generator(k, file, occMap);
    cout << "Our generated text : " << endl;
    cout <<  textGenerator(occMap); 
    return 0;
}

    // if the number isn't between 1-10 ,try again.
int whichModel() {
    while (true) {
        cout << "Enter the order number: " ;
        int num = getInteger();
        if (num > 10 || num < 1) {
            cout << "You've entered invalid number , try again," << endl;
        }
        else {
            return num;
            break;
        }
    }
    
}

   // it generates map , where key is k length 'word' and in the value , 
   // we have map , which has character as a key and int as a value.  
void generator(int k, ifstream& file, Map <string, Map<char,int>>& occMap) {
    string text = readText(file);

    for (int i = 0; i < text.length()-k; i++) {
        //read entire text as a string.
        string word = generateChars(i,k,text);

            if (!occMap.containsKey(word)) {
                Map<char, int> newMap;
                occMap[word] = newMap;
            }
            occMap[word][text[i + k]]++;
    }
    
}

// it generates string from i index according to k.
string generateChars(int i, int k, string text) {
    string word = "";

    for (int j = i; j < i+k; j++) {
        word += text[j];
    }
    return word;
}

// this method reads text from the file.
string readText(ifstream& file) {
    string txt = "";
    char ch;
    while (file.get(ch)) {
        txt += ch;
    }
    return txt;
}

//this method generates text.
string textGenerator(Map<string, Map<char, int>>& occMap) {

    //first word , which is most used.
    string mostUsed = mostfrequent(occMap);
    string result = mostUsed;
    string current = mostUsed;

    // its length has to be 2000.
    while (result.length() <= 2000) {
        char nextChar;
        Map<char, int>& nextChars = occMap[current];
       
        //total chars.
        int total = calculateTotal(nextChars);
      

        int randNum = randomInteger(0, total - 1);
        int coefficient = 0;
        for (char c : nextChars) {
            coefficient += nextChars[c];
            if (randNum < coefficient) {
                nextChar = c;
                break;
            }
        }

        result += nextChar;
        // updating string.
        current = current.substr(1,current.length()-1) + nextChar;
    }

    return result;
}

// most common word
string mostfrequent(Map<string, Map<char, int>>& occMap) {
    string s = "";
    int max = 0;
    for (string w : occMap) {
        int tot = 0;
        Map<char, int> newMap = occMap[w];
        for (char c : newMap) {
            tot += newMap[c];
        }

        if (tot > max) {
            max = tot;
            s = w;
        }
    }

    return s;
}

// it calculates total chars.
int calculateTotal(Map<char, int> smthMap) {
    int tot = 0;
    for (char ch : smthMap) {
        tot += smthMap[ch];
    }
    return tot;
}