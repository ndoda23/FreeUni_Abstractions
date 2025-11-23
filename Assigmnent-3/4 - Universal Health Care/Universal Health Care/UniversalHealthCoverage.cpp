/*
 * File: UniversalHealthCoverage.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the UniversalHealthCoverage problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */
#include <iostream>
#include <string>
#include "set.h"
#include "vector.h"
#include "console.h"
using namespace std;

/* Function: canOfferUniversalCoverage(Set<string>& cities,
 *                                     Vector< Set<string> >& locations,
 *                                     int numHospitals,
 *                                     Vector< Set<string> >& result);
 * Usage: if (canOfferUniversalCoverage(cities, locations, 4, result)
 * ==================================================================
 * Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 */
bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result);

bool helperFunc(Set<string>& cities,Vector<Set<string>>& locations,int index,
    int hospitalNum, Vector<Set<string>>& curr, Vector<Set<string>>& result);

// it prints the vector.
void printSet(Vector <Set<string>>);

int main() {

   // all cities.
    Set<string> cities;
    cities.add("A");
    cities.add("B");
    cities.add("C");
    cities.add("D");
    cities.add("E");
    cities.add("F");

    // all locations.
    Vector<Set<string>> locations;
    Set<string> s1;
    Set<string> s2;
    Set<string> s3;
    Set<string> s4;

    s1.add("A");
    s1.add("B");
    s1.add("C");

    s2.add("A");
    s2.add("C");
    s2.add("D");

    s3.add("B");
    s3.add("F");

    s4.add("C");
    s4.add("E");
    s4.add("F");


    locations.add(s1);
    locations.add(s2);
    locations.add(s3);
    locations.add(s4);

    // max number of hospitals.
    int numHospitals = 3;

    Vector<Set<string>> result;

    if (canOfferUniversalCoverage(cities, locations, numHospitals, result)) {
        cout << "It is possible. " << endl;
        printSet(result);
    }
    else {
        cout << "it is not possible with this amount of"  << " hospitals." << endl;
    }

    return 0;
}

// I have helper method in it.
bool canOfferUniversalCoverage(Set<string>& cities,
    Vector< Set<string> >& locations,
    int numHospitals,
    Vector< Set<string> >& result) {
    Vector<Set<string>> curr;
    int index = 0;
    return helperFunc(cities,locations,index,numHospitals,curr,result);

}

// it is the main method.
bool helperFunc(Set<string>& cities,Vector<Set<string>>& locations, int index,
                int hospitalNum , Vector<Set<string>>& curr , Vector<Set<string>>& result ) {
    
    // if there are no cities left.
    if (cities.isEmpty()) {
        result = curr;
        return true;
    }
    // if cities are left but we cant use more hospital.
    else if (!cities.isEmpty() && hospitalNum <= 0) {
        return false;
    }

    // if cities are lft but locations vector is empty.
    else if (!cities.isEmpty() && locations.isEmpty()) {
        return false;
    }
    else {
        for (int i = index; i < locations.size(); i++) {
            Set<string> covCities = locations[i];
            // i copy cities set
            Set<string> asd = cities;
            for (string s : covCities) {
                // I subtract cities from the set of cities.
                asd -= s;
            }
            curr.add(covCities);
            if (helperFunc(asd, locations,i+1,hospitalNum - 1, curr, result)) {
                return true;
            }
            //backtracking.
            curr.remove(curr.size()-1);
        }
    }
    return false;
}

// This method prints Vector<Set<string>>
void printSet(Vector <Set<string>> result) {
    for (Set<string> s : result) {
        cout << "{";
        for (string city : s) {
            cout << city;
        }
        cout << "}" << endl;
    }
}