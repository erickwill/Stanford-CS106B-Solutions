/* 
 * File: lexicon.h
 * ---------------
 * Defines the Lexicon class.
 * Last modified Tue Oct 10 20:47:22 PDT 2006 jzelenski
 */

#ifndef _lexicon_h
#define _lexicon_h

#include <string>
#include "genlib.h"
#include <set>

/*
 * Class: Lexicon
 * --------------
 * This class is used to represent a lexicon, or word list. The main difference
 * between the lexicon abstraction and a map or dictionary container is that
 * the lexicon does not provide any mechanism for storing definitions of words; 
 * the lexicon contains only words, with no associated information.  The lexicon 
 * supports very efficient lookup operations for words and prefixes. You can 
 * populate the lexicon by adding words one at a time or by constructing a lexicon
 * with words read from a file. Here is sample use of a Lexicon object:
 *
 *    Lexicon lex("lexicon.dat");    // read words from lexicon.dat file
 *    lex.add("doughnut");
 *  if (lex.containsPrefix("fru") || lex.containsWord("ball"))
 *        ...
 */

#include <cctype>

class Lexicon {

  public:
    
    /*
     * Constructor: Lexicon
     * Usage: Lexicon lex;
     *        Lexicon *lp = new Lexicon;
     * ---------------------------------
     * The constructor initializes a new empty lexicon.
     */
    Lexicon();
    
    /*
     * Constructor: Lexicon
     * Usage: Lexicon lex("lexicon.dat");
     *        Lexicon *lp = new Lexicon("lexicon.txt");
     * -----------------------------------------------
     * The constructor initializes a new lexicon which is populated with
     * the words read from the specified file.  The file is expected to
     * be either a special special binary format that represents a saved 
     * lexicon or a plain text file of words, one word per line. The
     * constructor reads the file and adds all of its words to this lexicon.
     * The file must be in the same folder as the program to be found. If 
     * the file doesn't exist or is malformed, Error is called to exit the program.
     */
    Lexicon(string filename);


   /*
     * Destructor: ~Lexicon
     * Usage: delete lp;
     * ------------------
     * The destructor deallocates any storage associated with the lexicon.
     */
    ~Lexicon();
    
    
   /*
    * Member function: size
    * Usage: count = lex.size();
    * --------------------------
    * This member function returns the number of words contained in this lexicon.
    */
    int size();
    
   /*
    * Member function: isEmpty
    * Usage: if (lex.isEmpty())...
    * --------------------------
    * This member function returns true if this lexicon contains no words, false otherwise.
    */
    bool isEmpty();

    
   /*
    * Member function: add
    * Usage: lex.add("computer");
    * ---------------------------
    * This member function adds the specified word to this lexicon.
    */
    void add(string word);
     
   /*
    * Member function: addWordsFromFile
    * Usage: lex.addWordsFromFile("words.txt");
    * -----------------------------------------
    * This member function reads the file and adds all of its words to this lexicon.
    * The file is expected to be either a special special binary format that represents 
    * a saved lexicon or a plain text file of words, one word per line.
    * The file must be in the same folder as the program to be found. If 
    * the file doesn't exist or is malformed, Error is called to exit the program.   
    */
    void addWordsFromFile(string filename);


   /*
    * Member function: containsWord
    * Usage: if (lex.containsWord("happy"))...
    * ----------------------------------------
    * This member function returns true if word is contained in this lexicon,
    * false otherwise.  Words are considered case-insensitively, "zoo" is the
    * same as "ZOO" or "zoo".
    */
    bool containsWord(string word);    
    
    
   /*
    * Member function: containsPrefix
    * Usage: lex.containsPrefix("mo");
    * --------------------------------
    * This member function returns true if any words in this lexicon begin with
    * prefix, false otherwise.  A word is defined to be a prefix of itself
    * and the empty string is a prefix of everything. Prefixes are considered 
    * case-insensitively, "mo" is a prefix of "MONKEY" or "Monday".
    */
    bool containsPrefix(string prefix);


