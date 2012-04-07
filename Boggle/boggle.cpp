/* File: boggle.cpp
 * ----------------
 * Your name here!
 */
 
#include "genlib.h"
#include "simpio.h"
#include "lexicon.h"
#include <iostream>
#include "extgraph.h"
#include "grid.h"
#include "random.h"
#include "gboggle.h"
#include "vector.h"

struct pointT
{
    int row;
    int col;
};

const int MIN_WORD_LENGTH = 4;
const int SMALL_BOARD_SIZE = 4;
const int BIG_BOARD_SIZE = 5;

string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
 "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
 
string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
"BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
"DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
     << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
     << "creating a grid of letters on which you try to form words. You go first, " 
     << "entering all the words you can find that are formed by tracing adjoining " 
     << "letters. Two letters adjoin if they are next to each other horizontally, " 
     << "vertically, or diagonally. A letter can only be used once in the word. Words "
     << "must be at least 4 letters long and can only be counted once. You score points "
     << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
     << "points, and so on. After your puny brain is exhausted, I, the super computer, "
     << "will find all the remaining words and double or triple your paltry score." << endl;
    
    cout << "\nHit return when you're ready...";
    GetLine();
    cout << endl;
}

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
     << "The good news is that you might improve your vocabulary a bit.  The "
     << "bad news is that you're probably going to lose miserably to this little "
     << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
* Sets board size.
*/
int BoardSize()
{
    int size;
    while(true)
    {
        cout << "1. Standard Board (4x4)" << endl;
        cout << "2. Big Board (5x5)" << endl;
        cout << "Select Board: ";
        size = GetInteger();
        if (size == 1)
        {
            size = SMALL_BOARD_SIZE;
            break;
        } 
        else if (size == 2)
        {
            size = BIG_BOARD_SIZE;
            break;
        }
    }
    return size;
}

/*
* Fills board grid with strings.
*/
void FillBoard(Grid<string> &board, Vector<string> &letters)
{
    int count = 0;
    for (int row = 0; row < board.numRows(); row++)
    {
        for (int col = 0; col < board.numCols(); col++)
        {
            board.setAt(row,col,letters[count]);
            count++;
        }
    }
}

/*
* Randomize cubes.
*/
void ShuffleBoardCubes(Grid<string> &board)
{
    for (int row = 0; row < board.numRows(); row++) 
    {
        for (int col = 0; col < board.numCols(); col++)
        {
            int randomRow = RandomInteger(row, board.numRows()-1);
            int randomCol = RandomInteger(col, board.numCols()-1);
            string tmp = board.getAt(row,col);
            board.setAt(row,col,board.getAt(randomRow,randomCol));
            board.setAt(randomRow,randomCol,tmp);
        }
    }
}

/*
* Gets a random letter from the cube's string.
*/
void ShuffleCubeLetters(Grid<string> &board)
{
    Grid<char> gameBoard(board.numRows(), board.numCols());
    for (int row = 0; row < board.numRows(); row++)
    {
        for (int col = 0; col < board.numCols(); col++)
        {
            string cubeString = board.getAt(row,col);
            int randomChar = RandomInteger(0,cubeString.size()-1);
            string letter = cubeString.substr(randomChar, randomChar+1);
            board.setAt(row,col,letter);
        }
    }
}

/*
* Update board graphics
*/
void DrawCubes(Grid<string> &board)
{
    for (int row = 0; row < board.numRows(); row++)
    {
        for (int col = 0; col < board.numCols(); col++)
        {
            string str = board.getAt(row,col);
            char letter = str[0];
            LabelCube(row,col,letter);
        }
    }
}

/*
* Takes an array of strings and loads them into a vector.
*/
Vector<string> StringArrayToVector(string arr[], int boardSize)
{
    Vector<string> vec;
    int size = boardSize * boardSize;
    for (int i = 0; i < size; i++)
    {
        vec.add(arr[i]);
    }
    return vec;
}

/*
* Splits a string and adds the letters to a vector.
*/
Vector<string> StringToVector(string str)
{
    Vector<string> vec;
    for (int i = 0; i < str.length(); i++)
    {
        vec.add(str.substr(i,i+1));
    }
    return vec;
}

/*
* Gets a string from the user to use as cube letters.
*/
Vector<string> GetUserCube(int boardSize)
{
    string letters;
    while(true)
    {
        cout << "Enter letters for board. Exactly " << boardSize * boardSize <<
            " letters required: ";
        letters = GetLine();
        if (letters.length() == boardSize * boardSize) break;
        cout << "Incorrect amount of letters." << endl;
    }
    letters = ConvertToUpperCase(letters);
    Vector<string> vecLetters = StringToVector(letters);
    return vecLetters;
}

