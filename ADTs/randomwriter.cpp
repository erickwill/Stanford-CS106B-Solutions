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

const int MAX_WORDS = 2000;

Map<Vector<char> > seedMap;
Vector<string> seedsWithDupes;
int orderK;

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
    seedMap.clear();
    seedsWithDupes.clear();
}

void ParseInputText(ifstream &in) {
    cout << "Analyzing input text..." << endl << endl;
    string line;
    while(in.good()) {
        line += in.get();
    }
    CreateSeedMap(line);
}

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

string GetInitialSeed() {
    Map<int> tally;
    int count;
    string seed;
    for (int i = 0; i < seedsWithDupes.size(); i++) {
        if (tally.containsKey(seedsWithDupes[i])) {
            count = tally.getValue(seedsWithDupes[i]);
            count++;
            tally.add(seedsWithDupes[i], count);
        } else {
            tally.add(seedsWithDupes[i], 1);
        }
    }
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

void GenerateRandomText(string seed) {
    Vector<char> seedChars;
    cout << seed;
    for (int i = 0; i < MAX_WORDS; i++) {
        if (seedMap.containsKey(seed))
            seedChars = seedMap.getValue(seed);
        else
            break;
        int size = seedChars.size();
        if (size == 0) break;
        int rand = RandomInteger(0, size-1);
        char c = seedChars[rand]; 
        cout << c;
        seed.erase(0,1);
        seed += c;
    }
    cout << endl << endl;
}