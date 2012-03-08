#include "randomwriter.h"
#include <iostream>
#include "genlib.h"
#include <fstream>
#include "simpio.h"
#include "map.h"
#include "vector.h"
#include "random.h"

void ParseInputText(ifstream &in);
void CreateSeedMap(string &line);
string GetInitialSeed();
void GenerateRandomText(string seed);

const int MAX_CHARS = 2000;

Map<Vector<char> > seedMap; // all seeds with corresponding characters
Vector<string> seedsWithDupes; // all seeds for finding most common
int orderK; // Markov order #

/*
* Main Random Writer function. Asks the user for the input text file and
* Markov order number. Generates a maximum of MAX_CHARS characters.
*/
void RandWriter() {
    Randomize();
    ifstream in;
    while(true) {
        cout << "Please enter filename containing source text: ";
        string fileName = GetLine();
        in.open(fileName.c_str());
        if (!in.fail()) break;
        in.clear();
        cout << "File open error." << endl;
    }
    while(true) {
        cout << "What order of analysis? (a number from 1 to 10): ";
        orderK = GetInteger();
        if (orderK >= 1 && orderK <= 10) break;
        cout << "Invalid number" << endl;
    }
    ParseInputText(in);
    in.close();
    string seed = GetInitialSeed();
    GenerateRandomText(seed);
    // clear containers for reuse
    seedMap.clear();
    seedsWithDupes.clear();
}

/* Logs seeds and associated characters from input text. */
void ParseInputText(ifstream &in) {
    cout << "Analyzing input text..." << endl << endl;
    string line;
    while(in.good()) {
        line += in.get();
    }
    CreateSeedMap(line);
}

/* Creates containers for seeds (size order_k) and their respective characters. */
void CreateSeedMap(string &line) {
    for (int i = 0; i < line.length() - orderK; i++) {
        string seedStr = line.substr(i, orderK);
        seedsWithDupes.add(seedStr);
        if (i + orderK < line.length()) {
            Vector<char> nextCharacters;
            char c = line[i+orderK];
            if (seedMap.containsKey(seedStr)) {
                nextCharacters = seedMap.getValue(seedStr);
                nextCharacters.add(c);
                seedMap.add(seedStr, nextCharacters);
            } else {
                nextCharacters.add(c);
                seedMap.add(seedStr, nextCharacters);
            }
        }
    }
}
/* Returns most abundant seed to start text generation. */
string GetInitialSeed() {
    Map<int> tally; // seed with occurence counter
    int count;
    string seed;
    // count and log seeds
    for (int i = 0; i < seedsWithDupes.size(); i++) {
        if (tally.containsKey(seedsWithDupes[i])) {
            count = tally.getValue(seedsWithDupes[i]);
            count++;
            tally.add(seedsWithDupes[i], count);
        } else {
            tally.add(seedsWithDupes[i], 1);
        }
    }
    // find highest count
    Map<int>::Iterator itr = tally.iterator();
    count = 0;
    while(itr.hasNext()) {
        string key = itr.next();
        if (tally.getValue(key) > count) {
            count = tally.getValue(key);
            seed = key;
        }
    }
    return seed;
}

/* 
* Generates and prints the random text using the initial seed for a
* maximum of MAX_CHARS characters.
*/
void GenerateRandomText(string seed) {
    Vector<char> seedChars;
    cout << seed; // start by printing the initial seed
    for (int i = 0; i < MAX_CHARS; i++) {
        if (seedMap.containsKey(seed))
            seedChars = seedMap.getValue(seed);
        else
            break; // seed not found
        int size = seedChars.size();
        if (size == 0) break; // seed has no associated characters
        int rand = RandomInteger(0, size-1);
        char c = seedChars[rand]; // choose next character at random
        cout << c;
        seed.erase(0,1); // remove first character from seed
        seed += c; // add new character to seed
    }
    cout << endl << endl;
}