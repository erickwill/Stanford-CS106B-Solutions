/*
 * Project: Assign1Soundex
 * Created by CS106 C++ Assignment Wizard 0.1
 *
 * Gets a name from the user and returns the
 * the Soundex code.
 */

#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include <string>
#include "strutils.h"
#include <ctype.h>

const int CODE_LENGTH = 4; // length of soundex code

string ParseName(string name);
char ParseChar(char c);

int main ()
{
    // get name to encode
    while(true) {
        cout << "Enter surname (RETURN to quit): ";
        string surname = GetLine();
        if (surname == "") exit(0); // exits program
        cout << "Soundex code for " << surname 
            << " is " << ParseName(surname) << endl;
    }
    return 0;
}

/*
* Returns an encoded string according to the 
* Soundex algorithm.
*/
string ParseName(string name) {
    string code;
    name = ConvertToUpperCase(name);
    for (int i = 0; i < name.length(); i++) {    
        if (i == 0)
            code += name[i]; // save first char
        if (i > 0 && isalpha(name[i]) != 0)
            code += ParseChar(name[i]);
    }
    
    for (int j = 0; j < code.length(); j++) {
        if (j > 0 && code[j] == code[j-1]) {
            code.erase(j,1); // delete duplicates
        } else if (code[j] == '0') {
            code.erase(j,1); // delete zeros
        }
    }
    
    if (code.length() < CODE_LENGTH) {
        int zeros = CODE_LENGTH - code.length();
        for (int k = 0; k < zeros; k++) {
            code += '0';  // padd zeros if code is too short
        }
    } else if (code.length() > CODE_LENGTH) {
        code = code.substr(0,CODE_LENGTH); // truncate if code is too long
    }
    return code;
}

/*
* Returns a Soundex encoded character. Should only accept
* alphabetic characters.
*/
char ParseChar(char c) {
    if (c == 'A' || c == 'E'|| c == 'I' || c == 'O' ||
        c == 'U' || c == 'H' || c == 'W' || c == 'Y') {
        return '0';    
    } else if (c == 'B' || c == 'F' || c == 'P' || c == 'V') {
        return '1';
    } else if (c == 'C' || c == 'G' || c == 'J' || c == 'K' || 
        c == 'Q' || c == 'S' || c == 'X' || c == 'Z') {
        return '2';
    } else if (c == 'D' || c == 'T') {
        return '3';
    } else if (c == 'M' || c == 'N') {
        return '4';
    } else if (c == 'L') {
        return '5';
    } else {
        return '6';
    }
}