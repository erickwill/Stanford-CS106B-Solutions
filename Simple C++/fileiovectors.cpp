/*
 * Project: File Processing and Vectors
 * Created by CS106 C++ Assignment Wizard 0.1
 * 
 * Reads in a list of scores from a file and
 * prints a histogram.
 *
 */

#include <iostream>
#include "simpio.h"
#include "genlib.h"
#include <fstream>
#include "vector.h"

const int RANKS = 10; // number of ranks in histogram

void GetStream(ifstream &in);
void GetScores(ifstream &in, Vector<int> &histogram);
void PrintHistogram(Vector<int> &histogram);
string PrintStars(int n);

int main () {
    // initialize vector and set values to zero
    Vector<int> histogram(RANKS);
    for (int i = 0; i < RANKS; i++) {
        histogram.add(0);
    }

    ifstream in;
    GetStream(in); // create input stream
    GetScores(in, histogram); // read in scores from file
    PrintHistogram(histogram);
    return 0;
}

/*
* Asks the user for a file name. Reprompts if
* there is an error opening the file.
*/
void GetStream(ifstream &in) {
    while(true) {
        cout << "Enter data filename: ";
        string fileName = GetLine();
        in.open(fileName.c_str());
        if (!in.fail()) break;
        in.clear();
        cout << "File open error." << endl;
    }
    return;
}

/*
* Reads in lines from a file. Converts the
* line into an integer and updates the vector.
*/
void GetScores(ifstream &in, Vector<int> &histogram) {
    string line;
    while(true) {
        getline(in, line);
        if (in.fail()) break; // end of file
        int score = StringToInteger(line);
        score /= 10;
        if (score >= 0 && score < 10)
            histogram[score]++;
    }
}

void PrintHistogram(Vector<int> &histogram) {
    for (int i = 0; i < RANKS; i++) {
        string stars = PrintStars(histogram[i]);
        printf("%02d-%02d: %s\n", (i*10), ((i*10)+9), stars.c_str());
    }
}

string PrintStars(int n) {
    string str;
    for (int i = 0; i < n; i++) {
        str += 'X';
    }
    return str;
}