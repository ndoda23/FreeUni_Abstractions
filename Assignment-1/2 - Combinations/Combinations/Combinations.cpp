/*
 * File: Combinations.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Combinations problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
using namespace std;

//mtavari method sadac rekursiulad xdeba ricxvis gamotvla wina oris jamis mixedvit, 
//basecase 1) tu n<k pasuxi iqneba 0 ,radgan sheudzlebelia k elementis amorcheva n-dan
//basecase 2) tu k=0 erti gza arsebobs amitom abrunebs 1-s
//basecase 3) tu k==n aqac erti gza arsebobs da daabrunebs 1-s.
int calculating(int n, int k) {
    if (n < k) {
        return 0;
    }
    if (k == 0) {
        return 1;
    }
    else if (k == n) {
        return 1;
    }
    else return calculating(n - 1, k - 1) + calculating(n - 1, k);
}

//mainshi vaprintineb sasurvel n,k - s da shemdeg vitvli C(n,k)-s. 
int main() {
    int n = getInteger("Value of positive n ");
    int k = getInteger("Value of positive k ");
    cout << "Answer is " << calculating(n,k) << endl;
    return 0;
}

