#include "genlib.h"
#include <iostream>
#include "lexicon.h"
#include "strutils.h"
#include "simpio.h"

void ListCompletions(string digitSequence, Lexicon & lex);
void RecursiveMnemonics(string prefix, string rest, Lexicon &lex);
string DigitLetters(char ch);
void RecursiveCompletion(Lexicon &lex, string prefix);

const string alphabet = "abcdefghijklnmopqrstuvwxyz";

int main () {
    Lexicon lex("lexicon.dat");
    string digitSequence;
    while(true) {
        cout << "-Only digits 2-9 have values" << endl <<
            "-Type \"exit\" to exit program" << endl << "Enter number sequence:";
        digitSequence = GetLine();
        if (digitSequence == "exit") exit(0);
        else if(digitSequence != "")
            ListCompletions(digitSequence, lex);
        cout << "**Finished!**" << endl << endl;
    }
    return 0;
}


void ListCompletions(string digitSequence, Lexicon & lex) {
    RecursiveMnemonics("", digitSequence, lex);    
}

void RecursiveMnemonics(string prefix, string rest, Lexicon &lex) {
    if (rest.length() == 0)
        RecursiveCompletion(lex, prefix);
    else {
        string options = DigitLetters(rest[0]);
        for (int i = 0; i < options.length(); i++)
            RecursiveMnemonics(prefix + options[i], rest.substr(1), lex);
    }
}

string DigitLetters(char ch) {
    switch (ch) {
        case '2': return ("ABC");
        case '3': return ("DEF");
        case '4': return ("GHI");
        case '5': return ("JKL");
        case '6': return ("MNO");
        case '7': return ("PQRS");
        case '8': return ("TUV");
        case '9': return ("WXYZ");
        default: return ""; // skip '1', '0', and any non-integer
    }
}

void RecursiveCompletion(Lexicon &lex, string prefix) {
    if (lex.containsWord(prefix)) {
        cout << ConvertToLowerCase(prefix) << endl;
    }

    for (int i = 0; i < alphabet.length(); i++) {
        if (lex.containsPrefix(prefix + alphabet[i]))
            // add letters to test if completion exist in lexicon
            RecursiveCompletion(lex, prefix + alphabet[i]);
    }
}