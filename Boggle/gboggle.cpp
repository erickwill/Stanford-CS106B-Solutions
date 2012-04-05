/*
 * File: gboggle.cpp
 * -----------------
 * Implements the graphics support functions for Boggle program.
 * Last modified Mon Oct 15 09:40:38 2007 juliez
 *
 *
 * General implementation notes
 * ----------------------------
 * This file implements the graphics support functions for the Boggle
 * program.  It makes straightforward use of the extended graphics
 * library to draw the cubes, board, letters, scoreboard, etc. The 
 * implementations of the individual functions are fairly simple 
 * and many of the helper functions do not require individual comments. 
 * For descriptions of the behavior of the exported functions, 
 * please see the interface file.
 */

#include "extgraph.h"
#include "gboggle.h"
#include "strutils.h"    // for IntegerToString, ConvertToLowerCase


/* Constants
 * ---------
 * To configure fixed spacing, fonts, etc.
 */
const double BOARD_BORDER = .25;        // width of border around the cubes
const double INDENT = 0.05;                // indent reserved at vertical edges of window
const double LABEL_HEIGHT = .70;        // space reserved at top of window for labels
const double HUMAN_PERCENTAGE = 0.38;    // percentage of space for word lists to apportion to human (vs computer)
const string CUBE_FONT = "Helvetica";    // Font for letters on cube face
const double FONT_PERCENTAGE = 0.6;     // percentage of cube used for letter
const string SCORE_FONT = "Helvetica";     // Font for scoreboard names & scores
const int SCORE_FONT_SIZE = 18;
const string WORD_FONT = "Geneva";        // Font for word lists
const int WORD_FONT_SIZE = 10;


/* 
 * A static struct is used to gather the state that is local to this module.
 * Mostly this is geometry calculated to fit the cubes, word lists,
 * and labels neatly within the current size of the graphics window.
 * In addition, the module tracks the count of words listed for each player
 * (to place additional words in the correct location in the displayed word list)
 * and the scores for each player (which must be saved and erased before updating).
 * There is also a 2-d array of the letters currently showing on the
 * cubes, to enable drawing them inverted for the highlighting function.
 */
 
struct rectT { double x, y, w, h; };

static struct {
    int numWords[2];        // num words currently displayed for each player
    int scores[2];            // scores for each player
    rectT scoreBox[2];        // rectangle enscribing the label + score for each player
    double wordColumnWidth;    // width of word list columns
    double cubeSize;        // size of cube
    double cubeFontSize;    // font size used for labelling cubes
    rectT board;            // rectangle enscribed the cubes w/ border
    int numRows, numCols;    // dimensions of cube layout on board
    char letters[MAX_DIMENSION][MAX_DIMENSION];
} gState;


/* 
 * These are the prototypes for functions private to this module.  These
 * helper functions are used to implement the functions which are exported 
 * from this module (see .h for prototypes of the exported functions)
 */
static void DrawBox(double x, double y, double width, double height);
static void FillBox(double x, double y, double width, double height,
                    double fill, string color);
static void DrawRoundedRect(double x, double y, double width, double height, double radius);
static void FillRoundedRect(double x, double y,double width, double height, double radius,
                            double fill, string color);
static void DrawAndFillRoundedRect(double x, double y, double width, double height,
                                   double radius, double fill,string color);
static void DrawCenteredChar(double centerX, double centerY, char ch);
static void DrawCube(int row, int col, char ch, bool invert);
static double CubeX(int col);
static double CubeY(int row);
static void DrawPlayerLabel(playerT player, string name);
static void DrawEmptyCubes();
static void DrawOneScore(playerT playerNum, int value);
static void AddToScoreForPlayer(int pointsToAdd, playerT playerNum);
static void InitColors();
static void CalculateGeometry(int numRows, int numCols);


/* 
 * Function: DrawBoard
 * -------------------
 * Exported function to draw a new board.  Sets up named colors, draws the yellow
 * border, draws the empty cubes, the labels on the scoreboard, and resets scores
 * to zero and words lists to empty.
 */
