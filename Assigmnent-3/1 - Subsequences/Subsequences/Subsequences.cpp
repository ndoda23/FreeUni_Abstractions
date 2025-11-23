/*
 * File: Subsequences.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Subsequences problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Given two strings, returns whether the second string is a
 * subsequence of the first string.
 */
bool isSubsequence(string text, string subsequence);

int main() {
    // [TODO: fill with your code]
    string text = getLine("Enter the text: ");
    string subsequence = getLine("Enter the subsequence: ");
    if (isSubsequence(text, subsequence)) {
        cout << "It is a subsequence." << endl;
    }
    else {
        cout << "It is not a subsequence." << endl;
    }
    return 0;
}

bool isSubsequence(string text, string subsequence) {

    // if both of them is empty.
    if (text.size() == 0 && subsequence.size() == 0) {
        return true;
    }
    // if subsequence is empty.
    else if (subsequence.empty()) {
        return true;
    }

    else if (text.empty()) {
        return false;
    }
    else {
        // if text's first char is not subsequence's first char.
        if (text[0] != subsequence[0]) {
            return isSubsequence(text.substr(1), subsequence);
        }
        // if text's first char is subsequence's first char we continue searching.
        else {
            return isSubsequence(text.substr(1), subsequence.substr(1));
        }
    }
}