   /*
    * Member function: clear
    * Usage: lex.clear();
    * -------------------
    * This member function removes all words from this lexicon. The lexicon
    * will be empty after being cleared.
    */
    void clear();
    

    /* 
     * SPECIAL NOTE: mapping support
     * -----------------------------
     * The lexicon supports a mapping operation which allows the client to access
     * words one by one.  In general, mapping is intended for _viewing_ words
     * and can behave unpredictably if you attempt to modify the lexicon's contents 
     * during mapping. 
     */        

    /*
     * Member function: mapAll
     * Usage: lexicon.mapAll(PrintToFile, outputStream);
     * -------------------------------------------------
     * This member function iterates through this lexicon
     * and calls the function fn once for each word, passing
     * the word and the client's data. That data can be of whatever 
     * type is needed for the client's callback.
     */
    template <typename ClientDataType>
      void mapAll(void (fn)(string word, ClientDataType &), ClientDataType &data);


    /*
     * Deep copying support
     * --------------------
     * This copy constructor and operator= are defined to make a
     * deep copy, making it possible to pass/return lexicons by value
     * and assign from one lexicon to another. The entire contents of 
     * the lexicon, including all words, are copied. Making copies is 
     * generally avoided because of the expense and thus, lexicons are 
     * typically passed by reference, however, when a copy is needed, 
     * these operations are supported.
     */
    Lexicon(const Lexicon &rhs);
    const Lexicon & operator=(const Lexicon &rhs);
              
                
  private:
    /* Everything from here to the end of the class is private, so
     * not accessible or intended for use for the client */

    
#ifdef _MSC_VER
#define LITTLE_ENDIAN 1
#define BYTE_ORDER LITTLE_ENDIAN
#endif

    struct Edge {
#if defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
        unsigned long letter:5;
        unsigned long lastEdge:1;
        unsigned long accept:1;
        unsigned long unused:1;
        unsigned long children:24;
#else
        unsigned long children:24;
        unsigned long unused:1;
        unsigned long accept:1;
        unsigned long lastEdge:1;
        unsigned long letter:5;
#endif
    };
    
    Edge *edges, *start;
    int numEdges, numDawgWords;
    std::set<string> otherWords;

    Edge *findEdgeForChar(Edge *children, char ch);
    Edge *traceToLastEdge(const string & s);
    void readBinaryFile(string filename);
    template <typename ClientDataType>
      void recMapAll(Edge *edge, bool first, string soFar, void (fn)(string, ClientDataType &), ClientDataType &data);

    unsigned int charToOrd(char ch) { return ((unsigned int)(tolower(ch) - 'a' + 1)); }
    char ordToChar(unsigned int ord) { return ((char)(ord - 1 + 'a')); }
    void copyContentsFrom(const Lexicon &rhs);
    
};


/* 
 * Because of the way C++ templates are compiled, we must put the implementation for
 * the mapping operation here in the header file. This is a bit quirky and seems to 
 * contradict the principles of encapsulation and abstraction. As a client of this class, 
 * you don't need to know anything about the internals and shouldn't depend on any 
 * implementation details. In fact, you probably should just stop reading right here!
 */


template <typename ClientDataType>
  void Lexicon::recMapAll(Edge *edge, bool first, string soFar, void (fn)(string word, ClientDataType &), ClientDataType &clientData)
    {
        if (!edge) return;
        Edge *curEdge;
        if (first) {
            curEdge = edge;
        } else {
            soFar += ordToChar(edge->letter);
            if (edge->accept) fn(soFar, clientData);
            if (!edge->children) return;
            curEdge = &edges[edge->children];
        }
        while (true) {
            recMapAll(curEdge, false, soFar, fn, clientData);
            if (curEdge->lastEdge) break;
            curEdge++;
        }
    }
    
template <typename ClientDataType>
  void Lexicon::mapAll(void (fn)(string word, ClientDataType &), ClientDataType &clientData)
    {
        recMapAll(start, true, "", fn, clientData);    // map over dawg
        for (set<string>::iterator itr = otherWords.begin(); itr != otherWords.end(); itr++)
            fn(*itr, clientData);                    // map over other set
    }

#endif