/*
* Ask the user to use a custom string for the cubes.
*/
bool UserCube()
{
    bool forceCube = false;
    while(true)
    {
        cout << "Force board configuration? (y/n) ";
        string response = GetLine();
        response = ConvertToLowerCase(response);
        if (response == "y" || response == "yes")
        {
            forceCube = true;
            break;
        } 
        else if (response == "n" || response == "no")
        {
            break;
        }
    }
    return forceCube;
}

/*
* Initializes game board.
*/
Grid<string> BoardSetup()
{
    int boardSize = BoardSize(); // get board size
    DrawBoard(boardSize,boardSize);
    Grid<string> board(boardSize, boardSize);
    bool userCubeRequested = UserCube();
    Vector<string> cube;

    if (userCubeRequested)
    {
        cube = GetUserCube(boardSize);
    }
    else if (boardSize == SMALL_BOARD_SIZE)
    {
        cube = StringArrayToVector(StandardCubes, boardSize);
    }
    else
    {
        cube = StringArrayToVector(BigBoggleCubes, boardSize);
    }

    FillBoard(board, cube);
    if (!userCubeRequested)
    {
        ShuffleBoardCubes(board);
        ShuffleCubeLetters(board);
    }

    DrawCubes(board);
    return board;
}

/*
* Checks if word has already been guessed.
*/
bool AlreadyGuessed(Vector<string> &guesses, string word)
{
    for (int i = 0; i < guesses.size(); i++)
    {
        if (guesses[i] == word)
            return true;
    }
    return false;
}

/*
* Highlights cubes that successfully formed a word.
*/
void Highlight(Vector<pointT> &cells)
{
    for (int i = 0; i < cells.size(); i++)
    {
        pointT pt = cells[i];
        HighlightCube(pt.row,pt.col,true);
    }
    Pause(1);
    for (int j = 0; j < cells.size(); j++)
    {
        pointT pt = cells[j];
        HighlightCube(pt.row,pt.col,false);
    }
}

/*
* Checks if a cube cell has already been used to build a word.
*/
bool AlreadyUsed(Vector<pointT> &cells, int row, int col)
{
    int numCells = cells.size();
    for (int i = 0; i < numCells; i++)
    {
        pointT pt = cells[i];
        if (pt.row == row && pt.col == col)
            return true;
    }
    return false;
}

/*
* Helper function for BoardHasWord.
* Recursively searches the game board for the user's guess word.
*/
bool FindWord(Grid<string> &board, string &word, int row, 
              int col, string &soFar, Vector<pointT> &cells, bool &found)
{
    int soFarLen = soFar.length();
    // stop if string gets too big
    if (soFarLen > word.length())
        return false;
    // stop if we can't match the word with this path
    if (word.substr(0,soFarLen) != soFar)
        return false;
    // word found!
    if (soFar == word)
    {
        found = true;
        return true;
    }

    // check all neighbor cubes
    for (int vert = -1; vert <= 1; vert++)
    {
        for (int horiz = -1; horiz <= 1; horiz++)
        {
            // watch for grid borders and used cubes
            // skip 0 as a value
            if (row + vert >= 0 && row + vert < board.numRows() && 
                col + horiz >= 0 && col + horiz < board.numCols() && 
                !AlreadyUsed(cells,row+vert,col+horiz) && 
                !(horiz == 0 && vert == 0))
            {
                soFar += board.getAt(row,col).substr(0,1);
                pointT pt;
                pt.row = row;
                pt.col = col;
                cells.add(pt); // log path point

                if (FindWord(board,word,row+vert,col+horiz,soFar,cells,found))
                    return true;
                else 
                {
                    // backtrack
                    if (soFar.length() > 0)
                    {
                        soFar = soFar.substr(0,soFar.length()-1);
                        cells.removeAt(cells.size()-1);
                    }
                }
            }
        }
    }
    return false;
}

/*
* Searches the board for a word. 
* Launches the helper function FindWord.
*/
bool BoardHasWord(Grid<string> board, string &word)
{
    bool found = false;
    int boardSize = board.numRows();
    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            // find a cube that matches the first letter of the word
            if (board.getAt(row,col).substr(0,1) == word.substr(0,1))
            {
                string soFar;
                Vector<pointT> cells;
                FindWord(board,word,row,col,soFar,cells,found);
                if (found) 
                {
                    Highlight(cells);
                    return found;
                }
            }
        }
    }
    return found;
}

