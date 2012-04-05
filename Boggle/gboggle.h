/*
 * File: gboggle.h
 * ---------------
 * The gboggle.h file defines the interface for a set of
 * functions that
 *
 *   1. Draw the boggle board
 *   2. Manage the word lists
 *   3. Update the scoreboard
 */

#ifndef _gboggle_h
#define _gboggle_h

#include "genlib.h"

/*
 * Type: playerT
 * -------------
 * This enumeration distinguishes the human and computer players.
 */
enum playerT {Human, Computer};


/*
 * Constant: MAX_DIMENSION
 * -----------------------
 * This constant determines the largest acceptable values for the board dimensions.
 */
const int MAX_DIMENSION = 5;

/*
 * Function: DrawBoard
 * Usage: DrawBoard(4, 4);
 * -----------------------
 * This function draws the empty layout of the board having the 
 * specified dimensions.  It should be called once at the beginning 
 * of each game, after calling InitGraphics() to erase the graphics 
 * window.  It will draw the cubes, board, and scoreboard labels.  
 * The scores and word lists are set to zero.  The boggle cubes are
 * drawn with blank faces, ready for letters to be set using the
 * LabelCube function. If either dimension is <= 0 or > MAX_DIMENSION,
 * an error is raised.
 */
void DrawBoard(int numRows, int numCols);

/*
 * Function: LabelCube
 * Usage: LabelCube(row, col, letter);
 * -----------------------------------
 * This function draws the specified letter on the face of the cube
 * at position (row, col).  The cubes are numbered from top to bottom
 * left to right starting  with zero. Therefore, the upper left corner is
 * is (0, 0); the lower right is (numRows-1, numCols-1).  Thus, the call
 *
 *      LabelCube(0, 3, 'D');
 *
 * would put a D in the top right corner cube. An error is raised if
 * row or col is out of range for this boggle board.
 */
void LabelCube(int row, int col, char letter);


/*
 * Function: HighlightCube
 * Usage: HighlightCube(row, col, flag);
 * -------------------------------------
 * This function highlights or unhighlights the specified cube
 * according to the setting of flag: if flag is true, the cube
 * is highlighted; if flag is false, the highlight is removed.
 * The highlight flag makes it possible for you to show which
 * cubes are using when forming a word on the board. An error is 
 * raised if row or col is out of range for this boggle board.
 */
void HighlightCube(int row, int col, bool flag);


/*
 * Function: RecordWordForPlayer
 * Usage: RecordWordForPlayer(word, player);
 * -----------------------------------------
 * This function records the specified word by adding it to
 * the screen display for the specified player and updating
 * the scoreboard accordingly.  Scoring is calculated as
 * follows:  a 4-letter word is worth 1 point, a 5-letter
 * is worth 2 points, and so on. An error is raised if player
 * is not a valid value for playerT (Human or Computer).
 */
void RecordWordForPlayer(string word, playerT player);


#endif
