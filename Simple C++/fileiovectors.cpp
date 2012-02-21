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
void AddScore(Vector<int> &histogram, int score);
void PrintHistogram(Vector<int> &histogram);
string PrintValue(int n);

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
		AddScore(histogram, score);
	}
	return;
}

/*
* Calculates where the given integer should be placed and
* increments the vector value.
*/
void AddScore(Vector<int> &histogram, int score) {
	if (score > 0 && score < 10)
		histogram[0]++;
	else if (score >= 10 && score < 20)
		histogram[1]++;
	else if (score >= 20 && score < 30)
		histogram[2]++;
	else if (score >= 30 && score < 40)
		histogram[3]++;
	else if (score >= 40 && score < 50)
		histogram[4]++;
	else if (score >= 50 && score < 60)
		histogram[5]++;
	else if (score >= 60 && score < 70)
		histogram[6]++;
	else if (score >= 70 && score < 80)
		histogram[7]++;
	else if (score >= 80 && score < 90)
		histogram[8]++;
	else if (score >= 90 && score < 100)
		histogram[9]++;
	return;
}

void PrintHistogram(Vector<int> &histogram) {
	cout << "0-9: " << PrintValue(histogram[0]) << endl;
	cout << "10-19: " << PrintValue(histogram[1]) << endl;
	cout << "20-29: " << PrintValue(histogram[2]) << endl;
	cout << "30-39: " << PrintValue(histogram[3]) << endl;
	cout << "40-49: " << PrintValue(histogram[4]) << endl;
	cout << "50-59: " << PrintValue(histogram[5]) << endl;
	cout << "60-69: " << PrintValue(histogram[6]) << endl;
	cout << "70-79: " << PrintValue(histogram[7]) << endl;
	cout << "80-89: " << PrintValue(histogram[8]) << endl;
	cout << "90-99: " << PrintValue(histogram[9]) << endl;
	return;
}

string PrintValue(int n) {
	string str;
	for (int i = 0; i < n; i++) {
		str += 'X';
	}
	return str;
}