/*
* Process user word. Determine if the word is valid, search
* the board and update graphics.
*/
void ParseGuess(Grid<string> &board, string word, 
                Vector<string> &guesses, Lexicon &lex)
{
    playerT player = Human;
    if (AlreadyGuessed(guesses,word))
    {    
        cout << "You've already guessed that word!" << endl;
        return;
    }
    if (!lex.containsWord(word) || word.length() < MIN_WORD_LENGTH)
    {    
        cout << "Not a valid word!" << endl;
        return;
    }
    // word found
    if (BoardHasWord(board,word))
    {
        guesses.add(word); // log guess
        RecordWordForPlayer(word, player); // update graphics
    }
    else
    {
        cout << "Couldn't find word on board." << endl;
    }
}

/*
* Get user guess words.
*/
Vector<string> PlayerTurn(Grid<string> &board, Lexicon &lex)
{
    Vector<string> guesses;
    string word;
    while(true)
    {
        cout << endl << "Enter a word: ";
        word = GetLine();
        word = ConvertToUpperCase(word);
        if (word == "") break;
        ParseGuess(board, word, guesses, lex);
    }    
    return guesses;
}

/*
* If the word has not been found already, update board graphics.
*/
void ParseCompWord(string &word, Vector<string> &foundWords)
{
    // check if word already found
    for (int i = 0; i < foundWords.size(); i++)
    {
        if (word == foundWords[i])
            return;
    }

    foundWords.add(word);
    playerT comp = Computer;
    RecordWordForPlayer(word,comp);
}

/*
* Helper function to recursively find all valid words
* on the game board.
*/
void FindCompWords(Grid<string> &board, Lexicon &lex, int row, int col, 
                   string soFar, Vector<string> &foundWords)
{    
    string letter = board.getAt(row,col); // cube letter
    string orig = board.getAt(row,col); // original cube string
    string current = soFar + letter.substr(0,1); // build word
    
    // word found
    if (current.length() >= MIN_WORD_LENGTH && lex.containsWord(current))
    {
        ParseCompWord(current,foundWords);    
        return;
    }

    // stop if the prefix isn't valid
    if (!lex.containsPrefix(soFar))
    {
        return;
    }

    // check all neighbor cubes
    for (int vert = -1; vert <= 1; vert++)
    {
        for (int horiz = -1; horiz <= 1; horiz++)
        {
            // watch for grid borders and used cubes
            if (row + vert >= 0 && row + vert < board.numRows() && 
                col + horiz >= 0 && col + horiz < board.numCols() && 
                board.getAt(row + vert, col + horiz) != "~" && 
                !(horiz == 0 && vert == 0))
            {
                board.setAt(row,col,"~"); // used cube
                FindCompWords(board,lex,row+vert,col+horiz,current,foundWords);
                board.setAt(row,col,orig); // reset cube for different letter arrangement
            }
        }
    }
}

/*
* Searches for all valid words on game board.
*/
void ComputerTurn(Grid<string> &board, Lexicon &lex, Vector<string> &foundWords)
{
    int boardSize = board.numRows();

    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            string soFar;
            FindCompWords(board,lex,row,col,soFar,foundWords);
        }
    }
}

/*
* Set up board and play game.
*/
void StartGame(Lexicon &lex)
{
    Grid<string> board = BoardSetup();
    Vector<string> foundWords = PlayerTurn(board,lex);
    ComputerTurn(board,lex,foundWords);
}

/*
* Asks user if they would like to play again.
* Relaunches game.
*/

bool PlayAgain()
{
    bool rerun = false;
    while(true)
    {
        cout << endl << "Play again? (Y/N) ";
        string answer = GetLine();
        answer = ConvertToUpperCase(answer);
        if (answer == "Y" || answer == "YES")
        {
            rerun = true;
            break;
        }
        else if (answer == "N" || answer == "NO")
        {
            rerun = false;
            break;
        }
    }
    return rerun;
}

int main()
{    
    Randomize();
    SetWindowSize(9, 5);
    InitGraphics();
    Welcome();
    GiveInstructions();
    Lexicon lex("lexicon.dat");
    StartGame(lex);

    while(true)
    {
        if(PlayAgain())
        {
            cout << endl;
            InitGraphics();
            StartGame(lex);
        }
        else
            exit(0);
    }
    return 0;
}