void DrawBoard(int numRows, int numCols)
{
    if (numRows < 0 || numRows > MAX_DIMENSION || numCols < 0 || numCols > MAX_DIMENSION)
        Error("DrawBoard called with invalid dimensions.");
        
    SetWindowTitle("Welcome to Boggle!");
    InitColors();
    CalculateGeometry(numRows, numCols);
    
            // Draws a filled rect underneath cubes in the oh-so-familiar yellow color
    FillBox(gState.board.x, gState.board.y, gState.board.w, gState.board.h, 0.5, "Board Color");
    DrawEmptyCubes();
    DrawPlayerLabel(Human, "Me");
    DrawPlayerLabel(Computer, "Computer");
}



/* 
 * Function: LabelCube
 * -------------------
 * Exported function used to draw letters on cubes.  Set the 2-d array
 * at this position to hold the letter and then draw the cube in the un-highlighted
 * version with the letter added.
 */ 
void LabelCube(int row, int col, char letter)
{
    if (row < 0 || row >= gState.numRows || col < 0 || col >= gState.numCols)
        Error("LabelCube called with invalid row, col arguments.");
    gState.letters[row][col] = letter;
    DrawCube(row, col, letter, false);
}


/* 
 * Function: HighlightCube
 * -----------------------
 * Exported function used to turn off and on highlight for cubes.  This function
 * is intended to highlight a cube which is currently being used to form a word.  
 */ 
void HighlightCube(int row, int col, bool flag)
{
    if (row < 0 || row >= gState.numRows || col < 0 || col >= gState.numCols)
        Error("LabelCube called with invalid row, col arguments.");
    DrawCube(row, col, gState.letters[row][col], flag);
}



/* 
 * Function: DrawEmptyCubes
 * ------------------------
 * Draws all of the cubes in position.  The cubes are drawn blank,
 * the client will fill in the letters by calling the exported function
 * function LabelCube.
 */
static void DrawEmptyCubes()
{
    for (int row = 0; row < gState.numRows; row++) 
        for (int col = 0; col < gState.numCols; col++) 
            DrawCube(row, col, ' ', false);    // erase all cubes, start with blank chars
}


/* 
 * Function: DrawCube
 * ------------------
 * Draws one cube in its position, including the letter centered in the middle of
 * the cube.  If invert flag is true, reverse background and letter colors.
 */
static void DrawCube(int row, int col, char ch, bool invert)
{
    DrawAndFillRoundedRect(CubeX(col), CubeY(row), gState.cubeSize, gState.cubeSize, 
                        gState.cubeSize/5.0, 1.0, (invert ? "Letter Color" : "Die Color"));
    SetPenColor((invert ? "Die Color" : "Letter Color"));                        
    DrawCenteredChar(CubeX(col) + gState.cubeSize/2.0, CubeY(row) + gState.cubeSize/2.0, ch);
}



/* 
 * Function: DrawPlayerLabel
 * -------------------------
 * Labels player word list with specified name and draws a line underneath the label.
 * It also resets the player's score and word list count back to zero.
 */
static void DrawPlayerLabel(playerT player, string name)
{
    SetPenColor("Label Color");
    MovePen(gState.scoreBox[player].x, gState.scoreBox[player].y);
    DrawLine(gState.scoreBox[player].w , 0);
    SetFont(SCORE_FONT);
    SetPointSize(SCORE_FONT_SIZE);
    MovePen(gState.scoreBox[player].x, gState.scoreBox[player].y + GetFontDescent());
    DrawTextString(name);
    gState.scores[player] = gState.numWords[player] = 0;
}



/* 
 * Function: DrawOneScore
 * ----------------------
 * Draws the specified score for the player according to player enum.
 */
static void DrawOneScore(playerT playerNum, int value)
{   
       SetFont(SCORE_FONT);
       SetPointSize(SCORE_FONT_SIZE);
    SetPenColor("Label Color");
    string str = IntegerToString(value);
       MovePen(gState.scoreBox[playerNum].x + gState.scoreBox[playerNum].w - TextStringWidth(str), 
               gState.scoreBox[playerNum].y + GetFontDescent());
    DrawTextString(str);
}

