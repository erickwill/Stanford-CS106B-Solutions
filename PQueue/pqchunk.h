/*
 * File: pqueue.h
 * --------------
 * Defines the interface for the priority queue class. 
 * 
 */
 
#ifndef _pqchunk_h
#define _pqchunk_h

#include "genlib.h"
#include "disallowcopy.h"

/*
 * Class: PQueue
 * -------------
 * This is the class for a priority queue.  This is not
 * simple FIFO queue, it is a priority queue, where elements are
 * retrieved in order of priority, not just by longevity in queue.
 * The elements are integers and the integer is assumed to represent 
 * the priority (larger integer is higher priority).
 */
class PQueue 
{
  public:
    /*
     * Constructor: PQueue
     * Usage: PQueue pq;
     *        PQueue *ppq = new PQueue;
     * ---------------------------------
     * Initializes a new pqueue to have no elements.  
     */
    PQueue();


    /*
     * Destructor: ~PQueue
     * Usage: delete ppq;
     * ------------------
     * Deallocates all the storage associated with this pqueue.
     */
    ~PQueue();


    /*
     * Member function: isEmpty
     * Usage: if (pq.isEmpty()) . . .
     * -------------------------------
     * Returns true if this pqueue contains no elements.
     */
    bool isEmpty();

    /*
     * Member function: size
     * Usage: nElemes = pq.size();
     * ---------------------------
     * Returns number of elements contained in this pqueue.
     */
    int size();

    /*
     * Member function: enqueue
     * Usage: pq.enqueue(val);
     * -----------------------
     * Adds the specified element to this pqueue. No effort is made to
     * avoid duplicates. 
     */
    void enqueue(int newElem);

    /*
     * Member function: eequeueMax
     * Usage: maxElem = pq.dequeueMax();
     * ---------------------------------
     * Removes the largest priority element from this pqueue and returns it.
     * If this pqueue is empty, this function raises an error.
     */
    int dequeueMax();

    /*
     * Member function: bytesUsed
     * Usage: numBytes = pq.bytesUsed();
     * ----------------------------------
     * This function would not usually be included as part of the class,
     * but this is here as part of evaluating the tradeoffs betweem
     * implementations. Given a pqueue, this function counts up
     * and return the total amount of space used given its
     * current contents.
     */
    int bytesUsed();
        
    /*
     * Member function: implementationName
     * Usage: cout << pq.implementationName();
     * ---------------------------------------
     * This operation would not usually be included as part of the class
     * class, but is included to help with analyzing and reporting results.
     * This member function returns a string that describes the PQueue
     * implementation strategy ("sorted linked list", "vector", etc.).
     */
    string implementationName();

    /*
     * Member function: printDebuggingInfo
     * Usage: pq.printDebuggingInfo();
     * -------------------------------
     * This operation would not usually be included as part of the class,
     * but is included to give you a hook to put any helpful debugging
     * print code (for example, something that prints the goopy details of
     * the internal structure). You don't need to implement this routine and can
     * ignore it entirely, but you may find it helpful to use as you are
     * doing development.
     */
    void printDebuggingInfo();

    /*
     * Member function: print
     * Usage: pq.();
     * -------------------------------
     * Prints all enqueued values.
     */
    void print();


  private:
    struct Cell {
        int *values;
        Cell *next;
        int BlocksInUse;
    };
    Cell *head;
    DISALLOW_COPYING(PQueue)
    PQueue::Cell *initCell(int newValue, Cell *nextCell);
    void shiftAdd(Cell *cell, int newValue);
    void splitAdd(Cell *cur, int newValue);
};

#endif