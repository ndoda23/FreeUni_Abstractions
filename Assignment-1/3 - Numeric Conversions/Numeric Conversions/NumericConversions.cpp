/*
 * File: NumericConversions.cpp
 * ---------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Numeric Conversions problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function prototypes */

string intToString(int);
int stringToInt(string);

/* Main program */

//shemoviyvan sasurvel ints,strings da shemdeg gamoviyeneb sasurvel
//methodebs.
int main() {
    // [TODO: fill in the code]
    int numberToString = getInteger("Enter number: ");
    cout << "Answer is " << intToString(numberToString) << endl;

    string strToInt = getLine("Enter string: ");
    cout << "Answer is " << stringToInt(strToInt) << endl;
    
    return 0;
}

// danashtvit vigeb titoeul bolo cifrs satitaod da shemdeg vawebeb mis wina cifrs , ritac
// sabolood gamodis charebisgan shedgenili string. tu uaryofiti int gvaq ubralod
// win minuss mivuwert.
string intToString(int n) {

    string zero = "0";
    if (n == 0) { 
        return zero; 
    };

    if (n < 0) {
        char minus =  '-';
        return minus + intToString(-n);
    }
   
    if (n / 10 == 0) {
        char ch = char(n % 10 + '0');
        string newStr = "";
        newStr += ch;
        return newStr;
    }
    int  m = n / 10;
    return intToString(m) + (char)('0' + (n % 10)); 
}

// vigeb tu ra aris pirveli cifri shemdeg imas vamravleb 10 is imden xarisxze , ramdeni cifricaa
// mis win , amas vaketeb pow methodit , roca bolo cifrze miva 10 is xarisxi ubralod nuli gaxdeba.
// roca stringad uaryofiti ricxvi gvaq sign sheicvleba , gaxdeba -1 da shedegshic shesabamisad
// aisaxeba. tu ricxvi dadebitia sign iqneba ubralod +1.
int stringToInt(string str) {
    int sign = 1;
    if (str.empty()) {
        return 0;
    }
    if (str[0] == '-') {
        string m = str.substr(1);
        str = m;
        sign = -1;
   }
    int power = 0;
    if (str.size() - 1 == 0)
        power = 1;
    else {
        power = pow(10, str.size() - 1);
    }   
   return ((int)(str[0] - '0') * power + stringToInt(str.substr(1)))*sign;
}
    