/* 
 * Function: EraseOldScore
 * ------------------------
 * I used to do this with SetEraseMode, but that left cruft behind, so instead
 * paint an opaque white box over the old score
 */
static void EraseOldScore(playerT playerNum, int value)
{   
       SetFont(SCORE_FONT);
       SetPointSize(SCORE_FONT_SIZE);
    string str = IntegerToString(value);
    FillBox(gState.scoreBox[playerNum].x + gState.scoreBox[playerNum].w - TextStringWidth(str), 
            gState.scoreBox[playerNum].y + GetFontDescent(),
            TextStringWidth(str), GetFontAscent(), 1.0, "White");
}

/* 
 * Function: AddToScoreForPlayer
 * -----------------------------
 * Updates a player's score by the specified number of points.
 * Note that we must first erase the old score before drawing
 * the new one (otherwise we scribble & create chaos).  The
 * scores are kept in an array that is private to this module.
 */
static void AddToScoreForPlayer(int pointsToAdd, playerT playerNum)
{
    EraseOldScore(playerNum, gState.scores[playerNum]); // erase old score
    gState.scores[playerNum] += pointsToAdd;
    DrawOneScore(playerNum, gState.scores[playerNum]); // re-draw new score
}


/* 
 * Function: RecordWordForPlayer
 * -----------------------------
 * Exported function to add a word to a player's list and update
 * their score accordingly.  It keeps track of the number of
 * words recorded for each player and position each successive word
 * in rows and columms from left to right, top to bottom.  It
 * also updates the scoreboards with point additions.
 */
void RecordWordForPlayer(string word, playerT player)
{
    if (player != Human && player != Computer)
        Error("RecordWordForPlayer called with invalid player argument.");
    word = ConvertToLowerCase(word);
    SetFont(WORD_FONT);
    SetPointSize(WORD_FONT_SIZE);
    SetPenColor("Word Color");
    int numWordsInRow = gState.scoreBox[player].w/gState.wordColumnWidth;
    int row = gState.numWords[player] / numWordsInRow;
    int col = gState.numWords[player] % numWordsInRow;
       MovePen(gState.scoreBox[player].x + col*gState.wordColumnWidth, 
               gState.scoreBox[player].y -(row+1)*GetFontHeight());
    DrawTextString(word);
    gState.numWords[player]++;
    AddToScoreForPlayer(word.length() - 3, player); // +1 pt for each letter over length 4
    if (col == numWordsInRow -1) UpdateDisplay(); // force update when completing a row
}


// Color table of named colors and their RGB components.  We will init the table once
// and define colors with those names so we can access colors by name later.
static struct {
    char *colorName;
    double red, green, blue;
} colorTable[] = {
    {"Board Color",     1, .86, 0.05}, // yellow-orange
    {"Die Color",      .9, .9, .9},    // almost white
    {"Letter Color", .01, .20, .85},// dark blue
    {"Word Color", .01, .50, .30},    // med green-blue
    {"Label Color",    .01, .45, .25},    // dark green-blue
}; 

static void InitColors()
{
    static bool beenHere = false;
     
    if (beenHere) return; // Only want to do this once
      beenHere = true;
    for (int i = 0; i < sizeof(colorTable)/sizeof(colorTable[0]); i++)
        DefineColor(colorTable[i].colorName, 
                       colorTable[i].red,
                       colorTable[i].green, 
                       colorTable[i].blue);
}


// Handy utility functions to return coordinates of the lower-left corner of 
// the cube at position (row, col).  The cubes are numbered from top to bottom, 
// left to right starting from zero. Therefore, the upper left corner is (0,0), 
// lower right is (numRows-1,numCols-1)

static double CubeX(int col)
{
    return gState.board.x + BOARD_BORDER/2.0 + gState.cubeSize*col;
}

static double CubeY(int row)
{
     return gState.board.y + gState.board.h - BOARD_BORDER/2.0 - gState.cubeSize*(row+1);
}
 
/* Function: CalculateGeometry
 * ---------------------------
 * This internal helper does all the messy math to work out how to divide up the space
 * within the current graphics window to neatly fit the board, the cubes, and the word lists.
 */
