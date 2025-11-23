
#include <iostream>
#include "console.h"
#include "random.h"

using namespace std;
//headsis mosvlis albatoba
const double prob = 0.5;

//method romelic printavs heads an tails imis mixedvit tu ra amovida,
//tu zedized sami heads amovida daasrulebs mushaobas.
int calculating() {
    int coinCounter = 0; 
    int headsCounter = 0;
    while (headsCounter < 3) {
        if (randomChance(prob) == true) {
            coinCounter++;
            cout << "heads" << endl;
            headsCounter++;
        }
        else {
            coinCounter++;
            cout << "tails" << endl;
            headsCounter = 0;
        }
    }
    return coinCounter;
}

//mainshi vaprintireb tu ramdeni svla dachirda zedized 3
//headsis amosasvlelad.
int main() {
    cout << "It took " << calculating() << " flips to get 3 consecutive heads." << endl;
    return 0;
}