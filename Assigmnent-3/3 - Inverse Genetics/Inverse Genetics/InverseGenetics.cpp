/*
 * File: InverseGenetics.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Inverse Genetics problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "simpio.h"
#include "map.h"
#include "console.h"
#include "vector.h"
using namespace std;

/* Function: listAllRNAStrandsFor(string protein,
 *                                Map<char, Set<string> >& codons);
 * Usage: listAllRNAStrandsFor("PARTY", codons);
 * ==================================================================
 * Given a protein and a map from amino acid codes to the codons for
 * that code, lists all possible RNA strands that could generate
 * that protein
 */
void listAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons);

/* Function: loadCodonMap();
 * Usage: Map<char, Lexicon> codonMap = loadCodonMap();
 * ==================================================================
 * Loads the codon mapping table from a file.
 */
Map<char, Set<string> > loadCodonMap();

// index for each codon
int index = 1;

void helperFunc(string protein, Map<char, Set<string>>& codons, Vector<string>& results);
string printingVec(Vector<string> vec);

int main() {
    /* Load the codon map. */
    Map<char, Set<string> > codons = loadCodonMap();

    string protein = getLine("Enter the protein name: ");
    listAllRNAStrandsFor(protein, codons);


    return 0;
}

// I use helper method in it.
void listAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons) {
    Vector<string> results;
    helperFunc(protein, codons, results);
}

void helperFunc(string protein, Map<char, Set<string>>& codons, Vector<string>& results){
    // if we have no more protein chars , thats it.
    if (protein.empty()) {
        string res = printingVec(results);
        cout << res << endl;
    }

    // set of first char codons
    Set<string> set = codons[protein[0]];

    for (string s : set) {
        results.add(s);
        helperFunc(protein.substr(1), codons, results);
        // backtracking.
        results.remove(results.size() - 1);
    }
}

// it prints the method.
string printingVec(Vector<string> vec) {
    string s = "";
    s += integerToString(index) + ": ";
    for (int i = 0; i < vec.size(); i++) {
        s += vec[i];
    }
    index++;
    return s;
}



/* You do not need to change this function. */
Map<char, Set<string> > loadCodonMap() {
    ifstream input("codons.txt");
    Map<char, Set<string> > result;

    /* The current codon / protein combination. */
    string codon;
    char protein;

    /* Continuously pull data from the file until all data has been
     * read.
     */
    while (input >> codon >> protein) {
        result[protein] += codon;
    }

    return result;
}