static void CalculateGeometry(int numRows, int numCols)
{    
    double boardSize = min(min(GetWindowWidth()/3, GetWindowHeight() - LABEL_HEIGHT), 2.7);
    gState.cubeSize = min((boardSize-BOARD_BORDER)/numRows, (boardSize-BOARD_BORDER)/numCols);
    gState.board.w = gState.board.h = boardSize;
    gState.board.y = GetWindowHeight() - LABEL_HEIGHT - gState.board.h;
    double leftover = GetWindowWidth() - gState.board.w - 2*INDENT;
    gState.scoreBox[Human].x = INDENT;
    gState.scoreBox[Human].y = gState.scoreBox[Computer].y = gState.board.y + gState.board.h;
    gState.scoreBox[Human].h = gState.scoreBox[Computer].h = LABEL_HEIGHT;
    gState.scoreBox[Human].w = leftover*HUMAN_PERCENTAGE;
    gState.board.x = gState.scoreBox[Human].x + gState.scoreBox[Human].w;
    gState.scoreBox[Computer].x = gState.board.x + gState.board.w + INDENT;
    gState.scoreBox[Computer].w = GetWindowWidth() - gState.scoreBox[Computer].x - INDENT;
    gState.numRows = numRows;
    gState.numCols = numCols;
    SetFont(WORD_FONT);
    SetPointSize(WORD_FONT_SIZE);
    gState.wordColumnWidth = TextStringWidth("mmmmm");    // col size is 5 letters wide
    SetFont(CUBE_FONT);
    // iterate to find largest font that fits within given percentage
     for (gState.cubeFontSize = 8; gState.cubeFontSize < 48; gState.cubeFontSize++) {
         SetPointSize(gState.cubeFontSize);
         if ((TextStringWidth("M") > FONT_PERCENTAGE*gState.cubeSize) || 
             (GetFontAscent() > FONT_PERCENTAGE*gState.cubeSize)) break;
    }
}
 
/* 
 * Function: DrawCenteredChar
 * --------------------------
 * Used to draw the letters in the center of the cube. 
 * Note that this function centers the char
 * both vertically and horizontally around the point specified.
 */ 
static void DrawCenteredChar(double centerX, double centerY, char ch)
{
    string s(1, ch);
      SetFont(CUBE_FONT);
     SetPointSize(gState.cubeFontSize);
    MovePen(centerX - TextStringWidth(s)/2, centerY - GetFontAscent()/2);
    DrawTextString(s);
}


/* Shape-drawing functions 
 * -----------------------
 * These routines use graphics library to draw rects, fill them in,
 * draw rounded corner rects and fill them in, too.
 *
 */

static void DrawBox(double x, double y, double width, double height)
{
    MovePen(x, y);
    DrawLine(width,0);
    DrawLine(0,height);
    DrawLine(-width,0);
    DrawLine(0,-height);
}

static void FillBox(double x, double y, double width, double height,
                    double fill, string color)
{
    SetPenColor(color);
    StartFilledRegion(fill);
    DrawBox(x, y, width, height);
    EndFilledRegion();
}


static void DrawRoundedRect (double x, double y, double width, double height, double radius)
{
    MovePen(x + radius, y);
    DrawLine(width -2*radius,0);        // line across bottom
    DrawArc(radius,270,90);                // arc on lower right corner
    DrawLine(0,height -2*radius);        // line up right side
    DrawArc(radius,0,90);                // arc on upper right corner
    DrawLine(-(width -2*radius), 0);    // line across top
    DrawArc(radius,90,90);                // arc on upper left corner
    DrawLine(0,-(height-2*radius));        // line down left side
    DrawArc(radius,180,90);                // arc on lower left corner
}


static void FillRoundedRect (double x, double y, double width, double height,
                             double radius, double fill, string color)
{
    SetPenColor(color);
    StartFilledRegion(fill);
    DrawRoundedRect(x,y,width, height, radius);
    EndFilledRegion();
}


static void DrawAndFillRoundedRect (double x, double y, double width, double height,
                                    double radius, double fill, string color)
{
    FillRoundedRect(x, y,width, height, radius, fill, color);
    SetPenColor("Black");
    DrawRoundedRect(x, y, width, height, radius);
}

